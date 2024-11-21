/***************************************************************************************
 * 
 * test.lex: creador de archivos txt para ejecutar t1.c  
 *
 * Programmer: Gabriel Valdez
 *
 * Santiago de Chile, 21/10/2024
 *
 **************************************************************************************/
%{
#include <stdio.h>
#include <stdlib.h>

int first_number, second_number, i;
FILE *output_file;
%}

%%

[0-9]+          { 
                    if (first_number == 0) {
                        first_number = atoi(yytext); 
                    } else {
                        second_number = atoi(yytext);
                    }
                }
\n              {}
.               {}

%%

int yywrap() {
    return 1; 
}

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Uso: %s <numero1> <numero2>\n", argv[0]);
        return 1;
    }
    first_number = 0;
    second_number = 0;

    first_number = atoi(argv[1]);
    second_number = atoi(argv[2]);

    output_file = fopen("data.txt", "w");

    if (output_file == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    fprintf(output_file, "%d\n", first_number);

    for (i = 0; i < first_number; i = i + 1) {
        fprintf(output_file, "9\n");
    }
    
    fprintf(output_file, "%d\n", second_number);

    for (i = 0; i < second_number; i = i + 1) {
        fprintf(output_file, "9\n");
    }

    fprintf(output_file, "\n");

    fclose(output_file);

    return 0;
}
