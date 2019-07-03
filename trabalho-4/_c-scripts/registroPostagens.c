#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

#include <stdio.h>
#include <stdlib.h>

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
            printf ("<strong>Usuario</strong>: %s<br>", nPostagem.usrOrigem);
            printf ("%s<br>", nPostagem.msg);
            printf ("Likes: %d Deslikes: %d<br>", nPostagem.like, nPostagem.deslike);
            printf ("<br>");
            i++;
        }
    }
}

int capturarQuery (char varname[15], char query_string[255], char resposta[15])
{
    char *p;
    char *q = resposta;
    //necessario incluir a biblioteca <string.h>
    p = strstr(query_string, varname);
    if (strstr(query_string, varname) == NULL)
        return 0;
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
    return 1;
}

typedef struct x
{
    int ID;
    char usrOrigem[10];
    int like;
    int deslike;
    char msg[180];
}postagem;

int main()
{
    int auxiliarID;
    postagem postUser;
    int auxiliar = 0;
    char* dados = NULL;
    char login[15], senha[15], mensagem[180];
    dados = getenv("QUERY_STRING");
    capturarQuery("login", dados, login);
    capturarQuery("senha", dados, senha);
    capturarQuery("post", dados, mensagem);
    strcpy(postUser.usrOrigem, login);
    postUser.like = 0;
    postUser.deslike = 0;
    strcpy(postUser.msg, mensagem);

    fp = fopen("../trabalho-4/_registros/registrosPostagens.bin", "rb");
    if (fp == NULL)
        auxiliar = 1;
    fclose(fp);

    fp = fopen("../trabalho-4/_registros/registrosPostagens.bin", "a+b");
    if (auxiliar)
        postUser.ID = 1;
    else
    {
        fseek(fp, -sizeof(postUser), SEEK_END);
        fread(&auxiliarID, sizeof(int), 1, fp);
        postUser.ID = auxiliarID + 1;
    }
    fclose(fp);
    fp = fopen("../trabalho-4/_registros/registrosPostagens.bin", "a+b");
    fwrite(&postUser, sizeof(postUser), 1, fp);
    fclose(fp);


    printf("%s%c%c\n", "Content-Type:text/html;charset=UTF-8",13,10);
    printf ("<!DOCTYPE html>");
    printf ("<html lang=\"pt-br\">");
    printf ("<head>");
    printf ("<title>Aba de Postagem</title>");
    printf ("<meta charset=\"utf-8\">");
    printf ("</head>");
    printf ("<body>");
    getLastestPost(10, "../trabalho-4/_registros/registrosPostagens.bin");
    printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", login);
    printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", senha);
    printf ("</body>");
    printf ("</html>");
}
