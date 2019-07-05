/*
 * Arquivo para recursos comuns a multiplos scripts.
 * Criado por: Vinicius
*/

/*** Protótipos ***/
void die(const char);
char * capturaQuery (const char, const char);


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

typedef struct {
    int id;
    char usrname[25];
    char fullName[60];
    int pin;
    char profilePicture[255];
    int likes;
    int deslikes;
} usuario;