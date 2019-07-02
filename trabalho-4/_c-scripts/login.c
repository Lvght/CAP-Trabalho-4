#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POST 5

void die(const char msg[255]) {
	printf("\n%s\n", msg);
	exit(1);
}

void capturaQuery (char varname[15], char query_string[255], char resposta[10])
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

int main() {
	FILE *fp = fopen("../trabalho-4/_registros/usuarios.txt", "a+");
	if (fp == NULL) die("Impossivel acessar o arquivo");
	char *dados = getenv("QUERY_STRING");

    //prefixo db_ = DataBase, ou seja, se refere aos dados salvos no arquivo
	char usrName[255], db_usrName[255], nome[255], aux[5];
	int pin, db_pin;
	int login = 0;

    // Captura os dados digitados pelo usuário
	capturaQuery("usrname", dados, usrName);
	capturaQuery("pin",     dados, aux    );
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
	printf("Content-Type: text/html\n\n");

	if (login) {

	    // LOGIN APROVADO
		// printf("Login aprovado!\nBem-vindo, %s.\n", nome);

        // Gera o arquivo de destino temporario do usuario
        FILE *fp = fopen("../trabalho-4/_registros/vinql-4578754acss.html", "w");

        // Abre o arquivo de postagens
        FILE *posts = fopen("../trabalho-4/_registros/postagens.txt", "r");


        /**** Escreve o conteudo adequado no arquivo de destino ****/
        char nomePost[100], msgPost[255];

        for (int i = 0; i < MAX_POST; i++) {
            fscanf(posts, "%*d %s %*d %*d %[^\n]", nomePost, msgPost);
            fprintf(fp, "Autor: %s<br>", nomePost);
            fprintf(fp, "Mensagem: %s<br>", msgPost);
        }

        // Redireciona o usuario para o arquivo de destino
		printf("<script>window.location='../trabalho-4/_registros/vinql-4578754acss.html'</script>");

	} else {

	    // LOGIN RECUSADO
		printf("Login recusado\n");

	}
}