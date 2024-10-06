#include <stdio.h>


void proceso_anding(unsigned short int ip[4], unsigned short int mascara_red[4],  unsigned short int resultado[4]){
    //unsigned short int mascara_red[4] = {255, 0, 0, 0};
    // se mandan todos los valores ya que dependiendo de su clase, sus bits de host y red cambian
    for (int i= 0; i<4; i++){
        resultado[i] = ip[i] & mascara_red[i];
    }
    
    //return resultado; // linea de codigo que no es completamente necesaria
} //funcion para obtener la IP madre o IP red

void proceso_broadcast(unsigned char clase, unsigned short int ip[4], unsigned short int resultado[4]){
    //unsigned short int mascara_red[4] = {255, 0, 0, 0};
    // se mandan todos los valores ya que dependiendo de su clase, sus bits de host y red cambian
    unsigned short int aux = 0;
    
    switch (clase){
        case 'A':
            for(int i=1; i<4; i++){
                aux = 255 - ip[i];
                resultado[i] = ip[i] + aux;
            } 
            
            
            break;
        case 'B':
            for(int i=2; i<4; i++){
                aux = 255 - ip[i];
                resultado[i] = ip[i] + aux;
            } 
            
            break;
        case 'C':
            for(int i=3; i<4; i++){
                aux = 255 - ip[i];
                resultado[i] = ip[i] + aux;
            } 
            
            break;
        default:
            printf("\nLos par%cmetros no entran dentro de ninguno de los casos, error.\n", 160);
        
        
    }

} 


