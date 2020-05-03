#include <stdio.h>
#include <float.h>

#define POOLSIZE  (256 * 1024)  // arbitrary size
#define SYMTABSIZE (1024*8)
#define MAXNAMESIZE 32
#define STRBSIZE 1024

typedef struct symStruct {
    int type;
    char name[MAXNAMESIZE];
    double value;
    union {
        char* funcp;
        double* list;
    } pointer;
    int layerNum;
} symbol;
symbol symtab[SYMTABSIZE];
int symPointer = 0;
int currentLayer = 0;

//int line = 0;   // line number
char* src, * old_src;

enum {
    debug, run
};
int compileState = 0;

double return_val = 0;

// tokens and classes (operators last and in precedence order)
enum {
    Num = 128, Char, Str, Array, Func,
    Else, If, Return, While, Print,
    Assign, OR, AND, Equal, Sym, FuncSym, ArraySym, Void,
    Nequal, LessEqual, GreatEqual, Inc, Dec
};
int token;  // current token
union tokenValue {
    symbol* ptr;
    double val;
} token_val;

char tempStrBuffer[STRBSIZE];

double function();
double stblock();
void eval();
double statement();
int boolOR();
int boolAND();
int boolexp();
double expression();
double factor();
double term();
void match(int tk);
void next();

void next() {
    char* last_pos;

    while (token = *src) {
        ++src;
        if (token == '\n') {                // a new line
            //line++;
        }
        else if (token == '#') {            // skip comments
            while (*src != 0 && *src != '\n') {
                src++;
            }
        }
        else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_')) {
            last_pos = src - 1;
            char nameBuffer[MAXNAMESIZE];
            nameBuffer[0] = token;
            while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_')) {
                nameBuffer[src - last_pos] = *src;
                src++;
            }
            nameBuffer[src - last_pos] = 0;
            int i;
            for (i = symPointer-1; i >= 0; --i) {
                if (strcmp(nameBuffer, symtab[i].name) == 0) {
                    if (symtab[i].type == Num || symtab[i].type == Char) {
                        token_val.ptr = &symtab[i];
                        token = Sym;
                    }
                    else if (symtab[i].type == Func) {
                        token_val.ptr = &symtab[i];
                        token = symtab[i].type;
                    }
                    else if (symtab[i].type == Array) {
                        token_val.ptr = &symtab[i];
                        token = symtab[i].type;
                    }
                    else {
                        token = symtab[i].type;
                    }
                    return;
                }
            }
            strcpy(symtab[symPointer].name, nameBuffer);
            symtab[symPointer].layerNum = currentLayer;
            symtab[symPointer].type = Void;
            token_val.ptr = &symtab[symPointer];
            symPointer++;
            token = Sym;
            return;
        }
        else if (token >= '0' && token <= '9') {
            token_val.val = token - '0';
            while (*src >= '0' && *src <= '9') {
                token_val.val = token_val.val * 10.0 + *src++ - '0';
            }
            if (*src == '.') {
                src++;
                int countDig = 1;
                while (*src >= '0' && *src <= '9') {
                    token_val.val = token_val.val + (*src++ - '0')/(10.0 * countDig++);
                }
            }
            token = Num;
            return;
        }
        else if (token == '\'') {
            token_val.val = *src;
            token = Char;
            return;
        }
        else if (token == '"' ) {
            last_pos = src;
            char tval;
            int numCount = 0;
            while (*src != 0 && *src != token) {
                src++;
                numCount++;          
            }

            src++;
            token_val.ptr = last_pos;
            return;
        }
        else if (token == '=') {            // parse '==' and '='
            if (*src == '=') {
                src++;
                token = Equal;
            }
            return;
        }
        else if (token == '+') {            // parse '+' and '++'
            if (*src == '+') {
                src++;
                token = Inc;
            }
            return;
        }
        else if (token == '-') {            // parse '-' and '--'
            if (*src == '-') {
                src++;
                token = Dec;
            }
            return;
        }
        else if (token == '!') {               // parse '!='
            if (*src == '=') {
                src++;
                token = Nequal;
            }
            return;
        }
        else if (token == '<') {               // parse '<=',  or '<'
            if (*src == '=') {
                src++;
                token = LessEqual;
            }
            return;
        }
        else if (token == '>') {                // parse '>=',  or '>'
            if (*src == '=') {
                src++;
                token = GreatEqual;
            }
            return;
        }
        else if (token == '|') {                // parse  '||'
            if (*src == '|') {
                src++;
                token = OR;
            }
            return;
        }
        else if (token == '&') {                // parse  '&&'
            if (*src == '&') {
                src++;
                token = AND;
            }
            return;
        }
        else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '!' || token == ';' || token == ',' ||
            token == '(' || token == ')' || token == '{' || token == '}' || token == '>' || token == '<' || token == '=' || token == '[' || token == ']') {
            return;
        }
        else if (token == ' ' || token == '\t') {        }
        else {
            printf("unexpected token: %d\n", token);
        }
    }
}

