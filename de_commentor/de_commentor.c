#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
enum State {
    NORMAL,START_SLASH, BETWEEN_SLASH,END_SLASH,BETWEEN_DOUBLE_QOUTE, DOUBLE_QOUTE_BACKSLASH, BETWEEN_SINGLE_QOUTE,SINGLE_QOUTE_BACSLASH
} ;

void normalStateHandler(enum State *state, char character) {
    
    if (character == '\"') {
        *state = BETWEEN_DOUBLE_QOUTE;
    } 
    else if (character == '\'') {
        *state = BETWEEN_SINGLE_QOUTE;
    } 
    else if (character == '/') {
        *state = START_SLASH;
    }

    if (*state != START_SLASH) {
        putchar(character);
    }
};
void betweenDoubleQouteStateHandler(enum State *state, char character) {

    if (character == '\\') {
        *state = DOUBLE_QOUTE_BACKSLASH;
    }
    else if (character == '\"') {
        *state = NORMAL;
    }
    putchar(character);

};

void doubleQouteBackSlashStateHandler(enum State *state, char character) {

    *state = BETWEEN_DOUBLE_QOUTE;
    putchar(character);

};

void betweenSingleQouteStateHandler(enum State *state, char character) {

    if (character == '\\') {
        *state = SINGLE_QOUTE_BACSLASH;
    } 
    else if (character == '\'') {
        *state = NORMAL;
    }
    putchar(character);

};

void singleQouteBackSlashStateHandler(enum State *state, char character) {

    *state = BETWEEN_SINGLE_QOUTE;
    putchar(character);

};
void slashStateHandler(enum State *state, char character,int *commentStartLine,int *curruntLine) {

    if (character == '*') {
        *state = BETWEEN_SLASH;
        *commentStartLine = *curruntLine;
    } 
    else if ((character == '/') && *state == START_SLASH) {
        putchar(character);
        *state = START_SLASH;
    }  
    else if (character == '/') {
        *state = START_SLASH;
    } 
    else {
        *state = NORMAL;
    }

    if (*state == NORMAL) {
        putchar('/');
        putchar(character);
    }

};
void BetweenSlashStateHandler(enum State *state, char character) {

    if (character == '*') {
        *state = END_SLASH;
    }

    if (character == '\n') {
        putchar(character);
    }

};
void EndSlashStateHandler(enum State *state, char character) {

    if (character == '/') {
        *state =NORMAL;
    } else if (character == '*') {
        *state = END_SLASH;
    } else {
        *state = BETWEEN_SLASH;
    }

    if (*state == NORMAL) {
        putchar(' ');
    }

};
void checkCompletion(enum State *state, int lastCommentline) {

    if (*state == BETWEEN_SLASH || *state == END_SLASH) {
        fprintf(stderr, "Error: Comment started on line  %d  is unterminated comment.\n", lastCommentline);
    }

}
void countLine(int *line, char character) {

    if (character == '\n') {
        *line += 1;
    }

}
void main(){

   enum State state = NORMAL;

    int line = 0;   
    int commentStartLine = 0;   

    for(;;){
        char character = getchar();
        countLine(&line, character);

        if (character == EOF) {
            checkCompletion(&state,  commentStartLine);
            break;
        }

        switch (state) {
            case NORMAL:
                normalStateHandler(&state, character);
            break;
            case BETWEEN_DOUBLE_QOUTE:
                betweenDoubleQouteStateHandler(&state, character);
            break;
            case DOUBLE_QOUTE_BACKSLASH:
                doubleQouteBackSlashStateHandler(&state, character);
            break;
            case BETWEEN_SINGLE_QOUTE:
                betweenSingleQouteStateHandler(&state, character);
            break;
            case SINGLE_QOUTE_BACSLASH:
                singleQouteBackSlashStateHandler(&state, character);
            break;
            case START_SLASH:
                slashStateHandler(&state, character,&commentStartLine,&line);
            break;
            case BETWEEN_SLASH:
                BetweenSlashStateHandler(&state, character);
            break;
            case END_SLASH:
                EndSlashStateHandler(&state, character);
            break;
        }
    }
}