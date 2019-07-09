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

void pegarDados (char interacao[10], char id[10], char usuario[10], char dados[50])
{
    char *p;
    p = dados;
    int i = 0;
    while (*p != '=')
    {
        interacao[i] = *p;
        p++;
        i++;
    }
    interacao[i] = '\0';

    p++;
    i = 0;
    while (*p != '.')
    {
        id[i] = *p;
        p++;
        i++;
    }
    id[i] = '\0';
    p++;
    i = 0;
    while (*p != '&')
    {
        usuario[i] = *p;
        p++;
        i++;
    }
    usuario[i] = '\n';
    usuario[i+1] = '\0';
}

int main()
{
    char login[15];
    char senha[15];
    char id[10];
    char usuario[10];
    char interacao[10];
    char buffer[100];
    char likes[10];
    char deslikes[10];
    char dados[100];
    fgets(dados, sizeof(dados), stdin);
    pegarDados(interacao, id, usuario, dados);
    capturarQuery("login", dados, login);
    capturarQuery("senha", dados, senha);

    char caminho[100];
    strcpy(caminho, "../trabalho-4/_registros/");
    strcat(caminho, id);
    strcat(caminho, ".txt");
    fp = fopen(caminho, "r+");
    rewind(fp);
    fgets(buffer, 100, fp);
    capturarQuery("Likes", buffer, likes);
    capturarQuery("Deslikes", buffer, deslikes);
    while (!feof(fp) && strcmp(buffer, usuario) != 0)
        fgets(buffer, 100, fp);
    if (strcmp(buffer, usuario) == 0);
    else
    {
            fseek(fp, 0, SEEK_END);
            fprintf (fp, "%s", usuario);
            rewind(fp);
            if (!(strcmp(interacao, "like")))
                fprintf (fp, "Likes=%d&Deslikes=%d", atoi(likes) + 1, atoi(deslikes));
            else
                fprintf (fp, "Likes=%d&Deslikes=%d", atoi(likes), atoi(deslikes) + 1);

    }
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
    printf ("</form>");
    printf ("<script>");
    printf ("document.getElementById(\"autosend\").submit();");
    printf ("</script>");
    printf ("</body>");
    printf ("</html>");






}