void match(int tk) {
    if (token == tk) {
        next();
    }
    else {
        printf("expected token: %d\n", tk);
        //exit(-1);
    }
}

/*------------------------------------------------------------*/

double term() {
    double temp = factor();
    while (token == '*' || token == '/') {
        if (token == '*') {
            match('*');
            temp *= factor();
        }
        else {
            match('/');
            temp /= factor();
        }
    }
    return temp;
}

double factor() {
    double temp = 0;
    if (token == '(') {
        match('(');
        temp = expression();
        match(')');
    }
    else if (token == Inc) {
        match(Inc);
        temp = token_val.ptr->value;
        (token_val.ptr->value)++;
        match(Sym);
    }
    else if (token == Dec) {
        match(Dec);
        temp = token_val.ptr->value;
        (token_val.ptr->value)--;
        match(Sym);
    }
    else if(token == Num ||  token == Char){
        temp = token_val.val;
        match(Num);
    }
    else if (token == Sym) {
        temp = token_val.ptr->value;
        match(Sym);
    }
    else if (token == FuncSym) {
        return function();
    }
    else if (token == ArraySym) {
        symbol* ptr = token_val.ptr;
        match('[');
        int index = (int)expression();
        if (index > 0 && index < token_val.ptr->value) {
            return token_val.ptr->pointer.list[index];
        }
        match(']');
    }
    return temp;
}

double expression() {
    double temp = term();
    while (token == '+' || token == '-') {
        if (token == '+') {
            match('+');
            temp += term();
        }
        else {
            match('-');
            temp -= term();
        }
    }
    return temp;
}

int boolexp() {
    if (token == '(') {
        match('(');
        int result = boolOR();
        match(')');
        return result;
    }
    else if (token == '!') {
        match('!');
        return boolexp();
    }
    double temp = expression();
    if (token == '>') {
        match('>');
        return temp > expression();
    }
    else if (token == '<') {
        match('<');
        return temp < expression();
    }
    else if (token == GreatEqual) {
        match(GreatEqual);
        return temp >= expression();
    }
    else if (token == LessEqual) {
        match(LessEqual);
        return temp <= expression();
    }
    else if (token == Equal) {
        match(Equal);
        return temp == expression();
    }
}

int boolAND() {
    int val = boolexp();           
    while (token == AND) {
        match(AND);
        if (val == 0)    return 0;         // short cut
        val = val & boolexp();
        if (val == 0) return 0;
    }
}

int boolOR() {
    int val = boolAND();
    while (token == OR) {
        match(OR);
        if (val == 1)    return 1;         // short cut
        val = val | boolAND();
    }
}

void skipStatments() {
    match('{');
    while (token && token != '}')
        next();
    match('}');
}

