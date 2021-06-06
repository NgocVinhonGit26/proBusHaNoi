#include "tools.h"

int strtolwr(char str[]){
    int i;
    for( i = 0; i < strlen(str); i++){
        str[i] = tolower(str[i]);
    }
    return 1;
}

void clearBuffer() {
    while(getchar() != '\n');
}

void fClearBuffer(FILE* ptr) {
    while(getc(ptr) != '\n');
}