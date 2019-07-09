#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funções utilizadas em praticamente todos os scripts.
#include "common.c"

int main() {

    printf("Content-Type: text/html\n\n");

    /*** DECLARAÇÕES ***/
    char dados[100], aux[5];
    usuario x;
    usuario auxiliar;
    FILE *ptrRegistro;

    /********************/

    // Lê os dados recebidos da página anterior
    fgets(dados, sizeof(dados), stdin);
   // printf("%s", dados);

    /*** Extrai os dados recebidos da página HTML e monta o objeto ***/
    // ID
    x.id = serial("../trabalho-4/_registros/usuarios.bin");
    if (x.id == -1)
        x.id++;
   // printf("<p>ID: %d", x.id);

    // Nome de usuário
    strcpy(x.usrname, capturaQuery("usrname", dados));
  //  printf("<br><br>Nome de usuario: %s", x.usrname);

    // Nome completo
    strcpy(x.fullName, capturaQuery("fullname", dados));
   // printf("<br><br>Nome completo: %s", x.fullName);

    // Senha
    strcpy(x.password, capturaQuery("password", dados));

    ptrRegistro = fopen("../trabalho-4/_registros/usuarios.bin", "a+b");
    if (ptrRegistro == NULL)
    {
        printf ("Erro ao abrir o arquivo de usuarios.");
        exit(1);
    }

    int i = 0;
    while (!feof(ptrRegistro) && strcmp(auxiliar.usrname, x.usrname) != 0)
    {
        fseek(ptrRegistro, i*sizeof(x), SEEK_SET);
        fread(&auxiliar, sizeof(x), 1, ptrRegistro);
        i++;
    }
    fclose(ptrRegistro);
    if (strcmp(auxiliar.usrname, x.usrname) == 0)
    {
       printf(" <!DOCTYPE html> "
              "  <html> "
              "  <head> "
              "      <meta charset='UTF-8'> "
              "      <title>AKB! - Usrname indisponivel :/</title> "
              "      <link href='../trabalho-4/_estilos/reset.css' rel='stylesheet'> "
              "      <link href='../trabalho-4/_estilos/classes.css' rel='stylesheet'> "
              "      <link href='../trabalho-4/_estilos/postagens.css' rel='stylesheet'> "
              "      <link href='../trabalho-4/_estilos/styles.css' rel='stylesheet'> "
              "  </head> "
              "  <body> "
              "  <header class='gradiente-3'> "
              "      <h1 class='fnt-lobster text-big'><a class='invisible-link' href='../trabalho-4/index.html'>Azkaboard!</a></h1> "
              "  </header> "
              "  <div class='usr-info initial-info'> "
              "      <span class='usrFullName'>Crie sua conta</span> "
              "      <span>Cadastre-se abaixo para acessar o seu perfil.</span> "
              "      <span style='background-color: #cccccc; padding: 15px 0; border-radius: 5px'>Excelente escolha, mas alguem ja usou esse nome de usuario. Escolha outro. </span> "
              "      <form method=\"post\" action=\"../cgi-bin/cadastro.cgi\" id=\"login-form\"> "
              "          <label for=\"usrname\">Nome de usuario (apenas letras)</label> <br> "
              "          <input name=\"usrname\" type=\"text\" id=\"usrname\" class=\"input-txt\" autocomplete=\"off\" pattern=\"[a-zA-Z]*\" maxlength=\"20\" required> "
              "           <label for=\"fullname\">Nome completo</label> <br> "
              "           <input name=\"fullname\" type=\"text\" id=\"fullname\" class=\"input-txt\" autocomplete=\"off\" pattern=\"^[a-zA-Z]*$\" maxlength=\"50\" required> "
              "           <label for=\"password\">Senha (apenas letras)</label> <br> "
              "           <input name=\"password\" type=\"password\" id=\"password\" class=\"input-txt\" autocomplete=\"off\" pattern=\"[a-zA-Z]*\" maxlength=\"40\" required> "
              "         <input type=\"submit\" value=\"Cadastre-se\"> "
              "       </form> "
              "   </div> "
              "   </body> "
              "  </html> "
              );
    }

    else
    {


        // Likes
        x.likes = 0;

        // Deslikes | Valor padrão = 1 para evitar posterior divisão por 0
        // Usuário começa sua conta com nenhuma estrela
        x.deslikes = 1;

        // Imagem de perfil | O usuário poderá alterar isso posteriormente
        strcpy(x.profilePicture, "default.jpg");

        // Insere o cabeçalho HTML

        printf("<!DOCTYPE html>\n"
               "<html>\n"
               "<head>\n"
               "    <meta charset='UTF-8'>\n"
               "    <title>Cadastro</title>\n"
               "</head>\n"
               "<body>\n"
               );

    //    exit(0);

        printf("<p>Os dados recebidos foram:</p>\n<p>Nome completo: %s</p>\n<p>Nome de usuário: %s</p>\n<p>Senha: %s</p>",
        x.fullName, x.usrname, x.password);

        /*** Salva os dados no arquivo de registro ***/
        // Abertura do arquivo e exibição de erro caso a abertura falhe
        if ( ( ptrRegistro = fopen("../trabalho-4/_registros/usuarios.bin", "a+b") ) == NULL)
        {
            printf("Não foi possível acessar o arquivo de registro! Execute o utilitario de inicializacao.\n");
            exit(1);
        }

        // salvamento
        int check = 0;
        fseek(ptrRegistro, (x.id)*sizeof(x), SEEK_SET);
        check = fwrite(&x, sizeof(x), 1, ptrRegistro);

        if (!check) {
            printf("<p>Falha ao registrar</p>");
        }


        /*** Exibição do resultado ***/


        printf ("<form action=\"postagem.cgi\" method=\"post\" id='autosend' >");
        printf ("<input type=\"hidden\" id=\"login\" name=\"login\" value=%s />", x.usrname);
        printf ("<input type=\"hidden\" id=\"senha\" name=\"senha\" value=%s />", x.password);
        printf ("<input type=\"hidden\" id=\"id\" name=\"id\" value=%d />", x.id);
        printf ("<input type=\"submit\" value=\"Ir para a dashboad\"/>");
        printf ("</form>");

        printf("<script>document.getElementById('autosend').submit();</script>");

        printf ("</html>");
    }

}
