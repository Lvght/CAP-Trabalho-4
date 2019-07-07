#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void capturarQuery (char varname[15], char query_string[255], char resposta[60])
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

//Modifiquei essa fun��o. Agora ela recebe o usuario que foi perquisado como par�metro.
int getLastestPostModificado(int n, const char path[255], char login[25], char senha[45], char id[10], char usuario[25])
{
   typedef struct x
    {
        int ID;
        char usrOrigem[25];
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
        int j = 1;
        while (i <= n && !feof(fp))
        {
            fseek(fp, -j*(sizeof(nPostagem)), SEEK_END);
            fread(&nPostagem, sizeof(nPostagem), 1, fp);
            //Houve modifica��o aqui tamb�m. Se a postagem lida tiver sido feita pelo usuario em quest�o, ela sera impressa, sen�o n�o.
            if (strcmp(nPostagem.usrOrigem, usuario) == 0 || strcmp(nPostagem.usrOrigem, "-1") == 0)
            {
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
                    printf ("<input type=\"hidden\" id=\"id\" name=\"id\" value=%s />", id);
                    printf ("<input type=\"submit\" value=%s />", "Like ");
                    printf ("(%d)", nPostagem.like);
                    printf ("</div>");
                    printf ("</form>");
                    printf ("<form action=\"DLike.cgi\" method=\"post\">");
                    printf ("<input type=\"hidden\" id=\"deslike\" name=\"deslike\" value=%s />", auxiliarDeslike);
                    printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", login);
                    printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", senha);
                    printf ("<input type=\"hidden\" id=\"id\" name=\"id\" value=%s />", id);
                    printf ("<input type=\"submit\" value=%s />", "Deslike ");
                    printf ("(%d)", nPostagem.deslike);
                    printf ("</div>");
                    printf ("</form>");
                    printf ("<br><br>");
                    printf ("</html>");
                }
                //a variavel i (que imprime i postagens do usuario) s� sera incrementada se a postagem tiver sido impressa
                i++;
            }
            //a variavel j sera incrementada sempre, garantindo que o fseek avance sempre de posi��o
            j++;
        }
    }
    return 1;
}

void pontuacao (char path[255], int idUsuario)
{
    FILE *fp;
    usuario x;
    fp = fopen(path, "rb");
    if (fp == NULL)
        printf ("Erro na pontuacao");
    else
    {
        //utiliza do acesso direto para achar o registro do usuario
        fseek (fp, idUsuario*sizeof(x), SEEK_SET);
        fread (&x, sizeof(x), 1, fp);
        printf ("Usuario: %s\nEstrelas: %d", x.usrname, x.likes/x.deslikes);
        fclose(fp);
    }

}


int main()
{
    usuario entrar;
    char dados[255], login[25], senha[45], id[15], usuario[25], qtsPostagens[10];
    fgets(dados, sizeof(dados), stdin);
    capturarQuery("login", dados, login);
    capturarQuery("senha", dados, senha);
    capturarQuery("id", dados, id);
    capturarQuery("usuario", dados, usuario);
    capturarQuery("carregar", dados, qtsPostagens);
    //mudar diretorio caso o nome do registro seja outro
    fp = fopen("../trabalho-4/_registros/usuarios.bin", "rb");
    fread (&entrar, sizeof(entrar), 1, fp);
    //faz uma busca para verificar se o usuario pesquisado esta registrado no arquivo
    int i = 0;
    while (!feof(fp) && strcmp(entrar.usrname, usuario) != 0)
    {
        fseek (fp, i*sizeof(entrar), SEEK_SET);
        fread (&entrar, sizeof(entrar), 1, fp);
        i++;
    }
    fclose (fp);
    //caso o usuario seja achado, vai ser criada uma pagina com as postagens
    if (strcmp (entrar.usrname, usuario) == 0)
    {
        printf("%s%c%c\n", "Content-Type:text/html;charset=UTF-8",13,10);
        printf ("<!DOCTYPE html>");
        printf ("<html lang=\"pt-br\">");
        printf ("<head>");
        printf ("<title>Perfil de %s</title>", usuario);
        printf ("<meta charset=\"utf-8\">");
        printf ("</head>");
        printf ("<body>");
        printf ("<form action=\"postagem.cgi\" method=\"post\">");
        printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", login);
        printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", senha);
        printf ("<input type=\"hidden\" id=\"id\" name=\"id\" value=%s />", id);
        printf ("<input type=\"submit\" value=\"Pagina Principal\"/>");
        printf ("</form>");
        pontuacao("../trabalho-4/_registros/usuarios.bin", entrar.id);
        printf ("<h2>Postagens de %s</h2>", usuario);
        printf ("<br>");
        getLastestPostModificado(atoi(qtsPostagens), "../trabalho-4/_registros/registroPostagens.bin", login, senha, id, usuario);
        printf ("<form action=\"carregarMaisPerfil.cgi\" method=\"post\">");
        printf ("<input type=\"submit\" value=\"Carregar mais\"/>");
        printf ("</div>");
        printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", login);
        printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", senha);
        //agora o id fica como imput escondido
        printf ("<input type=\"hidden\" id=\"id\" name=\"id\" value=%s />", id);
        printf ("<input type=\"hidden\" id=\"usuario\" name=\"usuario\" value=%s />", usuario);
        printf ("<input type=\"hidden\" id=\"quantidade\" name=\"carregar\" value=%d />", atoi(qtsPostagens) + 10);
        printf ("</form>");
        printf ("</body>");
        printf ("</html>");

    }
    //caso ele n�o seja encontrado, essa mensagem aparecera
    else
    {
        printf("%s%c%c\n", "Content-Type:text/html;charset=UTF-8",13,10);
        printf ("<!DOCTYPE html>");
        printf ("<html lang=\"pt-br\">");
        printf ("<head>");
        printf ("<title>Perfil de %s</title>", usuario);
        printf ("<meta charset=\"utf-8\">");
        printf ("</head>");
        printf ("<body>");
        printf ("Nao foi possivel achar o usuario");
        printf ("<form action=\"armazenarPostagem.cgi\" method=\"post\">");
        printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", login);
        printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", senha);
        printf ("<input type=\"hidden\" id=\"id\" name=\"id\" value=%s />", id);
        printf ("<input type=\"submit\" value=\"Pagina Principal\"/>");
        printf ("</form>");
        printf ("</body></html>");
    }



}
