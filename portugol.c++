#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class TOKEN {
public:
    string lexema;
    int state;
    string token;
    int linha;
    int coluna;

    TOKEN(int s, string l, string t, int lin, int col) {
        lexema = l;
        state = s;
        token = t;
        linha = lin;
        coluna = col;
    }

};

vector<TOKEN> TOKENS;
int pos_token = 0;
bool quebra = true;
bool erro = false;

void printar(string msg) {

    if(!quebra)
        cout << endl;

    cout << msg;
    quebra = false;
}

int dicionario_indice(char c) {

    if(c >= '0' && c <= '9')        return 0;
    else if((c >= 'a' && c <= 'z') || (c>= 'A' && c <= 'Z'))   return 1;
    else if(c == '"')               return 2;
    else if(c == '/')               return 3;
    else if(c == '{')               return 4;
    else if(c == '}')               return 5;
    else if(c == '.')               return 6;
    else if(c == ';')               return 7;
    else if(c == ',')               return 8;
    else if(c == ':')               return 9;
    else if(c == '[')               return 10;
    else if(c == ']')               return 11;
    else if(c == '(')               return 12;
    else if(c == ')')               return 13;
    else if(c == '=')               return 14;
    else if(c == '<')               return 15;
    else if(c == '>')               return 16;
    else if(c == '+')               return 17;
    else if(c == '-')               return 18;
    else if(c == '*')               return 19;
    else if(c == '\n' || c == '\r') return 20;
    else if(c == ' ' || c == '\t')  return 21;
    else if(c == '_')               return 22;
    else                            return -1;
}

string dicionario_token(int state) {

    if(state == 2)       return "ID";
    else if(state == 3)  return "NUM";
    else if(state == 5)  return "REAL";
    else if(state == 7)  return "STRING";
    else if(state == 8)  return "DIVISAO";
    else if(state == 10) return "COMMENT_LINHA";
    else if(state == 12) return "COMMENT_BLOCO";
    else if(state == 13) return "PONTO_VIRGULA";
    else if(state == 14) return "VIRGULA";
    else if(state == 15) return "DOIS_PONTOS";
    else if(state == 16) return "PONTO";
    else if(state == 17) return "ABRE_COLCHETES";
    else if(state == 18) return "FECHA_COLCHETES";
    else if(state == 19) return "ABRE_PARENTESES";
    else if(state == 20) return "FECHA_PARENTESES";
    else if(state == 21) return "IGUAL";
    else if(state == 22) return "MENOR";
    else if(state == 23) return "DIFERENTE";
    else if(state == 24) return "MENOR_IGUAL";
    else if(state == 25) return "ATRIBUICAO";
    else if(state == 26) return "MAIOR";
    else if(state == 27) return "MAIOR_IGUAL";
    else if(state == 28) return "MAIS";
    else if(state == 29) return "MENOS";
    else if(state == 30) return "VEZES";
    else                 return "ERRO";
}

unordered_map<string, string> reservadas = {

    {"algoritmo", "ALGORITMO"},
    {"inicio", "INICIO"},
    {"fim", "FIM"},
    {"variaveis", "VARIAVEIS"},
    {"inteiro", "INTEIRO"},
    {"real", "REAL"},
    {"caractere", "CARACTERE"},
    {"logico", "LOGICO"},
    {"vetor", "VETOR"},
    {"matriz", "MATRIZ"},
    {"tipo", "TIPO"},
    {"funcao", "FUNCAO"},
    {"procedimento", "PROCEDIMENTO"},
    {"se", "SE"},
    {"entao", "ENTAO"},
    {"senao", "SENAO"},
    {"enquanto", "ENQUANTO"},
    {"faca", "FACA"},
    {"para", "PARA"},
    {"de", "DE"},
    {"ate", "ATE"},
    {"passo", "PASSO"},
    {"repita", "REPITA"},
    {"leia", "LEIA"},
    {"imprima", "IMPRIMA"},
    {"verdadeiro", "VERDADEIRO"},
    {"falso", "FALSO"},
    {"e", "E"},
    {"ou", "OU"},
    {"nao", "NAO"},
    {"div", "DIV"},
};

