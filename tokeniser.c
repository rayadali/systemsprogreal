/*
 * tokenizer.c
 *Rayad Ali
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

//struct TokenizerT_ {
typedef struct TokenizerT_{
    //char separatorChars[50]; // first command line string storage
   // char tokens[]; //second command line string.  pass by reference
    // using pointers as required in the PDF outline
    char *separators;
    char *tokens;
}TokenizerT;
//typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

//my notes for using arrow notation
//foo->bar is equivalent to (*foo).bar -- it gets the member called bar from the struct that foo points to.
TokenizerT *TKCreate(char *separators, char *ts) {
/* this was code before I started using pointers    
int ;
    //count number of separators
//create array
char sepArray[strlen(separators)]; // length is determined by the length of string passed into the function
int countSep = 0; //keep count of the number and store, initialized to 0
*/

// using memcpy over strcpy for performance
// note for how malloc, memcpy works
// malloc creates a memory block of size in parameter
// void * memcpy ( void * destination, const void * source, size_t num );
//separators
    /*
    TokenizerT *tokeniser;
    tokeniser = (TokenizerT *) malloc(sizeof *tokeniser);
    size_t slength;
    slength = strlen(separators) + 1;
    char *sep;
    sep = malloc(slength);
    memcpy(sep, separators, slength);
    tokeniser->separators = sep;
     */
    
    //testing
    
    printf("from create %s\n",separators);
    printf("from create %s\n",ts);
    
    TokenizerT *tokeniser =  malloc(sizeof *tokeniser);
    size_t slength = (strlen(separators)+1);
    char *sep = malloc(slength); // creating memory space we add +1 for the \0
    memcpy(sep, separators, slength); // copies value in separators paramater passed into funtion to sep, length size of separators
    tokeniser->separators = sep;
    
    //
    
    printf("after create %s\n",sep);

//tokens
    size_t tlength = strlen(ts)+1;
    char *tok = malloc(tlength);
    memcpy(tok, ts, tlength);
    tokeniser->tokens = tok;
    
    //testing
    
    printf("after create %s\n",tok);
    
  return tokeniser;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
//  using the free function --deallocat memory allocated from malloc 
// how free works -- void free (void* ptr);
    free(tk->tokens); // free in reverse order of malloc
    free(tk->separators);
    free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

/*
newline (0x0a) \n
horizontal tab (0x09) \t
vertical tab (0x0b) \v
backspace (0x08) \b
carriage return (0x0d) \r
form feed (0x0c) \f
audible alert (0x07) \a
backslash (0x5c) \\
double quote (0x22) \”
*/

char *TKGetNextToken(TokenizerT *tk) {
//need to keep track of the separators given in the first string and exclude them from the second string
//possible implemention of the printing of each token on a new line in this section
    //creating variables
    char *nextToken = (char *)malloc(strlen(tk->tokens)+1);
    char *delimiter, *tokenPtr, *check = NULL;
    //*tokenPtr = tk->tokens; //getting error 
    //*nextToken = NULL; // getting error
    tokenPtr = tk->tokens;
    nextToken = NULL;
    
    //error handling for empty second string
    if(!tk->tokens) return NULL;
    if(tk->tokens == '\0') return NULL;
    //scanning
    while(tokenPtr[0] != '\0') //loop delimiter string until end
    {
        delimiter = tk->separators; //storing the separators to look out for
        //if ((tk->tokens) == *delimiter){ //match , getting error 
        if ((tk->tokens) == delimiter){
            if(tokenPtr == check){
                check++;//skip this
                break; // end this iteration
            }
            else{
                *tokenPtr = '\0'; //set to null character
                tokenPtr++; //move to next
                //if((nextToken = malloc(sizeof check) + 1 * sizeof(char) != NULL)){ // getting error
                if((*nextToken = malloc(sizeof check) + 1 * sizeof(char) != NULL)){
                    size_t tokenLength = strlen(check)+1; 
                    memcpy(nextToken, check, tokenLength); //getting the token and storing again using memcpy
                    tk->tokens = tokenPtr;
                    //testing
                    printf("end of%s\n",nextToken);
                    return nextToken;
                }
            }
        }
        else{
            delimiter++; // check for next
            }
        }
    tokenPtr++;// iterate to next delimiter
    if((nextToken = malloc(sizeof check) + 1 * sizeof(char)) != NULL){
        size_t tokenLength = strlen(check)+1; 
        memcpy(nextToken, check, tokenLength);
    }
    tk->tokens = tokenPtr;
    return nextToken;
}


/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
    
    // testing
    char name[]="ray";
    printf("%s\n",name);
    
    if (argc != 3) {
        printf("Invalid input. Require two string inputs\n");
        return 1;
    }
    char *token;
    char *seps = argv[1];
    //test vars getting stored correctly
    printf("%s\n",seps);
    char *string = argv[2];
    printf("%s\n",string);
    TokenizerT *stream = TKCreate(seps, string);
    while ((token = TKGetNextToken(stream)) != NULL ) {
        printf("%s\n", token);
        free(token); //freeing after processed
    }

    //free(token);
    TKDestroy(stream);

  return 0;
}
