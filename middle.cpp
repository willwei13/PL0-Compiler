#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
typedef struct wordtab{
    int type;           
    char id[20];        
    int lineNum;        
    int flag = 0;
};
wordtab wordTable[100];    
char word[20] = "\0";       
 typedef struct SymbolTable { 
    char type[20];
    char variable[20];
    int value;
};
SymbolTable symbolTable[50];    
typedef struct InterCode {
    char op[20];
    char arg1[20];
    char arg2[20];
    char result[20];
};
InterCode interCode[50];
typedef struct LabelTable {
    char name[10];
    int value = -1;
};
LabelTable lableTable[50];  

int grammarline = 0;       
int quaLine = 0;       
int wordIndex = 0;              
int symbolIndex = 0;            
int lableIndex = 0;
int gramerror = 0;     
int ssssss = 0;  
void semerrorprint() {
    printf("(语义错误,行号:%d)\n", wordTable[wordIndex - 1].lineNum);
    ssssss++;
}

void grerrorprint(int type) {
    if (type == 1) {
        if (grammarline != wordTable[wordIndex - 1].lineNum) {
            grammarline = wordTable[wordIndex - 1].lineNum;
            printf("(语法错误,行号:%d)\n", wordTable[wordIndex - 1].lineNum);
            gramerror++;
        }
    }
    else {
        if (grammarline != wordTable[wordIndex - 1].lineNum) {
            grammarline = wordTable[wordIndex - 1].lineNum;
            printf("(语法错误,行号:%d)\n", wordTable[wordIndex - 1].lineNum);
            gramerror++;
        }
    }
}
char words[20][20] ={ 
    "begin","call","const","do","end","if","odd","procedure",
    "read","then","var","while","write"
};   
int IsReservedWord(char* s){
    for (int i = 0; i < 20; i++) 
        if (strcmp(words[i], s) == 0) 
            return 1;
    return 0;
}
char* lab() {
    char temp[3];
    if (lableIndex < 10) {
        temp[0] = (char)(lableIndex + 48);
        temp[1] = '\0';
    }
    else {
        temp[0] = (char)(lableIndex / 10 + 48);
        temp[1] = (char)(lableIndex % 10 + 48);
        temp[2] = '\0';
    }
    strcpy(lableTable[lableIndex].name, "L");
    strcat(lableTable[lableIndex].name, temp);
    int lold = lableIndex;
    lableIndex++;
    return lableTable[lold].name;
}

void label(char s[]) {
    for (int j = 0; j < lableIndex; j++) {
        if (strcmp(lableTable[j].name, s) == 0) {
            lableTable[j].value = quaLine;
            return;
        }
    }
}

char* id(){
    int tt = 0;
    for (tt = 0; tt < strlen(wordTable[wordIndex].id); tt++) word[tt] = wordTable[wordIndex].id[tt];
    word[tt] = '\0';
    if (!IsReservedWord(word) && word[0] != '.'){
        wordIndex++;
        return wordTable[wordIndex - 1].id;
    }
    else grerrorprint(1);
}
void GenerateQuaternion(const char* op, const char* arg1, const char* arg2, const char* result, int e) {
    strcpy(interCode[e].op, op);
    strcpy(interCode[e].arg1, arg1);
    strcpy(interCode[e].arg2, arg2);
    strcpy(interCode[e].result, result);
}
void subroutine(char* Snext);   
void sentence(char* a);         
void com(char* Etrue, char* Efalse); 
void expression();              
void term();                    
void factor();                  
void lexanaly();         
void semanana();        
int Pow(int num, int j);                           
void number(int a);             
void identifier(int a);         
void constant(int a);           
void variable(int a);           
void procedure();  

char flags[39][20] ={
    "begin","call","const","do","end","if","odd","procedure",
    "read","then","var","while","write","(",")",",",";",".",
    "+","-","*","/","=","#","<",">","<=",">=",":=","0","1","2","3",
    "4","5","6","7","8","9",
};

