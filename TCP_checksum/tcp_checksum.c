#include <stdio.h>
#include <stdlib.h>

// codigo para obtener el checksum:
void funcionCheksum(unsigned char *pseudo_tcp, unsigned short tcpHeaderSize, unsigned char checksum_tcp[4]) {
    //unsigned char checksum[4];
    unsigned char sumas[5];
    unsigned char sum=0, aux, aux2;
    
    char i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < (tcpHeaderSize+12); j++)
        {
            if(j==28 || j==29) {
                continue;
            }
            else {
                if(j&1) {
                    if(i==0) {
                        sum += pseudo_tcp[j] & 0x0F;
                        
                    }
                    else if(i==1) {
                        sum += (pseudo_tcp[j] & 0xF0) >> 4;
                    }
                }
                else {
                    if(i==2) {
                        sum += pseudo_tcp[j] & 0x0F;
                    }
                    else if(i==3) {
                        sum += (pseudo_tcp[j] & 0xF0) >> 4;
                    }
                }
            }

        }

        sumas[i] = sum&0x0F;
        sum = sum >> 4;
        
    }

    sumas[4] = sum;
    j = 3;
    aux = 0;

    for (i = 0; i < 4; i++)
    {
        if(i==0) {
            sum = sumas[i] + sumas[4];
        }
        else {
            sum = aux + sumas[i];
        }
        aux = sum & 0xF0;
        sum = sum & 0x0F;
        checksum_tcp[j] = sum;
        j--;
    }

    printf("\nAntes: %x %x %x %x\n", checksum_tcp[0], checksum_tcp[1], checksum_tcp[2], checksum_tcp[3]);
    // sacar complementos a checksum
    for (i = 0; i < 4; i++)
    {
        checksum_tcp[i] = 0x0F - checksum_tcp[i];
    }

    printf("Checksum: %x %x %x %x\n", checksum_tcp[0], checksum_tcp[1], checksum_tcp[2], checksum_tcp[3]);
    aux = checksum_tcp[0] << 4 | checksum_tcp[1];
    aux2 = checksum_tcp[2] << 4 | checksum_tcp[3];
    if (aux == pseudo_tcp[28] && aux2 == pseudo_tcp[29]) {
        printf("ACK\n");
    } else if(pseudo_tcp[28] == 0 && pseudo_tcp[29] == 0) {
        printf("Checksum calculado: %x%x%x%x\n", checksum_tcp[0], checksum_tcp[1], checksum_tcp[2], checksum_tcp[3]);
    }
    else {
        printf("NACK\n");
    }    
    
}


// código para obtener la pseudocabecera
void pseudocabecera(unsigned char *trama, unsigned char IHL, unsigned char pseudo_tcp[128]){
    //unsigned char t_aux[128];
    unsigned char i = 0;
    // recordemos que el offset me va a indicar la longitud de mi pseudocabecera TCP
    // considerando que el tamaño mínimo es de 5 
    for(i= 0; i<4; i++){
        pseudo_tcp[i] = trama[i+26]; // se obtiene la direccion origen de la trama original
        pseudo_tcp[i+4] = trama[i+30]; // se obtiene la direccion destino de trama original
    }
    
    pseudo_tcp[8] = 0x00; // no se usa
    pseudo_tcp[9] = 0x06; // protocolo fijo siempre en 0x06
    
    // EL TAMAÑO EN BYTES SE OBTIENE DEL OFFSET 
    printf ("BIT DE OFFSET: %.2x \n", trama[IHL+12+14]);
    // 'nibble' del offset (bits altos del byte 12 de TCP)
    unsigned char dataOffset_nibble = (trama[IHL + 12 + 14] & 0xF0) >> 4;

    // Tamaño de la cabecera TCP en bytes
    unsigned short tcpHeaderSize = dataOffset_nibble * 4; 
    
    pseudo_tcp[10] = (tcpHeaderSize >> 8) & 0xFF; // parte alta
    pseudo_tcp[11] = tcpHeaderSize & 0xFF;        // parte baja
    
    //pseudo_tcp[10] = ((((trama[IHL+12+14]&0xF0)>>4)*4);
    //impresión para comprobar que es correcto la asignación del tamaño
    printf("Tamaño de cabecera TCP: %.2x %.2x \n",  pseudo_tcp[10],  pseudo_tcp[11]);

    // ... lo restante es lo que sigue despues de haber terminado IP 
    
    for(i= 0; i<tcpHeaderSize; i++){ // el for se repetira hasta alcanzar
    // el tamanio maximo de la pseudocabecera
        pseudo_tcp[12 + i] = trama[IHL+14+i]; 
    }
    
}


int main(){
    unsigned char t[128] ={0x02, 0xFF, 0x53, 0xC3, 0xE9, 0xAB, 0x00, 0xFF,
    0x66, 0x7F, 0xD4, 0x3C, 0x08, 0x00, 0x45, 0x02, 0x00, 0x30, 0x2C, 0x00, 0x40, 0x00, 0x80, 0x06,
    0x4B, 0x74, 0xC0, 0xA8, 0x01, 0x02, 0xC0, 0xA8, 0x01, 0x01, 0x04, 0x03, 0x00, 0x15, 0x00, 0x3B,
    0xCF, 0x44, 0x00, 0x00, 0x00, 0x00, 0x60, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04,
    0x05, 0xB4};
     unsigned char i;
    
    // revisamos primero que dicha trama realmente sea realmente IP:
    if ((t[12]<<8|t[13]) == 0x0800){

        // es trama IP 
        // nos toca revisar el campo de protocolo en t[23]
        if(t[23] == 0x06){
            // es protocolo TCP
            //ARMAMOS LA PSEUDOCABECERA TCP PARA EL CHECKSUM
            // llamaremos la función que arma dicha pseudocabecera
            
            // ocuparemos el campo del IHL de la de IP, recordemos que nos permite saber 
            // el tamaño total de la cabecera IP y donde termina IP empieza TCP
            unsigned char IHL = (t[14]&0x0F)*4;
            printf("TAMAÑO IHL IP: %d \n", IHL);
            unsigned char pseudo_tcp[128];
            pseudocabecera(t, IHL, pseudo_tcp);
            //imprimimos la pseudocabecera para comprobar
            printf("PSEUDOCABECERA: ");
            unsigned char dataOffset_nibble = (t[IHL + 12 + 14] & 0xF0) >> 4;
            unsigned short tcpHeaderSize = dataOffset_nibble * 4;
            for (i= 0; i<(tcpHeaderSize+12); i++){
                if( i==(tcpHeaderSize+11)){
                    printf(" %.2x", pseudo_tcp[i]);
                    
                }else {
                    printf(" %.2x:", pseudo_tcp[i]);
                }
            }
            
            // obtenemos el valor del checksum
            unsigned char checksum_tcp[4] = {0};
            funcionCheksum(pseudo_tcp, tcpHeaderSize, checksum_tcp);
            
            
            
            
        } else {
            // no es protocolo TCP entonces salimos del programa pq ya no cumple
            printf("No es Protocolo TPC\n");
            return 0;
        }
    }else{
        // si no es protocolo IP entonces, no tendra protocolo TCP, finalizamos programa
        printf("No es Protocolo IP\n");
        return 0;
    }
}
