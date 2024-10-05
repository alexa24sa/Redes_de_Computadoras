#include <stdio.h>


int proceso_anding(unsigned short int ip[4], unsigned short int resultado[4]){
    unsigned short int mascara_red[4] = {255, 0, 0, 0};
    for (int i= 0; i<3, i++){
        resultado[i] = ip[i] & mascara_red[i];
    }
    
    return resultado; // linea de codigo que no es completamente necesaria
} //funcion para obtener la IP madre o IP red

int main(){
    unsigned short int ip[3];
    unsigned short int mascara_red[3];
     
    //proceso para determinar la clase de la IP
    
    
    
}
