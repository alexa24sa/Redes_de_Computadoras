#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// *************************** SECCION LLC ***************************

void analizarLLC(unsigned char cabecera[36]){
    // if(tipo == 'T'){
        unsigned char SAPd = cabecera[14];
        unsigned char SAPo = cabecera[15];
        
        const char *ss[] = {"RR", "RNR", "REJ", "SREJ"};
		const char *uc[] = {"UI", "SIM", "-", "SARM", "UP", "-", "-", "SABM", "DISC", "-", "-", "SARME", "-", "-", "-", "SABME", "SNRM", "-", "-", "RSET", "-", "-", "-", "XID", "-", "-", "-", "SNRME"};
		const char *ur[] = {"UI", "RIM", "-", "DM", "-", "-", "-", "-", "RD", "-", "-", "-", "UA", "-", "-", "-", "-", "FRMR", "-", "-", "-", "-", "-", "XID", "-", "-", "-", "-"};
        
        //IMPRESIONES DE LOS SAP origen y SAP destino 
        printf("\nSAP Destino: %hx", SAPd);
        printf("\nSAP Origen: %hx", SAPo);
        
        switch(cabecera[16]&3)
        {
            case 1: //se trata de una trama de supervisión
                printf("\nEs una T-S : trama de supervisión");
                printf("\nControl: %s", ss[(cabecera[16]>>2)&3]);
                if(cabecera[17]&1){//El P/F = 1
                    printf("\nP/F : El bit P/F SÍ está encendido.");
                    if(cabecera[15]&1){ //LSB SAPo = 1  entonces F
                        printf("\nEs ------ F (respuesta):");
                        //printf("\nN(s) = %d", (cabecera[16]>>1)&0X7F);
                        printf("\nN(r) = %d - f", (cabecera[17]>>1)&0X7F);
                    }else{ //LSB SAPo = 0  entonces P
                        printf("\nEs ------ P (comando):");
                        //printf("\nN(s) = %d", (cabecera[16]>>1)&0X7F);
                        printf("\nN(r) = %d - p", (cabecera[17]>>1)&0X7F);
                    }
                    
                }else{
                    printf("\nP/F : El bit P/F NO está encendido.");
                    //printf("\nN(s) = %d", (cabecera[16]>>1)&0X7F);
                    printf("\nN(r) = %d", (cabecera[17]>>1)&0X7F);
                }
                
                break;
            case 3: 
{
                // Se trata de una trama no numerada
                char M = ((cabecera[16] >> 2) & 3) | ((cabecera[16] >> 3) & 28);
                printf("\n%hx", M);
                printf("\nEs una T-U : trama no numerada");
                if(cabecera[16] & 16) { // EL P/F = 1
                    printf("\nP/F : El bit P/F SÍ está encendido.");
                    if(cabecera[15] & 1) { // Revisamos el LSB de SAPo
                        printf("\nEs ------ F (respuesta):");
                        printf("\nControl: %s -- f", ur[M]);
                    } else {
                        printf("\nEs ------ P (comando):");
                        printf("\nControl: %s -- p", uc[M]);
                    }
                } else {
                    printf("\nP/F : El bit P/F NO está encendido.");
                }
                break;
            }
            default: //se trata de una trama de información
                printf("\nEs una T-I : trama de información");
                if(cabecera[17]&1){ //El P/F = 1
                    printf("\nP/F : El bit P/F SÍ está encendido.");
                    if(cabecera[15]&1){ //LSB SAPo = 1  entonces F
                        printf("\nEs ------ F (respuesta):");
                        printf("\nN(s) = %d", (cabecera[16]>>1)&0X7F);
                        printf("\nN(r) = %d - f", (cabecera[17]>>1)&0X7F);
                    }else{ //LSB SAPo = 0  entonces P
                        printf("\nEs ------ P (comando):");
                        printf("\nN(s) = %d", (cabecera[16]>>1)&0X7F);
                        printf("\nN(r) = %d - p", (cabecera[17]>>1)&0X7F);
                    }
                }else{
                    printf("\nP/F : El bit P/F NO está encendido.");
                    printf("\nN(s) = %d", (cabecera[16]>>1)&0X7F);
                    printf("\nN(r) = %d", (cabecera[17]>>1)&0X7F);
                }
                
                
                break;
        }
        
        
        
    // }
    
}


