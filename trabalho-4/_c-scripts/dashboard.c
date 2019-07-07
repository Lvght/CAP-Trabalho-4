#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.c"

void getLastestPost(int n, const char path[255])
{
   typedef struct x
    {
        int ID;
        char usrOrigem[10];
        int like;
        int deslike;
        char msg[180];
    }postagem;

    usuario u;

    postagem nPostagem;
    int i = 1;
    FILE *fp;
    fp = fopen (path, "rb");
    if (fp == NULL)
        printf ("Erro ao abrir o arquivo");
    else
    {
        while (i <= n)
        {
            fseek(fp, -i*(sizeof(nPostagem)), SEEK_END);
            fread(&nPostagem, sizeof(nPostagem), 1, fp);

            // Aborta ao chegar no começo do arquivo
            // if (nPostagem.ID == -1) return;

            getUsuarioByUsrname(nPostagem.usrOrigem, "../trabalho-4/_registros/usuarios.bin", &u);

            printf(
                   "<div class='postagem'>\n"
                   "    <span class='usrname'>%s | @%s</span>\n"
                   "    <span class='msg'>%s</span>\n    <div class='btnContainer'>\n"
                   "        <span class='like'>Like: %d</span>\n"
                   "        <span class='deslike'>Deslikes: %d</span>\n"
                   "    </div>\n"
                   "</div>",
                   u.fullName, nPostagem.usrOrigem, nPostagem.msg, nPostagem.like, nPostagem.deslike
                   );
            i++;
        }
    }
}

int main() {

    // Pega os dados da entrada-padrão
    // todo E se a entrada-padrão estiver vazia?
    char aux[10], dados[255];
    int usrID;
    usuario usr;

    // Lê a entrada-padrão e salva a string no char dados
    fgets(dados, sizeof(dados), stdin);

    // Captura o ID do usuário, enviado pelo formulário da página anterior
    strcpy(aux, capturaQuery("usrID", dados));
    usrID = atoi(aux);

    // Obtém os dados do usuário salvos em registro
    getUsuario(usrID, "../trabalho-4/_registros/usuarios.bin", &usr);

    // Imprime as informações de cabeçalho
    printf(
            "Content-Type: text/html\n\n"
            "<!doctype html>\n"
            "<head>\n"
            "    <meta charset=\"UTF-8\">\n"
            "    <title>Azkaboard</title>\n"
            "    <link href='../trabalho-4/_estilos/classes.css' rel='stylesheet'>\n"
            "</head>"
    );

    // Mensagem de saudação. Apenas para demonstrar que está funcionando.
    printf(
        "<h1>Login aprovado!</h1>"
        "Bem-vindo, <strong>%s, @%s (id: %d)</strong>.</h1><br><br>",
        usr.fullName, usr.usrname, usr.id
    );

    printf("Query: %s", dados);


    // imprime um formulário para efetuar as postagens
    printf(
        "<form id='form-post' action='postagem.cgi' method='post'>"
            "<input type='hidden' name='usrID' value='%d'>"
            "<input type='text' name='post'>"
            "<input type='submit' value='Enviar'>"
        "</form>",
        usrID
    );

    getLastestPost(5, "../trabalho-4/_registros/postagens.bin");

}