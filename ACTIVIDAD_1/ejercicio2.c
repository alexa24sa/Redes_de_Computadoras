/*2. Escribe una función en C que reciba un número y que imprima si es un número par o impar*/
#include <stdio.h>

int funcion_par_impar(int num){
    int flag;
    if((num>>0)&1){
        flag++;
    }
    return flag;
}

int main(){
    int numero, flag;
    printf("Ingrese su n%cmero: ", 163);
    scanf("%d", &numero);
    
    flag = funcion_par_impar(numero);
    
    if(flag == 1){
        printf("\nSu n%cmero es impar", 163);
    } else {
        printf("\nSu n%cmero es par", 163);
    }
    
    return 0;
}
