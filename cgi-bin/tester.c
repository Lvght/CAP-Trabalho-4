#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../trabalho-4/_c-scripts/common.c"

int main() {
    usuario u;
    usuario u2;
    char path[100] = "../trabalho-4/_registros/usuarios.bin";
    FILE *fp = fopen("../trabalho-4/_registros/usuarios.bin", "ab");

    if (fp == NULL) printf("Falha ao abrir.");

    fseek(fp, 0, SEEK_SET);
    fread(&u, sizeof(u), 1, fp);

    // strcpy(u.fullName, "Teste");

    printf("\n\nNome lido: %s\n", u.fullName);
    printf("Ultimo ID disponivel: %d", u.id);

    strcpy(u2.fullName, "Nome_teste");
    u2.id = serial(path);
    fwrite(&u2, sizeof(u2), 1, fp);
}

