#ifndef __DEPENDECIES_H__
#define __DEPENDECIES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum __YelTokenType {
    tok_undefined   = -1,
    tok_name = 100, // obj
    tok_number,     // const numeric literal - CNL
    tok_string,     // const string literal - CSL

    tok_binary_op = 400,
    tok_binary_op_pow,                  // **
    tok_binary_op_div,                  // /
    tok_binary_op_mul,                  // *
    tok_binary_op_percent,              // %
    tok_binary_op_plus,                 // +
    tok_binary_op_minus,                // -
    tok_binary_op_rsh,                  // >>
    tok_binary_op_lsh,                  // <<
    tok_binary_op_more,                 // >
    tok_binary_op_less,                 // <
    tok_binary_op_more_eq,              // >=
    tok_binary_op_less_eq,              // <=
    tok_binary_op_eq,                   // ==
    tok_binary_op_not_eq,               // !=
    tok_binary_op_and,                  // &
    tok_binary_op_or,                   // |
    tok_binary_op_log_and,              // &&
    tok_binary_op_log_or,               // ||
    tok_binary_op_div_assign = 422,     // /=
    tok_binary_op_mul_assign,           // *=
    tok_binary_op_percent_assign,       // %=
    tok_binary_op_plus_assign,          // +=
    tok_binary_op_minus_assign,         // -=
    tok_binary_op_rsh_assign,           // >>=
    tok_binary_op_lsh_assign,           // <<=
    tok_binary_op_and_assign = 435,     // &=
    tok_binary_op_or_assign,            // |=
    tok_binary_op_assign,               // =

    tok_unary_op = 500,
    tok_unary_op_pos    = tok_binary_op_plus,
    tok_unary_op_neg    = tok_binary_op_minus,
    tok_unary_op_not = 501,     // !
    tok_unary_op_inc,           // ++obj or obj++
    tok_unary_op_dec,           // --obj or obj--

    tok_op = 600, 
    tok_op_lpar,        // (
    tok_op_rpar,        // )
    tok_op_flbrk,       // {
    tok_op_frbrk,       // }
    tok_semicolon,      // ;
    tok_colon,          // :
    tok_comma,          // ,
    tok_op_follow,     // ->

    tok_word = 700,
    tok_word_func,      // func
    tok_word_return,    // return
    tok_word_defer,     // defer
    tok_word_break,     // break
    tok_word_noreturn,  // noreturn
    tok_word_Int,       // Int
    tok_word_Flt,       // Flt
    tok_word_Str,       // Str
    tok_word_Any,       // Any
} YelTokenType;

typedef struct __YelTokens {
    _Bool error;
    size_t length;
    size_t pointer;
    size_t *line;            // trace 
    size_t* start_symbol;   // trace
    YelTokenType* type;
    char** value;
    char* file_name;

    struct __Source* src_ptr;
} YelTokens;

typedef struct __Source {
    char* source_text;
    char* file_name;        // trace   
    size_t pointer;
    size_t length;
} Source;

typedef enum __YelEntities {
    en_end = -1,
    en_expr,
    en_stmt,
    en_decl
} YelEntities;

#define __dbreak() printf("debug call in <File: %s:%d>", __FILE__, __LINE__);getchar()
#define print_cur() printf("\ncur = %s\n", cur)
#define print_tokens() puts("tokens = {");\
    for (size_t i = 0; i < token_array.length; i++) {\
        printf("   [%d, '%s', %lu:%lu]\n", token_array.type[i], token_array.value[i], token_array.line[i], token_array.start_symbol[i]);\
    } puts("}")

#endif // __DEPENDECIES_H_