void sentence(char* Snext) {
    if (strcmp(wordTable[wordIndex].id, "if") == 0) {
        wordIndex++;
        char Etrue[4];
        char Efalse[4];
        strcpy(Etrue, lab());
        strcpy(Efalse, Snext);
        com(Etrue, Efalse);
        if (strcmp(wordTable[wordIndex].id, "then") == 0) {
            char S1next[4];
            label(Etrue);
            strcpy(S1next, Snext);
            wordIndex++;
            sentence(S1next);
        }
        else { grerrorprint(0); }
    }
    else if (strcmp(wordTable[wordIndex].id, "while") == 0) {
        wordIndex++;
        char Ebegin[4];
        char Etrue[4];
        char Efalse[4];
        strcpy(Ebegin, lab());
        label(Ebegin);
        strcpy(Etrue, lab());
        strcpy(Efalse, Snext);
        com(Etrue, Efalse);
 
        if (strcmp(wordTable[wordIndex].id, "do") != 0) {
            grerrorprint(1);
        }
        else {
            char S1next[4];
            strcpy(S1next, lab());
            label(Etrue);
            strcpy(S1next, Ebegin);
            wordIndex++;
            sentence(S1next);
        }
    }
    else if (strcmp(wordTable[wordIndex].id, "call") == 0) {
        wordIndex++;
        GenerateQuaternion("call", wordTable[wordIndex].id, "_", "_", quaLine++);
        char* c = id();
        int flag = 0;
        for (int j = 0; j < symbolIndex; j++) {
            if (strcmp(symbolTable[j].variable, c) == 0) {
                if (strcmp(symbolTable[j].type, "procedure") != 0) {
                    semerrorprint();
                }
                flag = 1;
            }
        }
        if (flag == 0) {
            semerrorprint();
        }
    }
    else if (strcmp(wordTable[wordIndex].id, "read") == 0) {
        wordIndex++;
        if (strcmp(wordTable[wordIndex].id, "(") == 0) {
            wordIndex++;
            GenerateQuaternion("read", wordTable[wordIndex].id, "_", "_", quaLine++);
            char* c = id();
            int flag = 0;
            for (int j = 0; j <= symbolIndex; j++) {
                if (strcmp(symbolTable[j].variable, c) == 0) {
                    if (strcmp(symbolTable[j].type, "procedure") == 0) {
                        semerrorprint();
                    }
                    flag = 1;
                }
            }
            if (flag == 0) {
                semerrorprint();
            }
            while (strcmp(wordTable[wordIndex].id, ",") == 0) {
                wordIndex++;
                char* ch = id();
                int flag1 = 0;
                for (int j = 0; j <= symbolIndex; j++) {
                    if (strcmp(symbolTable[j].variable, ch) == 0) {
                        if (strcmp(symbolTable[j].type, "procedure") == 0) {
                            semerrorprint();
                        }
                        flag1 = 1;
                    }
                }
                if (flag == 0) {
                    semerrorprint();
                }
 
            }
            if (strcmp(wordTable[wordIndex].id, ")") == 0) {
                wordIndex++;
            }
            else  grerrorprint(1);
        }
        else  grerrorprint(0);
    }
    else if (strcmp(wordTable[wordIndex].id, "write") == 0) {
        wordIndex++;
 
        if (strcmp(wordTable[wordIndex].id, "(") == 0) {
            wordIndex++;
            expression();
            GenerateQuaternion(wordTable[wordIndex - 2].id, wordTable[wordIndex - 3].id, wordTable[wordIndex - 1].id, "T1", quaLine++);
            while (strcmp(wordTable[wordIndex].id, ",") == 0) {
                wordIndex++;
                expression();
            }
            if (strcmp(wordTable[wordIndex].id, ")") == 0) {
                wordIndex++;
                GenerateQuaternion("write", "T1", "_", "_", quaLine++);
            }
            else  grerrorprint(0);
        }
        else {
            grerrorprint(1);
            expression();
            while (strcmp(wordTable[wordIndex].id, ",") == 0) {
                wordIndex++;
                expression();
            }
            if (strcmp(wordTable[wordIndex].id, ")") == 0) {
                wordIndex++;
            }
            else  grerrorprint(0);
        }
    }
    else if (strcmp(wordTable[wordIndex].id, "begin") == 0) {
        wordIndex++;
        char S1next[4];
        strcpy(S1next, lab());
        sentence(S1next);
        label(S1next);
        if (strcmp(wordTable[wordIndex].id, ";") == 0) {
            wordIndex++;
            char S2next[4];
            strcpy(S2next, lab());
            sentence(S2next);
            label(S2next);
        }
        else {
            grerrorprint(0);
        }
        while (strcmp(wordTable[wordIndex].id, ";") == 0) {
            wordIndex++;
            char S3next[4];
            strcpy(S3next, lab());
            sentence(S3next);
            label(S3next);
        }
        if (strcmp(wordTable[wordIndex].id, "end") == 0) {
            wordIndex++;
        }
        else { grerrorprint(0); return; }
 
    }
    else {
        int tt = 0;
        for (tt = 0; tt < strlen(wordTable[wordIndex].id); tt++) word[tt] = wordTable[wordIndex].id[tt];
        word[tt] = '\0';
        if (!IsReservedWord(word) && word[0] != '.') {
            id();
            if (strcmp(wordTable[wordIndex].id, ":=") == 0) {
                wordIndex++;
                expression();
                GenerateQuaternion(wordTable[wordIndex - 2].id, wordTable[wordIndex - 3].id, wordTable[wordIndex - 1].id, wordTable[wordIndex - 5].id, quaLine++);
                GenerateQuaternion("ret", "_", "_", "_", quaLine++);
            }
 
        }
    }
}




