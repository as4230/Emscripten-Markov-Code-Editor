#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <emscripten.h>

// A struct representing a single token in the code
typedef struct {
    char* name; // The name of the token
    int is_end; 
} Token;

// A struct representing a transition between two tokens
typedef struct {
    Token* source; // The source token of the transition
    Token* destination; // The destination token of the transition
    int count; // The number of times this transition occurs in the input code
} Transition_one;

// A struct representing a list of transitions
typedef struct {
    Transition_one** transitions; // An array of transitions
    int length; // The number of transitions in the list
} TransitionList_one;

// A struct representing a transition between two tokens
typedef struct {
    Token* previous; // The previous token in the sequence
    Token* source; // The source token of the transition
    Token* destination; // The destination token of the transition
    int count; // The number of times this transition occurs in the input code
} Transition_two;

// A struct representing a list of transitions
typedef struct {
    Transition_two** transitions; // An array of transitions
    int length; // The number of transitions in the list
} TransitionList_two;

// A struct representing a transition between two tokens
typedef struct {
    Token* previous2; // The second previous token in the sequence
    Token* previous1; // The previous token in the sequence
    Token* source; // The source token of the transition
    Token* destination; // The destination token of the transition
    int count; // The number of times this transition occurs in the input code
} Transition_three;

// A struct representing a list of transitions
typedef struct {
    Transition_three** transitions; // An array of transitions
    int length; // The number of transitions in the list
} TransitionList_three;

// A struct representing a transition between two tokens
typedef struct {
    Token* previous3; // The third previous token in the sequence
    Token* previous2; // The second previous token in the sequence
    Token* previous1; // The previous token in the sequence
    Token* source; // The source token of the transition
    Token* destination; // The destination token of the transition
    int count; // The number of times this transition occurs in the input code
} Transition_four;

// A struct representing a list of transitions
typedef struct {
    Transition_four** transitions; // An array of transitions
    int length; // The number of transitions in the list
} TransitionList_four;

// A struct representing a transition between two tokens
typedef struct {
    Token* previous4; // The third previous token in the sequence
    Token* previous3; // The third previous token in the sequence
    Token* previous2; // The second previous token in the sequence
    Token* previous1; // The previous token in the sequence
    Token* source; // The source token of the transition
    Token* destination; // The destination token of the transition
    int count; // The number of times this transition occurs in the input code
} Transition_five;

// A struct representing a list of transitions
typedef struct {
    Transition_five** transitions; // An array of transitions
    int length; // The number of transitions in the list
} TransitionList_five;

//model 1 list
TransitionList_one* transition_list_one;

//model 2 list
TransitionList_two* transition_list_two;

//model 3 list
TransitionList_three* transition_list_three;

//model 4 list
TransitionList_four* transition_list_four;

//model 5 list
TransitionList_five* transition_list_five;

// A helper function to create a new token with the given name

Token* create_token(char* name, int is_end) {
    Token* token = (Token*) malloc(sizeof(Token));
    if (token == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for token\n");
        exit(EXIT_FAILURE);
    }
    token->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
    strcpy(token->name, name);
    token->is_end = is_end;
    return token;
}

//
//
// Start of model one
//
//

void create_transition_list_one() {
    transition_list_one = malloc(sizeof(TransitionList_two));
    if (transition_list_one == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for transition list\n");
        exit(EXIT_FAILURE);
    }
    transition_list_one->transitions = NULL;
    transition_list_one->length = 0;
    //return transition_list;
}

// Add a new transition to the transition list

