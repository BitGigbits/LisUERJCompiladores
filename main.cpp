#include <iostream>
#include <vector>
#include <fstream>
#include <string>

//Alunos:
// Pedro Lucas Lessa Martins
// Diogo Luis de Freitas Corrêa

using namespace std;

//NOME TOKENS
#define AND  256
#define BREAK  257
#define DO  258
#define ELSE  259
#define ELSEIF  260
#define END  261
#define FALSE  262
#define FOR  263
#define FUNCTION  264
#define IF  265
#define IN  266
#define LOCAL  267
#define NIL  268
#define NOT  269
#define OR  270
#define REPEAT  271
#define RETURN  272
#define THEN  273
#define UNTIL  274
#define TRUE  275
#define WHILE  276


#define ID  277
#define NUM  278
#define LIT 279
#define RELOP  280

//ATRIBUTOS DE RELOP ~= | <= | >= | < | > | ==

#define LT 281 // <
#define LE 282 // <=
#define EQ 283 // ==
#define NE 284 // ~=
#define GT 285 // >
#define GE 286 // >=

// OPERADORES

#define PLUS 287 // +
#define SUBT 288 // -
#define MULT 289 // *
#define DIV 290 // /
#define POW 291 // ^
#define ATR 292 // =

// DELIM  ( | ) | { | } | [ | ] | ; | : | , | . | ..

// OP = OPEN
// CL = CLOSE

#define OP_P 293
#define CL_P 294
#define OP_K 295
#define CL_K 296
#define OP_C 297
#define CL_C 298
#define PTVIR 299
#define PTPT 300

// Definindo os delimiadores mas usaremos codigo da tabela ascii
 
struct Token{
 int nome_token;
 string atributo;
};


int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
bool is_res = false;
unsigned int i = 0;
string code;

vector<string> reservados = {"and", "break", "do", "else", "elseif", "end", "false", "for", "function",
"if", "in", "local", "nil", "not", "or", "repeat", "return", "then", "until", "true", "while"};
int count_tabela = 0;
vector<string> tabela;
string identifier = "";
string numeral = "";
string literal = "";

void readFile(){
	string nomeArquivo = "programa.txt";
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()){
        cout << "Nao foi possivel abrir o arquivo: " << nomeArquivo << "\n";
    }

    string linha;

    while (getline(arquivo, linha)) {
        code += linha + '\n';
    }

    arquivo.close();
}

int falhar(){
	switch(estado){

		case 0: partida = 12; break;

		case 12: partida = 15; break;
		
		case 15: partida = 23; break;

		case 23: partida = 27; break;

		case 27:
			printf("Erro no código.\n");
			partida = -1;
			break;

		default: printf("Erro do compilador");
	}

	return(partida);
}

