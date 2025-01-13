#include <stdio.h>

int main() {
    unsigned char t[128] = {0x00,  0x02,  0xb3,  0x9c,  0xae,  0xba,  0x00,  0x02,  0xb3,  0x9c,  0xdf,  0x1b,  0x00,  0x03,  0xf0,  0xf0,
                                0x7f};

    char i;

    printf("Analizador de trama: ");
    
    if((t[12] << 8 | t[13]) == 0x0800) {
        printf("Es un protocolo IP\n");

        char IHL = t[14] & 0x0F;

        // 1
        if(t[23] == 0x01) {
            printf("Es un protocolo ICMP\n");

            printf("Opciones: ");
            for (i = 0; i < (IHL *4 -20); i++)
            {
               printf("Opcion %d: %.2x", i+1, t[34+i]);
            }
            
        }
        // 2
        else if(t[23] == 17) {
            printf("Es un protocolo UDP\n");

            if(IHL > 5) {
                unsigned char offset = (t[20] & 0x1F) | t[21];

                printf("Offset: %d\n", offset);
            }
        }
    }

    // 3
    else if((t[12] << 8 | t[13]) <= 1500) {
        printf("Es un protocolo LLC\n");

        char tipo = t[16] & 3;

        if(tipo ==0 || tipo ==2) {
            char nr = t[17] & 0x7F;
            char ns = t[16] & 0x7F;

            printf("N(r): %d\n, N(s): %d", nr, ns);
        }
    }

    // 4
    else if((t[12] << 8 | t[13]) == 0x0806) {
        printf("Es un protocolo ARP\n");

        if((t[14] << 8 | t[15]) == 0x0001) {
            printf("ATM\n");

            if(t[20] << 8 | t[21] == 0x0002) {
                printf("ARP Reply\n");

                printf("Direccion MAC destino: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", t[32], t[33], t[34], t[35], t[36], t[37]);
            }
        }
    }

    // 5
    else if((t[12] << 8 | t[13]) == 0x0800) {
        printf("Es un protocolo IP\n");

        char IHL = t[14] & 0x0F;

        if(t[23] == 6) {
            printf("Es un protocolo TCP\n");

            if(t[25] == 0x20) {
                printf("Consulta urgente\n");

                printf("Numero de secuencia: %d\n", (t[38] << 24) | (t[39] << 16) | (t[40] << 8) | t[41]);
            }

        }
    }

    // 6
    else if((t[12] << 8 | t[13]) <= 1500) {
        printf("Es un protocolo LLC\n");

        char tipo = t[16] & 3;

        if(tipo == 1) {

            const char *ss[] = {"RR", "RNR", "REJ", "SREJ"};

            if(ss[(t[16]>>2)&3] == "REJ") {
                
                if(t[15] & 1) {
                    printf("Es REJ de respuesta\n");

                    printf("N(r): %d\n", (t[17]>>1)&0x7F);
                }
            }
        }

    }

    return 0;
}
