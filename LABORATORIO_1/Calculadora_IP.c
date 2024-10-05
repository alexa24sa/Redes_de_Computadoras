#include <stdio.h>


int proceso_anding(unsigned short int ip[4], unsigned short int mascara_red[4],  unsigned short int resultado[4]){
    //unsigned short int mascara_red[4] = {255, 0, 0, 0};
    // se mandan todos los valores ya que dependiendo de su clase, sus bits de host y red cambian
    for (int i= 0; i<4, i++){
        resultado[i] = ip[i] & mascara_red[i];
    }
    
    return resultado; // linea de codigo que no es completamente necesaria
} //funcion para obtener la IP madre o IP red

int main(){
    unsigned short int ip[4];
    unsigned short int aux_ip[4];
    unsigned short int mascara_red[4];
    unsigned short char clase;
    //el usuario ingresa el valor de su ip:
    printf("\t\t\tCALCULADORA DE IP:");
    printf("Ingrese el valor de su IP por espacios (considerando que tiene 4):\n");
    for(int i=0; i < 4; i++){
        printf("Ingrese el valor de su digito %d: ", i+1);
        scanf("%d", &ip[i]);
    }
    
    
    //--------- ALGORITMO DE SELECCIÓN DE LAS CLASES PARA CADA IP--------------
    //proceso para determinar la clase de la IP
    if(ip[0]&&128){ // si la condicion resulta verdadera
        //decimos que no es de clase A ya que la clase A no tiene el MSB encendido, siempre es 0
        if(ip[0]&&64){ //si la condicion resulta verdadera
        //decimos que no es de clase B ya que los primeros dos bits de la clase B es 10
            if(ip[0]&&32){ // si la condicion resulta verdadera
            //decimos que no es de clase C ya que los primeros tres bits de la clase C es 110
                if(ip[0]&&16){ // si la condicion resulta verdadera
                //decimos directamente que es de clase E ya que sus bits son 1111 
                    printf("Es clase E\n");
                    clase = 'E';
                }else{ // si no es verdadera, únicamente nos queda decir que es de clase D
                //ya que en este caso solo tendría los bits 1110 que corresponde a la clase D
                    printf("Es clase D\n");
                    clase = 'D';
                }
                
            }else{
                printf("Es clase C\n");
                clase = 'C';
            }
        }else{
            printf("Es clase B\n");
            clase = 'B';
        }
    }else{
        printf("Es clase A\n");
        clase = 'A';
        
    }
    
    
    
    
    
    
    return 0;
    
}
