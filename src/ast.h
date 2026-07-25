#pragma once
#include "utils.h"

typedef enum {
    // SYMBOLS
    TOKEN_PLUS, TOKEN_MINUS,
    TOKEN_STAR, TOKEN_SLASH,
    TOKEN_EQ,
    TOKEN_PAREN_LEFT, TOKEN_PAREN_RIGHT,
    TOKEN_BRACE_LEFT, TOKEN_BRACE_RIGHT,
    TOKEN_CURLY_LEFT, TOKEN_CURLY_RIGHT,
    TOKEN_BANG, TOKEN_AT,
    TOKEN_HASH, TOKEN_DOLLAR,
    TOKEN_PERCENT,
    TOKEN_QUOTE, TOKEN_DOUBLE_QUOTE,
    TOKEN_QUESTION,
    TOKEN_COMMA, TOKEN_SEMI, TOKEN_COLON,
    TOKEN_DOT,

    // OPERATORS
    TOKEN_PLUS_PLUS, TOKEN_PLUS_EQ,
    TOKEN_MINUS_MINUS, TOKEN_MINUS_EQ,
    TOKEN_MULTI_EQ, TOKEN_DIV_EQ,
    TOKEN_EQ_EQ, TOKEN_NOT_EQ,
    TOKEN_LT, TOKEN_LT_EQ,
    TOKEN_GT, TOKEN_GT_EQ,
    TOKEN_POW, TOKEN_XOR,
    TOKEN_LOGICAL_AND, TOKEN_LOGICAL_OR,
    TOKEN_BIT_OR, TOKEN_BIT_OR_EQ,
    TOKEN_BIT_AND, TOKEN_BIT_AND_EQ,
    TOKEN_SHIFT_LEFT, TOKEN_SHIFT_LEFT_EQ,
    TOKEN_SHIFT_RIGHT, TOKEN_SHIFT_RIGHT_EQ,
    TOKEN_BIT_NOT, TOKEN_XOR_EQ,

    // KEYWORDS
    TOKEN_WHILE, TOKEN_FOR,
    TOKEN_BREAK, TOKEN_RETURN,
    TOKEN_CONTINUE,
    TOKEN_IF, TOKEN_ELSE,
    TOKEN_IMPORT, TOKEN_STATIC,
    TOKEN_DEFER, TOKEN_COMPTIME,
    TOKEN_STRUCT, TOKEN_TYPE,
    TOKEN_CONST, TOKEN_ENUM,
    TOKEN_TRUE, TOKEN_FALSE,

    // TYPES
    TOKEN_CHAR,
    TOKEN_INT, TOKEN_FLOAT,
    TOKEN_BOOL,
    TOKEN_VOID, TOKEN_NULL,

    // LITERALS
    TOKEN_LIT_STR, TOKEN_LIT_CHAR,
    TOKEN_LIT_INT, TOKEN_LIT_FLOAT,
    TOKEN_IDENT,

    // OTHER
    TOKEN_EOF,
    TOKEN_UNKNOWN,
    TOKEN_COUNT,
} TokenType;

typedef enum {
    TYPE_VOID, TYPE_NULL,
    TYPE_INT, TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_BOOL, TYPE_CUSTOM,
} TypeKind;

typedef struct {
    TypeKind kind;
    StringStream name;
    int pointer_depth;
} DataType;

typedef struct {
    TokenType type;
    StringStream view;
    size_t line;
    size_t col;
} Token;

// NODES
typedef enum {
    // DECLARATIONS
    NODE_VAR_DECL,
    NODE_FUNC_DECL,
    NODE_STRUCT_DECL,

    // STATEMENTS
    NODE_BLOCK,
    NODE_IF,
    NODE_WHITE, NODE_FOR,
    NODE_RETURN, NODE_BREAK,
    NODE_CONTINUE,
    NODE_DEFER,
    NODE_EXPR_STMT,

    // EXPRESSIONS
    NODE_LITERAL,
    NODE_IDENTIFIER,
    NODE_BINOP,
    NODE_UNARY,
    NODE_CALL,
    NODE_MEMBER_ACCESS,
    NODE_INDEX,
    NODE_STRUCT_INIT,
    NODE_ARRAY_INIT,
} NodeKind;

typedef struct Node Node;

typedef struct {
    Node *left;
    Node *right;
    TokenType op;
} BinOpNode;

typedef struct {
    Node *operand;
    TokenType op;
} UnaryNode;

typedef struct {
    Token token;
} LiteralNode;

typedef struct {
    Token name;
} IdentifierNode;

typedef struct {
    Node **statements;
    size_t count;
} BlockNode;

typedef struct {
    Node *condition;
    Node *then_branch;
    Node *else_branch;
} IfNode;

typedef struct {
    Node *condition;
    Node *body;
} WhileNode;

typedef struct {
    Node *init;
    Node *condition;
    Node *increment;
    Node *body;
} ForNode;

typedef struct {
    Node *expr;
} ReturnNode;

typedef struct {
    Node *stmt;
} DeferNode;

typedef struct {
    Node *expr;
} ExprStmtNode;

typedef struct {
    Token name;
    DataType type;
    Node *initializer; // Can be NULL
    bool is_const;
    bool is_static;
} VarDeclNode;

typedef struct {
    Token name;
    DataType return_type;
    Node **params; // Array of VarDeclNodes
    size_t param_count;
    Node *body;
    bool is_comptime;
} FuncDeclNode;

typedef struct {
    Token name;
    Node **fields; // Array of VarDeclNodes and FuncDeclNodes
    size_t field_count;
} StructDeclNode;

typedef struct {
    Token name;
    Token *variants; // Array of identifier tokens
    size_t variant_count;
} EnumDeclNode;

typedef struct {
    Token alias_name;
    DataType target_type;
} TypeDeclNode;

typedef struct {
    StringStream path;
} ImportNode;

typedef struct {
    Node *callee;
    Node **args;
    size_t arg_count;
} CallNode;

typedef struct {
    Node *object;
    Token property;
} MemberAccessNode;

typedef struct {
    Node *array;
    Node *index;
} IndexNode;

typedef struct {
    Token type_name;
    Token *field_names;
    Node **field_values;
    size_t field_count;
} StructInitNode;

typedef struct {
    Node **elements;
    size_t element_count;
} ArrayInitNode;

struct Node {
    NodeKind kind;
    size_t line;
    size_t col;
    bool _export;
    
    union {
        BinOpNode binop;
        UnaryNode unary;
        LiteralNode literal;
        IdentifierNode identifier;
        BlockNode block;
        IfNode if_stmt;
        WhileNode while_stmt;
        ForNode for_stmt;
        ReturnNode return_stmt;
        DeferNode defer_stmt;
        ExprStmtNode expr_stmt;
        VarDeclNode var_decl;
        FuncDeclNode func_decl;
        StructDeclNode struct_decl;
        EnumDeclNode enum_decl;
        TypeDeclNode type_decl;
        ImportNode import_stmt;
        CallNode call;
        MemberAccessNode member_access;
        IndexNode index;
        StructInitNode struct_init;
        ArrayInitNode array_init;
    } as;
    bool _comptime;
    Node *next;
};
