#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

int capturarQuery (char varname[15], char query_string[255], char resposta[65])
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

void pegarDados (char interacao[10], char id[10], char usuario[25], char dados[255])
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

//essa função recebe o diretorio do arquivo de registro dos usuarios, o id de quem fez a postagem e a interação (like/deslike)
void alterarPontuacao (char path[255], int IDPostUser, char interacao[10])
{
    FILE *fp;
    typedef struct {
        int id;
        char usrname[25];
        char fullName[60];
        char password[45];
        char profilePicture[255];
        int likes;
        int deslikes;
    } usuario;
    usuario alterar;
    fp = fopen(path, "r+b");
    //procura o registro do usuario que recebeu o like/deslike
    fseek (fp, IDPostUser*sizeof(alterar), SEEK_SET);
    fread(&alterar, sizeof(alterar), 1, fp);
    if (strcmp(interacao, "like") == 0)
        alterar.likes += 1;
    else
        alterar.deslikes += 1;
    fseek (fp, IDPostUser*sizeof(alterar), SEEK_SET);
    fwrite(&alterar, sizeof(alterar), 1, fp);
    fclose(fp);
}

int main()
{
    char login[25], senha[45], idPostagem[10], usuario[25], interacao[10], buffer[100], likes[10], deslikes[10], dados[255], IDPostUser[10], id[10];
    fgets(dados, sizeof(dados), stdin);
    pegarDados(interacao, idPostagem, usuario, dados);
    capturarQuery("login", dados, login);
    capturarQuery("senha", dados, senha);
    //recebe o id
    capturarQuery("id", dados, id);

    char caminho[100];
    strcpy(caminho, "../trabalho-4/_registros/");
    strcat(caminho, idPostagem);
    strcat(caminho, ".txt");
    fp = fopen(caminho, "r+");
    rewind(fp);
    fgets(buffer, 100, fp);
    capturarQuery("Likes", buffer, likes);
    capturarQuery("Deslikes", buffer, deslikes);
    //armazena o id de quem postou o arquivo na variavel para ser usado na função logo abaixo.
    fgets(IDPostUser, 100, fp);
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
            //caso o like seja contabilizado, será chamada a função que altera a pontuação
            alterarPontuacao("../trabalho-4/_registros/usuarios.bin", atoi(IDPostUser), interacao);

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
    printf ("<input type=\"hidden\" id=\"id\" name=\"id\" value=%s />", id);
    printf ("</form>");
    printf ("<script>");
    printf ("document.getElementById(\"autosend\").submit();");
    printf ("</script>");
    printf ("</body>");
    printf ("</html>");






}
