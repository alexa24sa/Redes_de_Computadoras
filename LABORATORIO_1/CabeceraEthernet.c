#include <stdio.h>

int main() {

    unsigned char cabecera [14];
    short temp; //Almacenar valores temporales
    char i; //para iteraciones
    unsigned short int TOT;

    printf("Ingresa la cabecera Ethernet: \n");

    for (i = 0; i < 14; i++)
    {
        // Guardar todos los valores en hexadecimal
        printf("Ingresa el campo %d: ", i+1);
        scanf("%hx", &temp);
        cabecera[i] = temp & 255;
        fflush(stdin);
    }

    // Imprimir toda la cabecer
    printf("\n");
    printf("La  cabecera es la siguiente: ");
    for (i = 0; i < 14; i++)
    {
        printf("%x, ", cabecera[i]);
    }

    // Imprimir Mac destino
    printf("\nLa MAC destino es: \n");
    for (i = 0; i < 6; i++)
    {
        printf("%x, ", cabecera[i]);
    }

    // imprimir MAC origen
    printf("\nLa MAC origen es: \n");
    for (i = 6; i < 12; i++)
    {
        printf("%x, ", cabecera[i]);
    }

    TOT = (cabecera[12] << 8) | cabecera[13];

    if(TOT <= 1500) {
        printf("\n\nEs un protocolo LLC");
    }
    else if(TOT == 2048) {
        printf("\n\nEs un protocolo IP");
    }
    else if(TOT == 2054) {
        printf("\n\nEs un protocolo ARP");
    }

    return 0;
}