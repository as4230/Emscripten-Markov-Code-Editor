#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// A struct representing a single token in the code
typedef struct {
    char* name; // The name of the token
    int is_end; // 1 if this token represents the end of a line, 0 otherwise
} Token;

// A struct representing a list of tokens
typedef struct {
    Token** tokens; // An array of tokens
    int length; // The number of tokens in the list
} TokenList;

// A struct representing a transition between tokens
typedef struct {
    Token* previous3; // The third previous token in the sequence
    Token* previous2; // The second previous token in the sequence
    Token* previous1; // The previous token in the sequence
    Token* source; // The source token of the transition
    Token* destination; // The destination token of the transition
    int count; // The number of times this transition occurs in the input code
} Transition;

// A struct representing a list of transitions
typedef struct {
    Transition** transitions; // An array of transitions
    int length; // The number of transitions in the list
} TransitionList;

TransitionList* create_transition_list() {
    TransitionList* transition_list = malloc(sizeof(TransitionList));
    if (transition_list == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for transition list\n");
        exit(EXIT_FAILURE);
    }
    transition_list->transitions = NULL;
    transition_list->length = 0;
    return transition_list;
}

Token* create_token(char* name, int is_end) {
    //printf("NEM: '%s'", name);
    Token* token = (Token*) malloc(sizeof(Token));
    token->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
    strcpy(token->name, name);
    token->is_end = is_end;
    return token;
}

TokenList* create_token_list() {
    TokenList* token_list = malloc(sizeof(TokenList));
    token_list->tokens = NULL;
    token_list->length = 0;
    return token_list;
}

void add_token(TokenList* token_list, char* name, int is_end) {
    Token* token = create_token(name, is_end);
    token_list->tokens = realloc(token_list->tokens, sizeof(Token) * (token_list->length + 1));
    token_list->tokens[token_list->length] = token;
    token_list->length++;
    //free(token);
}

Token* get_token(TokenList* token_list, int index) {
    if (index >= 0 && index < token_list->length) {
        return token_list->tokens[index];
    }
    return NULL;
}


void update_transition_count(TransitionList* transition_list, Token* previous1, Token* previous2, Token* previous3, Token* source, Token* destination) {
    for (int i = 0; i < transition_list->length; i++) {
        Transition* transition = transition_list->transitions[i];
        if (strcmp(transition->previous1->name, previous1->name) == 0 &&
            strcmp(transition->previous2->name, previous2->name) == 0 &&
            strcmp(transition->previous3->name, previous3->name) == 0 &&
            strcmp(transition->source->name, source->name) == 0 &&
            strcmp(transition->destination->name, destination->name) == 0) {
            transition->count++;
            return;
        }
    }
    transition_list->length++;
    transition_list->transitions = realloc(transition_list->transitions, transition_list->length * sizeof(Transition));
    Transition* new_transition = (Transition*)malloc(sizeof(Transition));
    new_transition->previous1 = previous1;
    new_transition->previous2 = previous2;
    new_transition->previous3 = previous3;
    new_transition->source = source;
    new_transition->destination = destination;
    new_transition->count = 1;
    transition_list->transitions[transition_list->length-1] = new_transition;
}

void train_model(TransitionList* transition_list, TokenList* token_list, Token* dummy, Token* end_token) {    
    // Initialize the previous1, previous2, and current tokens
    Token *previous1 = dummy;
    Token *previous2 = dummy;
    Token *previous3 = dummy;
    Token *current = dummy;
    Token *destination = NULL;

    // Loop through each token in the token list
    for (int i = 0; i < token_list->length; i++) {
        // Set the destination token
        destination = token_list->tokens[i];

        // If this is not one of the first two tokens
        if (previous2 != NULL) {
            // Update the transition count from previous1, previous2, and current to destination
            update_transition_count(transition_list, previous1, previous2, previous3, current, destination);
        }

        // Update the previous tokens
        previous1 = previous2;
        previous2 = previous3;
        previous3 = current;
        current = destination;
    }

    // Add a final transition from the last two tokens to the end of line token
    update_transition_count(transition_list, previous1, previous2, previous3, current, end_token);
}

