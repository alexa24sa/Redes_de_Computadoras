/*1. Escribe una función en C que reciba un caracter y devuelva la cantidad de bits encendidos*/
#include <stdio.h>

// Función para contar los bits encendidos en un carácter
int contarBitsEncendidos(char variable) {
    int flag = 0; // Contador para los bits encendidos
    for (int i = 0; i < 8; i++) { // Un char tiene 8 bits
        if ((variable >> i) & 1) { // Desplazamiento y verificación de cada bit
            flag++;
            
            /*
            EJEMPLO DEL FUNCIONAMIENTO, SI SE INTRODUJERA: a = 01100001
            Entonces al ir recorriendo to right dependiendo del valor de 0
            nos quedaría algo como:
            i = 0:
            01100001
            and    1 
           ----------
            00000001
           LO QUE NOS DARÍA TRUE en la sentencia dentro del IF y añade +1 
            a la flag
           
            i = 1:
            0011000
            and   1 
           ----------
            0000000
            
            LO QUE SE MANTENDRÍA DE ESTA FORMA HASTA TERMINAR DE CONTAR LOS BITS
            
            */
        }
    }
    return flag;
}

int main() {
    char variable;
    printf("Coloca el caracter: ");
    scanf("%c", &variable);
    
    // Llamada a la función y muestra del resultado
    int bitsEncendidos = contarBitsEncendidos(variable);
    //se coloca la función que imprime el caracter en hexadecimal para
    //hacer la comprobación más rápida del reesultado final
    printf("\nTu caracter en hexadecimal es: 0x%x", variable);
    printf("\nLa cantidad de bits encendidos es: %d\n", bitsEncendidos);

    return 0;
}
