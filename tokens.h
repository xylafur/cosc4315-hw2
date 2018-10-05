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

};

char * NAMES [] = {"", "DEF", "COLON", "RETURN", "INDENT", "DEDENT", "PRINT",
                   "IF", "ELSE", "FOR", "WHILE", "IDENTIFIER", "NEWLINE",
                   "STRING", "NUMBER", "REAL", "STAR", "PLUS", "SLASH",
                   "MINUS", "EQUALSEQUALS", "EQUALS", "LPARENTH", "RPARENTH",
                   "COMMA", "BACKSLASH"};


