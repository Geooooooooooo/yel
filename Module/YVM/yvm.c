#include "yvm.h"

void print_disassembly_opcode(OPCODEWORD w, size_t count) {
    static int next = 0;
    // 1 = name
    // 2 = address
    // 3 = const
    // 4 = num
    // 0 = default

    if (next == 1) {
        next = 0;
        printf("%s (%p)\n", (char*)w, w);
        return;
    }
    else if (next == 2) {
        next = 0;
        printf("%p (to %llu)\n", w, w);
        return;
    }
    else if (next == 3) {
        next = 0;
        printf("%p (const address)\n", w);
        return;
    }
    else if (next == 4) {
        next = 0;
        printf("%llu\n", w);
        return;
    }

    switch (w)
    {
    case OP_HALT:       printf("%llu:\tHALT\n", count);break;
    case LOAD_VALUE:    printf("%llu:\tLOAD_VALUE\t", count);next=1;break;
    case LOAD_CONST:    printf("%llu:\tLOAD_CONST\t", count);next=3;break;
    case POP_VALUE:     printf("%llu:\tPOP_VALUE\n", count);break;
    case DUP_VALUE:     printf("%llu:\tDUP_VALUE\n", count);break;
    case UNARY_POS:     printf("%llu:\tUNARY_POS\n", count);break;
    case UNARY_NOT:     printf("%llu:\tUNARY_NOT\n", count);break;
    case UNARY_LOGICAL_NOT:printf("%llu:\tLOGICAL_NOT\n", count);break;
    case UNARY_NEG:     printf("%llu:\tUNARY_NEG\n", count);break;
    case UNARY_INC:     printf("%llu:\tUNARY_INC\n", count);break;
    case UNARY_DEC:     printf("%llu:\tUNARY_DEC\n", count);break;
    case BYNARY_OP:     printf("%llu:\tBYNARY_OP\t", count);break;
    case BYNARY_POW:    printf("%llu (**)\n", BYNARY_POW);break;
    case BYNARY_DIV:    printf("%llu (/)\n", BYNARY_DIV);break;
    case BYNARY_MUL:    printf("%llu (*)\n", BYNARY_MUL);break;
    case BYNARY_MOD:    printf("%llu (%%)\n", BYNARY_MOD);break;
    case BYNARY_ADD:    printf("%llu (+)\n", BYNARY_ADD);break;
    case BYNARY_SUB:    printf("%llu (-)\n", BYNARY_SUB);break;
    case BYNARY_RSH:    printf("%llu (>>)\n", BYNARY_RSH);break;
    case BYNARY_LSH:    printf("%llu (<<)\n", BYNARY_LSH);break;
    case BYNARY_MORE:   printf("%llu (>)\n", BYNARY_MORE);break;
    case BYNARY_MORE_EQ:printf("%llu (>=)\n", BYNARY_MORE_EQ);break;
    case BYNARY_LESS:   printf("%llu (<)\n", BYNARY_LESS);break;
    case BYNARY_LESS_EQ:printf("%llu (<=)\n", BYNARY_LESS_EQ);break;
    case BYNARY_EQ:     printf("%llu (==)\n", BYNARY_EQ);break;
    case BYNARY_NOT_EQ: printf("%llu (!=)\n", BYNARY_NOT_EQ);break;
    case BYNARY_AND:    printf("%llu (&)\n", BYNARY_AND);break;
    case BYNARY_OR:     printf("%llu (|)\n", BYNARY_OR);break;
    case BYNARY_LOGICAL_AND:printf("%llu (and)\n", BYNARY_LOGICAL_AND);break;
    case BYNARY_LOGICAL_OR:printf("%llu (or)\n", BYNARY_LOGICAL_OR);break;
    case OP_JUMP_TO:    printf("%llu:\tJUMP_TO\t\t", count);next=2;break;
    case OP_POP_JUMP_ZERO:printf("%llu:\tPOP_JUMP_ZERO\t", count);next=2;break;
    case OP_CALL:       printf("%llu:\tCALL\t\t", count);next=4;break;
    case OP_RET:        printf("%llu:\tRET\n", count);break;
    case OP_BRK:        printf("%llu:\tBRK\n", count);break;
    case OP_STORE:      printf("%llu:\tSTORE\t\t", count);next=1;break;
    case MAKE_FUNC:     printf("%llu:\tMAKE_FUNC\n", count);break;
    default:            printf("%p\n", w);break;
    }
}

