#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "hangman.h"
#include <string.h>


int get_word(char secret[]){
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if( fp == NULL ){
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do{
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;
        // seek to the random position of file
        fseek(fp, random, SEEK_SET);
        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if( result != EOF )
            break;
    }while(1);

    fclose(fp);

    return 0;
}
/**
 * Funkcia detekuje, ci hrac uhadol heslo
 * @parameter: pole secret obsahuje obsahuje hracom hadane slovo
 * @parameter: pole letters_guessed retazec hadanych znakov
 * @return: 1, ak je slovo uhadnute, 0 v ostatnych pripadoch.
 */
int is_word_guessed(const char secret[], const char letters_guessed[]) {

    if (!secret || !letters_guessed) {
        return 0;
    }

    int i = 0, c = strlen(secret), g = 0;

    while (i < strlen(secret)) {
        int j = 0;
        while (j < strlen(letters_guessed)) {
            if (secret[i] == letters_guessed[j]) {
                g++;
                break;
            }
            j++;
        }
        i++;
    }

    return (g == c) ? 1 : 0;
}

/**
 * Funkcia vrati retazec reprezentujuci hadane slovo
 * Tento retazec obsahuje viditelne pismena, ktore uz boli hracom uhadnute na spravnej pozicii.
 * Ostatne neuhadnute znaky su zobrazene znakom '_'.
 * @parameter: pole secret obsahuje retazec malych pismen tajneho slova
 * @parameter: pole letters_guessed obsahuje retazec znakov, ktore hrac uz hadal
 * @parameter: pole guessed_word reprezentuje aktualny stav hadaneho slova (vystupny parameter)
 */
void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]) {

    int i = 0;
    while (i < strlen(secret)) {
        int j = 0;
        int found = 0;
        while (j < strlen(letters_guessed)) {
            if (secret[i] == letters_guessed[j]) {
                guessed_word[i] = letters_guessed[j];
                found = 1;
                break;
            }
            j++;
        }
        if (!found) {
            guessed_word[i] = '_';
        }
        i++;
    }
}

/**
 * Vrati znaky, ktore su este k dispozicii pre hadanie
 * Funkcia vrati mnozinu malych pismen v alfabetickom poradi. 
 * Tato mnozina obsahuje vsetky znaky, okrem tych, ktore uz boli hadane.
 * @parameter: pole letters_guessed obsahuje male pismena, ktore uz boli hadane
 * @parameter: pole available_letters obsahuje pismena, ktore su este k dispozicii
 */
void get_available_letters(const char letters_guessed[], char available_letters[]) {

    const char* alpha = "abcdefghijklmnopqrstuvwxyz";
    int lg = strlen(letters_guessed), la = strlen(alpha), guessed = 0, i = 0;

    while (i < la) {
        int j = 0;
        while (j < lg) {
            if (alpha[i] == letters_guessed[j]) {
                break;
            }
            j++;
        }
        if (j == lg) {
            available_letters[guessed++] = alpha[i];
        }
        i++;
    }

    available_letters[guessed] = '\0';
}

/**
 * Startuje interaktivitu vasej hry.
 *
 * @parameter: pole znakov secret s hadanym slovom
 * Funkcia je uz vytvorena, mozete ju volat z hlavneho programu
 * Ak sa vam nepaci, mozete si ju aj preprogramovat
 */
void hangman(const char secret[]) {
    int nc = 8;
    int c;
    char letters_guessed[100] = "";
    char available_letters[30];
    char guessed_word[strlen(secret) + 1];

    strcpy(available_letters, "abcdefghijklmnopqrstuvwxyz");
    int i = 0;
    while (i < strlen(secret)) {
        guessed_word[i] = '_';
        i++;
    }
    guessed_word[i] = '\0';

    printf("Welcome to the game, Hangman!\n");
    printf("I am thinking of a word that is %lu letters long.\n", strlen(secret));
    printf("-------------\n");
    while (!is_word_guessed(secret, letters_guessed) && nc > 0) {
        printf("You have %d guesses left.\n", nc);
        printf("Available letters: %s\n", available_letters);
        printf("Please guess a letter: ");
        c = getchar(); 
        getchar();

        if (strchr(letters_guessed, c) == NULL) {
            letters_guessed[strlen(letters_guessed)] = c;
            letters_guessed[strlen(letters_guessed) + 1] = '\0';
            get_available_letters(letters_guessed, available_letters);

            if (strchr(secret, c) == NULL) {
                printf("Oops! '%c' is not a valid letter: %s\n",c, guessed_word);
                nc--;
            } else {
                get_guessed_word(secret, letters_guessed, guessed_word);
                printf("Good guess:  %s\n", guessed_word);
            }

            i = 0;
            while (i < strlen(guessed_word)) {
                printf("%c ", guessed_word[i]);
                i++;
            }
            printf("\n");

        } else {
            printf("Oops! You've already guessed that letter: %s\n", guessed_word);
            i = 0;
            while (i < strlen(guessed_word)) {
                printf("%c ", guessed_word[i]);
                i++;
            }
            printf("\n");
        }


                printf("-------------\n");
    }

    if (!is_word_guessed(secret, guessed_word)) {
        printf("Sorry, bad guess. The word was %s.\n", secret);
    } else {
        printf("Congratulations, you won!\n");
    }
}


