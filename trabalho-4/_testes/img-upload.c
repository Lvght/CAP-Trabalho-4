#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>

int main() {
    printf("Content-Type: text/html\n\n");

    FILE *f2;

    unsigned char b;

    printf("This program copies a given file.\n\n");
    printf("Enter file name:\n\n");
    printf("Processing..\n\n");
    f2=fopen("output.jpg","wb"); /*Open the file Output*/

    b=5;


    while (stdin != EOF) {
        fread(&b,1,1,stdin);
        fwrite(&b,1,1,f2);
    }

    fclose(f2); /*close the file output*/
    printf("Done. File output.out created.");
}
