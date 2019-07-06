#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POST 7

#include "common.c"

int main() {
	FILE *fp = fopen("../trabalho-4/_registros/usuarios.bin", "rb");
	if (fp == NULL) die("Impossivel acessar o arquivo");


	// Cria um objeto do tipo usuário para armazenar as informações do arquivo de dados
	usuario db;

	// Pega os dados da entrada-padrão
	// todo E se a entrada-padrão estiver vazia?
    char dados[100];
    fgets(dados, sizeof(dados), stdin);

    //prefixo db_ = DataBase, ou seja, se refere aos dados salvos no arquivo
	char usrName[255], db_usrName[255], nome[255], aux[5];
	int pin, db_pin;
	int login = 0;

    // Captura os dados digitados pelo usuário
    strcpy(usrName, capturaQuery("usrname", dados));
    strcpy(aux, capturaQuery("pin", dados));
	pin = atoi(aux);

    // escaneia até achar a combinação ou até o fim do arquivo
    fseek(fp, 0, SEEK_SET);
    fread(&db, sizeof(db), 1, fp);

    printf("Content-Type: text/html\n\n");

	while ( !feof(fp) && !login ) {
        fread(&db, sizeof(db), 1, fp);

        printf(" | Nome: %s | Senha: %d <br><br>", db.usrname, db.pin);

//		// strcmp retorna 0 caso ambas as strings sejam iguais
//		// compara os dados capturados com os do arquivo
		if (!strcmp(db.usrname, usrName) && db.pin == pin)
			login = 1;
	}

	// resultados
	printf(
	    "Content-Type: text/html\n\n"
	    "<!doctype html>"
        "<html>"

        "<head>"
            "<meta charset=\"UTF-8\">"
            "<title>Azkaboard</title>"
        "</head>"
	);

	// Login recusado - O usuário recebe a chance de tentar novamente
	if (!login) {

	    printf(
	        "<h1>Não foi possível verificar seu login</h1>"
	        "<p>Verifique seus dados e tente novamente</p>"
	        "<form method=\"post\" action=\"login.cgi\">"
	            "<input name=\"usrname\" placeholder=\"Nome de usuário\">"
	            "<input name=\"pin\" placeholder=\"Código de acesso\">"
	            "<input type=\"submit\" value=\"Fazer login\">"
	        "</form>"
	        "<hl>"
	        "<br>"
	        "<br>"
	        "<a href=\"../trabalho-4/index.html\">Voltar para a página inicial</a>"
	    );

    // Login aprovado
    } else {

        // Imprime um formulário oculto para preservar as informações do usuário
        printf(
        "<form method=\"post\" action=\"dashboard.cgi\" id=\"autosend\">"
            "<input type=\"hidden\" value=\"%d\" name=\"usrID\">"
        "</form>",
        db.id
        );

        printf("<br><strong>%d</strong>", db.id);

        printf("<script>\n    document.forms[\"autosend\"].submit();\n</script>");
	}
}