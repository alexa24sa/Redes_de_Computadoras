#include <stdio.h>

void funcionCheksum(unsigned char *message, unsigned short * checksum) {
    unsigned short sumas[5];
    unsigned short sum=0, aux;
    
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
        printf("Suma: %x\n", sum);
        printf("Aux: %x\n", aux);
        checksum[j] = sum;
        j--;
    }
    
    
}

int main () {
    unsigned char message[20] = {0x45, 0x00, 0x01, 0x9c, 0xd7, 0xde, 0x40, 0x00, 0x80, 0x06, 0x88, 0x9d, 0xc0, 0xa8, 0x01, 0x43, 0x94, 0xcc, 0x3A, 0xdd};
    unsigned short checksum[4];
    funcionCheksum(message, checksum);
    printf("Checksum: %x %x %x %x\n", checksum[0], checksum[1], checksum[2], checksum[3]);
    return 0;
}