int Pow(int num, int j) {
    for (int a = 1; a < j; a++) {
        num = num * num;
    }
    return num;
}
void lexanaly() {
    int i = 0, key = 0, lineNum = 1, inputFlag = 0, wordFlag = 0;
    char input;
    char word[100];

    while ((input = getchar()) != EOF) {
        switch (input) {
            case 'a' ... 'z':
            case 'A' ... 'Z':
            case '0' ... '9':
                if (!key) {
                    switch (input) {
                        case '0' ... '9':
                            inputFlag = 1;
                            break;
                    }
                } else {
                    if (inputFlag) {
                        switch (input) {
                            case 'a' ... 'z':
                            case 'A' ... 'Z':
                                wordFlag = 1;
                                break;
                        }
                    }
                }
                word[key++] = input;
                word[key] = '\0';
                continue;
            default:
                if (strcmp(word, "") != 0) {
                    strcpy(wordTable[i].id, word);
                    wordTable[i].lineNum = lineNum;
                    strcpy(word, "");
                    key = 0;
                    if (wordFlag) {
                        wordFlag = 0;
                        wordTable[i].flag = 1;
                    }
                    if (strlen(wordTable[i].id) > 8) {
                        wordTable[i].flag = 1;
                    }
                    i++;
                    inputFlag = 0;
                }

                switch (input) {
                    case ' ':
                    case 10:
                    case 13:
                    case '\t':
                        if (input == '\n')
                            lineNum++;
                        continue;
                    default:
                        word[0] = input;
                        switch (word[0]) {
                            case ':':
                            case '>':
                            case '<':
                                input = getchar();
                                switch (input) {
                                    case '=':
                                        word[1] = input;
                                        word[2] = '\0';
                                        break;
                                    default:
                                        strcpy(wordTable[i].id, word);
                                        wordTable[i].lineNum = lineNum;
                                        strcpy(word, "");
                                        key = 0;
                                        i++;
                                        break;
                                }
                                break;
                            case '/':
                                strcpy(word, "");
                                key = 0;
                                input = getchar();
                                switch (input) {
                                    case '/':
                                        while (input != 10)
                                            input = getchar();
                                        lineNum++;
                                        break;
                                    case '*':
                                        if (input == 10)
                                            lineNum++;
                                        while (1) {
                                            char c;
                                            input = getchar();
                                            if (input == 10)
                                                lineNum++;
                                            if (input == '*' && (c = getchar()) == '/')
                                                break;
                                        }
                                        break;
                                }
                                break;
                            default:
                                word[1] = '\0';
                                strcpy(wordTable[i].id, word);
                                wordTable[i].lineNum = lineNum;
                                strcpy(word, "");
                                key = 0;
                                i++;
                                break;
                        }
                        break;
                }
        }
    }
 
    for (int j = 0; j < i; j++) {
        for (int k = 0; k < 38; k++) {
            if ((strcmp(wordTable[j].id, flags[k])) == 0) {
                if (k >= 0 && k < 13) {
                    wordTable[j].type = 1;
                    wordTable[j].flag = 0;
                }
                else if (k >= 13 && k < 18)
                    wordTable[j].type = 5;
                else if (k >= 18 && k < 29)
                    wordTable[j].type = 4;
                else if (k >= 29 && k < 39)
                    wordTable[j].type = 3;
                break;
            }
            else if (wordTable[j].id[0] >= '0' && wordTable[j].id[0] <= '1' && wordTable[j].flag == 0) {
                wordTable[j].type = 6;
            }
            else if (wordTable[j].id[0] == '@' || wordTable[j].id[0] == '&' || wordTable[j].id[0] == '!') {
                wordTable[j].type = 7;
                wordTable[j].flag = 1;
            }
            else if (strlen(wordTable[j].id) > 8) {
                if (wordTable[j].id[0] >= '0' && wordTable[j].id[0] <= '1') {
                    wordTable[j].type = 8;
                }
                else {
                    wordTable[j].type = 10;
                }
            }
            else if (wordTable[j].flag == 1) {
                wordTable[j].type = 9;
            }
            else {
                wordTable[j].type = 2;
            }
        }
    }
}



