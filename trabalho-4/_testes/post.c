#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    char dados[40];
    char *ptrDados;

    // obtem os dados do stdin
    fgets(dados, 40, stdin);


    char * aux = getenv("CONTENT_LENGTH");
    int tamanho = strtol(aux, NULL, 10);
    tamanho *= sizeof(char);

    ptrDados = malloc(tamanho + 1);
    fgets(ptrDados, tamanho + 1, stdin);

    printf("Content-Type: text/html\n\n");
    printf("Valor lido - POST: %s<br><br>", dados);
    printf("Valor lido - GET: %s", ptrDados);
}