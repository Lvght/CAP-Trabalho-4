#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scripts.c"

#define STD_POST_AMNT 10

void perfil (char login[25], char senha[45], char id[15])
{
    printf("<form action=\"entrarPerfil.cgi\" method=\"post\">\n"
           "    <div id=\"subtitle\"> \n"
           "        <h2 class='txt-indicator'> Procure por um usuario </h2> \n"
           "    </div>\n    <div id=\"msg\">\n"
           "        <input type=\"text\" id=\"usuario\" class='input-txt' name=\"usuario\"/>\n"
           "        <input type=\"submit\" class='btn' id='btn-busca' value=\"Enviar\"/>\n"
           "    </div>\n"
           "    <input type=\"hidden\" id=\"login\" name=\"login\" value=%s />\n"
           "    <input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />\n"
           "    <input type=\"hidden\" id=\"id\" name=\"id\" value=%s />\n"
           "</form>",
           login, senha, id );
}

//Essa função imprime a pontuação do usuario
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

void capturarQuery (char varname[15], char query_string[255], char resposta[50])
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

//agora a função tem como parametro o id do usuario, para ser passado para os outros cgi
// n = número de postagens
int getLastestPost(int n, const char path[255], char login[25], char senha[45], char id[10])
{
   typedef struct x
    {
        int ID;
        char usrOrigem[25];
        int like;
        int deslike;
        char msg[180];
    } postagem;

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
        // Exibe a quantidade de mensagem especificada pelo parâmetro 'n'
        // i é um contador
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

            // Interrompe o script caso chegue na mensagem de sentinela ID = 1.
            if (nPostagem.ID == 1)
                return 0;

            itoa (nPostagem.ID, auxiliarLike, 10);
            strcat(auxiliarLike, ".");
            strcat(auxiliarLike, login);
            itoa (nPostagem.ID, auxiliarDeslike, 10);
            strcat(auxiliarDeslike, ".");
            strcat(auxiliarDeslike, login);



            //region Container da postagem
            printf("<div class='post-container'>");

            // Botão para o perfil do usuário
            printf("<form action=\"entrarPerfil.cgi\" method=\"post\">\n"
                  "    <input type=\"hidden\" name=\"usuario\" value = %s > <!-- Usrname -->\n"
                  "    <input type=\"hidden\" id=\"login\" name=\"login\" value = %s /> <!-- Here -->\n"
                  "    <input type=\"hidden\" id=\"senha\" name=\"senha\" value = %s />\n"
                  "    <input type=\"hidden\" id=\"id\" name=\"id\" value=%s />\n"
                  "    <input type=\"submit\" class='post-btn-usrname' value = @%s />\n"
                  "</form>",
                  nPostagem.usrOrigem, login, senha, id, nPostagem.usrOrigem );

            // Mensagem do usuário
            printf("<span class='post-usrmsg'>%s</span>", nPostagem.msg);

            // Abre a div btn-container
            printf("<div class='btn-container'>");

            // Botão de like
            printf("<form action=\"DLike.cgi\" method=\"post\">\n"
                   "    <input type=\"hidden\" name=\"like\" value=%s />\n"
                   "    <input type=\"hidden\" name=\"login\" value=%s />\n"
                   "    <input type=\"hidden\" name=\"senha\" value=%s />\n"
                   "    <input type=\"hidden\" name=\"id\" value=%s />\n"
                   "    <input type=\"submit\" value= \"%d %s%s\" class='post-btn-like'/>\n"
                   "</form>\n",
                    auxiliarLike, login, senha, id, nPostagem.like, "Like", nPostagem.like > 2 ? "s" : "" );

            // Botão de deslike
            printf("<form action=\"DLike.cgi\" method=\"post\">\n"
                   "    <input type=\"hidden\" id=\"deslike\" name=\"deslike\" value=%s />\n"
                   "    <input type=\"hidden\" id=\"login\" name=\"login\" value=%s />\n"
                   "    <input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />\n"
                   "    <input type=\"hidden\" id=\"id\" name=\"id\" value=%s />\n"
                   "    <input type=\"submit\" value=\"%d %s%s\" class='post-btn-deslike' />\n"
                   "</form>" ,
                   auxiliarDeslike, login, senha, id, nPostagem.deslike, "Deslike", nPostagem.deslike > 2 ? "s" : "" );

            // Fecha a div btn-container
            printf("</div>");

            // Fecha a div post-container
            printf("</div>");

            //endregion

            i++;
        }
    }
    return 1;
}