// *************************** SECCION ARP ***************************
void analizarARP(unsigned char cabecera[36]) {

	// Tipo de direccion HW
	if ((cabecera[14]<<8 | cabecera[15]) == 1) { /*
	se suman los bits de cabecera[14] con cabecera[15] para que se complenten los 2 bytes
	de tipo de direcciones de hadware que se pueden revisar del mapa de memoria
	si al unirlos nos da un valor de 1, es decir:
	0000 0000 0000 0001
	según las cofiguraciones proporcionadas en clase, tenemos lo siguiente:
	00 01 Ethernet
	00 06 IEE 802 Token
	00 0F Frame Relay 
	00 01 ATM 
	
	*/
		printf("Tipo de direccion HW: Ethernet\n");
	} else if((cabecera[14]<<8 | cabecera[15]) == 6){
		printf("Tipo de direccion HW: IEEE 802 LAN\n");
	} else {
		printf("Tipo de direccion HW: Otro %d\n", cabecera[14]<<8 | cabecera[15]);
	}

	// Tipo de direccion de protocolo
	if ((cabecera[16]<<8 | cabecera[17]) == 0x0800) {
		printf("Tipo de direccion de protocolo: IPv4\n");
	} else {
		printf("Tipo de direccion de protocolo: Otro, %.2x: %.2x\n", cabecera[16], cabecera[17]);
	}

	// Longitud de la direccion HW
	printf("Tamaño de la direccion HW: %d bytes\n", cabecera[18]);
	if(cabecera[18] == 6) {
		printf("Ethernet / IEEE 802\n");
	}

	// Longitud de la direccion de protocolo
	printf("Tamaño de la direccion de protocolo: %d bytes\n", cabecera[19]);

	// Operacion
	if ((cabecera[20]<<8 | cabecera[21]) == 1) {
		printf("Operacion: Request\n");
	} else if ((cabecera[20]<<8 | cabecera[21]) == 2) {
		printf("Operacion: Reply\n");
	} else {
		printf("Operacion: Otro\n");
	}

	// Direccion MAC origen
	printf("Direccion MAC origen: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", cabecera[22], cabecera[23], cabecera[24], cabecera[25], cabecera[26], cabecera[27]);

	// Direccion IP origen
	printf("Direccion IP origen: %d.%d.%d.%d\n", cabecera[28], cabecera[29], cabecera[30], cabecera[31]);

	// Direccion MAC destino
	printf("Direccion MAC destino: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", cabecera[32], cabecera[33], cabecera[34], cabecera[35], cabecera[36], cabecera[37]);

	// Direccion IP destino
	printf("Direccion IP destino: %d.%d.%d.%d\n", cabecera[38], cabecera[39], cabecera[40], cabecera[41]);

}


