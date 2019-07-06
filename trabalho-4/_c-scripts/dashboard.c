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

            printf(
                   "<div class='postagem'>\n"
                   "    <span class='usrname'>Usuário: %s</span>\n"
                   "    <span class='msg'>Mensagem: %s</span>\n"
                   "    <span class='like'>Like: %d</span>\n"
                   "    <span class='deslike'>Deslikes: %d</span> <br><br>\n"
                   "</div>",
                   nPostagem.usrOrigem, nPostagem.msg, nPostagem.like, nPostagem.deslike
                   );

//            printf ("Usuario: %s\n", nPostagem.usrOrigem);
//            printf ("%s\n", nPostagem.msg);
//            printf ("Likes: %d Deslikes: %d\n", nPostagem.like, nPostagem.deslike);
//            printf ("\n");
            i++;
        }
    }
}

int main() {

    // Pega os dados da entrada-padrão
    // todo E se a entrada-padrão estiver vazia?
    char aux[10], dados[100];
    int usrID;

    // Lê a entrada-padrão e salva a string no char dados
    fgets(dados, sizeof(dados), stdin);

    // Captura o ID do usuário, enviado pelo formulário da página anterior
    strcpy(aux, capturaQuery("usrID", dados));
    usrID = atoi(aux);

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
    usuario dat;
    getUsuario(usrID, "../trabalho-4/_registros/usuarios.bin", &dat);
    printf(
        "<h1>Login aprovado!</h1>"
        "Bem-vindo, <strong>%s, @%s (id: %d)</strong>.</h1><br><br>",
        dat.fullName, dat.usrname, dat.id
    );





    FILE *fpc = fopen("../trabalho-4/_registros/usuarios.bin", "rb");
    char path[100] = "../trabalho-4/_registros/usuarios.bin";
    usuario u;

    fseek(fpc, 0*sizeof(u), SEEK_SET);
    fread(&u, sizeof(u), 1, fpc);
    printf(" | Nome: %s | Senha: %s | ID: %d<br><br>", u.usrname, u.password, u.id);

    getUsuario(1, path, &u);
    printf(" | Nome: %s | Senha: %s | ID: %d<br><br>", u.usrname, u.password, u.id);

//    getUsr(fpc, 2, &u);
//    printf(" | Nome: %s | Senha: %s | ID: %d<br><br>", u.usrname, u.password, u.id);








    // imprime um formulário para efetuar as postagens
    printf(
        "<form id='form-post' action='armazenarPostagem.cgi' method='post'>"
            "<input type='hidden' name='usrID' value='%d'>"
            "<input type='text' name='post'>"
            "<input type='submit' value='Enviar'>"
        "</form>",
        usrID
    );

    getLastestPost(5, "../trabalho-4/_registros/postagens.bin");

}