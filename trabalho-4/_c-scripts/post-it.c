#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../trabalho-4/_c-scripts/common.c"

typedef struct x
{
    int ID;
    char usrOrigem[15];
    int like, deslike;
    char msg[180];
} postagem;

int main() {
    // lê os dados da entrada-padrão
    char fullQuery[1000];
    fgets(fullQuery, sizeof(dados), stdin);

    // declarações
    char usrName[15], mgs[180], aux[15];
    int pin;

    // Inicializa as strings
    memset(usrName, '\0', sizeof(usrName));
    memset(msg, '\0', sizeof(msg));
    memset(aux, '\0', sizeof(aux));

    // Extrai os dados obtidos na entrada-padrão
    strcpy(usrName, capturaQuery("usrname", fullQuery));
    strcpy(aux, capturaQuery("pin", fullQuery));
    pin = atoi(aux);

    // Monta a postagem
    postagem post;
    post.ID = -1;   //todo Fazer função para identificar o ID correto
    post.usrOrigem = usrName;
    post.like = 0;
    post.deslike = 0;

    // Abre o arquivo de interação da postagem
    FILE *interact = NULL;
    char path[255];
    strcat()

    // Salva a postagem no arquivo de postagens
    FILE *posts = fopen("../trabalho-4/_posts.bin", "rb+");
    fseek(posts, 0, SEEK_END);
    fwrite(&post, sizeof(post), posts);

}