// *************************** Funcion Checksum ***************************
void funcionCheksum(unsigned char *message) {
    unsigned short checksum[4];
    unsigned short sumas[5];
    unsigned short sum=0, aux, aux2;
    
    char i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 20; j++)
        {
            if(j==10 || j==11) {
                continue;
            }
            else {
                if(j&1) {
                    if(i==0) {
                        sum += message[j] & 0x0F;
                        
                    }
                    else if(i==1) {
                        sum += (message[j] & 0xF0) >> 4;
                    }
                }
                else {
                    if(i==2) {
                        sum += message[j] & 0x0F;
                    }
                    else if(i==3) {
                        sum += (message[j] & 0xF0) >> 4;
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
        checksum[j] = sum;
        j--;
    }

    printf("Checksum: %x %x %x %x\n", checksum[0], checksum[1], checksum[2], checksum[3]);
    aux = checksum[0] << 4 | checksum[1];
    aux2 = checksum[2] << 4 | checksum[3];
    if (aux == message[10] && aux2 == message[11]) {
        printf("ACK\n");
    } else if(message[10] == 0 && message[11] == 0) {
        printf("Checksum calculado: %x%x%x%x\n", checksum[0], checksum[1], checksum[2], checksum[3]);
    }
    else {
        printf("NACK\n");
    }    
    
}   

char menu() {
	char opcion;
	printf("Menu:\n");
	printf("1. Seleccionar trama existente\n");
	printf("2. Ingresar trama manualmente\n");
	printf("3. Leer trama desde archivo\n");
	printf("Selecciona una opcion: ");
	scanf("%c", &opcion);

	return opcion;
}

int main() {

    unsigned char trama[][200] =
			{		{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x03, 0xf0, 0xf0,
					 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x05, 0x90, 0x6d}, //trama1
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x03, 0xf0, 0xf1,
					 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x90, 0x6d}, //trama2
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x04, 0xf0, 0xf0,
					 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0xa3, 0x90, 0x6d}, //trama3
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf2, 0x90, 0x6d}, //trama4
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x12, 0xf0, 0xf0,
					 0x00, 0x01, 0x0e, 0x00, 0xff, 0xef, 0x19, 0x8f, 0xbc, 0x05, 0x7f, 0x00, 0x23, 0x00, 0x7f, 0x23,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x91, 0x6d}, //trama5
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x12, 0xf0, 0xf0,
					 0x00, 0x03, 0x0e, 0x00, 0xff, 0xef, 0x17, 0x81, 0xbc, 0x05, 0x23, 0x00, 0x7f, 0x00, 0x23, 0x7f,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x91, 0x6d}, //trama6
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdf, 0x91, 0x6d}, //trama7
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xac, 0x92, 0x6d}, //trama8
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0xac, 0xf0, 0xf0,
					 0x02, 0x02, 0x0e, 0x00, 0xff, 0xef, 0x16, 0x04, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x7f, 0x23,
					 0xff, 0x53, 0x4d, 0x42, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x09,
					 0x00, 0x77, 0x00, 0x02, 0x50, 0x43, 0x20, 0x4e, 0x45, 0x54, 0x57, 0x4f, 0x52, 0x4b, 0x20, 0x50,
					 0x52, 0x4f, 0x47, 0x52, 0x41, 0x4d, 0x20, 0x31, 0x2e, 0x30, 0x00, 0x02, 0x4d, 0x49, 0x43, 0x52,
					 0x4f, 0x53, 0x4f, 0x46, 0x54, 0x20, 0x4e, 0x45, 0x54, 0x57, 0x4f, 0x52, 0x4b, 0x53, 0x20, 0x33,
					 0x2e, 0x30, 0x00, 0x02, 0x44, 0x4f, 0x53, 0x20, 0x4c, 0x4d, 0x31, 0x2e, 0x32, 0x58, 0x30, 0x30,
					 0x32, 0x00, 0x02, 0x44, 0x4f, 0x53, 0x20, 0x4c, 0x41, 0x4e, 0x4d, 0x41, 0x4e, 0x32, 0x2e, 0x31,
					 0x00, 0x02, 0x57, 0x69, 0x6e, 0x64, 0x6f, 0x77, 0x73, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x57, 0x6f,
					 0x72, 0x6b, 0x67, 0x72, 0x6f, 0x75, 0x70, 0x73, 0x20, 0x33, 0x2e, 0x31, 0x61, 0x00, 0x02, 0x4e,
					 0x54, 0x20, 0x4c, 0x4d, 0x20, 0x30, 0x2e, 0x31, 0x32, 0x00, 0x00, 0xfb, 0x92, 0x6d, 0x86, 0xdf}, //trama9
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7b, 0x93, 0x6d}, //trama10
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x5f, 0xf0, 0xf0,
					 0x02, 0x04, 0x0e, 0x00, 0xff, 0xef, 0x16, 0x0c, 0x00, 0x00, 0x28, 0x00, 0x28, 0x00, 0x23, 0x7f,
					 0xff, 0x53, 0x4d, 0x42, 0x72, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x09,
					 0x11, 0x05, 0x00, 0x02, 0x02, 0x00, 0x01, 0x00, 0x68, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
					 0x7f, 0x07, 0x00, 0x80, 0x03, 0x02, 0x00, 0x00, 0x00, 0xe5, 0xfe, 0x29, 0x25, 0x7c, 0xc2, 0x01,
					 0x2c, 0x01, 0x08, 0x08, 0x00, 0x7f, 0x07, 0x00, 0x80, 0x32, 0x3e, 0xb9, 0x3d, 0x00, 0xca, 0x93}, //trama11
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x94, 0x6d}, //trama12
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x91, 0xf0, 0xf0,
					 0x04, 0x04, 0x0e, 0x00, 0xff, 0xef, 0x16, 0x0c, 0x00, 0x00, 0x28, 0x00, 0x28, 0x00, 0x7f, 0x23,
					 0xff, 0x53, 0x4d, 0x42, 0x73, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x09,
					 0x0d, 0x75, 0x00, 0x5d, 0x00, 0x68, 0x0b, 0x02, 0x00, 0x00, 0x00, 0x7f, 0x07, 0x00, 0x80, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x45,
					 0x53, 0x43, 0x4f, 0x4d, 0x00, 0x57, 0x69, 0x6e, 0x64, 0x6f, 0x77, 0x73, 0x20, 0x34, 0x2e, 0x30,
					 0x00, 0x57, 0x69, 0x6e, 0x64, 0x6f, 0x77, 0x73, 0x20, 0x34, 0x2e, 0x30, 0x00, 0x04, 0xff, 0x00,
					 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x17, 0x00, 0x20, 0x00, 0x5c, 0x5c, 0x50, 0x52, 0x4f, 0x47,
					 0x59, 0x44, 0x45, 0x53, 0x41, 0x5c, 0x49, 0x50, 0x43, 0x24, 0x00, 0x49, 0x50, 0x43, 0x00, 0x00}, //trama13
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x95, 0x6d}, //trama14
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x46, 0xf0, 0xf0,
					 0x04, 0x06, 0x0e, 0x00, 0xff, 0xef, 0x16, 0x0c, 0x00, 0x00, 0x28, 0x00, 0x28, 0x00, 0x23, 0x7f,
					 0xff, 0x53, 0x4d, 0x42, 0x73, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x82, 0x09,
					 0x03, 0x75, 0x00, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0x00, 0x00, 0x00, 0x04, 0x00,
					 0x49, 0x50, 0x43, 0x00, 0x00, 0x81, 0x95, 0x6d, 0x86, 0xcb, 0x94, 0x6d, 0x86, 0x0d, 0x09, 0x0e}, //trama15
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x96, 0x6d}, //trama16
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x7e, 0xf0, 0xf0,
					 0x06, 0x06, 0x0e, 0x00, 0xff, 0xef, 0x16, 0x0c, 0x00, 0x00, 0x28, 0x00, 0x28, 0x00, 0x7f, 0x23,
					 0xff, 0x53, 0x4d, 0x42, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x82, 0x0a,
					 0x0e, 0x20, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x88, 0x13, 0x00,
					 0x00, 0x00, 0x00, 0x20, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x5c,
					 0x50, 0x49, 0x50, 0x45, 0x5c, 0x4c, 0x41, 0x4e, 0x4d, 0x41, 0x4e, 0x00, 0x68, 0x00, 0x57, 0x72,
					 0x4c, 0x65, 0x68, 0x44, 0x7a, 0x00, 0x42, 0x31, 0x36, 0x42, 0x42, 0x44, 0x7a, 0x00, 0x01, 0x00,
					 0x00, 0x10, 0xff, 0xff, 0xff, 0xff, 0x45, 0x53, 0x43, 0x4f, 0x4d, 0x00, 0x00, 0x6f, 0x96, 0x6d}, //trama17
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbe, 0x96, 0x6d}, //trama18
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5d, 0x97, 0x6d}, //trama19
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x7e, 0xf0, 0xf0,
					 0x08, 0x08, 0x0e, 0x00, 0xff, 0xef, 0x16, 0x0c, 0x00, 0x00, 0x28, 0x00, 0x28, 0x00, 0x7f, 0x23,
					 0xff, 0x53, 0x4d, 0x42, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0b,
					 0x0e, 0x20, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x88, 0x13, 0x00,
					 0x00, 0x00, 0x00, 0x20, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x5c,
					 0x50, 0x49, 0x50, 0x45, 0x5c, 0x4c, 0x41, 0x4e, 0x4d, 0x41, 0x4e, 0x00, 0x68, 0x00, 0x57, 0x72,
					 0x4c, 0x65, 0x68, 0x44, 0x7a, 0x00, 0x42, 0x31, 0x36, 0x42, 0x42, 0x44, 0x7a, 0x00, 0x01, 0x00,
					 0x00, 0x10, 0x00, 0x00, 0x00, 0x80, 0x45, 0x53, 0x43, 0x4f, 0x4d, 0x00, 0x00, 0xac, 0x97, 0x6d}, //trama20
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0x97, 0x6d}, //trama21
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x98, 0x6d}, //trama22
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x12, 0xf0, 0xf0,
					 0x0a, 0x0b, 0x0e, 0x00, 0xff, 0xef, 0x14, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x7f, 0x23,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x99, 0x98, 0x6d}, //trama23
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0x99, 0x6d}, //trama24
					{0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0xac, 0x44, 0x4d, 0x02, 0x00, 0x8b, 0xf0, 0xf0,
					 0x03, 0x2c, 0x00, 0xff, 0xef, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x34, 0x20,
					 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1b, 0x49, 0x42, 0x4d,
					 0x53, 0x45, 0x52, 0x56, 0x45, 0x52, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0xff, 0x53, 0x4d,
					 0x42, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00,
					 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x56, 0x00, 0x03, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00,
					 0x17, 0x00, 0x5c, 0x4d, 0x41, 0x49, 0x4c, 0x53, 0x4c, 0x4f, 0x54, 0x5c, 0x42, 0x52, 0x4f, 0x57,
					 0x53, 0x45, 0x00, 0x09, 0x04, 0x33, 0x17, 0x00, 0x00, 0x00, 0x9b, 0x99, 0x6d, 0x86, 0x99, 0x98}, //trama25
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x35, 0xf0, 0xf0,
					 0x0c, 0x0a, 0x0e, 0x00, 0xff, 0xef, 0x16, 0x04, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x7f, 0x23,
					 0xff, 0x53, 0x4d, 0x42, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x50,
					 0x00, 0x00, 0x00, 0x45, 0xf1, 0x99, 0x6d, 0x86, 0x45, 0x99, 0x6d, 0x86, 0x1f, 0x09, 0x52, 0x5b}, //trama26
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x35, 0xf0, 0xf0,
					 0x0a, 0x0e, 0x0e, 0x00, 0xff, 0xef, 0x16, 0x0c, 0x00, 0x00, 0x28, 0x00, 0x28, 0x00, 0x23, 0x7f,
					 0xff, 0x53, 0x4d, 0x42, 0x71, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x50,
					 0x00, 0x00, 0x00, 0x00, 0x40, 0x9a, 0x6d, 0x86, 0x9b, 0x99, 0x6d, 0x86, 0x20, 0x09, 0x75, 0x5b}, //trama27
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x12, 0xf0, 0xf0,
					 0x0e, 0x0d, 0x0e, 0x00, 0xff, 0xef, 0x14, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x7f, 0x23,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0x9a, 0x6d}, //trama28
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xde, 0x9a, 0x6d}, //trama29
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x12, 0xf0, 0xf0,
					 0x10, 0x0d, 0x0e, 0x00, 0xff, 0xef, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x23,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x9b, 0x6d}, //trama30
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x04, 0xf0, 0xf1,
					 0x01, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x9b, 0x6d}, //trama31
					{0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x03, 0xf0, 0xf0,
					 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcb, 0x9b, 0x6d}, //trama32
					{0x00, 0x02, 0xb3, 0x9c, 0xdf, 0x1b, 0x00, 0x02, 0xb3, 0x9c, 0xae, 0xba, 0x00, 0x03, 0xf0, 0xf1,
					 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x9c, 0x6d},//trama 33

					{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x23, 0x8b, 0x46, 0xe9, 0xad, 0x08, 0x06, 0x00, 0x04, /*TRAMA a */
					 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x23, 0x8b, 0x46, 0xe9, 0xad, 0x94, 0xcc, 0x39, 0xcb,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0xcc, 0x39, 0xfe}, 

					{0x00, 0x23, 0x8b, 0x46, 0xe9, 0xad, 0x00, 0x1f, 0x45, 0x9d, 0x1e, 0xa2, 0x08, 0x16, 0x00, 0x01, /*TRAMA b */
					 0x08, 0x00, 0x06, 0x04, 0x00, 0x02, 0x00, 0x1f, 0x45, 0x9d, 0x1e, 0xa2, 0x94, 0xcc, 0x39, 0xfe,
					 0x00, 0x23, 0x8b, 0x46, 0xe9, 0xad, 0x94, 0xcc, 0x39, 0xcb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

					{0x00, 0x1f, 0x45, 0x9d, 0x1e, 0xa2, 0x00, 0x23, 0x8b, 0x46, 0xe9, 0xad, 0x08, 0x00, 0x46, 0x00, /* TRAMA c */
					 0x80, 0x42, 0x04, 0x55, 0x34, 0x11, 0x80, 0x11, 0x6b, 0xf0, 0x94, 0xcc, 0x39, 0xcb, 0x94, 0xcc,
					 0x67, 0x02, 0xaa, 0xbb, 0xcc, 0xdd, 0x04, 0x0c, 0x00, 0x35, 0x00, 0x2e, 0x85, 0x7c, 0xe2, 0x1a,
					 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x77, 0x77, 0x77, 0x03, 0x69,
					 0x73, 0x63, 0x05, 0x65, 0x73, 0x63, 0x6f, 0x6d, 0x03, 0x69, 0x70, 0x6e, 0x02, 0x6d, 0x78, 0x00,
					 0x00, 0x1c, 0x00, 0x01}};
    
    unsigned char T[][128] = {
        {
            0x00, 0x23, 0x8b, 0x46, 0xe9, 0xad, 0x00, 0x1f, 0x45, 0x9d, 0x1e, 0xa2, 0x08, 0x00, 0x45, 0x00,
            0x00, 0x6f, 0x90, 0x30, 0x40, 0x00, 0xfb, 0x11, 0x24, 0xe7, 0x94, 0xcc, 0x67, 0x02, 0x94, 0xcc,
            0x39, 0xcb, 0x00, 0x35, 0x04, 0x0c, 0x00, 0x5b, 0xe8, 0x60, 0xe2, 0x1a, 0x85, 0x80, 0x00, 0x01,
            0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x03, 0x77, 0x77, 0x77, 0x03, 0x69, 0x73, 0x63, 0x05, 0x65,
            0x73, 0x63, 0x6f, 0x6d, 0x03, 0x69, 0x70, 0x6e, 0x02, 0x6d, 0x78, 0x00, 0x00, 0x1c, 0x00, 0x01,
            0xc0, 0x14, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x0e, 0x10, 0x00, 0x21, 0x04, 0x64, 0x6e, 0x73,
            0x31, 0xc0, 0x1a, 0x03, 0x74, 0x69, 0x63, 0xc0, 0x1a, 0x77, 0xec, 0xdf, 0x29, 0x00, 0x00, 0x2a,
            0x30, 0x00, 0x00, 0x0e, 0x10, 0x00, 0x12, 0x75, 0x00, 0x00, 0x00, 0x2a, 0x30
        },
        {
            0x00, 0x1f, 0x45, 0x9d, 0x1e, 0xa2, 0x00, 0x23, 0x8b, 0x46, 0xe9, 0xad, 0x08, 0x00, 0x45, 0x00,
            0x00, 0x3c, 0x04, 0x57, 0x00, 0x00, 0x80, 0x00, 0x98, 0x25, 0x94, 0xcc, 0x39, 0xcb, 0x94, 0xcc,
            0x3a, 0xe1, 0x08, 0x00, 0x49, 0x5c, 0x03, 0x00, 0x01, 0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
            0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
            0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69
        },
        {
            0x02, 0xff, 0x53, 0xc3, 0xe9, 0xab, 0x00, 0xff, 0x66, 0x7f, 0xd4, 0x3c, 0x08, 0x00, 0x45, 0x00,
            0x00, 0x30, 0x2c, 0x00, 0x40, 0x00, 0x80, 0x06, 0x4b, 0x74, 0xc0, 0xa8, 0x01, 0x02, 0xc0, 0xa8,
            0x01, 0x01, 0x04, 0x03, 0x00, 0x15, 0x00, 0x3b, 0xcf, 0x44, 0x00, 0x00, 0x00, 0x00, 0x70, 0x20,
            0x20, 0x00, 0x0c, 0x34, 0x00, 0x00, 0x02, 0x04, 0x05, 0xb4, 0x01, 0x01, 0x04, 0x02
        },
        {0x45, 0x00, 0x01, 0x9c, 0xd7, 0xde, 0x40, 0x00, 0x80, 0x06, 0x88, 0x9d, 0xc0, 0xa8, 0x01, 0x43, 0x94, 0xcc, 0x3A, 0xdd}
    };
    unsigned short tot; //Almacenar valor del ToT
    unsigned char i, tipo, n_trama; //para iteraciones
	char opcion;
	unsigned char cabecera[64];

    do
	{
		printf("Ingresa la trama a analizar: ");
    	scanf("%hhu", &n_trama);
	} while (n_trama<1 || n_trama>4);

    for(i=0; i<=128; i++) {
        cabecera[i] = T[n_trama-1][i];
    }

    funcionCheksum(cabecera);

    printf("\t\t%c%cANALISIS DE TRAMAS%c%c \n", 176, 177, 177, 176);
    printf("\nBautista Coello Alexandra");
    printf("\nTaboada Montiel Enrique");
    // Imprimir toda la cabecera
    printf("\n");
    printf("La  trama es la siguiente: ");
    for (i = 0; i < 36; i++)
    {
        if(i==35){
            printf("%.2x", cabecera[i]);
        }else {
           printf("%.2x:", cabecera[i]); 
        }
    }
    
    printf("\n");
	printf("\t\t%c%cCABECERA ETHERNET%c%c \n", 176, 177, 177, 176);
    // Imprimir Mac destino
    printf("\nLa MAC destino es: \n");
    for (i = 0; i < 6; i++)
    {
        if(i==5){
    		printf("%.2x ", cabecera[i]);
		}else{
			printf("%.2x: ", cabecera[i]);
		}
    }

    // imprimir MAC origen
    printf("\nLa MAC origen es: \n");
    for (i = 6; i < 12; i++)
    {
    	if(i==11){
    		printf("%.2x ", cabecera[i]);
		}else{
			printf("%.2x: ", cabecera[i]);
		}
    }

    tot = (cabecera[12] << 8) | cabecera[13];
    
    if(tot <= 1500) {
        printf("\n\nEs un protocolo LLC");

		printf("\t\t%c%cCABECERA LLC%c%c \n", 176, 177, 177, 176);

		printf("\nTamaño (LLC): %hd bytes", tot);
        
        tipo = 'L';
        
        analizarLLC(cabecera);
    }
    else if(tot == 2048) {
        printf("\n\nEs un protocolo IP\n");
        tipo = 'I';

		printf("\t\t%c%cANALIZADOR CHECKSUM%c%c \n", 176, 177, 177, 176);

		funcionCheksum(cabecera+14);
    }
    else if(tot == 2054) {
        printf("\n\nEs un protocolo ARP\n");
        printf("\t\t%c%cANALISIS ARP%c%c \n", 176, 177, 177, 176);
        tipo = 'A';

		analizarARP(cabecera);

    }else{
        printf("\n\nEs OTRO protocolo.\n");
    }


    return 0;
}
