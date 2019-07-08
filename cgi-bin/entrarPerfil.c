#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scripts.c"

FILE *fp;

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

//Modifiquei essa função. Agora ela recebe o usuario que foi perquisado como parâmetro.
int getLastestPostModificado(int n, const char path[255], char login[25], char senha[45], char id[10], char usr[25])
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
            //Houve modificação aqui também. Se a postagem lida tiver sido feita pelo usuario em questão, ela sera impressa, senão não.
            if (strcmp(nPostagem.usrOrigem, usr) == 0 || strcmp(nPostagem.usrOrigem, "-1") == 0)
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

                // Condição de início de arquivo
                // Só pode possivelmente ocorrer com o primeiro usuário cadastrado
                if ( nPostagem.ID == 1 )
                    return 0;


                itoa (nPostagem.ID, auxiliarLike, 10);
                strcat(auxiliarLike, ".");
                strcat(auxiliarLike, login);
                itoa (nPostagem.ID, auxiliarDeslike, 10);
                strcat(auxiliarDeslike, ".");
                strcat(auxiliarDeslike, login);

                // Usuário de origem
                printf("<p class='usrname'>Usuario: %s</p>", nPostagem.usrOrigem);

                // Mensagem em si
                printf("<p class='msg'>Mensagem: %s</p>", nPostagem.msg);

                // Botão de like
                printf("<form action=\"DLike.cgi\" method=\"post\">\n"
                       "    <input type=\"hidden\" id=\"like\" name=\"like\" value=%s />\n"
                       "    <input type=\"hidden\" id=\"login\" name=\"login\" value=%s />\n"
                       "    <input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />\n"
                       "    <input type=\"hidden\" id=\"id\" name=\"id\" value=%s />\n"
                       "    <input type=\"submit\" value=%s />\n"
                       "    (%d)\n"
                       "</form>",
                       auxiliarLike, login, senha, id, "Like ", nPostagem.like);

                // Botão de deslike
                printf("<form action=\"DLike.cgi\" method=\"post\">\n"
                       "    <input type=\"hidden\" id=\"deslike\" name=\"deslike\" value=%s />\n"
                       "    <input type=\"hidden\" id=\"login\" name=\"login\" value=%s />\n"
                       "    <input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />\n"
                       "    <input type=\"hidden\" id=\"id\" name=\"id\" value=%s />\n"
                       "    <input type=\"submit\" value=%s />\n"
                       "    (%d)\n"
                       "</form>",
                       auxiliarDeslike, login, senha, id, "Deslike", nPostagem.deslike );

                printf ("<br><br>");
                printf ("</html>");

                //a variavel i (que imprime i postagens do usuario) só sera incrementada se a postagem tiver sido impressa
                i++;
            }
            //a variavel j sera incrementada sempre, garantindo que o fseek avance sempre de posição
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
    char dados[255], login[25], senha[45], id[15], usr[25];

    // Lê a entrada-padrão
    fgets(dados, sizeof(dados), stdin);

    capturarQuery("login", dados, login);
    capturarQuery("senha", dados, senha);
    capturarQuery("id", dados, id);
    capturarQuery("usuario", dados, usr);

    usuario db;
    getUsuarioByUsrname(login, "../trabalho-4/_registros/usuarios.bin", &db);

    // mudar diretorio caso o nome do registro seja outro
    fp = fopen("../trabalho-4/_registros/usuarios.bin", "rb");
    fread (&entrar, sizeof(entrar), 1, fp);

    //faz uma busca para verificar se o usuario pesquisado esta registrado no arquivo
    int i = 0;
    while (!feof(fp) && strcmp(entrar.usrname, usr) != 0)
    {
        fseek (fp, i*sizeof(entrar), SEEK_SET);
        fread (&entrar, sizeof(entrar), 1, fp);
        i++;
    }

    fclose (fp);

    // caso o usuario seja encontrado, vai ser exibida uma pagina com as postagens
    if (strcmp (entrar.usrname, usr) == 0)
    {
        printf("%s%c%c\n", "Content-Type:text/html;charset=UTF-8",13,10);

        // Demais dados do usuário
        usuario db;
        getUsuarioByUsrname(usr, "../trabalho-4/_registros/usuarios.bin", &db);

        // Cabeçalho html
        printf("<!DOCTYPE html>\n"
               "<html>\n"
               "<head>\n"
               "    <meta charset='UTF-8'>\n"
               "    <title>AKB! Posts de @%s, %s</title>\n"
               "    <link href='../trabalho-4/_estilos/postagens.css' rel='stylesheet'>\n"
               "</head>\n"
               "<body>",
               db.usrname, db.fullName );

        // Botão para voltar à página inicial
        printf("<form action=\"postagem.cgi\" method=\"post\">\n"
               "    <input type=\"hidden\" id=\"login\" name=\"login\" value=%s />\n"
               "    <input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />\n"
               "    <input type=\"hidden\" id=\"id\" name=\"id\" value=%s />\n"
               "    <input type=\"submit\" value=\"Pagina Principal\"/>\n"
               "</form>",
               login, senha, id );


        printf ("<h2>Postagens de %s, @%s</h2>", db.fullName, db.usrname);
        printf ("<h2>%d Estrelas</h2>", getPoints(db.id, "../trabalho-4/_registros/usuarios.bin"));
        printf ("<br>");
        getLastestPostModificado(10, "../trabalho-4/_registros/registroPostagens.bin", login, senha, id, usr);
        printf ("<form action=\"carregarMaisPerfil.cgi\" method=\"post\">");
        printf ("<input type=\"submit\" value=\"Carregar mais\"/>");
        printf ("</div>");
        printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", login);
        printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", senha);
        //agora o id fica como imput escondido
        printf ("<input type=\"hidden\" id=\"id\" name=\"id\" value=%s />", id);
        printf ("<input type=\"hidden\" id=\"usuario\" name=\"usuario\" value=%s />", usr);
        printf ("<input type=\"hidden\" id=\"quantidade\" name=\"carregar\" value=\"20\" />");
        printf ("</form>");
        printf ("</body>");
        printf ("</html>");

    }
    //caso ele não seja encontrado, essa mensagem aparecera
    else
    {
        printf("%s%c%c\n", "Content-Type:text/html;charset=UTF-8",13,10);
        printf ("<!DOCTYPE html>");
        printf ("<html lang=\"pt-br\">");
        printf ("<head>");
        printf ("<title>Perfil de %s</title>", usr);
        printf ("<meta charset=\"utf-8\">");
        printf ("</head>");
        printf ("<body>");
        printf ("Nao foi possivel achar o usuario");
        printf ("<form action=\"postagem.cgi\" method=\"post\">");
        printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", login);
        printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", senha);
        printf ("<input type=\"hidden\" id=\"id\" name=\"id\" value=%s />", id);
        printf ("<input type=\"submit\" value=\"Pagina Principal\"/>");
        printf ("</form>");
        printf ("</body></html>");
    }



}
