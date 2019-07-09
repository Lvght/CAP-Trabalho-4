/*
 * Criado por: Vinicius
 * Descrição: Este programa inicializa os arquivos de registro. Execute ele ao rodar a rede
 * pela primeira vez.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.c"

int main() {
    usuario u;
    postagem p;
    int escolha;

    // Pede confirmação
    printf("Executar este programa ira apagar todos os registros, caso eles ja existam\nContinuar? [0 = Nao]/[1 = Sim]\nDigite aqui: ");
    scanf("%d", &escolha);

    if (!escolha)
        exit(0);

    // Reseta ou cria os arquivos
    FILE *ptrCadastro = fopen("../trabalho-4/_registros/usuarios.bin", "wb");
    FILE *ptrPostagem = fopen("../trabalho-4/_registros/registroPostagens.bin", "wb");

    // Monta os valores de sentinela de usuario
    u.id = 0;
    strcpy(u.usrname, "-1");
    strcpy(u.password, "std_password");
    strcpy(u.fullName, "std_fullname");
    u.likes = -1;
    u.deslikes = -1;

    // Monta os valores de sentinela de postagem
    p.ID = 0;
    strcpy(p.usrOrigem, "-1");
    printf("Usr: %s", p.usrOrigem);
    strcpy(p.msg, "Lorem ipsum dolor sit amet...");
    p.deslike = p.like = -1;

    // Salva o usuario padrão
    fwrite(&u, sizeof(u), 1, ptrCadastro);

    // Salva a postagem padrão
    fwrite(&p, sizeof(p), 1, ptrPostagem);

    // Avisa sobre o término
    printf("Os arquivos foram inicializados com sucesso.\n\n");
}