SIZE_REF* data_float_segment;
SIZE_REF* data_bool_segment;
SIZE_REF* data_int_segment;

unsigned long long data_float_segment_len;
unsigned long long data_int_segment_len;
unsigned long long data_bool_segment_len;

void yel_init_data_seg() {
    data_float_segment = (SIZE_REF*)__builtin_malloc(sizeof(SIZE_REF));
    data_float_segment_len = 0;

    data_int_segment = (SIZE_REF*)__builtin_malloc(sizeof(SIZE_REF));
    data_int_segment_len = 0;

    data_bool_segment = (SIZE_REF*)__builtin_malloc(sizeof(SIZE_REF));
    data_bool_segment_len = 0;
}

void yel_free_data_seg() {
    __builtin_free(data_float_segment);
    __builtin_free(data_int_segment);
    __builtin_free(data_bool_segment);
}

SIZE_REF yel_alloc_Flt_data(__float128 _Val) {
    for (unsigned long long i = 0; i < data_float_segment_len; i++) {
        if (*(__float128*)data_float_segment[i] == _Val) {
            return (SIZE_REF)data_float_segment[i];
        }
    }
    
    data_float_segment = (SIZE_REF*)__builtin_realloc(data_float_segment, (data_float_segment_len+1) * sizeof(SIZE_REF));
    __float128* tmp = (__float128*)__builtin_malloc(sizeof(__float128));
    *tmp = _Val;
    data_float_segment[data_float_segment_len] = tmp;
    ++data_float_segment_len;
    return (SIZE_REF)data_float_segment[data_float_segment_len-1];
}

SIZE_REF yel_alloc_Int_data(__int128_t _Val) {
    for (unsigned long long i = 0; i < data_int_segment_len; i++) {
        if (*(__int128_t*)data_int_segment[i] == _Val) {
            return (SIZE_REF)data_int_segment[i];
        }
    }
    
    data_int_segment = (SIZE_REF*)__builtin_realloc(data_int_segment, (data_int_segment_len+1) * sizeof(SIZE_REF));
    __int128_t* tmp = (__int128_t*)__builtin_malloc(sizeof(__int128_t));
    *tmp = _Val;
    data_int_segment[data_int_segment_len] = tmp;
    ++data_int_segment_len;
    return (SIZE_REF)data_int_segment[data_int_segment_len-1];
}

SIZE_REF yel_alloc_Str_data(char* _Val) {
    return (SIZE_REF)_Val;
}

SIZE_REF yel_alloc_Bool_data(_Bool _Val) {
    for (unsigned long long i = 0; i < data_bool_segment_len; i++) {
        if (*(_Bool*)data_bool_segment[i] == _Val) {
            return (SIZE_REF)data_bool_segment[i];
        }
    }
    
    data_bool_segment = (SIZE_REF*)__builtin_realloc(data_bool_segment, (data_bool_segment_len+1) * sizeof(SIZE_REF));
    _Bool* tmp = (_Bool*)__builtin_malloc(sizeof(_Bool));
    *tmp = _Val;
    data_bool_segment[data_bool_segment_len] = tmp;
    ++data_bool_segment_len;
    return (SIZE_REF)data_bool_segment[data_bool_segment_len-1];
}

OPCODES* yel_init_stack(size_t stack_size) {
    OPCODES* stack = (OPCODES*)__builtin_malloc((size_t)(stack_size * sizeof(OPCODES)));
    if (stack == NULL) {
        puts("memory is correpted!");
        return NULL;
    }

    return stack;
}

void yel_run(OPCODES* stack, OPCODES* opcodes, size_t stack_size) {
    register unsigned long long ip = 0;             // instruntion pointer
    register unsigned long long sp = 0;             // stack pointer

_main_loop:
    switch (opcodes->codes[ip]) {
    case OP_HALT:
        goto _halt;

    case POP_VALUE:
        --sp;
        break;

    case DUP_VALUE:
        stack[sp+1] = stack[sp];
        ++sp;
        break;
    }
    
    goto _main_loop;

_halt:
}