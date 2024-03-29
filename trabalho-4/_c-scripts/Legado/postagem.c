#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void capturarQuery (char varname[15], char query_string[255], char resposta[15])
{
    char *p;
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
}

int getLastestPost(int n, const char path[255], char login[15], char senha[15])
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
    char caminho[100];
    char like[10];
    char deslike[10];
    char buffer[100];
    int i = 1;
    char auxiliarLike[100];
    char auxiliarDeslike[100];
    FILE *fp;
    fp = fopen (path, "rb");
    if (fp == NULL)
    {
        printf ("Nenhuma postagem.");
        fclose(fp);
    }
    else
    {
        while (i <= n)
        {
            fseek(fp, -i*(sizeof(nPostagem)), SEEK_END);
            fread(&nPostagem, sizeof(nPostagem), 1, fp);
            fclose(fp);
            strcpy(caminho, "../trabalho-4/_registros/");
            char auxiliarID[10];
            itoa (nPostagem.ID, auxiliarID, 10);
            strcat(caminho, auxiliarID);
            strcat(caminho, ".txt");
            fp = fopen(caminho, "r+");
            rewind(fp);
            fgets(buffer, 100, fp);
            capturarQuery("Likes", buffer, like);
            capturarQuery("Deslikes", buffer, deslike);
            fclose(fp);
            nPostagem.like = atoi(like);
            nPostagem.deslike = atoi(deslike);
            fp = fopen (path, "rb");
            if (!(strcmp(nPostagem.usrOrigem, "-1")))
                return 0;
            else
            {
                itoa (nPostagem.ID, auxiliarLike, 10);
                strcat(auxiliarLike, ".");
                strcat(auxiliarLike, login);
                itoa (nPostagem.ID, auxiliarDeslike, 10);
                strcat(auxiliarDeslike, ".");
                strcat(auxiliarDeslike, login);
                printf ("<strong>Usuario</strong>: %s<br>", nPostagem.usrOrigem);
                printf ("%s<br>", nPostagem.msg);
                printf ("<form action=\"DLike.cgi\" method=\"post\">");
                printf ("<input type=\"hidden\" id=\"like\" name=\"like\" value=%s />", auxiliarLike);
                printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", login);
                printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", senha);
                printf ("<input type=\"submit\" value=%s />", "Like ");
                printf ("(%d)", nPostagem.like);
                printf ("</div>");
                printf ("</form>");
                printf ("<form action=\"DLike.cgi\" method=\"post\">");
                printf ("<input type=\"hidden\" id=\"deslike\" name=\"deslike\" value=%s />", auxiliarDeslike);
                printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", login);
                printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", senha);
                printf ("<input type=\"submit\" value=%s />", "Deslike ");
                printf ("(%d)", nPostagem.deslike);
                printf ("</div>");
                printf ("</form>");
                printf ("<br><br>");
            }
            i++;
        }
    }
    return 1;
}

int main()
{
    char login[15], senha[15];

    char dados[100];
    fgets(dados, sizeof(dados), stdin);
    capturarQuery("login", dados, login);
    capturarQuery("senha", dados, senha);

    printf("%s%c%c\n", "Content-Type:text/html;charset=UTF-8",13,10);
    printf ("<!DOCTYPE html>");
    printf ("<html lang=\"pt-br\">");
    printf ("<head>");
    printf ("<title>Aba de Postagem</title>");
    printf ("<meta charset=\"utf-8\">");
    printf ("</head>");
    printf ("<body>");
    printf ("<form action=\"armazenarPostagem.cgi\" method=\"post\">");
    printf ("<div id=\"title\"> <h1> Bem vindo <strong>%s</strong></h1> </div>", login);
    printf ("<div id=\"subtitle\"> <h2> Digite aqui a sua mensagem </h2> </div>");
    printf ("<div id=\"msg\">");
    printf ("<input type=\"text\" id=\"postagem\" name=\"post\"/>");
    printf ("<input type=\"submit\" value=\"Enviar\"/>");
    printf ("</div>");
    printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", login);
    printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", senha);
    printf ("</form>");
    printf ("<br>");
    getLastestPost(10, "../trabalho-4/_registros/registroPostagens.bin", login, senha);
    printf ("</body>");
    printf ("</html>");
}