void destroy_transition_list(TransitionList* transition_list) {
    if (transition_list != NULL) {
        for (int i = 0; i < transition_list->length; i++) {
            Transition* transition = transition_list->transitions[i];
            if (transition != NULL) {
                free(transition);
            }
        }
        free(transition_list->transitions);
        free(transition_list);
    }
}

void destroy_token_list(TokenList* token_list) {
    for (int i = 0; i < token_list->length; i++) {
        Token* token = token_list->tokens[i];
        free(token->name);
        free(token);
    }
    free(token_list->tokens);
    free(token_list);
}

void save_transition_list(TransitionList* transition_list, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Write the number of transitions to the file
    fwrite(&transition_list->length, sizeof(int), 1, fp);

    // Write each transition to the file
    for (int i = 0; i < transition_list->length; i++) {
        Transition* transition = transition_list->transitions[i];

        // Write the length of each string to the file, followed by the string itself
        int length = strlen(transition->previous3->name) + 1;
        fwrite(&length, sizeof(int), 1, fp);
        fwrite(transition->previous3->name, sizeof(char), length, fp);

        length = strlen(transition->previous2->name) + 1;
        fwrite(&length, sizeof(int), 1, fp);
        fwrite(transition->previous2->name, sizeof(char), length, fp);

        length = strlen(transition->previous1->name) + 1;
        fwrite(&length, sizeof(int), 1, fp);
        fwrite(transition->previous1->name, sizeof(char), length, fp);

        length = strlen(transition->source->name) + 1;
        fwrite(&length, sizeof(int), 1, fp);
        fwrite(transition->source->name, sizeof(char), length, fp);

        length = strlen(transition->destination->name) + 1;
        fwrite(&length, sizeof(int), 1, fp);
        fwrite(transition->destination->name, sizeof(char), length, fp);

        fwrite(&transition->count, sizeof(int), 1, fp);
    }

    fclose(fp);
}

Token* generate_next_token(TransitionList* transition_list, Token* previous1, Token* previous2, Token* previous3, Token* source) {
    // Initialize variables for the total count and the selected token
    int total_count = 0;
    Token* selected_token = NULL;
    
    // Loop through each transition in the transition list
    for (int i = 0; i < transition_list->length; i++) {
        Transition* transition = transition_list->transitions[i];
        // If the transition matches the two previous tokens
        if (strcmp(transition->previous1->name ,previous1->name) == 0 &&  strcmp(transition->previous2->name ,previous2->name) == 0 && strcmp(transition->previous3->name ,previous3->name) == 0 && strcmp(transition->source->name, source->name) == 0) {
            // Add the count of the transition to the total count
            total_count += transition->count;
        }
    }

    // If there are no transitions that match the previous two tokens, return NULL
    if (total_count == 0) {
        return NULL;
    }
    
    // Generate a random number between 0 and the total count
    int random_number = rand() % total_count;
    
    // Loop through each transition again, selecting a transition with probability proportional to its count
    double cumulative_probability = 0.0;
    for (int i = 0; i < transition_list->length; i++) {
        Transition* transition = transition_list->transitions[i];
        // If the transition matches the two previous tokens
        if (strcmp(transition->previous1->name ,previous1->name) == 0 &&  strcmp(transition->previous2->name ,previous2->name) == 0 && strcmp(transition->previous3->name ,previous3->name) == 0 && strcmp(transition->source->name, source->name) == 0) {
            // Calculate the probability of this transition based on its count and the total count
            double probability = (double) transition->count / (double) total_count;
            cumulative_probability += probability;
            // If the cumulative probability is greater than the random number, select this transition
            if (cumulative_probability > (double) random_number / (double) total_count) {
                selected_token = transition->destination;
                break;
            }
        }
    }
    
    // Return the selected token
    return selected_token;
}

