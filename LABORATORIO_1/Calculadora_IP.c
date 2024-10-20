#include <stdio.h>


void proceso_anding(unsigned char ip[4], unsigned char mascara_red[4],  unsigned char resultado[4]){
    //unsigned short int mascara_red[4] = {255, 0, 0, 0};
    // se mandan todos los valores ya que dependiendo de su clase, sus bits de host y red cambian
    for (unsigned char i= 0; i<4; i++){
        resultado[i] = ip[i] & mascara_red[i];
    }
    
    //return resultado; // linea de codigo que no es completamente necesaria
} //funcion para obtener la IP madre o IP red

void proceso_broadcast2(unsigned char ip[4], unsigned char mascara_red[4], unsigned char resultado[4]){
    
    //negamos la mascara_red:
    for(unsigned char i = 0; i<4; i++){
        mascara_red[i] = ~mascara_red[i];
    }
    
    //aplicamos una xor en todas las posiciones de la IP con la mascara_red
    for(unsigned char i = 0; i < 4; i++){
        resultado[i] = ip[i] | mascara_red[i];
    }
    
}

void tipo_de_ip(unsigned char ip[4], unsigned char mascara_red[4]){
    unsigned char ip_red[4], ip_broadcast[4];
    
    //Obtencion de la IP de RED
    ip_red[0]=ip[0]&mascara_red[0];
    ip_red[1]=ip[1]&mascara_red[1];
    ip_red[2]=ip[2]&mascara_red[2];
    ip_red[3]=ip[3]&mascara_red[3];

    //Obtencion de la IP de Broadcast
    ip_broadcast[0]=ip[0]|(~mascara_red[0]);
    ip_broadcast[1]=ip[1]|(~mascara_red[1]);
    ip_broadcast[2]=ip[2]|(~mascara_red[2]);
    ip_broadcast[3]=ip[3]|(~mascara_red[3]);

    if((ip[0]^ip_red[0])||(ip[1]^ip_red[1])||(ip[2]^ip_red[2])||(ip[3]^ip_red[3])){
        /*Supongamos el caso de que nuestra ip fuera la siguiente = 1.0.0.0 
            entonces aquí lo que se hace es comparar con la máscara de RED, de entrada sabemos que la IP anterior 
            ese de clase A ya que el primer byte está dentro del rango de 0 a 127, por ende es que es de clase A 
            Entonces decimos que su máscara de RED es la siguiente: 
            Máscara de Red = 255.0.0.0 (en la máscara de Red se encienden todos los bits de Red según la clase
            como sabemos que en las de clase A la correspondencia es R.H.H.H es por eso que toma esos valores
            ¿Qué pasaría después?, se hará la comparación XOR bit a bit con la IP  con la IP de RED:
            en las xor para que exista un 1 sabemos que debe haber al menos un 1 encendido, si ambos bits son 1 o 0 (los dos)
            entonces la salida será cero:
            Recordatorio: Un byte corresponde a 8 bits que está compuesto por las siguientes potencias:
            128 64 32 16 8 4 2 1
            
            
            OPERACIÓN:
                  1.0.0.0                                                00000001.0.0.0
            XOR 255.0.0.0      RECORDEMOS QUE SON OPERACIONES CON BYTES: 11111111.0.0.0
            -------------                                               ´--------------
                254.0.0.0                                                11111110.0.0.0
            pos: 0  1 2 3 ----- (posiciones de los bits)
            Posteriormente se hace la OR con cada resultado de cada bit
            Entonces tenemos que:
               254            RECORDAR QUE SON OPERACIONES CON BYTES:   11111110
            OR   0                                                      00000000 
            OR   0                                                      00000000
            OR   0                                                      00000000
            OR   0                                                      00000000
            OR   0                                                      00000000
            OR   0                                                      00000000
            OR   0                                                      00000000    
            ------                                                      --------
               255                                                      11111110
            
            PERO COMO ESTAMOS TRABAJANDO CON UN IF, VEREMOS QUE NOS VA DAR QUE EL RESULTADO DE TODA ESA OPERACIÓN ES verdadera
            ENTONCES, en ese caso puede que nuestra IP no sea de RED porque para cualquier valor del primer byte puede dar VERDADERO,
            (excepto por el caso de la IP 255.0.0.0 que corresponderia a la IP de RED)
            (254.0.0.0) XOR (255.0.0.0) = 1.0.0.0   Y la OPERACIÓN BIT A BIT ---> (1) OR (0) OR (0) OR (0) = 1 
            En el caso que todo sea FALSO, ES PORQUE EXACTAMENTE TODOS los bits de inicio, coinciden con la IP de RED
            POR ENDE en el ELSE de IF se imprime que corresponde a una IP de RED
            
            
            Y es por eso que para la ip de Broadcast no importa si algunos de los bits de RED estan encendidos, nosotros estamos 
            interesados en saber que los bits de HOST están encendidos, por esa razón es que se coloca el IF DE BROADCAST dentro
            del IF de RED y de la misma manera, en el caso de que todo sea falso, decimos que se trata de una IP de BROADCAST
            
            )
        
        */
        
        if((ip[0]^ip_broadcast[0])||(ip[1]^ip_broadcast[1])||(ip[2]^ip_broadcast[2])||(ip[3]^ip_broadcast[3])){
             // esta condicion será verdadera si existe una combinación de bits de RED y HOST encendidos:
            printf(" Es una IP de tipo HOST.\n");
        }else{
            
            printf(" Es una IP de tipo BROADCAST.\n");
        }
        
        
    }else{
        printf(" Es una IP de tipo RED.\n");
    }
}


