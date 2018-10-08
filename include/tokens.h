#ifndef TOKEN_HEADER
    #define TOKEN_HEADER

enum tokens {
//Function related
    DEF = 1,
    COLON,
    RETURN,

    INDENT,
    DEDENT,
 
    PRINT,
    IF,
    ELSE,
    FOR,
    WHILE,
    IDENTIFIER,
    NEWLINE,

    STRING,
    NUMBER,
    REAL,

//operators
    STAR,
    PLUS,
    SLASH,
    MINUS,
    EQUALSEQUALS,
    EQUALS,

    LPARENTH,
    RPARENTH,
    COMMA,

    BACKSLASH,
    WHITESPACE,

    TRUE,
    FALSE,
    COMMENT

};


#endif
