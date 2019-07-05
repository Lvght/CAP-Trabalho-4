#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *capturaQuery (const char varname[15], const char query_string[255])
{
    char *p;
    char resposta[500];
    char *q = resposta;
    //necessario incluir a biblioteca <string.h>
    p = strstr(query_string, varname);
    p += strlen(varname) + 1;
    while (*p != '&' && *p != '\0')
    {
        if (*p == '+')
        {
            *q = ' ';
        }
        else
        {
            *q = *p;
        }
        q++;
        p++;
    }
    *q = '\0';

    char *out = resposta;
    return out;
}

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
            printf ("Usuario: %s\n", nPostagem.usrOrigem);
            printf ("%s\n", nPostagem.msg);
            printf ("Likes: %d Deslikes: %d\n", nPostagem.like, nPostagem.deslike);
            printf ("\n");
            i++;
        }
    }
}

int main() {

    // Pega os dados da entrada-padrão
    // todo E se a entrada-padrão estiver vazia?
    char dados[100], usrName[15], nomeComp[50], aux[10];
    int pin;

    // Lê a entrada-padrão e salva a string no char dados
    fgets(dados, sizeof(dados), stdin);

    // Captura os dados enviados pelo formulário da página anterior
    strcpy(usrName, capturaQuery("usrname", dados));
    strcpy(aux, capturaQuery("pin", dados));
    strcpy(nomeComp, capturaQuery("nomeComp", dados));
    pin = atoi(aux);

    // Imprime as informações de cabeçalho
    printf(
            "Content-Type: text/html\n\n"
            "<!doctype html>"
            "<html>"

            "<head>"
            "<meta charset=\"UTF-8\">"
            "<title>Azkaboard</title>"
            "</head>"
    );

    // Mensagem de saudação. Apenas para demonstrar que está funcionando.
    printf(
        "<h1>Login aprovado!</h1>"
        "Bem-vindo, <strong>%s (%s)</strong>.</h1><br><br>",
        nomeComp, usrName
    );



}