void proceso_broadcast(unsigned char clase, unsigned char ip[4], unsigned char resultado[4]){
    //unsigned short int mascara_red[4] = {255, 0, 0, 0};
    // se mandan todos los valores ya que dependiendo de su clase, sus bits de host y red cambian
    unsigned char aux = 0;
    
    switch (clase){
        case 'A':
            for(unsigned char i=1; i<4; i++){
                aux = 255 - ip[i];
                resultado[i] = ip[i] + aux;
            } 
            
            
            break;
        case 'B':
            for(unsigned char i=2; i<4; i++){
                aux = 255 - ip[i];
                resultado[i] = ip[i] + aux;
            } 
            
            break;
        case 'C':
            for(unsigned char i=3; i<4; i++){
                aux = 255 - ip[i];
                resultado[i] = ip[i] + aux;
            } 
            
            break;
        default:
            printf("\nLos par%cmetros no entran dentro de ninguno de los casos, error.\n", 160);
        
        
    }

} 


int main(){
    unsigned char ip[4];
    unsigned char mascara_red[4];
    unsigned char aux_ip[4];
    unsigned char clase;
    //el usuario ingresa el valor de su ip:
    printf("\t\t\tCALCULADORA DE IP:\n");
    printf("Ingrese el valor de su IP por espacios (considerando que tiene 4):\n");
    for(int i=0; i < 4; i++){
        printf("Ingrese el valor de su digito %d: ", i+1);
        scanf("%hhu", &ip[i]);
    }
    
    printf("Su IP es la siguiente: \n");
    for(int i = 0; i< 4; i++){
        if (i == 3) {
            printf("%hhu", ip[i]); // Sin punto después del último número
        } else {
            printf("%hhu.", ip[i]);
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
            /*if(ip[1]==255){
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
            }*/
            
        
        
        
        
        
            //Para obtener su IP de red:
            // Asignar valores a mascara_red
            mascara_red[0] = 255;
            mascara_red[1] = 0;
            mascara_red[2] = 0;
            mascara_red[3] = 0;
            tipo_de_ip(ip, mascara_red);
            
            proceso_anding(ip, mascara_red, aux_ip);
            
            
            printf("Su IP madre o IP de Red es: \n");
            for(unsigned char i=0; i < 4; i++){
                if (i == 3) {
                     printf("%hhu", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%hhu.", aux_ip[i]);
                }
            }
            
            
            //Para obtener la IP de broadcast:
            /*for (int i = 0; i < 4; i++){
                mascara_red[i] = ~mascara_red[i];
            }*/
            proceso_broadcast2(ip, mascara_red, aux_ip);
            printf("\nSu IP de broadcast es: \n");
            for(unsigned char i =0; i < 4; i++){
                if (i == 3) {
                     printf("%hhu", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%hhu.", aux_ip[i]);
                }
            }
            
            break;
            
            
        case 'B':
            // ------PARA CONOCER EL TIPO DE MI IP:-------
            printf("El tipo de su IP es: ");
            /*if(ip[2]==255){
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
            }*/
        
        
        
        
            //Para obtener su IP de red:
            // Asignar valores a mascara_red
            mascara_red[0] = 255;
            mascara_red[1] = 255;
            mascara_red[2] = 0;
            mascara_red[3] = 0;
            tipo_de_ip(ip, mascara_red);
            
            proceso_anding(ip, mascara_red, aux_ip);
            printf("Su IP madre o IP de Red es: \n");
            for(unsigned char i=0; i < 4; i++){
                if (i == 3) {
                     printf("%hhu", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%hhu.", aux_ip[i]);
                }
            }
            
            
            //Para obtener la IP de broadcast:
            /*for (int i = 0; i < 4; i++){
                mascara_red[i] = ~mascara_red[i];
            }*/
            
            proceso_broadcast2(ip, mascara_red, aux_ip);
            printf("\nSu IP de broadcast es: \n");
            for(unsigned char i=0; i < 4; i++){
                if (i == 3) {
                     printf("%hhu", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%hhu.", aux_ip[i]);
                }
            }
            
            break;
            
            
            
        case 'C':
            // ------PARA CONOCER EL TIPO DE MI IP:-------
            printf("El tipo de su IP es: ");
            /*if(ip[3]==255){
                printf("Es de tipo broadcast.\n");
            }else{
                if(ip[3]==0){
                    printf("Es de tipo Red.\n");  
                }else{
                    printf("Es de tipo Host.\n");
                }
            }*/
        
        
            //Para obtener su IP de red:
            // Asignar valores a mascara_red
            mascara_red[0] = 255;
            mascara_red[1] = 255;
            mascara_red[2] = 255;
            mascara_red[3] = 0;
            tipo_de_ip(ip, mascara_red);
            
            proceso_anding(ip, mascara_red, aux_ip);
            printf("Su IP madre o IP de Red es: \n");
            for(unsigned char i =0; i < 4; i++){
                if (i == 3) {
                     printf("%hhu", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%hhu.", aux_ip[i]);
                }
            }
            
            
            //Para obtener la IP de broadcast:
            /*for (int i = 0; i < 4; i++){
                mascara_red[i] = ~mascara_red[i];
            }*/
            proceso_broadcast2(ip, mascara_red, aux_ip);
            printf("\nSu IP de broadcast es: \n");
            for(unsigned char i =0; i < 4; i++){
                if (i == 3) {
                     printf("%hhu", aux_ip[i]); // Sin punto después del último número
                } else {
                     printf("%hhu.", aux_ip[i]);
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
