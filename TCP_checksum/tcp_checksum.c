#include <stdio.h>
#include <stdlib.h>
/*
TRAMAS DE PRUEBA;

PRIMERO LA DE LA TAREA QUE TIENE CHECKSUM = 0000
0x02, 0xFF, 0x53, 0xC3, 0xE9, 0xAB, 0x00, 0xFF,
    0x66, 0x7F, 0xD4, 0x3C, 0x08, 0x00, 0x45, 0x02, 0x00, 0x30, 0x2C, 0x00, 0x40, 0x00, 0x80, 0x06,
    0x4B, 0x74, 0xC0, 0xA8, 0x01, 0x02, 0xC0, 0xA8, 0x01, 0x01, 0x04, 0x03, 0x00, 0x15, 0x00, 0x3B,
    0xCF, 0x44, 0x00, 0x00, 0x00, 0x00, 0x60, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04,
    0x05, 0xB4
    
    
    
LUEGO OTRA DE INTERNET CON ALGUNAS ESPECIFICACIONES QUE TIENE CHECKSUM INCORRECTA
0x00, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,  // ETH dest
0x11, 0x22, 0x33, 0x44, 0x55, 0x66,  // ETH src
0x08, 0x00,                          // Type=IPv4

// --- IP header (20 bytes) ---
0x45, 0x00,                          // Version=4, IHL=5, DSCP=0
0x00, 0x28,                          // Total length=0x0028=40
0x00, 0x01,                          // Identification
0x40, 0x00,                          // Flags=2 (DF), Offset=0
0x40,                                // TTL=64
0x06,                                // Protocol=TCP
0xBA, 0xAD,                          // IP Checksum (ficticio)
0xAC, 0x10, 0x00, 0x01,              // Source IP = 172.16.0.1
0xAC, 0x10, 0x00, 0xFE,              // Dest IP   = 172.16.0.254

// --- TCP header (20 bytes) ---
0x1F, 0x90,                          // Src Port = 8080
0x01, 0xBB,                          // Dst Port = 443 (HTTPS)
0x12, 0x34, 0x56, 0x78,              // Sequence Number
0x87, 0x65, 0x43, 0x21,              // Acknowledgment Number
0x50, 0x18,                          // DataOffset=5, Flags=PSH(0x08)+ACK(0x10)=0x18
0x00, 0x80,                          // Window size
0xFE, 0xED,                          // TCP Checksum (ficticio)
0x00, 0x00                           // Urgent Pointer


ACÁ HAY OTRA PERO ESTA SI TIENE EL CHECKSUM CORRECTO:
// Ethernet (14 bytes)
0x00, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,
0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB,
0x08, 0x00,

// IP (20 bytes)
0x45, 0x00,
0x00, 0x28,
0x00, 0x02,
0x40, 0x00,
0x40, 0x06,
0x26, 0xDC,        // Checksum IP correcto (0x26DC)
0x0A, 0x00, 0x00, 0x01,  // 10.0.0.1
0x0A, 0x00, 0x00, 0x02,  // 10.0.0.2

// TCP (20 bytes)
0x1F, 0x41,        // Src Port = 8001
0x00, 0x50,        // Dst Port = 80
0x00, 0x00, 0x00, 0x00,  
0x00, 0x00, 0x00, 0x00,  
0x50, 0x02,        // Offset=5, Flags=SYN
0xFA, 0xF0,        
0x81, 0x5E,        // Checksum TCP = 0x813E
0x00, 0x00


OTRA TRAMA CON EL CHECKSUM CORRECTO:

// Ethernet (14 bytes)
0x00, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,
0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB,
0x08, 0x00,

// IP (20 bytes)
0x45, 0x00,
0x00, 0x28,
0x00, 0x02,
0x40, 0x00,
0x40, 0x06,
0x26, 0xDC,        // Checksum IP correcto (0x26DC)
0x0A, 0x00, 0x00, 0x01,  // 10.0.0.1
0x0A, 0x00, 0x00, 0x02,  // 10.0.0.2

// TCP (20 bytes)
0x1F, 0x41,        // Src Port = 8001
0x00, 0x50,        // Dst Port = 80
0x00, 0x00, 0x00, 0x00,  
0x00, 0x00, 0x00, 0x00,  
0x50, 0x02,        // Offset=5, Flags=SYN
0xFA, 0xF0,        
0x81, 0x5E,        // Checksum TCP = 0x815E
0x00, 0x00






*/








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

    //printf("\nAntes: %x %x %x %x\n", checksum_tcp[0], checksum_tcp[1], checksum_tcp[2], checksum_tcp[3]);
    // sacar complementos a checksum
    for (i = 0; i < 4; i++)
    {
        checksum_tcp[i] = 0x0F - checksum_tcp[i];
    }

    printf("\nChecksum: %x %x %x %x\n", checksum_tcp[0], checksum_tcp[1], checksum_tcp[2], checksum_tcp[3]);
    //printf("\nChecksum: %x %x \n", pseudo_tcp[28], pseudo_tcp[29]);
    aux = checksum_tcp[0] << 4 | checksum_tcp[1];
    aux2 = checksum_tcp[2] << 4 | checksum_tcp[3];
    //printf("\nChecksum: %x %x \n", aux, aux2);
    if ((aux == pseudo_tcp[28]) && (aux2 == pseudo_tcp[29])) {
        printf("\nEs CORRECTO el checksum.\n");
    } else if((pseudo_tcp[28] == 0) && (pseudo_tcp[29] == 0)) {
        printf("\nEl checksum de la trama original vale 00 00, se imprime el valor calculado...\n");
        printf("Checksum calculado: %x%x%x%x\n", checksum_tcp[0], checksum_tcp[1], checksum_tcp[2], checksum_tcp[3]);
    }
    else {
        printf("\nEs INCORRECTO el valor del checksum.\n");
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
    //printf ("BIT DE OFFSET: %.2x \n", trama[IHL+12+14]);
    // 'nibble' del offset (bits altos del byte 12 de TCP)
    unsigned char dataOffset_nibble = (trama[IHL + 12 + 14] & 0xF0) >> 4;

    // Tamaño de la cabecera TCP en bytes
    unsigned short tcpHeaderSize = dataOffset_nibble * 4; 
    
    pseudo_tcp[10] = (tcpHeaderSize >> 8) & 0xFF; // parte alta
    pseudo_tcp[11] = tcpHeaderSize & 0xFF;        // parte baja
    
    //pseudo_tcp[10] = ((((trama[IHL+12+14]&0xF0)>>4)*4);
    //impresión para comprobar que es correcto la asignación del tamaño
    //printf("Tamaño de cabecera TCP: %.2x %.2x \n",  pseudo_tcp[10],  pseudo_tcp[11]);

    // ... lo restante es lo que sigue despues de haber terminado IP 
    
    for(i= 0; i<tcpHeaderSize; i++){ // el for se repetira hasta alcanzar
    // el tamanio maximo de la pseudocabecera
        pseudo_tcp[12 + i] = trama[IHL+14+i]; 
    }
    
}