void number(int a) {  
    int num = 0;
    int j = strlen(wordTable[wordIndex].id) - 1;
    while (j >= 0) {
        num += (int)(wordTable[wordIndex].id[j] - 48) * Pow(10, j);
        j--;
    }
    symbolTable[a].value = num;
    wordIndex++;
}

void identifier(int a) {
    strcpy(symbolTable[a].variable, wordTable[wordIndex].id);
    wordIndex++;
}
void semanana() {
    procedure();
    if (ssssss == 0) {
        printf("语义正确\n");
        for (int o = 0; o < lableIndex; o++) {
            for (int p = 0; p < quaLine; p++) {
                if (strcmp(interCode[p].result, lableTable[o].name) == 0) {
                    char typ[10];
                    char tem[3];
                    strcpy(typ, "$");
                    if (lableTable[o].value + 1 < 10) {
                        tem[0] = (char)(lableTable[o].value + 1 + 48);
                        tem[1] = '\0';
                    }
                    else {
                        tem[0] = (char)((lableTable[o].value + 1) / 10 + 48);
                        tem[1] = (char)((lableTable[o].value + 1) % 10 + 48);
                        tem[2] = '\0';
                    }
                    strcat(typ, tem);
                    strcpy(interCode[p].result, typ);
                }
            }
        }
 
        printf("中间代码:\n");
        for (int i = 0; i < quaLine; i++)
            printf("(%d)(%s,%s,%s,%s)\n", i + 1, interCode[i].op, interCode[i].arg1, interCode[i].arg2, interCode[i].result);
 
        printf("符号表:\n");
        for (int i = 0; i < symbolIndex; i++) {
            if (strcmp(symbolTable[i].type, "procedure") == 0) {
                if (i != symbolIndex - 1)
                    printf("%s %s\n", symbolTable[i].type, symbolTable[i].variable);
                else
                    printf("%s %s", symbolTable[i].type, symbolTable[i].variable);
            }
            else {
                if (i != symbolIndex - 1)
                    printf("%s %s %d\n", symbolTable[i].type, symbolTable[i].variable, symbolTable[i].value);
                else
                    printf("%s %s %d", symbolTable[i].type, symbolTable[i].variable, symbolTable[i].value);
            }
        }
    }
}
 

