#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

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
    char dados[100];
    fgets(dados, sizeof(dados), stdin);
    char login[15], senha[15], mensagem[180], id[10];
    capturarQuery("login", dados, login);
    capturarQuery("senha", dados, senha);
    //agora o cgi recebe tamb�m o id do usuario
    capturarQuery("id", dados, id);
    capturarQuery("post", dados, mensagem);
    strcpy(postUser.usrOrigem, login);
    postUser.like = 0;
    postUser.deslike = 0;
    strcpy(postUser.msg, mensagem);

    fp = fopen("../trabalho-4/_registros/registroPostagens.bin", "rb");
    if (fp == NULL)
        auxiliar = 1;
    fclose(fp);

    fp = fopen("../trabalho-4/_registros/registroPostagens.bin", "a+b");
    if (auxiliar)
        postUser.ID = 1;
    else
    {
        fseek(fp, -sizeof(postUser), SEEK_END);
        fread(&auxiliarID, sizeof(int), 1, fp);
        postUser.ID = auxiliarID + 1;
    }
    fclose(fp);
    fp = fopen("../trabalho-4/_registros/registroPostagens.bin", "a+b");
    fwrite(&postUser, sizeof(postUser), 1, fp);
    fclose(fp);
    char caminho[100];
    char stringID[10];
    itoa (postUser.ID, stringID, 10);
    strcpy(caminho, "../trabalho-4/_registros/");
    strcat(caminho, stringID);
    strcat(caminho, ".txt");
    fp = fopen(caminho, "w");
    //agora, caso o usuario fa�a uma postagem, seu id sera armazenado logo abaixo do numero de likes e deslikes
    fprintf (fp, "Likes=%d&Deslikes=%d\n%s\n%s\n", postUser.like, postUser.deslike, id, postUser.usrOrigem);
    fclose(fp);


    printf("%s%c%c\n", "Content-Type:text/html;charset=UTF-8",13,10);
    printf ("<!DOCTYPE html>");
    printf ("<html lang=\"pt-br\">");
    printf ("<head>");
    printf ("<title>Aba de Postagem</title>");
    printf ("<meta charset=\"utf-8\">");
    printf ("</head>");
    printf ("<body>");
    printf ("<form method=\"post\" action=\"postagem.cgi\" id=\"autosend\">");
    printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", login);
    printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", senha);
    //necessario passar o id para o proximo cgi
    printf ("<input type=\"hidden\" id=\"id\" name=\"id\" value=%s />", id);
    printf ("</form>");
    printf ("<script>");
    printf ("document.getElementById(\"autosend\").submit();");
    printf ("</script>");
    printf ("</body>");
    printf ("</html>");
}
