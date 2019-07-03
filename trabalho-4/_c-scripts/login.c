#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POST 7

void die(const char msg[255]) {
	printf("\n%s\n", msg);
	exit(1);
}

char *capturaQuery (const char varname[15], const char query_string[255])
{
    char *p;
    char resposta[500];
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

    char *out = resposta;
    return out;
}

int main() {
	FILE *fp = fopen("../trabalho-4/_registros/usuarios.txt", "a+");
	if (fp == NULL) die("Impossivel acessar o arquivo");

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
	while (!feof(fp) && !login) {
		fscanf(fp, "%s %d %*d %*d %*s %[^\n]\n", db_usrName, &db_pin, nome);

		// strcmp retorna 0 caso ambas as strings sejam iguais
		// compara os dados capturados com os do arquivo
		if (!strcmp(db_usrName, usrName) && db_pin == pin)
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

	if (!login) {

	    // LOGIN RECUSADO
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

	} else {
	    // LOGIN APROVADO
		printf("Login aprovado! Bem-vindo, %s.<br><br>", nome);
		printf(
		    "<form method=\"post\" action=\"post-it.cgi\">"
		        "<input name=\"usrname\" type=\"hidden\" value=\"%s\">"
		        "<input name=\"pin\" type=\"hidden\" value=\"%d\">"
		        "<input name=\"msg\" placeholder=\"Digite o post aqui\">"
		        "<input type=\"submit\" value=\"Enviar\">"
		    "</form>"
		    "<br>"
		    "<br>"
		    "<br>",

		    usrName, pin
		);

        // Gera o arquivo de destino temporario do usuario
        FILE *fp;
        /* todo O nome do arquivo deve ser randomizado. O mesmo arquivo deve ser excluído no logout */
        if ( (fp = fopen("../trabalho-4/_registros/vinql-4578754acss.html", "w")) == NULL )
            printf(
                "<strong>Abertura do arquivo de acesso falhou!</strong>"
                "<script>window.stop()</script>"
             );

        // Abre o arquivo de postagens
        FILE *posts;
        if ( (posts = fopen("../trabalho-4/_registros/postagens.txt", "r")) == NULL )
            printf(
                "<strong>Abertura do arquivo de postagens falhou!</strong>"
                "<script>window.stop()</script>"
            );


        /**** Escreve o conteudo adequado no arquivo de destino ****/
        // todo De fato, escrever as coisas em um arquivo e redirecionar.
        char fullQuery[522], postName[522], postMsg[522];
        char auxNome[522], auxMsg[522];

        for (int i = 0; i < MAX_POST; i++) {
            // Pega a query completa
            fgets(fullQuery, sizeof(fullQuery), posts);
            printf("Query completa: %s<br>", fullQuery);

            // salva as variaveis
            strcpy(postName, capturaQuery("user", fullQuery));
            strcpy(postMsg, capturaQuery("msg", fullQuery));

            printf("<strong>Postado por</strong>: %s<br>", postName);
            printf("<strong>Mensagem</strong>: %s<br><hr>", postMsg);
        }

	}
}