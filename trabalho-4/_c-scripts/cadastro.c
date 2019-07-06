#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funções utilizadas em praticamente todos os scripts.
#include "common.c"

int main() {

    printf("Content-Type: text/html\n\n");

    /*** DECLARAÇÕES ***/
    char dados[100], aux[5];
    usuario x;
    FILE *ptrRegistro;

    /********************/

    // Lê os dados recebidos da página anterior
    fgets(dados, sizeof(dados), stdin);
    printf("%s", dados);
//    exit(0);

    /*** Extrai os dados recebidos da página HTML e monta o objeto ***/
    // ID
    x.id = serial("../trabalho-4/_registros/usuarios.bin");
    printf("<p>ID: %d", x.id);

    // Nome de usuário
    strcpy(x.usrname, capturaQuery("usrname", dados));
    printf("<br><br>Nome de usuario: %s", x.usrname);
//    exit(0);

    // Nome completo
    strcpy(x.fullName, capturaQuery("fullname", dados));
    printf("<br><br>Nome completo: %s", x.fullName);
//    exit(0);

    // Código numérico (PIN)
    strcpy(aux, capturaQuery("pin", dados));
    x.pin = atoi(aux);

    // Likes
    x.likes = 0;

    // Deslikes | Valor padrão = 1 para evitar posterior divisão por 0
    // Usuário começa sua conta com nenhuma estrela
    x.deslikes = 1;

    // Imagem de perfil | O usuário poderá alterar isso posteriormente
    strcpy(x.profilePicture, "default.jpg");

    // Insere o cabeçalho HTML

    printf("<!DOCTYPE html>\n"
           "<html>\n"
           "<head>\n"
           "    <meta charset='UTF-8'>\n"
           "    <title>Cadastro</title>\n"
           "</head>\n"
           "<body>\n"
           );

//    exit(0);

    printf("<p>Os dados recebidos foram:</p>\n<p>Nome completo: %s</p>\n<p>Nome de usuário: %s</p>\n<p>Senha: %d</p>",
    x.fullName, x.usrname, x.pin);

    /*** Salva os dados no arquivo de registro ***/
    // Abertura do arquivo e exibição de erro caso a abertura falhe
    if ( ( ptrRegistro = fopen("../trabalho-4/_registros/usuarios.bin", "rb+") ) == NULL)
    {
        printf("Não foi possível acessar o arquivo de registro! Execute o utilitario de inicializacao.\n");
        exit(1);
    }

    // salvamento
    int check = 0;
    check = fwrite(&x, sizeof(x), 1, ptrRegistro);

    if (!check) {
        printf("<p>Falha ao registrar</p>");
    }


    /*** Exibição do resultado ***/

    printf("<p>Voce foi cadastrado com sucesso</p>\n<a href='../../cgi-bin/dashboard.cgi'></a>");

}
