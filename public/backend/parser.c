#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <clang-c/Index.h>

/**
 * @brief When I was making a C code editor which would attempt to compile code on the client side 
 * I used this to parse through a C file dataset in order to make the AST for the C files so I could get the tokens easily.
 * I personally had to compile it by typing: 
 * clang -o parser parser.c `pkg-config --cflags --libs glib-2.0` -lclang -I/usr/lib/llvm-10/include 
 * into my command line
 */
int main(int argc, char **argv) {

    // Initialize the libclang library
    CXIndex index = clang_createIndex(0, 0);
    if (!index) {
        fprintf(stderr, "Failed to initialize libclang\n");
        return 1;
    }

    // Loop through all input files
    FILE *fp_out = fopen("data.txt", "w");
    if (!fp_out) {
        fprintf(stderr, "Failed to open output file\n");
        return 1;
    }
    char filename[256];
    for (int i = 1; i <= 149; i++) {
        sprintf(filename, "../../data/%d.c", i);
        CXTranslationUnit tu = clang_parseTranslationUnit(index, filename, NULL, 0, NULL, 0, CXTranslationUnit_None);
        if (!tu) {
            fprintf(stderr, "Failed to parse input file %s\n", filename);
            continue;
        }

        // Print the syntax tree
        CXCursor cursor = clang_getTranslationUnitCursor(tu);
        CXSourceRange range = clang_getCursorExtent(cursor);
        CXToken *tokens;
        unsigned int num_tokens;
        clang_tokenize(tu, range, &tokens, &num_tokens);
        for (unsigned int j = 0; j < num_tokens; j++) {
            CXString token_string = clang_getTokenSpelling(tu, tokens[j]);
            fwrite(clang_getCString(token_string), strlen(clang_getCString(token_string)), 1, fp_out);
            if (j < num_tokens - 1) {
                fwrite("/CODE_DELIMITER/", 1, strlen("/CODE_DELIMITER/"), fp_out);
            }
            clang_disposeString(token_string);
        }
        fwrite("\n", 1, 1, fp_out);

        // Clean up
        clang_disposeTokens(tu, tokens, num_tokens);
        clang_disposeTranslationUnit(tu);
    }

    fclose(fp_out);
    clang_disposeIndex(index);
    return 0;
}