Token** generate_tokens(TransitionList* transition_list, Token* previous1, Token* previous2, Token* previous3, Token* source) {
    Token** tokens = (Token**) malloc(8 * sizeof(Token*));
    
    // tokens[0] = source;
    // tokens[1] = generate_next_token(transition_list, previous, source);
    // tokens[2] = generate_next_token(transition_list, source, tokens[1]);
    // tokens[3] = generate_next_token(transition_list, tokens[1], tokens[2]);
    // tokens[4] = generate_next_token(transition_list, tokens[2], tokens[3]);

    tokens[0] = generate_next_token(transition_list, previous1, previous2, previous3, source);
    tokens[1] = generate_next_token(transition_list, previous2, previous3, source, tokens[0]);
    tokens[2] = generate_next_token(transition_list, previous3, source, tokens[0], tokens[1]);
    tokens[3] = generate_next_token(transition_list, source, tokens[0], tokens[1], tokens[2]);
    tokens[4] = generate_next_token(transition_list, tokens[0], tokens[1], tokens[2], tokens[3]);
    tokens[5] = generate_next_token(transition_list, tokens[1], tokens[2], tokens[3], tokens[4]);
    tokens[6] = generate_next_token(transition_list, tokens[2], tokens[3], tokens[4], tokens[5]);
    tokens[7] = generate_next_token(transition_list, tokens[3], tokens[4], tokens[5], tokens[6]);


    return tokens;
}

int main(int argc, char const *argv[]) {
    srand(time(NULL));

    TransitionList* transition_list = create_transition_list();
    TokenList* token_list = create_token_list();

    FILE* file = fopen("data.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error: failed to open file\n");
        exit(EXIT_FAILURE);
    }

    char line[100000];
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, "/CODE_DELIMITER/");
        while (token != NULL) {
            if (token[strlen(token)-1] == '\n') {
                token[strlen(token)-1] = '\0';
            }
            add_token(token_list, token, 0);
            token = strtok(NULL, "/CODE_DELIMITER/");
        }
    }
    add_token(token_list, "", 1);

    Token *dummy = create_token("", 1);
    Token *end_token = create_token("", 1);
    train_model(transition_list, token_list, dummy, end_token);

    save_transition_list(transition_list, "model_4.bin");

    destroy_transition_list(transition_list);
    destroy_token_list(token_list);
    free(dummy->name);
    free(dummy);
    free(end_token->name);
    free(end_token);
    

    // char *first = "for";
    // char *second = "(";
    // char *third = "int";
    // char *four = "i";

    // Token* token_first = (Token*) malloc(sizeof(Token));
    // token_first->name = (char*) malloc((strlen(first) + 1) * sizeof(char));
    // strcpy(token_first->name, first);
    // token_first->is_end = 0;

    // Token* token_second = (Token*) malloc(sizeof(Token));
    // token_second->name = (char*) malloc((strlen(second) + 1) * sizeof(char));
    // strcpy(token_second->name, second);
    // token_second->is_end = 0;

    // Token* token_third = (Token*) malloc(sizeof(Token));
    // token_third->name = (char*) malloc((strlen(third) + 1) * sizeof(char));
    // strcpy(token_third->name, third);
    // token_third->is_end = 0;

    // Token* token_four = (Token*) malloc(sizeof(Token));
    // token_four->name = (char*) malloc((strlen(four) + 1) * sizeof(char));
    // strcpy(token_four->name, four);
    // token_four->is_end = 0;

    // Token** tokens = generate_tokens(transition_list, token_first, token_second, token_third, token_four);
    // printf("for ( int i ");
    // for(int i = 0; i < 8; i++) {
    //     printf("%s ", tokens[i]->name);
    // }
    // printf("\n");

    return 0;
}