void procedure()  {
    char Snext[4];
    GenerateQuaternion("syss", "_", "_", "_", quaLine++);
    strcpy(Snext, lab());
    subroutine(Snext);
    if (strcmp(wordTable[wordIndex].id, ".") == 0)
    {
        label(Snext);
        GenerateQuaternion("syse", "_", "_", "_", quaLine++);
    }
    else { grerrorprint(0); return; }
 
}

void subroutine(char* Snext) {
    char S1next[4];
    strcpy(S1next, lab());
    if (strcmp(wordTable[wordIndex].id, "const") == 0)
    {
        wordIndex++;
        constant(symbolIndex++);
        while (strcmp(wordTable[wordIndex].id, ",") == 0)
        {
            wordIndex++;
            constant(symbolIndex++);
        }
        if (strcmp(wordTable[wordIndex].id, ";") == 0) {
            wordIndex++;
            char S2next[4];
            strcpy(S2next, lab());
            subroutine(S2next);
            label(S2next);
        }
        else {
            grerrorprint(0);
        }
    }
    else if (strcmp(wordTable[wordIndex].id, "var") == 0) {
        wordIndex++;
        for (int o = 0; o < symbolIndex; o++) {
            if (strcmp(symbolTable[o].variable, wordTable[wordIndex].id) == 0) {
                semerrorprint();
            }
        }
        variable(symbolIndex++);
        while (strcmp(wordTable[wordIndex].id, ",") == 0)
        {
            wordIndex++;
            variable(symbolIndex++);
        }
        if (strcmp(wordTable[wordIndex].id, ";") == 0) {
            wordIndex++;
            char S2next[4];
            strcpy(S2next, lab());
            subroutine(S2next);
            label(S2next);
        }
        else {
            grerrorprint(0);
        }
 
    }
    else if (strcmp(wordTable[wordIndex].id, "procedure") == 0) {
        wordIndex++;
 
        id();
        GenerateQuaternion("procedure", wordTable[wordIndex - 1].id, "_", "_", quaLine++);
        strcpy(symbolTable[symbolIndex].type, "procedure");
        strcpy(symbolTable[symbolIndex].variable, wordTable[wordIndex - 1].id);
        symbolIndex++;
        if (strcmp(wordTable[wordIndex].id, ";") == 0)
        {
            wordIndex++;
            char S2next[4];
            strcpy(S2next, lab());
            subroutine(S2next);
            label(S2next);
            if (strcmp(wordTable[wordIndex].id, ";") == 0)
            {
                wordIndex++;
 
            }
            else {
                grerrorprint(1);
 
            }
        }
        else {
            grerrorprint(0);
        }
        while (strcmp(wordTable[wordIndex].id, "procedure") == 0)
        {
            wordIndex++;
            id();
            GenerateQuaternion("procedure", wordTable[wordIndex - 1].id, "_", "_", quaLine++);
            strcpy(symbolTable[symbolIndex].type, "procedure");
            strcpy(symbolTable[symbolIndex].variable, wordTable[wordIndex - 1].id);
            symbolIndex++;
            if (strcmp(wordTable[wordIndex].id, ";") == 0)
            {
                wordIndex++;
                char S3next[4];
                strcpy(S3next, lab());
                subroutine(S3next);
                label(S3next);
                if (strcmp(wordTable[wordIndex].id, ";") == 0) {
                    wordIndex++;
                }
                else  grerrorprint(1);
            }
            else {
                grerrorprint(0);
            }
        }
 
    }
 
    sentence(S1next);
    label(S1next);
 
}

