/*
 * Arquivo para recursos comuns a multiplos scripts.
 * Criado por: Vinicius
*/

typedef struct {
    int id;
    char usrname[25];
    char fullName[60];
    char password[45];
    char profilePicture[255];
    int likes;
    int deslikes;
} usuario;

typedef struct {
    int ID;
    int ID_Origem;
    int like;
    int deslike;
    char msg[180];
} postagem;


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

int serial(const char * path) {
    // Devolve o ID do último usuário cadastrado incrementado em 1.

    usuario x;
    FILE *fp;
    int output;

    // Abre o arquivo especificado | Volta -1 caso a abertura falhe
    if ( (fp = fopen(path, "rb")) == NULL ) return -1;

    // busca o ultimo usuario cadastrado
    fseek(fp, -sizeof(x), SEEK_END);
    fread(&x, sizeof(x), 1, fp);

    return x.id + 1;
}

void getUsuario(int usrID, const char path[100], usuario * result) {
    usuario aux;
    FILE *fp;
    fp = fopen(path, "rb");

    fseek(fp, usrID*sizeof(aux), SEEK_SET);
    fread(&aux, sizeof(aux), 1, fp);

    result->id = aux.id;
    result->likes = aux.likes;
    result->deslikes = aux.deslikes;

    strcpy(result->usrname, aux.usrname);
    strcpy(result->fullName, aux.fullName);
    strcpy(result->profilePicture, aux.profilePicture);
    strcpy(result->password, aux.password);
}

void getUsuarioByUsrname(const char * usrName, const char path[100], usuario * result) {
    usuario aux;
    FILE *fp;
    fp = fopen(path, "rb");

    int found = 0;

    while (!feof(fp) && !found) {
        fread(&aux, sizeof(aux), 1, fp);

        if ( !strcmp(usrName, aux.usrname) )
            found = 1;
    }

    result->id = aux.id;
    result->likes = aux.likes;
    result->deslikes = aux.deslikes;

    strcpy(result->usrname, aux.usrname);
    strcpy(result->fullName, aux.fullName);
    strcpy(result->profilePicture, aux.profilePicture);
    strcpy(result->password, aux.password);
}

//void getUsr(FILE *fp, int usrID, usuario * result) {
//    usuario aux;
//    fseek(fp, usrID*sizeof(aux), SEEK_SET);
//    fread(&aux, sizeof(aux), 1, fp);
//
//    result->id = aux.id;
//    result->likes = aux.likes;
//    result->deslikes = aux.deslikes;
//    result->pin = aux.pin;
//
//    strcpy(result->usrname, aux.usrname);
//    strcpy(result->fullName, aux.fullName);
//    strcpy(result->profilePicture, aux.profilePicture);
//}