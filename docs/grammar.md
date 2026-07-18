# Vise Grammar

This document outlines the syntax for Vise using Extended Backus-Naur Form (EBNF) notation. 

*Note: This is a work in progress and will evolve as the language matures.*

### Lexical Rules
```ebnf
<identifier> ::= [a-zA-Z_] [a-zA-Z0-9_]*
<number>     ::= [0-9]+ ( "." [0-9]+ )?
<string>     ::= '"' <any_character>* '"'
```

### Declarations
```ebnf
<variable_decl> ::= [ "static" ] [ "const" ] <type> <identifier> [ "=" <expression> ] ";"
<function_decl> ::= [ <decorator_list> ] <type> <identifier> "(" <parameter_list> ")" <block>
<struct_decl>   ::= [ <decorator_list> ] "struct" <identifier> "{" <struct_body> "}" ";"
```

### Decorators
```ebnf
<decorator_list> ::= "@" <identifier> [ "(" <expression_list> ")" ]
                   | "@[" <identifier_list> "]"
```

### Types
```ebnf
<base_type> ::= "int" | "char" | "bool" | "void" | <identifier>
<type>      ::= <base_type> { "*" }
```

### Statements & Blocks
```ebnf
<block>     ::= "{" <statement>* "}"
<statement> ::= <variable_decl>
              | <expression> ";"
              | "return" [ <expression> ] ";"
              | "defer" <statement>
              | <if_statement>
```