void add_transition_one(Token* source, Token* destination, int count) {
    // Check if the transition already exists in the list
    for (int i = 0; i < transition_list_one->length; i++) {
        Transition_one* transition = transition_list_one->transitions[i];
        if (transition->source == source && transition->destination == destination) {
            //transition->count++;
            return;
        }
    }

    // If the transition does not exist, create a new transition and add it to the list
    Transition_one* transition = (Transition_one*) malloc(sizeof(Transition_one));
    if (transition == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for transition\n");
        exit(EXIT_FAILURE);
    }
    transition->source = source;
    transition->destination = destination;
    transition->count = count;

    // Resize the transitions array and add the new transition
    transition_list_one->transitions = (Transition_one**) realloc(transition_list_one->transitions, sizeof(Transition_one*) * (transition_list_one->length + 1));
    if (transition_list_one->transitions == NULL) {
        fprintf(stderr, "Error: failed to resize transitions array\n");
        exit(EXIT_FAILURE);
    }
    transition_list_one->transitions[transition_list_one->length] = transition;
    transition_list_one->length++;

}

EMSCRIPTEN_KEEPALIVE
void load_transition_list_one(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Read the number of transitions from the file
    int num_transitions;
    fread(&num_transitions, sizeof(int), 1, fp);

    // Create a new transition list
    create_transition_list_one();

    // Read each transition from the file and add it to the transition list
    for (int i = 0; i < num_transitions; i++) {
        // Read the length of each string from the file, followed by the string itself
        int length;
        char* str;

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* source = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* destination = create_token(str, 0);

        int count;
        fread(&count, sizeof(int), 1, fp);

        // Add the transition to the transition list
        add_transition_one(source, destination, count);

        // Free the string
        free(str);
    }

    fclose(fp);
}


