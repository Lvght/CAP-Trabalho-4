#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POST 7

#include "common.c"

int main() {
	FILE *fp = fopen("../trabalho-4/_registros/usuarios.bin", "rb");
	if (fp == NULL) die("Content-Type: text/html\n\nImpossivel acessar o arquivo");


    printf("Content-Type: text/html\n\n");


	// Cria um objeto do tipo usuário para armazenar as informações do arquivo de dados
	usuario db;

	// Pega os dados da entrada-padrão
    char dados[100];
    fgets(dados, sizeof(dados), stdin);

//    printf("Query completa: %s<br>", dados);

    //prefixo db_ = DataBase, ou seja, se refere aos dados salvos no arquivo
	char usrName[255], db_usrName[255], nome[255], aux[5], password[255];
	int login = 0;

    // Captura os dados digitados pelo usuário
    strcpy(usrName, capturaQuery("usrname", dados));
    strcpy(password, capturaQuery("password", dados));

    // escaneia até achar a combinação ou até o fim do arquivo
    fseek(fp, 0, SEEK_SET);
    fread(&db, sizeof(db), 1, fp);


//    printf("Senha recebida: <br><br>");

    int namecheck = 0;
    int passcheck = 0;

    // Escaneia até o fim do arquivo ou até achar o nome do usuário
	while ( !feof(fp) && !namecheck ) {
        fread(&db, sizeof(db), 1, fp);

//        printf(" | Nome: %s | Senha: %s <br><br>", db.usrname, db.password);

        // strcmp retorna 0 caso ambas as strings sejam iguais
		// compara os dados capturados com os do arquivo

		if ( !strcmp(db.usrname, usrName) )
		    namecheck = 1;

        if ( namecheck && !strcmp(db.password, password) )
            passcheck = 1;

        if (passcheck && namecheck)
            login = 1;
	}

	// resultados

    // Imprime o cabeçalho
    printf("<!DOCTYPE html>\n"
           "<html>\n\n"
           "<head>\n"
           "    <meta charset='UTF-8'>\n"
           "    <title>Login nao foi aceito</title>\n"
           "    <link href='../trabalho-4/_estilos/classes.css' rel='stylesheet'>\n"
           "    <link href='../trabalho-4/_estilos/reset.css' rel='stylesheet'>\n"
           "    <link href='../trabalho-4/_estilos/postagens.css' rel='stylesheet'>\n"
           "    <link href='../trabalho-4/_estilos/styles.css' rel='stylesheet'>\n"
           "</head>\n"
           "<body>"
           );


	// Login recusado - O usuário recebe a chance de tentar novamente
	if (!login) {

        // Header
        printf("<header class='gradiente-3'>\n"
               "    <h1 class='fnt-lobster text-big'>Azkaboard!</h1>\n"
               "    <a href='../trabalho-4/index.html' class='dot gradiente-btn btn btn-sair'>Sair</a>\n"
               "</header>");

        printf ("<div class='erro'>\n"
                "    <h1>Seus dados nao puderam ser verificados</h1>\n"
                "    <p>A combinaçao de usuario e senha que voce forneceu nao pode ser verificada em nossos registros. Tente novamente.</p>\n"
                "</div>");

        printf("<form method=\"post\" action=\"login.cgi\" id=\"login-form\">\n\n"
               "    <label for=\"usrname\">Nome de usuario</label> <br>\n"
               "    <input name=\"usrname\" type=\"text\" id=\"usrname\" class=\"input-txt\" autocomplete=\"off\" pattern=\"[a-zA-Z]*\" maxlength=\"20\" required>\n\n"
               "    <label for=\"password\">Senha</label> <br>\n    <input name=\"password\" type=\"password\" id=\"password\" class=\"input-txt\" autocomplete=\"off\" pattern=\"[a-zA-Z]*\" maxlength=\"40\" required>\n\n"
               "    <input type=\"submit\" value=\"Enviar\">\n"
               "</form>");

//	    printf("<form method=\"post\" action=\"../cgi-bin/login.cgi\" id=\"login-form\">\n\n    <label for=\"usrname\">Nome de usuario</label> <br>\n    <input name=\"usrname\" type=\"text\" id=\"usrname\" class=\"input-txt\" autocomplete=\"off\" pattern=\"[a-zA-Z]*\" maxlength=\"20\" required>\n\n    <label for=\"password\">Senha</label> <br>\n    <input name=\"password\" type=\"password\" id=\"password\" class=\"input-txt\" autocomplete=\"off\" pattern=\"[a-zA-Z]*\" maxlength=\"40\" required>\n\n    <input type=\"submit\" value=\"Tentar novamente\">\n</form>\n\n"
//	        "<hl>"
//	        "<br>"
//              "<br>"
//              "<a href=\"../trabalho-4/index.html\">Voltar para a página inicial</a>"
//        );

    // Login aprovado
    } else {

        // Imprime um formulário oculto para preservar as informações do usuário
        printf(
        "<form method=\"post\" action=\"postagem.cgi\" id=\"autosend\">"
            "<input type=\"hidden\" value=\"%s\" name=\"login\">"
            "<input type=\"hidden\" value=\"%s\" name=\"senha\">"
            "<input type=\"hidden\" value=\"%d\" name=\"id\">"
        "</form>",
        usrName, password, db.id
        );

//        printf("<br><strong>%d</strong>", db.id);

        printf("<script>\n"
               "    document.getElementById('autosend').submit();\n"
               "</script>");
	}
}