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

    TOKEN(int s, string l, string t) {
        lexema = l;
        state = s;
        token = t;
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
    else if(c >= 'a' && c <= 'z')   return 1;
    else if(c == '"')               return 2;
    else if(c == ' ')               return 3;
    else if(c == '/')               return 4;
    else if(c == '{')               return 5;
    else if(c == '}')               return 6;
    else if(c == '.')               return 7;
    else if(c == ';')               return 8;
    else if(c == ',')               return 9;
    else if(c == ':')               return 10;
    else if(c == '[')               return 11;
    else if(c == ']')               return 12;
    else if(c == '(')               return 13;
    else if(c == ')')               return 14;
    else if(c == '=')               return 15;
    else if(c == '<')               return 16;
    else if(c == '>')               return 17;
    else if(c == '+')               return 18;
    else if(c == '-')               return 19;
    else if(c == '*')               return 20;
    else if(c == '\n')              return 21;
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
    {"inicio", "ALGORITMO"},
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

int automato[31][22] = {
    
    // colunas:
    //  0-9, a-z, ", ", /, {, }, ., ;, ,, :, [, ], (, ), =, <, >, +, -, *, \n
    //   0    1   2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  21
/*0*/  { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1*/  { 3,   2,  6, 0, 8,11, 0,16,13,14,15,17,18,19,20,21,22,26,28,29,30, 0},
/*2*/  { 2,   2,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // ID
/*3*/  { 3,   0,  0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // NUM
/*4*/  { 5,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*5*/  { 5,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // REAL
/*6*/  { 6,   6,  0, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    //  0-9, a-z, ", ", /, {, }, ., ;, ,, :, [, ], (, ), =, <, >, +, -, *, \n
    //   0    1   2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  21
/*7*/  { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // STRING
/*8*/  { 0,   0,  0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // DIVISAO
/*9*/  { 9,   9,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,10},
/*10*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // COMMENT LINHA
/*11*/ {11,  11, 11,11,11, 0,12,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11},
/*12*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // COMMENT BLOCO
/*13*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // PONTO_VIRGULA
/*14*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // VIRGULA
/*15*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // DOIS_PONTOS
    //  0-9, a-z, ", ", /, {, }, ., ;, ,, :, [, ], (, ), =, <, >, +, -, *, \n 
    //   0    1   2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  21
/*16*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // PONTO
/*17*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // ABRE_COLCHETES
/*18*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // FECHA_COLCHETES
/*19*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // ABRE_PARENTESES
/*20*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // FECHA_PARENTESES
/*21*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // IGUAL
/*22*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,24, 0,23, 0,25, 0, 0}, // MENOR
/*23*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // DIFERENTE
    //  0-9, a-z, ", ", /, {, }, ., ;, ,, :, [, ], (, ), =, <, >, +, -, *, \n
    //   0    1   2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  21
/*24*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // MENOR_IGUAL
/*25*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // ATRIBUICAO
/*26*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,27, 0, 0, 0, 0, 0, 0}, // MAIOR
/*27*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // MAIOR IGUAL
/*28*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // MAIS
/*29*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // MENOS
/*30*/ { 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // VEZES

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

    int i = 0;

    while (i < tam) {

        if (s[i] == ' ' || s[i] == '\n' || s[i] == '\r') {
            i++;
            continue;
        }

        int state = 1;
        int last_final_state = -1;
        int last_final_pos = -1;

        string token = "";

        int j = i;

        while (j < tam) {

            int ind = dicionario_indice(s[j]);
            if(ind == -1) break;
            int prox_estado = automato[state][ind];

            if (prox_estado == 0)
                break;

            token += s[j];
            state = prox_estado;

            if (finais[state]) {
                last_final_state = state;
                last_final_pos = j;
            }

            j++;
        }

        if (last_final_state != -1) {

            string token = dicionario_token(last_final_state);
            string lexema = s.substr(i, last_final_pos - i + 1);

            if(token == "ID")
                isPalavraReservada(lexema, token);

            TOKENS.push_back(TOKEN(last_final_state, lexema, token));

            i = last_final_pos + 1; 
        }
        
        else {
            TOKENS.push_back(TOKEN(-1, string(1, s[i]), dicionario_token(-1)));
            i++; 
        }
    }
}


int main() {

    string cadeia;
    while (getline(cin, cadeia)) { 
        TOKENS.clear();
        analisador_lexico(cadeia);

        // for(auto t : TOKENS) {
        //     cout << t.token << ": " << t.lexema << endl;
        // }
    }

    return 0;
}