Token* generate_next_token_one(Token* source) {
    // Initialize variables for the total count and the selected token
    int total_count = 0;
    Token* selected_token = NULL;
    
    // Loop through each transition in the transition list
    for (int i = 0; i < transition_list_one->length; i++) {
        Transition_one* transition = transition_list_one->transitions[i];
        // If the transition matches the two previous tokens
        if (strcmp(transition->source->name, source->name) == 0) {
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
    for (int i = 0; i < transition_list_one->length; i++) {
        Transition_one* transition = transition_list_one->transitions[i];
        // If the transition matches the two previous tokens
        if (strcmp(transition->source->name, source->name) == 0) {
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

EMSCRIPTEN_KEEPALIVE
char** generate_tokens_one(char* source_string) {
    char** tokens_strings = (char**) malloc(2 * sizeof(char*));
    Token* source = create_token(source_string, 0);

    Token* tokens[2];

    tokens[0] = generate_next_token_one(source);
    tokens[1] = generate_next_token_one(tokens[0]);

    for (int i = 0; i < 2; i++) {
        if (tokens[i] == NULL) {
            tokens_strings[i] = NULL;
        } else {
            tokens_strings[i] = tokens[i]->name;
        }
    }

    free(source->name);
    free(source);


    return tokens_strings;
}

//
//
// Start of model two 
//
//

void create_transition_list_two() {
    transition_list_two = malloc(sizeof(TransitionList_two));
    if (transition_list_two == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for transition list\n");
        exit(EXIT_FAILURE);
    }
    transition_list_two->transitions = NULL;
    transition_list_two->length = 0;
    //return transition_list;
}

// Add a new transition to the transition list
void add_transition_two(Token* previous, Token* source, Token* destination, int count) {
    // Check if the transition already exists in the list
    for (int i = 0; i < transition_list_two->length; i++) {
        Transition_two* transition = transition_list_two->transitions[i];
        if (transition->previous == previous && transition->source == source && transition->destination == destination) {
            //transition->count++;
            return;
        }
    }

    // If the transition does not exist, create a new transition and add it to the list
    Transition_two* transition = (Transition_two*) malloc(sizeof(Transition_two));
    if (transition == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for transition\n");
        exit(EXIT_FAILURE);
    }
    transition->previous = previous;
    transition->source = source;
    transition->destination = destination;
    transition->count = count;

    // Resize the transitions array and add the new transition
    transition_list_two->transitions = (Transition_two**) realloc(transition_list_two->transitions, sizeof(Transition_two*) * (transition_list_two->length + 1));
    if (transition_list_two->transitions == NULL) {
        fprintf(stderr, "Error: failed to resize transitions array\n");
        exit(EXIT_FAILURE);
    }
    transition_list_two->transitions[transition_list_two->length] = transition;
    transition_list_two->length++;

}

EMSCRIPTEN_KEEPALIVE
void load_transition_list_two(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Read the number of transitions from the file
    int num_transitions;
    fread(&num_transitions, sizeof(int), 1, fp);

    // Create a new transition list
    create_transition_list_two();

    // Read each transition from the file and add it to the transition list
    for (int i = 0; i < num_transitions; i++) {
        // Read the length of each string from the file, followed by the string itself
        int length;
        char* str;

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* previous = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* source = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* destination = create_token(str, 0);

        int count;
        fread(&count, sizeof(int), 1, fp);

        // Add the transition to the transition list
        add_transition_two(previous, source, destination, count);

        // Free the string
        free(str);
    }

    fclose(fp);
}


Token* generate_next_token_two(Token* previous, Token* source) {
    // Initialize variables for the total count and the selected token
    int total_count = 0;
    Token* selected_token = NULL;
    
    // Loop through each transition in the transition list
    for (int i = 0; i < transition_list_two->length; i++) {
        Transition_two* transition = transition_list_two->transitions[i];
        // If the transition matches the two previous tokens
        if (strcmp(transition->previous->name ,previous->name) == 0 && strcmp(transition->source->name, source->name) == 0) {
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
    for (int i = 0; i < transition_list_two->length; i++) {
        Transition_two* transition = transition_list_two->transitions[i];
        // If the transition matches the two previous tokens
        if (strcmp(transition->previous->name ,previous->name) == 0 && strcmp(transition->source->name, source->name) == 0) {
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

EMSCRIPTEN_KEEPALIVE
char** generate_tokens_two(char* previous_string, char* source_string) {
    char** tokens_strings = (char**) malloc(4 * sizeof(char*));
    Token* source = create_token(source_string, 0);
    Token* previous = create_token(previous_string, 0);

    Token* tokens[4];

    tokens[0] = generate_next_token_two(previous, source);
    tokens[1] = generate_next_token_two(source, tokens[0]);
    tokens[2] = generate_next_token_two(tokens[0], tokens[1]);
    tokens[3] = generate_next_token_two(tokens[1], tokens[2]);

    for (int i = 0; i < 4; i++) {
        if (tokens[i] == NULL) {
            tokens_strings[i] = NULL;
        } else {
            tokens_strings[i] = tokens[i]->name;
        }
    }

    free(source->name);
    free(source);
    free(previous->name);
    free(previous);

    return tokens_strings;
}

//
//
// Start of model three
//
//

void create_transition_list_three() {
    transition_list_three = malloc(sizeof(TransitionList_three));
    if (transition_list_three == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for transition list\n");
        exit(EXIT_FAILURE);
    }
    transition_list_three->transitions = NULL;
    transition_list_three->length = 0;
    //return transition_list;
}

// Add a new transition to the transition list

void add_transition_three(Token* previous2, Token* previous1, Token* source, Token* destination, int count) {
    // Check if the transition already exists in the list
    for (int i = 0; i < transition_list_three->length; i++) {
        Transition_three* transition = transition_list_three->transitions[i];
        if (transition->previous2 == previous2 && transition->previous1 == previous1 && transition->source == source && transition->destination == destination) {
            //transition->count++;
            return;
        }
    }

    // If the transition does not exist, create a new transition and add it to the list
    Transition_three* transition = (Transition_three*) malloc(sizeof(Transition_three));
    if (transition == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for transition\n");
        exit(EXIT_FAILURE);
    }
    transition->previous2 = previous2;
    transition->previous1 = previous1;
    transition->source = source;
    transition->destination = destination;
    transition->count = count;

    // Resize the transitions array and add the new transition
    transition_list_three->transitions = (Transition_three**) realloc(transition_list_three->transitions, sizeof(Transition_three*) * (transition_list_three->length + 1));
    if (transition_list_three->transitions == NULL) {
        fprintf(stderr, "Error: failed to resize transitions array\n");
        exit(EXIT_FAILURE);
    }
    transition_list_three->transitions[transition_list_three->length] = transition;
    transition_list_three->length++;

}

EMSCRIPTEN_KEEPALIVE
void load_transition_list_three(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Read the number of transitions from the file
    int num_transitions;
    fread(&num_transitions, sizeof(int), 1, fp);

    // Create a new transition list
    create_transition_list_three();

    // Read each transition from the file and add it to the transition list
    for (int i = 0; i < num_transitions; i++) {
        // Read the length of each string from the file, followed by the string itself
        int length;
        char* str;

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* previous2 = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* previous1 = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* source = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* destination = create_token(str, 0);

        int count;
        fread(&count, sizeof(int), 1, fp);

        // Add the transition to the transition list
        add_transition_three(previous2, previous1, source, destination, count);

        // Free the string
        free(str);
    }

    fclose(fp);
}


Token* generate_next_token_three(Token* previous1, Token* previous2, Token* source) {
    // Initialize variables for the total count and the selected token
    int total_count = 0;
    Token* selected_token = NULL;
    
    // Loop through each transition in the transition list
    for (int i = 0; i < transition_list_three->length; i++) {
        Transition_three* transition = transition_list_three->transitions[i];
        // If the transition matches the two previous tokens
        if (strcmp(transition->previous1->name ,previous1->name) == 0 &&  strcmp(transition->previous2->name ,previous2->name) == 0 && strcmp(transition->source->name, source->name) == 0) {
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
    for (int i = 0; i < transition_list_three->length; i++) {
        Transition_three* transition = transition_list_three->transitions[i];
        // If the transition matches the two previous tokens
        if (strcmp(transition->previous1->name ,previous1->name) == 0 &&  strcmp(transition->previous2->name ,previous2->name) == 0 && strcmp(transition->source->name, source->name) == 0) {
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

EMSCRIPTEN_KEEPALIVE
char** generate_tokens_three(char* previous1_string, char* previous2_string, char* source_string) {
    char** tokens_strings = (char**) malloc(6 * sizeof(char*));
    Token* source = create_token(source_string, 0);
    Token* previous2 = create_token(previous2_string, 0);
    Token* previous1 = create_token(previous1_string, 0);

    Token* tokens[6];

    tokens[0] = generate_next_token_three(previous1, previous2, source);
    tokens[1] = generate_next_token_three(previous2, source, tokens[0]);
    tokens[2] = generate_next_token_three(source, tokens[0], tokens[1]);
    tokens[3] = generate_next_token_three(tokens[0], tokens[1], tokens[2]);
    tokens[4] = generate_next_token_three(tokens[1], tokens[2], tokens[3]);
    tokens[5] = generate_next_token_three(tokens[2], tokens[3], tokens[4]);

    for (int i = 0; i < 6; i++) {
        if (tokens[i] == NULL) {
            tokens_strings[i] = NULL;
        } else {
            tokens_strings[i] = tokens[i]->name;
        }
    }

    free(source->name);
    free(source);
    free(previous1->name);
    free(previous1);
    free(previous2->name);
    free(previous2);

    return tokens_strings;
}

//
//
// Start of model four
//
//

void create_transition_list_four() {
    transition_list_four = malloc(sizeof(TransitionList_four));
    if (transition_list_four == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for transition list\n");
        exit(EXIT_FAILURE);
    }
    transition_list_four->transitions = NULL;
    transition_list_four->length = 0;
    //return transition_list;
}

// Add a new transition to the transition list

void add_transition_four(Token* previous3, Token* previous2, Token* previous1, Token* source, Token* destination, int count) {
    // Check if the transition already exists in the list
    for (int i = 0; i < transition_list_four->length; i++) {
        Transition_four* transition = transition_list_four->transitions[i];
        if (transition->previous3 == previous3 && transition->previous2 == previous2 && transition->previous1 == previous1 && transition->source == source && transition->destination == destination) {
            //transition->count++;
            return;
        }
    }

    // If the transition does not exist, create a new transition and add it to the list
    Transition_four* transition = (Transition_four*) malloc(sizeof(Transition_four));
    if (transition == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for transition\n");
        exit(EXIT_FAILURE);
    }
    transition->previous3 = previous3;
    transition->previous2 = previous2;
    transition->previous1 = previous1;
    transition->source = source;
    transition->destination = destination;
    transition->count = count;

    // Resize the transitions array and add the new transition
    transition_list_four->transitions = (Transition_four**) realloc(transition_list_four->transitions, sizeof(Transition_four*) * (transition_list_four->length + 1));
    if (transition_list_four->transitions == NULL) {
        fprintf(stderr, "Error: failed to resize transitions array\n");
        exit(EXIT_FAILURE);
    }
    transition_list_four->transitions[transition_list_four->length] = transition;
    transition_list_four->length++;

}

EMSCRIPTEN_KEEPALIVE
void load_transition_list_four(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Read the number of transitions from the file
    int num_transitions;
    fread(&num_transitions, sizeof(int), 1, fp);

    // Create a new transition list
    create_transition_list_four();

    // Read each transition from the file and add it to the transition list
    for (int i = 0; i < num_transitions; i++) {
        // Read the length of each string from the file, followed by the string itself
        int length;
        char* str;

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* previous3 = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* previous2 = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* previous1 = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* source = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* destination = create_token(str, 0);

        int count;
        fread(&count, sizeof(int), 1, fp);

        // Add the transition to the transition list
        add_transition_four(previous3, previous2, previous1, source, destination, count);

        // Free the string
        free(str);
    }

    fclose(fp);
}


Token* generate_next_token_four(Token* previous1, Token* previous2, Token* previous3, Token* source) {
    // Initialize variables for the total count and the selected token
    int total_count = 0;
    Token* selected_token = NULL;
    
    // Loop through each transition in the transition list
    for (int i = 0; i < transition_list_four->length; i++) {
        Transition_four* transition = transition_list_four->transitions[i];
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
    for (int i = 0; i < transition_list_four->length; i++) {
        Transition_four* transition = transition_list_four->transitions[i];
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

EMSCRIPTEN_KEEPALIVE
char** generate_tokens_four(char* previous1_string, char* previous2_string, char* previous3_string, char* source_string) {
    char** tokens_strings = (char**) malloc(8 * sizeof(char*));
    Token* source = create_token(source_string, 0);
    Token* previous3 = create_token(previous3_string, 0);
    Token* previous2 = create_token(previous2_string, 0);
    Token* previous1 = create_token(previous1_string, 0);

    Token* tokens[8];

    tokens[0] = generate_next_token_four(previous1, previous2, previous3, source);
    tokens[1] = generate_next_token_four(previous2, previous3, source, tokens[0]);
    tokens[2] = generate_next_token_four(previous3, source, tokens[0], tokens[1]);
    tokens[3] = generate_next_token_four(source, tokens[0], tokens[1], tokens[2]);
    tokens[4] = generate_next_token_four(tokens[0], tokens[1], tokens[2], tokens[3]);
    tokens[5] = generate_next_token_four(tokens[1], tokens[2], tokens[3], tokens[4]);
    tokens[6] = generate_next_token_four(tokens[2], tokens[3], tokens[4], tokens[5]);
    tokens[7] = generate_next_token_four(tokens[3], tokens[4], tokens[5], tokens[6]);

    for (int i = 0; i < 8; i++) {
        if (tokens[i] == NULL) {
            tokens_strings[i] = NULL;
        } else {
            tokens_strings[i] = tokens[i]->name;
        }
    }

    free(source->name);
    free(source);
    free(previous1->name);
    free(previous1);
    free(previous2->name);
    free(previous2);
    free(previous3->name);
    free(previous3);

    return tokens_strings;
}

//
//
// Start of model five
//
//
void create_transition_list_five() {
    transition_list_five = malloc(sizeof(TransitionList_five));
    if (transition_list_five == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for transition list\n");
        exit(EXIT_FAILURE);
    }
    transition_list_five->transitions = NULL;
    transition_list_five->length = 0;
    //return transition_list;
}

// Add a new transition to the transition list
void add_transition_five(Token* previous4, Token* previous3, Token* previous2, Token* previous1, Token* source, Token* destination, int count) {
    // Check if the transition already exists in the list
    for (int i = 0; i < transition_list_five->length; i++) {
        Transition_five* transition = transition_list_five->transitions[i];
        if (transition->previous4 == previous4 && transition->previous3 == previous3 && transition->previous2 == previous2 && transition->previous1 == previous1 && transition->source == source && transition->destination == destination) {
            //transition->count++;
            return;
        }
    }

    // If the transition does not exist, create a new transition and add it to the list
    Transition_five* transition = (Transition_five*) malloc(sizeof(Transition_five));
    if (transition == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for transition\n");
        exit(EXIT_FAILURE);
    }
    transition->previous4 = previous4;
    transition->previous3 = previous3;
    transition->previous2 = previous2;
    transition->previous1 = previous1;
    transition->source = source;
    transition->destination = destination;
    transition->count = count;

    // Resize the transitions array and add the new transition
    transition_list_five->transitions = (Transition_five**) realloc(transition_list_five->transitions, sizeof(Transition_five*) * (transition_list_five->length + 1));
    if (transition_list_five->transitions == NULL) {
        fprintf(stderr, "Error: failed to resize transitions array\n");
        exit(EXIT_FAILURE);
    }
    transition_list_five->transitions[transition_list_five->length] = transition;
    transition_list_five->length++;

}

EMSCRIPTEN_KEEPALIVE
void load_transition_list_five(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Read the number of transitions from the file
    int num_transitions;
    fread(&num_transitions, sizeof(int), 1, fp);

    // Create a new transition list
    create_transition_list_five();

    // Read each transition from the file and add it to the transition list
    for (int i = 0; i < num_transitions; i++) {
        // Read the length of each string from the file, followed by the string itself
        int length;
        char* str;

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* previous4 = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* previous3 = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* previous2 = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* previous1 = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* source = create_token(str, 0);

        fread(&length, sizeof(int), 1, fp);
        str = malloc(sizeof(char) * (length + 1));
        fread(str, sizeof(char), length, fp);
        str[length] = '\0';
        Token* destination = create_token(str, 0);

        int count;
        fread(&count, sizeof(int), 1, fp);

        // Add the transition to the transition list
        add_transition_five(previous4, previous3, previous2, previous1, source, destination, count);

        // Free the string
        free(str);
    }

    fclose(fp);
}


Token* generate_next_token_five(Token* previous1, Token* previous2, Token* previous3, Token* previous4, Token* source) {
    // Initialize variables for the total count and the selected token
    int total_count = 0;
    Token* selected_token = NULL;
    
    // Loop through each transition in the transition list
    for (int i = 0; i < transition_list_five->length; i++) {
        Transition_five* transition = transition_list_five->transitions[i];
        // If the transition matches the two previous tokens
        if (strcmp(transition->previous1->name ,previous1->name) == 0 &&  strcmp(transition->previous2->name ,previous2->name) == 0 && strcmp(transition->previous3->name ,previous3->name) == 0 && strcmp(transition->previous4->name ,previous4->name) == 0 && strcmp(transition->source->name, source->name) == 0) {
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
    for (int i = 0; i < transition_list_five->length; i++) {
        Transition_five* transition = transition_list_five->transitions[i];
        // If the transition matches the two previous tokens
        if (strcmp(transition->previous1->name ,previous1->name) == 0 &&  strcmp(transition->previous2->name ,previous2->name) == 0 && strcmp(transition->previous3->name ,previous3->name) == 0 && strcmp(transition->previous4->name ,previous4->name) == 0 && strcmp(transition->source->name, source->name) == 0) {
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

EMSCRIPTEN_KEEPALIVE
char** generate_tokens_five(char* previous1_string, char* previous2_string, char* previous3_string, char* previous4_string, char* source_string) {
    char** tokens_strings = (char**) malloc(10 * sizeof(char*));
    Token* source = create_token(source_string, 0);
    Token* previous4 = create_token(previous4_string, 0);
    Token* previous3 = create_token(previous3_string, 0);
    Token* previous2 = create_token(previous2_string, 0);
    Token* previous1 = create_token(previous1_string, 0);

    Token* tokens[10];

    tokens[0] = generate_next_token_five(previous1, previous2, previous3, previous4, source);
    tokens[1] = generate_next_token_five(previous2, previous3, previous4, source, tokens[0]);
    tokens[2] = generate_next_token_five(previous3, previous4,  source, tokens[0], tokens[1]);
    tokens[3] = generate_next_token_five(previous4, source, tokens[0], tokens[1], tokens[2]);
    tokens[4] = generate_next_token_five(source, tokens[0], tokens[1], tokens[2], tokens[3]);
    tokens[5] = generate_next_token_five(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
    tokens[6] = generate_next_token_five(tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
    tokens[7] = generate_next_token_five(tokens[2], tokens[3], tokens[4], tokens[5], tokens[6]);
    tokens[8] = generate_next_token_five(tokens[3], tokens[4], tokens[5], tokens[6], tokens[7]);
    tokens[9] = generate_next_token_five(tokens[4], tokens[5], tokens[6], tokens[7], tokens[8]);

    for (int i = 0; i < 10; i++) {
        if (tokens[i] == NULL) {
            tokens_strings[i] = NULL;
        } else {
            tokens_strings[i] = tokens[i]->name;
        }
    }

    free(source->name);
    free(source);
    free(previous4->name);
    free(previous4);
    free(previous3->name);
    free(previous3);
    free(previous2->name);
    free(previous2);
    free(previous1->name);
    free(previous1);



    return tokens_strings;
}

int main(int argc, char const *argv[]) {
    //load_transition_list_one("model_1.bin");
    //load_transition_list_two("model_2.bin");
    //load_transition_list_three("model_3.bin");
    //load_transition_list_four("model_4.bin");
    //load_transition_list_five("model_5.bin");


    
    // for (int i = 0; i < 10; i++) {
    //     Transition* transition = transition_list->transitions[i];
    //         printf("PREV: '%s' SOURCE: '%s' NEXT: '%s' \n", transition_list->transitions[i]->previous->name, transition_list->transitions[i]->source->name, transition_list->transitions[i]->destination->name);
    // }

    // char *first = "for";
    // char *second = "i";

    // Token* token_first = (Token*) malloc(sizeof(Token));
    // token_first->name = (char*) malloc((strlen(first) + 1) * sizeof(char));
    // strcpy(token_first->name, first);
    // token_first->is_end = 0;

    // Token* token_second = (Token*) malloc(sizeof(Token));
    // token_second->name = (char*) malloc((strlen(second) + 1) * sizeof(char));
    // strcpy(token_second->name, second);
    // token_second->is_end = 0;

    // char** tokens = generate_tokens_two(first, second);
    // printf("for i ");
    // for(int i = 0; i < 4; i++) {
    //     printf("%s ", tokens[i]);
    // }
    // printf("\n");

    return 0;
}
