/***************************************************************************************
 * 
 * t1.c: multiplicacion de numeros de gran longitud  
 *
 * Programmer: Gabriel Valdez
 *
 * Santiago de Chile, 21/10/2024
 *
 **************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void mess(char *m){
    printf("\nUsage: %s -G -D MEM < data.txt \n", m);
    printf("G = {V: 1 digito por byte , S: 2 digito por byte , D: base de 256}\n");
    printf("D = {V: mostrar respuesta, S: solo tiempo}\n");
    printf("MEM = límite de memoria en bytes\n");
}

void empaquetar_digitos(unsigned char *numeros,unsigned int longitud) {
    unsigned int i, digito1, digito2;

    for (i = 0; i < longitud / 2; i = i + 1) {
        scanf("%1d", &digito1);
        scanf("%1d", &digito2);

        numeros[i] = (digito1 << 4) | digito2;
    }

    if (longitud % 2 != 0) {
        scanf("%1d", &digito1);
        numeros[i] = digito1 << 4;
    }
}

int obtener_posicion_significativo(unsigned char byte, unsigned int posicion) {
    if (posicion == 0) {
        return (byte >> 4) & 0x0F;
    } else {
        return byte & 0x0F;
    }
}

unsigned char buscar_posicion_digito(unsigned char *num, unsigned int len_digitos,unsigned int posicion) {

    unsigned int indice,byte_indice,digito_indice;

    indice = len_digitos - 1 - posicion; 
    byte_indice = indice / 2;
    digito_indice = indice % 2;

    return obtener_posicion_significativo(num[byte_indice], digito_indice);
}

void colocar_digito(unsigned char *num,unsigned int len_digitos,unsigned int posicion, unsigned char digit) {

    unsigned int indice,byte_indice,digito_indice;

    indice = len_digitos - 1 - posicion;
    byte_indice = indice / 2;
    digito_indice = indice % 2;

    if (digito_indice == 0) {
        
        num[byte_indice] = (num[byte_indice] & 0x0F) | ((digit & 0x0F) << 4);
    } else {
        
        num[byte_indice] = (num[byte_indice] & 0xF0) | (digit & 0x0F);
    }
}

void imprimir_numero_en_bits(unsigned char *numeros, unsigned int len_digitos) {
    int i,leading_zero; 
    unsigned char digito;
    
    leading_zero = 1;

    printf("El resultado es: ");
    for (i = len_digitos - 1; i >= 0; i = i - 1) {
        digito = buscar_posicion_digito(numeros, len_digitos, i);
        if (leading_zero) {
            if (digito != 0) {
                leading_zero = 0;
                printf("%d", digito);
            }
        } else {
            printf("%d", digito);
        }
    }
    if (leading_zero) {
        printf("0");
    }
    printf("\n");
}

 void llenar_areglo( unsigned char *numeros,unsigned int longitud) {
    int i;
    
    for (i = 0; i < longitud; i = i + 1) {
        if (scanf("%s", &numeros[i]) != 1) {
            printf("Error al leer el número %d.\n", i + 1);
            return ;  
        }
    }
}

void multiply2digbyte(unsigned char *num1, unsigned int len1_digitos, unsigned char *num2, unsigned int len2_digitos, unsigned char *resultado) {
    unsigned int lenres_digitos, lenres_bytes, carry, sum, posicion;
    unsigned char digit1, digit2, res_digit;
    int i, j;

    lenres_digitos = len1_digitos + len2_digitos;
    lenres_bytes = (lenres_digitos + 1) / 2;

    for (i = 0; i < lenres_bytes; i = i + 1) {
        resultado[i] = 0;
    }

    for (i = 0; i < len1_digitos; i = i + 1) {

        digit1 = buscar_posicion_digito(num1, len1_digitos, i);
        carry = 0;

        for (j = 0; j < len2_digitos; j = j + 1) {

            digit2 = buscar_posicion_digito(num2, len2_digitos, j);
            posicion = i + j;
            sum = digit1 * digit2 + buscar_posicion_digito(resultado, lenres_digitos, posicion) + carry;
            res_digit = sum % 10;
            carry = sum / 10;
            colocar_digito(resultado, lenres_digitos, posicion, res_digit);

        }
        posicion = i + len2_digitos;

        while (carry > 0) {

            sum = buscar_posicion_digito(resultado, lenres_digitos, posicion) + carry;
            res_digit = sum % 10;
            carry = sum / 10;
            colocar_digito(resultado, lenres_digitos, posicion, res_digit);
            posicion = posicion + 1;

        }
    }
}



void multiply1digbyte(unsigned char *num1, unsigned char *num2, unsigned char *resultado1digbyte,unsigned int len1,unsigned int len2) {

    unsigned int digit1,digit2,product;
    int i,j;

    for (i = 0; i < len1 + len2+1; i = i + 1) {
        resultado1digbyte[i] = 0;
    }

    for ( i = len1 - 1; i >= 0; i = i - 1) {
        for ( j = len2 - 1; j >= 0; j = j - 1) {

            digit1 = num1[i] - '0'; 
            digit2 = num2[j] - '0'; 

            product = digit1 * digit2 + resultado1digbyte[i + j + 1]; 
            resultado1digbyte[i + j + 1] = product % 10; 
            resultado1digbyte[i + j] = resultado1digbyte[i + j] + product / 10;

        }
    }
}

void convertir_a_base256(unsigned char **num_base256, unsigned int *len_base256, unsigned char *numero_decimal, unsigned int len_decimal, double *required_memory256) {
    unsigned int len_resultado, capacidad, carry, valor;
    unsigned char *resultado;
    int indice_decimal, i;

    capacidad = len_decimal * 2 / 3 + 1;
    resultado = (unsigned char *)calloc(capacidad, sizeof(unsigned char));

    len_resultado = 0;

    for (indice_decimal = 0; indice_decimal < len_decimal; indice_decimal = indice_decimal + 1) {
        carry = numero_decimal[indice_decimal] - '0';

        for (i = 0; i < len_resultado; i = i + 1) {

            valor = resultado[i] * 10 + carry;
            resultado[i] = valor % 256;
            carry = valor / 256;

        }

        while (carry > 0) {

            resultado[len_resultado++] = carry % 256;
            carry = carry / 256;

        }
    }

    *num_base256 = (unsigned char *)calloc(len_resultado, sizeof(unsigned char));
    *required_memory256 = *required_memory256 + len_resultado * sizeof(unsigned char);

    for (i = 0; i < len_resultado; i = i + 1) {
        (*num_base256)[i] = resultado[len_resultado - i - 1];
    }

    *len_base256 = len_resultado;
    free(resultado);
}


void multiply_base256(unsigned char *num1, unsigned int len1, unsigned char *num2, unsigned int len2, unsigned char **resultado, unsigned int *len_res, double *required_memory256) {
    unsigned int lenres, carry, indiceaux, product;
    unsigned char *temp;
    int i, j, indice;

    lenres = len1 + len2;
    temp = (unsigned char *)calloc(lenres, sizeof(unsigned char));

    for (i = 0; i < len1; i = i + 1) {
        carry = 0;

        for (j = 0; j < len2; j = j + 1) {
            indiceaux = i + j;
            product = (unsigned int)num1[len1 - 1 - i] * (unsigned int)num2[len2 - 1 - j] + temp[indiceaux] + carry;
            temp[indiceaux] = (unsigned char)(product % 256);
            carry = product / 256;
        }

        temp[i + len2] = temp[i + len2] + carry;
    }

    indice = lenres;

    while (indice > 0 && temp[indice - 1] == 0) {
        indice = indice - 1;
    }

    *len_res = indice;
    *resultado = (unsigned char *)calloc(*len_res, sizeof(unsigned char));
    *required_memory256 = *required_memory256 + (*len_res) * sizeof(unsigned char);

    for (i = 0; i < *len_res; i = i + 1) {
        (*resultado)[i] = temp[*len_res - i - 1];
    }

    free(temp);
}

void imprimir_numero_decimal(unsigned char *num_base256, unsigned int len_base256) {

    int capacidad, len_resultado, resto, valor; 
    unsigned char *resultado, *temp;
    int len_temp, i, indice;

    capacidad = len_base256 * 3 + 1;
    resultado = (unsigned char *)calloc(capacidad, sizeof(unsigned char));
    len_resultado = 0;

    temp = (unsigned char *)calloc(len_base256, sizeof(unsigned char));
    for (i = 0; i < len_base256; i = i + 1) {
        temp[i] = num_base256[i]; 
    }
    len_temp = len_base256;

    while (len_temp > 0) {
        resto = 0;

        for (i = 0; i < len_temp; i = i + 1) {

            valor = resto * 256 + temp[i];
            temp[i] = valor / 10;
            resto = valor % 10;
        }

        resultado[len_resultado++] = resto + '0';  

        indice = 0;
        while (len_temp > 0 && temp[indice] == 0) {
            indice = indice + 1;
        }

        if (indice > 0) {
            for (i = indice; i < len_temp; i = i + 1) {
                temp[i - indice] = temp[i];  
            }
            len_temp = len_temp - indice;  
        }
    }

    for (i = len_resultado; i > 0; i = i - 1) {
        printf("%c", resultado[i - 1]);  
    }

    printf("\n");

    free(temp);
    free(resultado);
}

void imprimir_numeros_empaquetados(unsigned char *numeros, unsigned int len_digitos, const char *nombre) {
    unsigned int i;
    unsigned char mas_significativo, menos_significativo;
    unsigned int len_bytes;

    len_bytes = (len_digitos + 1) / 2;

    printf("%s empaquetado:\n", nombre);
    for (i = 0; i < len_bytes; i = i + 1) {
        printf("Byte %d: ", i);

        mas_significativo = (numeros[i] >> 4) & 0x0F;
        menos_significativo = numeros[i] & 0x0F;

        if (2 * i < len_digitos) {
            printf("%d ", mas_significativo);
        }
        if (2 * i + 1 < len_digitos) {
            printf("%d", menos_significativo);
        }
        printf("\n");
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    unsigned int len1, len2, lenres,lenaux1,lenaux2,lenauxres;
    unsigned char *num1, *num2, *resultado1digbyte,*aux1,*aux2,*resultado2digbyte,*num1_decimal,*num2_decimal,*num1_base256, *num2_base256, *resultado_base256;
    clock_t start2, end2,start1,end1,start3,end3;
    double time1,time2,time3,memory_limit, required_memory1digbyte,required_memory2digbyte,required_memory256;
    int indice,i,len1_base256,len2_base256,lenres_base256;
    
    len1_base256 = 0;
    len2_base256 = 0;
    lenres_base256 = 0;

    if (argc == 4) {
    memory_limit = strtoull(argv[3], NULL, 10) * 1024 * 1024;
    
    if (strcmp(argv[1], "-V") == 0){ 

     scanf("%u", &len1);
     num1 = (unsigned char *)calloc(len1 + 1, sizeof(unsigned char));
     llenar_areglo(num1, len1);

     scanf("%u", &len2);
     num2 = (unsigned char *)calloc(len2 + 1, sizeof(unsigned char));
     llenar_areglo(num2, len2);
     
     resultado1digbyte = (unsigned char *)calloc(len1 + len2 + 1, sizeof(unsigned char));     

     required_memory1digbyte = (len1 + 1) * sizeof(unsigned char) + 
                          (len2 + 1) * sizeof(unsigned char) + 
                          (len1 + len2 + 1) * sizeof(unsigned char);

    if(required_memory1digbyte > memory_limit){
        printf("se sobrepaso el limite de memoria ingresado\n");
        return 1; 
    }
                          
     start1 = clock();
     multiply1digbyte(num1, num2, resultado1digbyte, len1, len2);
     end1 = clock();

     time1 = (double)(end1 - start1) / CLOCKS_PER_SEC * 1000;

     if (strcmp(argv[2], "-V") == 0) {
            printf("El resultado es: ");
            indice = 0;
            while (indice < len1 + len2 && resultado1digbyte[indice] == 0) {
                indice = indice + 1; 
            }
            if (indice == len1 + len2) {
                printf("0\n"); 
            } else {
                for (i = indice; i < len1 + len2; i = i + 1) {
                    printf("%c", resultado1digbyte[i] + '0'); 
                }
                printf("\n");
            }

        }
      
        printf("\nEl tiempo de ejecución en segundos es: %f\n", time1 / 1000);
        printf("La memoria necesaria para 1 digito por byte es (%lf MB).\n", required_memory1digbyte / (1024 * 1024));
        printf("La cantidad de dígitos de num1 es: %i.\n", len1);
        printf("La cantidad de dígitos de num2 es: %i.\n", len2);
        printf("La cantidad de dígitos del resultado es: %i.\n", len1 + len2 + 1);
      
       free(num1);
       free(num2); 
       free(resultado1digbyte);
    }
    
    if (strcmp(argv[1], "-S") == 0){
    
    scanf("%i", &len1);
    lenaux1 = (len1 + 1)/2;
    aux1 = (unsigned char *)calloc(lenaux1, sizeof(unsigned char));
    empaquetar_digitos(aux1,len1);  

    scanf("%i", &len2);
    lenaux2 = (len2 + 1)/2;
    aux2 = (unsigned char *)calloc(lenaux2, sizeof(unsigned char));
    empaquetar_digitos(aux2,len2);  

    if (strcmp(argv[2], "-V") == 0){
        imprimir_numeros_empaquetados(aux1, len1, "num1");
        imprimir_numeros_empaquetados(aux2, len2, "num2");
    }

    lenres = len1 + len2;
    lenauxres = (lenres + 1)/2;

    resultado2digbyte = (unsigned char *)calloc(lenauxres, sizeof(unsigned char));

    required_memory2digbyte = (lenaux1) * sizeof(unsigned char) + 
                              (lenaux2) * sizeof(unsigned char) + 
                              (lenauxres) * sizeof(unsigned char);

    if(required_memory2digbyte > memory_limit){
    printf("se sobrepaso el limite de memoria ingresado\n");
    return 1; 
    }

    start2 = clock();
    multiply2digbyte(aux1, len1, aux2, len2, resultado2digbyte);
    end2 = clock();

    if (strcmp(argv[2], "-V") == 0){
        imprimir_numeros_empaquetados(resultado2digbyte, lenres, "Resultado");
    }

    time2 = (double)(end2 - start2) / CLOCKS_PER_SEC * 1000;

    if (strcmp(argv[2], "-V") == 0) {
        imprimir_numero_en_bits(resultado2digbyte, lenres);
    }

    printf("\nEl tiempo de ejecución en segundos es: %f\n", time2 / 1000);
    printf("La memoria necesaria para 2 digito por byte es (%lf MB).\n", required_memory2digbyte / (1024 * 1024));
    printf("La cantidad de bytes de num1 es: %i.\n", lenaux1);
    printf("La cantidad de bytes de num2 es: %i.\n", lenaux2);
    printf("La cantidad de bytes del resultado es: %i.\n", lenauxres);
           
    free(aux1);
    free(aux2);
    free(resultado2digbyte);
}


    if (strcmp(argv[1], "-D") == 0){
    required_memory256 = 0;

    scanf("%i", &len1);
    num1_decimal = (unsigned char *)calloc(len1 + 1, sizeof(unsigned char));
    llenar_areglo(num1_decimal, len1);

    scanf("%i", &len2);
    num2_decimal = (unsigned char *)calloc(len2 + 1, sizeof(unsigned char));
    llenar_areglo(num2_decimal, len2);

    convertir_a_base256(&num1_base256, &len1_base256, num1_decimal, len1, &required_memory256);
    convertir_a_base256(&num2_base256, &len2_base256, num2_decimal, len2, &required_memory256);

    if (required_memory256 > memory_limit) {
        printf("Se sobrepasó el límite de memoria ingresado.\n");
        return 1;  
    }

    if (strcmp(argv[2], "-V") == 0){
        printf("num1 en base 256:\n");
        for ( i = 0; i < len1_base256; i = i + 1) {
            printf("%2X", num1_base256[i]);
        }
        printf("\n");

        printf("num2 en base 256:\n");
        for (i = 0; i < len2_base256; i = i + 1) {
            printf("%2X", num2_base256[i]);
        }
        printf("\n");
    }

    clock_t start3 = clock();
    multiply_base256(num1_base256, len1_base256, num2_base256, len2_base256, &resultado_base256, &lenres_base256, &required_memory256);
    clock_t end3 = clock();  

     if (required_memory256 > memory_limit) {
        printf("Se sobrepasó el límite de memoria ingresado.\n");
        return 1;  
    }

    if (strcmp(argv[2], "-V") == 0){
        printf("Resultado en base 256:\n");
        for ( i = 0; i < lenres_base256; i = i + 1) {
            printf("%2X", resultado_base256[i]);
        }
        printf("\n");

        printf("Resultado en decimal:\n");
        imprimir_numero_decimal(resultado_base256, lenres_base256);
    }

    time3 = (double)(end3 - start3) / CLOCKS_PER_SEC * 1000;
    printf("\nEl tiempo de ejecución en segundos es: %f\n", time3 / 1000);
     printf("La memoria necesaria para base 256 es (%lf MB) \n", required_memory256 / (1024.0 * 1024.0));
    printf("La cantidad de dígitos de num1 es: %i.\n", len1_base256);
    printf("La cantidad de dígitos de num2 es: %i.\n", len2_base256);
    printf("La cantidad de dígitos del resultado es : %i.\n", lenres_base256);

    free(num1_decimal);
    free(num2_decimal);
    free(num1_base256);
    free(num2_base256);
    free(resultado_base256);      
    }

    

    } else {
        mess(argv[0]);
    }

    return 0;
}