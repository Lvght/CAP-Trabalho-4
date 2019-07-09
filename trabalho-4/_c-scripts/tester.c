#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../trabalho-4/_c-scripts/common.c"

int main() {
    usuario u;
    usuario u2;
    char path[100] = "../trabalho-4/_registros/usuarios.bin";
    FILE *fp = fopen("../trabalho-4/_registros/usuarios.bin", "rb");

    if (fp == NULL) printf("Falha ao abrir.");

    fseek(fp, -1* sizeof(u), SEEK_END);
    fread(&u, sizeof(u), 1, fp);

    // strcpy(u.fullName, "Teste");

    printf("\n\nNome lido: %s\n", u.fullName);
    printf("ID: %d\n", u.id);
    printf("Username: %s\n", u.usrname);
    printf("Senha: %d", u.pin);

//    strcpy(u2.fullName, "Nome_teste");
//    u2.id = serial(path);
//    fwrite(&u2, sizeof(u2), 1, fp);
}

