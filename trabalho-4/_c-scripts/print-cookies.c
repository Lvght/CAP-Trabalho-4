#include <stdio.h>
#include <stdlib.h>

int main() {
    char *dados;
    dados = getenv("HTTP_COOKIES");

    printf("%s%c%c\n","Content-Type:text/html;charset=UTF-8",13,10);
    printf("<script>alert(document.cookie);</script>");
    printf("Cookies: %s", dados);
}