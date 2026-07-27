#include "core/lexer.h"
#include "core/parser.h"

#include "arena.h"
#include "utils.h"

#include <stdio.h>

int main(void) {
    const char *file_path = "main.vz";
    StringStream ss = read_file(file_path);

    Arena a = arena_new(1024*1024);
    Lexer lex = lexer_new(ss, &a);
    Parser p = parser_new(&lex, &a, file_path);

    Node *cur = parser_next(&p);
    while (cur) {
        switch (cur->kind) {
            case NODE_IMPORT: {
                char *ss_dumped = sstream_to_cstr(&a, cur->as.import_stmt.path);
                printf("%zu:%zu; import: %s\n", cur->line, cur->col, ss_dumped);
                break;
            };
            default: break;
        }
        cur = parser_next(&p);
    }

    arena_free(&a);
    return 0;
}