int main(){
    unsigned char t[128] ={
// Ethernet (14 bytes)
0x00, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,
0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB,
0x08, 0x00,

// IP (20 bytes)
0x45, 0x00,
0x00, 0x28,
0x00, 0x02,
0x40, 0x00,
0x40, 0x06,
0x26, 0xDC,        // Checksum IP correcto (0x26DC)
0x0A, 0x00, 0x00, 0x01,  // 10.0.0.1
0x0A, 0x00, 0x00, 0x02,  // 10.0.0.2

// TCP (20 bytes)
0x1F, 0x41,        // Src Port = 8001
0x00, 0x50,        // Dst Port = 80
0x00, 0x00, 0x00, 0x00,  
0x00, 0x00, 0x00, 0x00,  
0x50, 0x02,        // Offset=5, Flags=SYN
0xFA, 0xF0,        
0x81, 0x5E,        // Checksum TCP = 0x815E
0x00, 0x00


};
     unsigned char i;
    
    // revisamos primero que dicha trama realmente sea realmente IP:
    if ((t[12]<<8|t[13]) == 0x0800){

        // es trama IP 
        // nos toca revisar el campo de protocolo en t[23]
        if(t[23] == 0x06){
            // es protocolo TCP
            //ARMAMOS LA PSEUDOCABECERA TCP PARA EL CHECKSUM
            // llamaremos la función que arma dicha pseudocabecera
            printf("\t==ANALIZADOR DE CHECKSUM PARA PSEUDOCABECERA TCP==\n");
            printf("Autores: Bautista Coello Alexandra & Taboada Montiel Enrique");
            // ocuparemos el campo del IHL de la de IP, recordemos que nos permite saber 
            // el tamaño total de la cabecera IP y donde termina IP empieza TCP
            unsigned char IHL = (t[14]&0x0F)*4;
            //printf("TAMAÑO IHL IP: %d \n", IHL);
            unsigned char pseudo_tcp[128];
            pseudocabecera(t, IHL, pseudo_tcp);
            //imprimimos la pseudocabecera para comprobar
            //printf("PSEUDOCABECERA: ");
            unsigned char dataOffset_nibble = (t[IHL + 12 + 14] & 0xF0) >> 4;
            unsigned short tcpHeaderSize = dataOffset_nibble * 4;
            /*for (i= 0; i<(tcpHeaderSize+12); i++){
                if( i==(tcpHeaderSize+11)){
                    printf(" %.2x", pseudo_tcp[i]);
                    
                }else {
                    printf(" %.2x:", pseudo_tcp[i]);
                }
            }*/
            
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
