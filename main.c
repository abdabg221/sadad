#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hangman.h"

int main() {
    char secret[100];  
    if (get_word(secret) != 0) {
        return 1;      }

    hangman(secret);

    return 0;
}