Token proximo_token(){
	Token token;
	char c;
	while(code[cont_sim_lido] != '\0'){
		switch(estado){
			case 0:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n')){
					estado = 0;
					cont_sim_lido++;
				}
				else if(c == '<') estado = 1;
				else if(c == '=') estado = 4;
				else if(c == '~') estado = 7;
				else if(c == '>') estado = 9;
				else if(c == '+') estado = 18;
				else if(c == '-') estado = 19;
				else if(c == '*') estado = 20;
				else if(c == '/') estado = 21;
				else if(c == '^') estado = 22;
				else if(c == '(') estado = 31;
				else if(c == ')') estado = 32;
				else if(c == '{') estado = 33;
				else if(c == '}') estado = 34;
				else if(c == '[') estado = 35;
				else if(c == ']') estado = 36;
				else if(c == ';') estado = 37;
				else if(c == ',') estado = 38;
				else if(c == '.'){
					cont_sim_lido++;
					if(c == '.'){
						estado = 39;
					}else{
						cont_sim_lido--;
						estado = 40;
					}
				}else if(c == ':') estado = 41;
				else{
					 estado = falhar();
				}
				break;
			case 1:
				cont_sim_lido++;
				c = code[cont_sim_lido];

				if((c == ' ')||(c == '\n')){
						cont_sim_lido++;
						c = code[cont_sim_lido];
					}

				if(c == '=') estado = 2;
				else estado = 3;
				break;
			case 2:
				cont_sim_lido++;
				printf("<relop, LE>\n");
				token.nome_token = RELOP;
				token.atributo = to_string(LE);
				estado = 0;
				return(token);
				break;
			case 3:
				printf("<relop, LT>\n");
				token.nome_token = RELOP;
				token.atributo = to_string(LT);
				estado = 0;
				return(token);
				break;
            case 4:
				cont_sim_lido++;
				c = code[cont_sim_lido];

				if((c == ' ')||(c == '\n')){
					cont_sim_lido++;
					c = code[cont_sim_lido];
				}

				if(c == '=') estado = 6;
				else estado = 5;
				break;    
			case 5: 
				printf("<=, >\n");
				token.nome_token = ATR;
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			case 6:
				cont_sim_lido++;
				printf("<relop, EQ>\n");
				token.nome_token = RELOP;
				token.atributo = to_string(EQ);
				estado = 0;
				return(token);
				break;
            case 7:
				cont_sim_lido++;
				c = code[cont_sim_lido];

				if((c == ' ')||(c == '\n')){
					cont_sim_lido++;
					c = code[cont_sim_lido];
				}
				
				if(c == '='){
					estado = 8;
				}else{
					printf("Erro, nao ha caractere valido apos o ~\n");
				}
				break;    
			case 8:
				cont_sim_lido++;
				printf("<relop, NE>\n");
				token.nome_token = RELOP;
				token.atributo = to_string(NE);
				estado = 0;
				return(token);
				break;
            case 9:
				cont_sim_lido++;
				c = code[cont_sim_lido];

				if((c == ' ')||(c == '\n')){
					cont_sim_lido++;
					c = code[cont_sim_lido];
				}

				if(c == '=') estado = 11;
				else estado = 10;
				break;   
			case 10:
				printf("<relop, GT>\n");
				token.nome_token = RELOP;
				token.atributo = to_string(GT);
				estado = 0;
				return(token);
				break;
			case 11:
				cont_sim_lido++;
				printf("<relop, GE>\n");
				token.nome_token = RELOP;
				token.atributo = to_string(GE);
				estado = 0;
				return(token);
				break;
			case 12:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n')){
					estado = 0;
					cont_sim_lido++;
				}
				else if(isalpha(c) || c == '_'){
					identifier += c;
					estado = 13;
				}
				else{
					 estado = falhar();
				}
				break;
			case 13:
				cont_sim_lido++;
				c = code[cont_sim_lido];

                if(isalpha(c) || isdigit(c) || c == '_' ){
					identifier += c;
					estado = 13;
				}else{
					estado = 14;
				}

				break;
            case 14:
				is_res = false;
				i = 0;
				for(; i < reservados.size(); i++){
					// Neste for percorreremos as palavras reservadas vendo se é uma ou não
					if(identifier == reservados[i]){
						is_res = true;
						break;
					}
				}
				if(is_res){
					printf("<%s, >\n", identifier.c_str());	// Exibe o token da palavra reservada
					token.nome_token = i + 256;				// Como está ordenado de acordo com as definições de cada palavra reservada, estou somando
					token.atributo = ' ';					// o índice que parou no for com 256 para obter o código da palavra reservada correta
				}else{
					tabela.push_back(identifier);			// Coloca o identificador na tabela
					printf("<ID, %d>\n", count_tabela);		// Exibe o token na tela
					token.nome_token = ID;				
					token.atributo = to_string(count_tabela);
					count_tabela++;							// Incrementa a variável que controla quantidade de identificadores
				}
				identifier = "";							// Retorna a variável que guarda identificador para valor vazio
				estado = 0;
				return(token);
				break;
			case 15:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n')){
					estado = 0;
					cont_sim_lido++;
				}
				if(isdigit(c)){
					numeral += c;
					estado = 16;
				}else{
					estado = falhar();
				}
				break;
			case 16:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(isdigit(c)){
					numeral += c;
				}else{
					estado = 17;
				}
				break;
			case 17:
				printf("<numero, %s>\n", numeral.c_str());
				token.nome_token = NUM;
				token.atributo = numeral;
				numeral = "";
				estado = 0;
				return(token);
				break;
			case 18:
				cont_sim_lido++;
				printf("<+, >\n");
				token.nome_token = PLUS;
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			case 19:
				cont_sim_lido++;
				if(c == '-'){
					estado = 0;
					estado = falhar();
					cont_sim_lido--;
				}else{
					printf("<-, >\n");
					token.nome_token = SUBT;
					token.atributo = ' ';
					estado = 0;
					return(token);
				}
				break;
			case 20:
				cont_sim_lido++;
				printf("<*, >\n");
				token.nome_token = MULT;
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			case 21:
				cont_sim_lido++;
				printf("</, >\n");
				token.nome_token = DIV;
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			case 22:
				cont_sim_lido++;
				printf("<^, >\n");
				token.nome_token = POW;
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
				
			case 23:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n')){
					estado = 0;
					cont_sim_lido++;
				}
				if(c == '\"'){
					estado = 24;
				}else{
					estado = falhar();
				}
				break;	

			case 24:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				
				if(c == '\\'){
					estado = 25;
				}else if(c == '\"'){
					cont_sim_lido++;
					estado = 26;
				}
				literal += c;
				break;

			case 25:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				literal += c;
				estado = 24;
				break;
			case 26:
				printf("<literal, \"%s>\n", literal.c_str());
				token.nome_token = LIT;
				token.atributo = literal;
				literal = "";
				estado = 0;
				return(token);
				break;
			case 27:
				c = code[cont_sim_lido];
				if(c == '-'){
					cont_sim_lido++;
					c = code[cont_sim_lido];
					if(c == '-'){
						cont_sim_lido++;
						estado = 28;
					}else{
						printf("Erro no comentario\n");
					}
				}else{
					estado = falhar();
				}
				break;
			case 28:
				c = code[cont_sim_lido];
				if(c = '['){
					cont_sim_lido++;
					c = code[cont_sim_lido];
					if(c == '['){
						estado = 30;
					}
				}else{
					cont_sim_lido++;
				}
				if(c == '-'){
					cont_sim_lido++;
					c = code[cont_sim_lido];
					if(c == '-'){
						cont_sim_lido++;
						estado = 29;
					}else{
						printf("Erro no comentario\n");
					}
				}else{

				}
				break;
			case 29:
				break;
			case 30:
				break;
			case 31:
				cont_sim_lido++;
				printf("<+, >\n");
				token.nome_token = PLUS;
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			default:
				printf("Erro no compilador");
				break;
		}
	}
	token.nome_token = EOF;
	token.atributo = -1;
	return(token);
}

int main (){
	Token token;
	readFile();
    while(code[cont_sim_lido] != '\0'){
		token = proximo_token();
	}

	return 0;
}