double statement() {
    if (token == If) {
        match(If);
        match('(');
        int boolresult = boolOR();
        match(')');
        if (boolresult) {
            if (DBL_MAX == stblock()) {
                return DBL_MAX;
            }
        }
        else skipStatments();
        if (token == Else) {
            match('Else');
            if (!boolresult) {
                if (DBL_MAX == stblock()) {
                    return DBL_MAX;
                }
            }
            else skipStatments();
        }
    }
    else if (token == While) {
        match(While);
        char* whileStartPos = src;
        char* whileStartOldPos = old_src;
        int boolresult;
        do {
            src = whileStartPos;
            old_src = whileStartOldPos;
            token = '(';
            match('(');
            boolresult = boolOR();
            match(')');
            if (boolresult) {
                if (DBL_MAX == stblock()) {
                    return DBL_MAX;
                }
            }
            else skipStatments();
        }while (boolresult);
    }
    else if (token == Sym) {
        symbol* s = token_val.ptr;
        match(Sym);
        match('=');
        if (token == Str) {
            s->pointer.funcp = token_val.ptr;
            s->type = Str;
        }
        else if (token == Char) {
            s->value = token_val.val;
            s->type = Char;
        }
        else{
            s->value = expression();
            s->type = Num;
        }
    }
    else if (token == Array) {
        match(Array);
        symbol* s = token_val.ptr;
        match(Sym);
        match('(');
        int length = (int)expression();
        match(')');
        s->pointer.list = malloc(sizeof(symbol) * length + 1);
        s->value = length;
        s->type = Array;
    }
    else if (token == Func) {
        match(Func);
        symbol* s = token_val.ptr;
        s->type = Func;
        s->pointer.funcp = src;
        match(Sym);
        skipStatments();
    }
    else if (token == Return) {
        match(Return);
        match('(');
        return_val = expression();
        match(')');
        return DBL_MAX;
    }
    else if (token == Print) {
        match(Print);
        match('(');
        double temp = expression();
        match(')');
        printf("%lf\n", temp);
    }
    return 0;
}

double stblock() {
    match('{');
    while (token != '}') {
        if (DBL_MAX == statement()) {
            return DBL_MAX;
        }
        match(';');
    }
    match('}');
}

void eval() {
    next();
    while (token != 0) {
        statement();
        match(';');
    }
}

double function() {
    currentLayer++;

    char* StartPos = src;
    char* StartOldPos = old_src;
    symbol* s = token_val.ptr;
    match(FuncSym);
    match('(');
    while (token != ')') {
        symtab[symPointer] = *token_val.ptr;
        strcpy(symtab[symPointer].name, token_val.ptr->name);
        symtab[symPointer].layerNum = currentLayer;
        symPointer++;
        match(Sym);
        match(',');
    }
    match(')');
    old_src = src = s->pointer.funcp;
    stblock();
    src = StartPos;
    old_src = StartOldPos;

    while (symtab[symPointer - 1].layerNum == currentLayer) {
        symPointer--;
    }
    currentLayer--;
    return return_val;
}

/*----------------------------------------------------------------*/

int main(int argc, char** argv)
{
    int i, fd;
    src = "array func else if return while print";
    for (i = Array; i <= Print; ++i) {
        next();
        symtab[symPointer -1].type = i;
    }
    if (!(src = old_src = malloc(POOLSIZE))) {
        printf("could not malloc(%d) for source area\n", POOLSIZE);
        return -1;
    }

    if (argc > 0) {
        if (**argv == '-' && (*argv)[1] == 'd') {
            compileState = debug;
            ++argv; --argc;
        }
        else {
            compileState = run;
            ++argv; --argc;
        }
    }
    if (argc < 1) {
        printf("usage: tryc [-d] file ...\n");
        return 1;
    }

    if ((fd = open(*argv, 0)) < 0) {                // read the source file
        printf("could not open(%s)\n", *argv);
        return -1;
    }
    if ((i = read(fd, src, POOLSIZE - 1)) <= 0) {
        printf("read() returned %d\n", i);
        return -1;
    }
    src[i] = 0; // add EOF character
    close(fd);
    eval();
}