int main(){
    unsigned short int ip[4];
    unsigned short int mascara_red[4];
    unsigned short int aux_ip[4];
    unsigned char clase;
    //el usuario ingresa el valor de su ip:
    printf("\t\t\tCALCULADORA DE IP:\n");
    printf("Ingrese el valor de su IP por espacios (considerando que tiene 4):\n");
    for(int i=0; i < 4; i++){
        printf("Ingrese el valor de su digito %d: ", i+1);
        scanf("%hd", &ip[i]);
    }
    
    printf("Su IP es la siguiente: \n");
    for(int i = 0; i< 4; i++){
        if (i == 3) {
            printf("%d", ip[i]); // Sin punto después del último número
        } else {
            printf("%d.", ip[i]);
        }
    }
    
    printf("\n");
    
    //--------- ALGORITMO DE SELECCIÓN DE LAS CLASES PARA CADA IP--------------
    //proceso para determinar la clase de la IP
    printf("\nClase de IP: ");
    if(ip[0]&256){
        printf("Fuera de los  límites establecidos, se cierra el programa.\n");
        return 0;
    } else {
        if(ip[0]&128){ // si la condicion resulta verdadera
            //decimos que no es de clase A ya que la clase A no tiene el MSB encendido, siempre es 0
            if(ip[0]&64){ //si la condicion resulta verdadera
            //decimos que no es de clase B ya que los primeros dos bits de la clase B es 10
                if(ip[0]&32){ // si la condicion resulta verdadera
                //decimos que no es de clase C ya que los primeros tres bits de la clase C es 110
                    if(ip[0]&16){ // si la condicion resulta verdadera
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
    }
    
    
    
    
    switch(clase)
    {
        case 'A':
            // ------PARA CONOCER EL TIPO DE MI IP:-------
            printf("El tipo de su IP es: ");
            if(ip[1]==255){
                if(ip[2]==255){
                    if(ip[3]==255){
                        printf("Es de tipo broadcast.\n");
                    }else{
                        printf("Es de tipo Host.\n");
                    }
                }else{
                   printf("Es de tipo Host.\n"); 
                }
            }else{
                if(ip[1]==0){
                   if(ip[2]==0){
                       if(ip[3]==0){
                         printf("Es de tipo Red.\n"); 
                       }
                   } else {
                       printf("Es de tipo Host.\n");
                   } 
                }else{
                    printf("Es de tipo Host.\n");
                }
            }
        
        
        
        
        
            //Para obtener su IP de red:
            // Asignar valores a mascara_red
            mascara_red[0] = 255;
            mascara_red[1] = 0;
            mascara_red[2] = 0;
            mascara_red[3] = 0;
            proceso_anding(ip, mascara_red, aux_ip);
            
            
            printf("Su IP madre o IP de Red es: \n");
            for(int i =0; i < 4; i++){
                if (i == 3) {
                     printf("%d", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%d.", aux_ip[i]);
                }
            }
            
            
            //Para obtener la IP de broadcast:
            /*for (int i = 0; i < 4; i++){
                mascara_red[i] = ~mascara_red[i];
            }*/
            proceso_broadcast(clase, ip, aux_ip);
            printf("\nSu IP de broadcast es: \n");
            for(int i =0; i < 4; i++){
                if (i == 3) {
                     printf("%d", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%d.", aux_ip[i]);
                }
            }
            
            break;
            
            
        case 'B':
            // ------PARA CONOCER EL TIPO DE MI IP:-------
            printf("El tipo de su IP es: ");
            if(ip[2]==255){
                if(ip[3]==255){
                    printf("Es de tipo broadcast.\n");
                }else{
                   printf("Es de tipo Host.\n"); 
                }
            }else{
                if(ip[2]==0){
                   if(ip[3]==0){
                       printf("Es de tipo Red.\n"); 
                   }else {
                       printf("Es de tipo Host.\n");
                   } 
                }else{
                    printf("Es de tipo Host.\n");
                }
            }
        
        
        
        
            //Para obtener su IP de red:
            // Asignar valores a mascara_red
            mascara_red[0] = 255;
            mascara_red[1] = 255;
            mascara_red[2] = 0;
            mascara_red[3] = 0;
            proceso_anding(ip, mascara_red, aux_ip);
            printf("Su IP madre o IP de Red es: \n");
            for(int i =0; i < 4; i++){
                if (i == 3) {
                     printf("%d", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%d.", aux_ip[i]);
                }
            }
            
            
            //Para obtener la IP de broadcast:
            /*for (int i = 0; i < 4; i++){
                mascara_red[i] = ~mascara_red[i];
            }*/
            proceso_broadcast(clase, ip, aux_ip);
            printf("\nSu IP de broadcast es: \n");
            for(int i =0; i < 4; i++){
                if (i == 3) {
                     printf("%d", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%d.", aux_ip[i]);
                }
            }
            
            break;
            
            
            
        case 'C':
            // ------PARA CONOCER EL TIPO DE MI IP:-------
            printf("El tipo de su IP es: ");
            if(ip[3]==255){
                printf("Es de tipo broadcast.\n");
            }else{
                if(ip[3]==0){
                    printf("Es de tipo Red.\n");  
                }else{
                    printf("Es de tipo Host.\n");
                }
            }
        
        
            //Para obtener su IP de red:
            // Asignar valores a mascara_red
            mascara_red[0] = 255;
            mascara_red[1] = 255;
            mascara_red[2] = 255;
            mascara_red[3] = 0;
            proceso_anding(ip, mascara_red, aux_ip);
            printf("Su IP madre o IP de Red es: \n");
            for(int i =0; i < 4; i++){
                if (i == 3) {
                     printf("%d", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%d.", aux_ip[i]);
                }
            }
            
            
            //Para obtener la IP de broadcast:
            /*for (int i = 0; i < 4; i++){
                mascara_red[i] = ~mascara_red[i];
            }*/
            proceso_broadcast(clase, ip, aux_ip);
            printf("\nSu IP de broadcast es: \n");
            for(int i =0; i < 4; i++){
                if (i == 3) {
                     printf("%d", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%d.", aux_ip[i]);
                }
            }
            
            break;
            
            
            
        case 'E' || 'D':
            printf("Las IP de clase E y D estan reservadas para otras actividades.\n");
            printf("Por lo tanto no se pueden proporcionar los datos de IP Red y broadcast.\n");
            break;
            
            
            
        default:
        
            printf("Rangos fuera de los establecidos.\n");
            break;
        
    }
    return 0;
    
}