void constant(int a) {
    strcpy(symbolTable[a].type, "const");
    identifier(a);
    GenerateQuaternion(symbolTable[a].type, symbolTable[a].variable, "_", "_", quaLine++);
    if (strcmp(wordTable[wordIndex].id, "=") == 0)
    {
        wordIndex++;
        number(a);
        GenerateQuaternion("=", wordTable[wordIndex - 1].id, "_", wordTable[wordIndex - 3].id, quaLine++);
    }
    else { grerrorprint(0); }
}

void variable(int a) {
    strcpy(symbolTable[a].type, "var");
    id();
    strcpy(symbolTable[a].variable, wordTable[wordIndex - 1].id);
    GenerateQuaternion(symbolTable[a].type, symbolTable[a].variable, "_", "_", quaLine++);
}


void com(char* Etrue, char* Efalse) {
    expression();
    char op1[5];
    strcpy(op1, "j");
    char op2[5];
    strcpy(op2, "j");
    char ch[6][3] = { "=","<",">","<=",">=","#" };
    if (strcmp(wordTable[wordIndex].id, "=") == 0 || strcmp(wordTable[wordIndex].id, "#") == 0 || strcmp(wordTable[wordIndex].id, "<") == 0 || strcmp(wordTable[wordIndex].id, "<=") == 0 || strcmp(wordTable[wordIndex].id, ">") == 0 || strcmp(wordTable[wordIndex].id, ">=") == 0) {
        int oldi = wordIndex;
        int la;
        for (int o = 0; o < 6; o++) {
            if (strcmp(ch[o], wordTable[wordIndex].id) == 0) {
                la = o;
                break;
            }
        }
        strcat(op1, ch[la]);
        strcat(op2, ch[5 - la]);
        wordIndex++;
        expression();
        GenerateQuaternion(op1, wordTable[oldi - 1].id, wordTable[oldi + 1].id, Etrue, quaLine++);
        if (strcmp(wordTable[oldi - 2].id, "while") == 0) {
            GenerateQuaternion(op2, wordTable[oldi - 1].id, wordTable[oldi + 1].id, Efalse, quaLine++);
        }
    }
    else if (strcmp(wordTable[wordIndex].id, "odd") == 0) {
        wordIndex++;
        expression();
    }
    else  grerrorprint(0);
}

void expression()         {
    term();
    if (strcmp(wordTable[wordIndex].id, "+") == 0 || strcmp(wordTable[wordIndex].id, "-") == 0){
        char ch = wordTable[wordIndex].id[0];
        wordIndex++;
        term();
        while (strcmp(wordTable[wordIndex].id, "+") == 0 || strcmp(wordTable[wordIndex].id, "-") == 0){
            char ch = wordTable[wordIndex].id[0];
            wordIndex++;
            term();
        }
    }
}

void term() {
    factor();
    while (strcmp(wordTable[wordIndex].id, "*") == 0 || strcmp(wordTable[wordIndex].id, "/") == 0) {
        wordIndex++;
        factor();
    }
}

void factor() {
    if (strcmp(wordTable[wordIndex].id, "(") == 0) {
        wordIndex++;
        expression();
        if (strcmp(wordTable[wordIndex].id, ")") == 0) {
            wordIndex++;
        }
        else grerrorprint(0);
    }
    else {
        if (wordTable[wordIndex].type == 2) {
            int flag = 0;
            for (int j = 0; j < symbolIndex; j++) {
                if (strcmp(wordTable[wordIndex].id, symbolTable[j].variable) == 0) {
                    flag = 1;
                }
            }
            if (flag == 0) {
                semerrorprint();
            }
        }
        wordIndex++;
    }
}
 
int main() {
    lexanaly();
    semanana();
    return 0;
}