int main()
{
    char login[25], senha[45], dados[255], id[10];

    // Lê a entrada padrão
    fgets(dados, sizeof(dados), stdin);

    capturarQuery("login", dados, login);
    capturarQuery("senha", dados, senha);

    // Adquire os dados do usuário
    usuario db;
    getUsuarioByUsrname(login, "../trabalho-4/_registros/usuarios.bin", &db);


    //Agora o programa recebe também o id único do usuário. Ele é necessário para ser armazenado no junto do arquivo (da postagem) quando o usuario fizer alguma postagem
    capturarQuery("id", dados, id);

    printf("%s%c%c\n", "Content-Type:text/html;charset=UTF-8",13,10);

    // Imprime o cabeçalho
    printf("<!DOCTYPE html>\n"
           "<html>\n\n"
           "<head>\n"
           "    <meta charset='UTF-8'>\n"
           "    <title>AKB! - @%s</title>\n"
           "    <link href='../trabalho-4/_estilos/classes.css' rel='stylesheet'>\n"
           "    <link href='../trabalho-4/_estilos/reset.css' rel='stylesheet'>\n"
           "    <link href='../trabalho-4/_estilos/postagens.css' rel='stylesheet'>\n"
           "    <link href='../trabalho-4/_estilos/styles.css' rel='stylesheet'>\n"
           "</head>\n"
           "<body>",
           db.usrname);


    //region Cabeçalho do site

    // Header
    printf("<header class='gradiente-3'>\n"
           "    <h1 class='fnt-lobster text-big'>Azkaboard!</h1>\n"
           "    <a href='../trabalho-4/index.html' class='dot gradiente-btn btn btn-sair'>Sair</a>\n"
           "</header>");

    // Formulário oculto para verificar o próprio perfil
    printf("<form action=\"entrarPerfil.cgi\" method=\"post\" id='checkProfile'>\n"
           "    <input type=\"hidden\" name=\"usuario\" value = %s > <!-- Usrname -->\n"
           "    <input type=\"hidden\" id=\"login\" name=\"login\" value = %s /> <!-- Here -->\n"
           "    <input type=\"hidden\" id=\"senha\" name=\"senha\" value = %s />\n"
           "    <input type=\"hidden\" id=\"id\" name=\"id\" value=%s />"
           "\n"
           "</form>",
           db.usrname, db.usrname, db.password, id );

    // Saudação
    printf("<div class='usr-info'>\n"
           "    <img src='../trabalho-4/_img/%destrelas.png' class='img-pontuacao'>\n"
           "    <span class='usrFullName'>%s</span>\n"
           "    <span class='usrNickName' onclick='document.forms[\"checkProfile\"].submit()'>@%s</span>\n"
           "</div>",
           getPoints(db.id, "../trabalho-4/_registros/usuarios.bin"), db.fullName, db.usrname );

    //endregion

    // Subtopo
    printf("<div class='subtop-container'>");

    // Formulário de busca de perfil
    printf("<div class='top-container' id='search-form-container'>");
    perfil(db.usrname, db.password, id);
    printf("</div>");

    // Campo de postagem
    printf("<div class='top-container' id='post-form-container'>");
    printf("<form action='armazenarPostagem.cgi' method='post'>\n"
           "    <div id=\"subtitle\">\n"
           "        <h2 class='txt-indicator'> Digite aqui a sua mensagem </h2> \n"
           "    </div>\n"
           "    <div id=\"msg\">\n"
           "        <input type='text' id=\"postagem\" name=\"post\" class='input-txt'/>\n"
           "        <input type='submit' class='btn' id='btn-submit' value='Enviar' />\n"
           "    </div>\n"
           "    <input type='hidden' id=\"login\" name=\"login\" value=%s />\n"
           "    <input type='hidden' id=\"senha\" name=\"senha\" value=%s />\n"
           "     <!--agora o id fica como imput escondido-->\n"
           "    <input type='hidden' id=\"id\" name=\"id\" value=%s />\n"
           "</form>",
           login, login, senha, id );
    printf("</div>");

    // Encerra o subtop-container
    printf("</div>");

    // Exibe 'n = 10' postagens na tela
    getLastestPost(STD_POST_AMNT, "../trabalho-4/_registros/registroPostagens.bin", db.usrname, db.password, id);



    // Botão "Carregar mais"
    printf("<form action=\"carregarMais.cgi\" method=\"post\">\n"
           "    <input type=\"submit\" value=\"Carregar mais\" id='btn-carregar-mais'>\n"
           "    <input type=\"hidden\" id=\"login\" name=\"login\" value=%s />\n"
           "    <input type=\"hidden\" id=\"quantidade\" name=\"carregar\" value=\"20\" />\n"
           "    <input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />\n"
           "    <input type=\"hidden\" id=\"id\" name=\"id\" value=%s />\n"
           "</form>",
           db.usrname, db.password, id );

    // Finaliza o documento
    printf ("</body>");
    printf ("</html>");
}