int automato[31][23] = {
    
    // colunas:
    //  0-9, a-z, ", /, {, }, ., ;, ,, :, [, ], (, ), =, <, >, +, -, *, \n  space  _
    //   0    1   2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19  20  21    22
/*0*/  { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0},
/*1*/  { 3,   2,  6, 8,11, 0,16,13,14,15,17,18,19,20,21,22,26,28,29,30, 0,  0,    0},
/*2*/  { 2,   2,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    2}, // ID
/*3*/  { 3,   0,  0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // NUM
/*4*/  { 5,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0},
/*5*/  { 5,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // REAL
/*6*/  { 6,   6,  7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0,  6,    6},
    //  0-9, a-z, ", /, {, }, ., ;, ,, :, [, ], (, ), =, <, >, +, -, *, \n
    //   0    1   2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19  20  21
/*7*/  { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // STRING
/*8*/  { 0,   0,  0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // DIVISAO
/*9*/  { 9,   9,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,10,  9,    9},
/*10*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // COMMENT LINHA
/*11*/ {11,  11, 11,11,11,12,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,  11,  11},
/*12*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // COMMENT BLOCO
/*13*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // PONTO_VIRGULA
/*14*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // VIRGULA
/*15*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // DOIS_PONTOS
    //  0-9, a-z, ", /, {, }, ., ;, ,, :, [, ], (, ), =, <, >, +, -, *, \n 
    //   0    1   2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19  20  
/*16*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // PONTO
/*17*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // ABRE_COLCHETES
/*18*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // FECHA_COLCHETES
/*19*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // ABRE_PARENTESES
/*20*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // FECHA_PARENTESES
/*21*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // IGUAL
/*22*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,24, 0,23, 0,25, 0, 0,  0,    0}, // MENOR
/*23*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // DIFERENTE
    //  0-9, a-z, ", /, {, }, ., ;, ,, :, [, ], (, ), =, <, >, +, -, *, \n
    //   0    1   2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19  20  
/*24*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // MENOR_IGUAL
/*25*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // ATRIBUICAO
/*26*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,27, 0, 0, 0, 0, 0, 0,  0,    0}, // MAIOR
/*27*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // MAIOR IGUAL
/*28*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // MAIS
/*29*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // MENOS
/*30*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,    0}, // VEZES

};

string isPalavraReservada(string lexema, string &token) {

    for(char &c : lexema)
        c = tolower(c);

    if(reservadas.count(lexema)) {
        token = reservadas[lexema];
        return token;
    }

    return "";
}

void analisador_lexico(string s) {

    vector<bool> finais(31, false);
    finais[2] = true; finais[3] = true; finais[5] = true; finais[7] = true;
    finais[8] = true; finais[10] = true; finais[12] = true; finais[13] = true;
    finais[14] = true; finais[15] = true; finais[16] = true; finais[17] = true;
    finais[18] = true; finais[19] = true; finais[20] = true; finais[21] = true;
    finais[22] = true; finais[23] = true; finais[24] = true; finais[25] = true;
    finais[26] = true; finais[27] = true; finais[28] = true; finais[29] = true;
    finais[30] = true; 

    int tam = s.size();

    int linha = 1;
    int coluna = 1;

    int i = 0;

    while (i < tam) {

        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r') {

            if(s[i] == '\n') {
                linha++;
                coluna = 1;
            }

            else 
                coluna++;
            

            i++;
            continue;
        }

        int state = 1;
        int last_final_state = -1;
        int last_final_pos = -1;

        string token = "";

        int j = i;

        int linha_token = linha;
        int coluna_token = coluna;
        while (j < tam) {

            int ind = dicionario_indice(s[j]);
            if(ind == -1) break;
            int prox_estado = automato[state][ind];

            if (prox_estado == 0)
                break;

            token += s[j];
            if(s[j] == '\n') {
                linha++;
                coluna = 1;
            }

            else 
                coluna++;
            
            state = prox_estado;

            if (finais[state]) {
                last_final_state = state;
                last_final_pos = j;
            }

            j++;
        }

        if (last_final_state != -1) {

            string nome_token = dicionario_token(last_final_state);
            string lexema = s.substr(i, last_final_pos - i + 1);

            if(nome_token == "ID") {
                string lexema_minusculo = lexema;
                for(char &c : lexema_minusculo) c = tolower(c);
                
                if(reservadas.count(lexema_minusculo)) {
                    nome_token = reservadas[lexema_minusculo];
                }
            }

            if (nome_token != "COMMENT_LINHA" && nome_token != "COMMENT_BLOCO") {
                TOKENS.push_back(TOKEN(last_final_state, lexema, nome_token, linha_token, coluna_token));
            }

            i = last_final_pos + 1; 
        }
        
        else {
            TOKENS.push_back(TOKEN(-1, string(1, s[i]), dicionario_token(-1), linha, coluna));
            printar("ERRO LEXICO. Linha: " + to_string(linha-1) + " Coluna: " + to_string(coluna) + " -> '" + string(1, s[i]) + "'"); 
            exit(1);
            i++; 
        }
    }
}

// FUNCOES GRAMATICA

TOKEN token_atual() {
    int size = TOKENS.size();
    if(pos_token < size)
        return TOKENS[pos_token];
    return TOKEN(-1, "", "EOF", -1, -1);
}

void eat(string esperado) {

    if(erro) return;

    if(esperado == token_atual().token) 
        pos_token++;
    else {
        printar("ERRO DE SINTAXE. Linha: " + to_string(token_atual().linha-1) + " Coluna: " + to_string(token_atual().coluna) + " -> '" + token_atual().lexema + "'"); 
        exit(1);
    }
}

void Programa();
void ProcedimentoFuncao();
void DeclaraProcedimento();
void DeclaraFuncao();
void Parametros();
void DeclaraParametros();
void BlocoVariaveis();
void Declaracoes();
void DeclaraTipo();
void DeclaraVariaveis();
void DeclaraIdentificador();
void X1();
void VetorMatriz();
void Dimensao();
void X2();
void TipoBasico();
void BlocoComandos();
void ListaComandos();
void Comandos();
void X3();
void X4();
void X5();
void X7();
void Expressao();
void ExpressaoLinha();
void ExpressaoSimples();
void ExpressaoSimplesLinha();
void Termo();
void TermoLinha();
void Fator();
void Variavel();
void X6();
void ExprIter();


// ======================================================
// Programa
// ======================================================

void Programa() {

    if(erro) return;

    if(token_atual().token == "ALGORITMO") {

        eat("ALGORITMO");
        eat("ID");
        eat("PONTO_VIRGULA");

        BlocoVariaveis();
        ProcedimentoFuncao();
        BlocoComandos();

        eat("PONTO");
    }

    else {
        printar("ERRO DE SINTAXE. Linha: " + to_string(token_atual().linha-1) + " Coluna: " + to_string(token_atual().coluna) + " -> '" + token_atual().lexema + "'");  
        exit(1);
    }
}


// ======================================================
// ProcedimentoFuncao
// ======================================================

void ProcedimentoFuncao() {

    if(erro) return;

    if(token_atual().token == "PROCEDIMENTO") {

        DeclaraProcedimento();
        ProcedimentoFuncao();
    }

    else if(token_atual().token == "FUNCAO") {

        DeclaraFuncao();
        ProcedimentoFuncao();
    }

    else
        return;
}


// ======================================================
// DeclaraProcedimento
// ======================================================

void DeclaraProcedimento() {

    if(erro) return;

    eat("PROCEDIMENTO");
    eat("ID");

    Parametros();

    eat("PONTO_VIRGULA");

    DeclaraParametros();
    BlocoVariaveis();
    BlocoComandos();

    eat("PONTO_VIRGULA");
}


// ======================================================
// DeclaraFuncao
// ======================================================

void DeclaraFuncao() {

    if(erro) return;

    eat("FUNCAO");
    eat("ID");

    Parametros();

    eat("DOIS_PONTOS");

    TipoBasico();

    eat("PONTO_VIRGULA");

    DeclaraParametros();
    BlocoVariaveis();
    BlocoComandos();

    eat("PONTO_VIRGULA");
}


// ======================================================
// Parametros
// ======================================================

void Parametros() {

    if(erro) return;

    if(token_atual().token == "ABRE_PARENTESES") {

        eat("ABRE_PARENTESES");

        DeclaraIdentificador();

        eat("FECHA_PARENTESES");
    }
}


// ======================================================
// DeclaraParametros
// ======================================================

void DeclaraParametros() {

    if(erro) return;

    if(
        token_atual().token == "TIPO" ||
        token_atual().token == "INTEIRO" ||
        token_atual().token == "REAL" ||
        token_atual().token == "CARACTERE" ||
        token_atual().token == "LOGICO" ||
        token_atual().token == "ID"
    ) {

        Declaracoes();
    }
}


// ======================================================
// BlocoVariaveis
// ======================================================

void BlocoVariaveis() {

    if(erro) return;

    if(token_atual().token == "VARIAVEIS") {

        eat("VARIAVEIS");

        Declaracoes();
    }
}


// ======================================================
// Declaracoes
// ======================================================

void Declaracoes() {

    if(erro) return;

    if(token_atual().token == "TIPO") {

        DeclaraTipo();
        Declaracoes();
    }

    else if(
        token_atual().token == "INTEIRO" ||
        token_atual().token == "REAL" ||
        token_atual().token == "CARACTERE" ||
        token_atual().token == "LOGICO" ||
        token_atual().token == "ID"
    ) {

        DeclaraVariaveis();
        Declaracoes();
    }
}


// ======================================================
// DeclaraTipo
// ======================================================

void DeclaraTipo() {

    if(erro) return;

    eat("TIPO");

    eat("ID");

    eat("IGUAL");

    VetorMatriz();

    eat("ABRE_COLCHETES");

    Dimensao();

    eat("FECHA_COLCHETES");

    TipoBasico();

    eat("PONTO_VIRGULA");
}


// ======================================================
// DeclaraVariaveis
// ======================================================

void DeclaraVariaveis() {

    if(erro) return;

    TipoBasico();

    eat("DOIS_PONTOS");

    DeclaraIdentificador();

    eat("PONTO_VIRGULA");
}


// ======================================================
// DeclaraIdentificador
// ======================================================

void DeclaraIdentificador() {

    if(erro) return;

    eat("ID");

    X1();
}


// ======================================================
// X1
// ======================================================

void X1() {

    if(erro) return;

    if(token_atual().token == "VIRGULA") {

        eat("VIRGULA");

        DeclaraIdentificador();
    }
}


// ======================================================
// VetorMatriz
// ======================================================

void VetorMatriz() {

    if(erro) return;

    if(token_atual().token == "VETOR")
        eat("VETOR");

    else if(token_atual().token == "MATRIZ")
        eat("MATRIZ");

    else {
        printar("ERRO DE SINTAXE. Linha: " + to_string(token_atual().linha-1) + " Coluna: " + to_string(token_atual().coluna) + " -> '" + token_atual().lexema + "'");  
        exit(1);
    }
}


// ======================================================
// Dimensao
// ======================================================

void Dimensao() {

    if(erro) return;

    eat("NUM");

    eat("DOIS_PONTOS");

    eat("NUM");

    X2();
}


// ======================================================
// X2
// ======================================================

void X2() {

    if(erro) return;

    if(token_atual().token == "VIRGULA") {

        eat("VIRGULA");

        Dimensao();
    }
}


// ======================================================
// TipoBasico
// ======================================================

void TipoBasico() {

    if(erro) return;

    if(token_atual().token == "INTEIRO")
        eat("INTEIRO");

    else if(token_atual().token == "REAL")
        eat("REAL");

    else if(token_atual().token == "CARACTERE")
        eat("CARACTERE");

    else if(token_atual().token == "LOGICO")
        eat("LOGICO");

    else if(token_atual().token == "ID")
        eat("ID");

    else {
        printar("ERRO DE SINTAXE. Linha: " + to_string(token_atual().linha-1) + " Coluna: " + to_string(token_atual().coluna) + " -> '" + token_atual().lexema + "'");  
        exit(1);
    }
}


// ======================================================
// BlocoComandos
// ======================================================

void BlocoComandos() {

    if(erro) return;

    eat("INICIO");

    ListaComandos();

    eat("FIM");
}


// ======================================================
// ListaComandos
// ======================================================

void ListaComandos() {

    if(erro) return;

    Comandos();

    eat("PONTO_VIRGULA");

    if(
        token_atual().token == "ID" ||
        token_atual().token == "SE" ||
        token_atual().token == "ENQUANTO" ||
        token_atual().token == "PARA" ||
        token_atual().token == "REPITA" ||
        token_atual().token == "LEIA" ||
        token_atual().token == "IMPRIMA"
    ) {

        ListaComandos();
    }
}


// ======================================================
// Comandos
// ======================================================

void Comandos() {

    if(erro) return;

    if(token_atual().token == "ID") {

        eat("ID");

        X7();
    }

    else if(token_atual().token == "SE") {

        eat("SE");

        Expressao();

        eat("ENTAO");

        ListaComandos();

        X4();
    }

    else if(token_atual().token == "ENQUANTO") {

        eat("ENQUANTO");

        Expressao();

        eat("FACA");

        ListaComandos();

        eat("FIM");

        eat("ENQUANTO");
    }

    else if(token_atual().token == "PARA") {

        eat("PARA");

        eat("ID");

        eat("DE");

        Expressao();

        eat("ATE");

        Expressao();

        X5();
    }

    else if(token_atual().token == "REPITA") {

        eat("REPITA");

        ListaComandos();

        eat("ATE");

        Expressao();
    }

    else if(token_atual().token == "LEIA") {

        eat("LEIA");

        eat("ABRE_PARENTESES");

        Variavel();

        eat("FECHA_PARENTESES");
    }

    else if(token_atual().token == "IMPRIMA") {

        eat("IMPRIMA");

        eat("ABRE_PARENTESES");

        ExprIter();

        eat("FECHA_PARENTESES");
    }

    else {
        printar("ERRO DE SINTAXE. Linha: " + to_string(token_atual().linha-1) + " Coluna: " + to_string(token_atual().coluna) + " -> '" + token_atual().lexema + "'");  
        exit(1);
    }
}


void X7() {
     if(erro) return;

    // Variavel <- Expressao
    if(
        token_atual().token == "ABRE_COLCHETES" ||
        token_atual().token == "ATRIBUICAO"
    ) {

        X6();

        eat("ATRIBUICAO");

        Expressao();
    }

    // chamada de procedimento
    else {

        X3();
    }
}

// ======================================================
// X3
// ======================================================

void X3() {

    if(erro) return;

    if(token_atual().token == "ABRE_PARENTESES") {

        eat("ABRE_PARENTESES");

        ExprIter();

        eat("FECHA_PARENTESES");
    }
}


// ======================================================
// X4
// ======================================================

void X4() {

    if(erro) return;

    if(token_atual().token == "FIM") {

        eat("FIM");

        eat("SE");
    }

    else if(token_atual().token == "SENAO") {

        eat("SENAO");

        ListaComandos();

        eat("FIM");

        eat("SE");
    }
}


// ======================================================
// X5
// ======================================================

void X5() {

    if(erro) return;

    if(token_atual().token == "FACA") {

        eat("FACA");

        ListaComandos();

        eat("FIM");

        eat("PARA");
    }

    else if(token_atual().token == "PASSO") {

        eat("PASSO");

        Expressao();

        eat("FACA");

        ListaComandos();

        eat("FIM");

        eat("PARA");
    }
}


// ======================================================
// Expressao
// ======================================================

void Expressao() {

    if(erro) return;

    ExpressaoSimples();

    ExpressaoLinha();
}


// ======================================================
// Expressao'
// ======================================================

void ExpressaoLinha() {

    if(erro) return;

    if(token_atual().token == "IGUAL") {

        eat("IGUAL");

        ExpressaoSimples();

        ExpressaoLinha();
    }

    else if(token_atual().token == "DIFERENTE") {

        eat("DIFERENTE");

        ExpressaoSimples();

        ExpressaoLinha();
    }

    else if(token_atual().token == "MENOR") {

        eat("MENOR");

        ExpressaoSimples();

        ExpressaoLinha();
    }

    else if(token_atual().token == "MENOR_IGUAL") {

        eat("MENOR_IGUAL");

        ExpressaoSimples();

        ExpressaoLinha();
    }

    else if(token_atual().token == "MAIOR") {

        eat("MAIOR");

        ExpressaoSimples();

        ExpressaoLinha();
    }

    else if(token_atual().token == "MAIOR_IGUAL") {

        eat("MAIOR_IGUAL");

        ExpressaoSimples();

        ExpressaoLinha();
    }
}

// ======================================================
// ExpressaoSimples
// ======================================================

void ExpressaoSimples() {

    if(erro) return;

    if(
        token_atual().token == "ID" ||
        token_atual().token == "NUM" ||
        token_atual().token == "REAL" ||
        token_atual().token == "STRING" ||
        token_atual().token == "VERDADEIRO" ||
        token_atual().token == "FALSO" ||
        token_atual().token == "NAO" ||
        token_atual().token == "ABRE_PARENTESES"
    ) {

        Termo();

        ExpressaoSimplesLinha();
    }

    else if(token_atual().token == "MAIS") {

        eat("MAIS");

        Termo();

        ExpressaoSimplesLinha();
    }

    else if(token_atual().token == "MENOS") {

        eat("MENOS");

        Termo();

        ExpressaoSimplesLinha();
    }

    else {
        printar("ERRO DE SINTAXE. Linha: " + to_string(token_atual().linha-1) + " Coluna: " + to_string(token_atual().coluna) + " -> '" + token_atual().lexema + "'");  
        exit(1);
    }
}


// ======================================================
// ExpressaoSimples'
// ======================================================

void ExpressaoSimplesLinha() {

    if(erro) return;

    if(token_atual().token == "MAIS") {

        eat("MAIS");

        Termo();

        ExpressaoSimplesLinha();
    }

    else if(token_atual().token == "MENOS") {

        eat("MENOS");

        Termo();

        ExpressaoSimplesLinha();
    }

    else if(token_atual().token == "OU") {

        eat("OU");

        Termo();

        ExpressaoSimplesLinha();
    }

    // epsilon
    else
        return;
}


// ======================================================
// Termo
// ======================================================

void Termo() {

    if(erro) return;

    Fator();

    TermoLinha();
}


// ======================================================
// Termo'
// ======================================================

void TermoLinha() {

    if(erro) return;

    if(token_atual().token == "VEZES") {

        eat("VEZES");

        Fator();

        TermoLinha();
    }

    else if(token_atual().token == "DIVISAO") {

        eat("DIVISAO");

        Fator();

        TermoLinha();
    }

    else if(token_atual().token == "DIV") {

        eat("DIV");

        Fator();

        TermoLinha();
    }

    else if(token_atual().token == "E") {

        eat("E");

        Fator();

        TermoLinha();
    }

    // epsilon
    else
        return;
}


// ======================================================
// Fator
// ======================================================

void Fator() {

    if(erro) return;

    if(token_atual().token == "ABRE_PARENTESES") {

        eat("ABRE_PARENTESES");

        Expressao();

        eat("FECHA_PARENTESES");
    }

    else if(token_atual().token == "NAO") {

        eat("NAO");

        Fator();
    }

    else if(token_atual().token == "NUM")
        eat("NUM");

    else if(token_atual().token == "REAL")
        eat("REAL");

    else if(token_atual().token == "VERDADEIRO")
        eat("VERDADEIRO");

    else if(token_atual().token == "FALSO")
        eat("FALSO");

    else if(token_atual().token == "STRING")
        eat("STRING");

    else if(token_atual().token == "ID") {

        eat("ID");

        // identificador ( ExprIter )
        if(token_atual().token == "ABRE_PARENTESES") {

            eat("ABRE_PARENTESES");

            ExprIter();

            eat("FECHA_PARENTESES");
        }

        // Variavel -> identificador X6
        else {

            X6();
        }
    }

    else {
        printar("ERRO DE SINTAXE. Linha: " + to_string(token_atual().linha-1) + " Coluna: " + to_string(token_atual().coluna) + " -> '" + token_atual().lexema + "'");  
        exit(1);
    }
}


// ======================================================
// Variavel
// ======================================================

void Variavel() {

    if(erro) return;

    eat("ID");

    X6();
}


// ======================================================
// X6
// ======================================================

void X6() {

    if(erro) return;

    if(token_atual().token == "ABRE_COLCHETES") {

        eat("ABRE_COLCHETES");

        ExprIter();

        eat("FECHA_COLCHETES");
    }

    // epsilon
    else
        return;
}


// ======================================================
// ExprIter
// ======================================================

void ExprIter() {

    if(erro) return;

    Expressao();

    if(token_atual().token == "VIRGULA") {

        eat("VIRGULA");

        ExprIter();
    }
}


void analisador_sintatico() {

    pos_token = 0;
    erro = false;

    Programa();

    // if(!erro) {

    //     int size = TOKENS.size();
    //     if(pos_token == size) 
    //         printar("CADEIA ACEITA");
    // }
}

int main() {

    string linha, cadeia;
    while (getline(cin, linha)) { 
        cadeia += '\n';
        cadeia += linha;
    }

    TOKENS.clear();
    analisador_lexico(cadeia);
    analisador_sintatico();
    
    printar("PROGRAMA CORRETO.");
    // for(auto t : TOKENS) {
    //     cout << t.token << ": " << t.lexema << endl;
    // }
    return 0;
}