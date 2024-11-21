1-  flex test.lex    (ejecuta el .lex que crea un data.txt)
2-  gcc lex.yy.c -o test -lfl 
3-  ./test NUM NUM    ==> NUM = [0-9]+    (se crea un data.txt con largos NUM NUM)
4-  gcc t1.c -o t1
5-  ./t1 -G -D MEM  < data.txt  (compilación del código en los distintos modos especificados)


G = V: 1 digito por byte , S: 2 digito por byte , D: base de 256
D = V: mostrar respuesta, S: solo tiempo
MEM = límite de memoria en bytes

además habrá 2 txt , memoria y time. los cuales se usaron para crear el grafico dígitos vs memoria y dígitos vs tiempo de ejecución. respectivamente.