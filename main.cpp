#include <iostream>
#include <vector>
#include <fstream>
#include <string>

// Alunos:
// 	Pedro Lucas Lessa Martins
// 	Diogo Luis de Freitas Corrêa

using namespace std;

// NOME TOKENS

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

// ALGUNS OPERADORES

#define PLUS 287 // +
#define SUBT 288 // -
#define MULT 289 // *
#define DIV 290 // /
#define POW 291 // ^
#define ATR 292 // =

// Definindo os delimiadores mas usaremos codigo da tabela ascii

#define DOUBLEDOT 293 // ..
 
struct Token{
 int nome_token;
 string atributo;
};

// Variáveis para controle dos estados
int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
int cont_colchete = 0;
int cont_integer = 0;
int cont_linhas = 0;
bool is_res = false;
bool var_exist = false;
unsigned int i = 0;
string code;
Token tok;

// Vector que guarda as palavras reservadas, tabela de símbolos e tokens
vector<string> reservados = {"and", "break", "do", "else", "elseif", "end", "false", "for", "function",
"if", "in", "local", "nil", "not", "or", "repeat", "return", "then", "until", "true", "while"};
int count_tabela = 0;
vector<string> tabela;
vector<Token> tokens;

// Variáveis auxiliares de leitura
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

// Função para evitar repetição e linhas extras quando se cria um token de alguns operadores
void get_nome_token(char c, Token &token, int &cont){
	if(estado == 42){
		token.nome_token = DOUBLEDOT;	// Pega o  código ASCII e guarda
	}else{
		token.nome_token = (int)c;	// Pega o  código ASCII e guarda
	}
	token.atributo = ' ';
	estado = 0;
	cont++;
	tokens.push_back(token);
}

int falhar(){
	// Função falhar para trocar de máquina
	switch(estado){

		case 0: partida = 12; break;

		case 12: partida = 15; break;

		case 15: partida = 18; break;

		case 18: partida = 24; break;

		case 24: partida = 31; break;

		case 31:
			printf("Erro no codigo introduzido, provavelmente de simbolo nao existente.\n");
			exit(3);
			break;

		default: 
			printf("Erro do compilador");
			exit(1);
			break;
	}

	return(partida);
}

Token gera_token(){
	Token token;
	char c;
	while(code[cont_sim_lido] != '\0'){
		// cout << "Estado: " << estado << endl;
		// cout << "Caractere: " << code[cont_sim_lido] << endl;
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
				printf("<relop, LE> ");
				token.nome_token = RELOP;
				token.atributo = to_string(LE);
				estado = 0;
				tokens.push_back(token);
				return(token);
				break;
			case 3:
				printf("<relop, LT> ");
				token.nome_token = RELOP;
				token.atributo = to_string(LT);
				estado = 0;
				tokens.push_back(token);
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
				printf("<=, > ");
				token.nome_token = ATR;
				token.atributo = ' ';
				estado = 0;
				tokens.push_back(token);
				return(token);
				break;
			case 6:
				cont_sim_lido++;
				printf("<relop, EQ> ");
				token.nome_token = RELOP;
				token.atributo = to_string(EQ);
				estado = 0;
				tokens.push_back(token);
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
					exit(4);
				}
				break;    
			case 8:
				cont_sim_lido++;
				printf("<relop, NE> ");
				token.nome_token = RELOP;
				token.atributo = to_string(NE);
				estado = 0;
				tokens.push_back(token);
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
				printf("<relop, GT> ");
				token.nome_token = RELOP;
				token.atributo = to_string(GT);
				estado = 0;
				tokens.push_back(token);
				return(token);
				break;
			case 11:
				cont_sim_lido++;
				printf("<relop, GE> ");
				token.nome_token = RELOP;
				token.atributo = to_string(GE);
				estado = 0;
				tokens.push_back(token);
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
					printf("<%s, > ", identifier.c_str());	// Exibe o token da palavra reservada
					token.nome_token = i + 256;				// Como está ordenado de acordo com as definições de cada palavra reservada, estou somando
					token.atributo = ' ';					// o índice que parou no for com 256 para obter o código da palavra reservada correta
				}else{
					for(i = 0; i < tabela.size(); i++){
						if(identifier == tabela[i]){
							printf("<ID, %d> ", i);
							var_exist = true;
							token.nome_token = ID;
							token.atributo = to_string(i);
							break;
						}
					}
					if(!var_exist){
						tabela.push_back(identifier);
						printf("<ID, %d> ", count_tabela);	
						token.nome_token = ID;
						token.atributo = to_string(count_tabela);
						count_tabela++;
					}						
				}
				tokens.push_back(token);
				identifier = "";
				var_exist = false;
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
					cont_integer++;
					estado = 16;
				}else{
					estado = falhar();
				}
				break;
			case 16:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(isdigit(c) && cont_integer < 10){
					cont_integer++;
					numeral += c;
				}else{
					if(cont_integer == 10){
						printf("Erro, estouro do limite do numero.\n");
						exit(1);
					}
					estado = 17;
				}
				break;
			case 17:
				printf("<numero, %s> ", numeral.c_str());
				token.nome_token = NUM;
				token.atributo = numeral;
				numeral = "";
				cont_integer = 0;
				estado = 0;
				tokens.push_back(token);
				return(token);
				break;
			case 18:
				c = code[cont_sim_lido];
				if(c == '+') estado = 19;
				else if(c == '-') estado = 20;
				else if(c == '*') estado = 21;
				else if(c == '/') estado = 22;
				else if(c == '^') estado = 23;
				else estado = falhar();
				break;
			case 19:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 20:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(c == '-'){
					estado = 28;
					cont_sim_lido--;
				}else{
					get_nome_token(c, token, cont_sim_lido);
					return(token);
				}
				break;
			case 21:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 22:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 23:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 24:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n')){
					estado = 0;
					cont_sim_lido++;
				}
				if(c == '\"'){
					literal += c;
					estado = 25;
				}else{
					estado = falhar();
				}
				break;	
			case 25:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				
				if(c == '\\'){
					estado = 26;
				}else if(c == '\"'){
					cont_sim_lido++;
					estado = 27;
				}
				literal += c;
				break;
			case 26:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				literal += c;
				estado = 25;
				break;
			case 27:
				printf("<literal, %s> ", literal.c_str());
				token.nome_token = LIT;
				token.atributo = literal;
				literal = "";
				estado = 0;
				tokens.push_back(token);
				return(token);
				break;
			case 28:
				c = code[cont_sim_lido];
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
					estado = falhar();
				}
				break;
			case 29:
				c = code[cont_sim_lido];
				if(c == '['){
					cont_sim_lido++;
					c = code[cont_sim_lido];
					if(c == '['){
						cont_colchete++;
						estado = 30;
						break;
					}else{
						cont_sim_lido--;
					}
				}

				if(c == ']'){
					cont_sim_lido++;
					c = code[cont_sim_lido];
					if(c == ']'){
						cont_colchete--;
					}else{
						cont_sim_lido--;
					}
				}

				if((c == '\n' || c == ' ') && cont_colchete == 0){
					cont_sim_lido++;
					estado = 0;
				}else{
					cont_sim_lido++;
				}
				break;
			case 30:
				if(c == '['){
					cont_sim_lido++;
					c = code[cont_sim_lido];
					if(c == '['){
						cont_colchete++;
					}else{
						estado = 29;
					}
				}else{
					estado = 29;
					cont_sim_lido++;
				}
				break;
			case 31:
				if((c == ' ')||(c == '\n')){
					estado = 0;
					cont_sim_lido++;
				}
				else if(c == '(') estado = 32;
				else if(c == ')') estado = 33;
				else if(c == '{') estado = 34;
				else if(c == '}') estado = 35;
				else if(c == '[') estado = 36;
				else if(c == ']') estado = 37;
				else if(c == ';') estado = 38;
				else if(c == ':') estado = 39;
				else if(c == ',') estado = 40;
				else if(c == '.'){
					cont_sim_lido++;
					c = code[cont_sim_lido];
					if(c == '.'){
						estado = 42;
					}else{
						estado = 41;
						cont_sim_lido--;
					}
				}
				else{
					 estado = falhar();
				}
				break;
			case 32:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 33:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 34:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 35:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 36:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 37:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 38:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 39:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 40:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 41:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			case 42:
				get_nome_token(c, token, cont_sim_lido);
				return token;
				break;
			default:
				printf("Erro no compilador\n");
				break;
		}
	}
	token.nome_token = EOF;
	token.atributo = -1;
	return(token);
}

int main (){

	void erro(), block(), Stmt();
	void Exps(), Exp(), Vars();

	readFile();
	while(code[cont_sim_lido] != '\0'){
		block();
	}

	if(cont_colchete > 0){
		printf(" Erro! Comentario nao fechado.\n");
	}

	return 0;
}

void erro(int erro_id){
	switch (erro_id){
	case 1:
		cout << "Erro de início do bloco.";
		break;
	case 2:
		cout << "Erro no fechamento de statement, faltou ponto e virgula.";
		break;
	case 3:
		cout << "Erro no fechamento do bloco.";
		break;
	case 4:
		cout << "Erro na atribuicao.";
		break;
	default:
		break;
	}
	exit(erro_id);
}

void Var(){
	tok = gera_token();
	cout << tabela[stoi(tok.atributo)];
	return;
}

void Vars(){
	Var();
Verifica_Var:
	tok = gera_token();
	if(tok.nome_token == 44){
		cout << ", ";
		Var();
		goto Verifica_Var;
	}
}

void Exp(){
	return;
}

void Exps(){
	Exp();
	tok = gera_token();
	if(tok.nome_token == 40){
		cout << "( ";
	}
}

void Stmt(){
	void block();
	tok = gera_token();
	if(tok.nome_token == 277){ // 277 é o ID
		Vars();
		if(tok.nome_token == 292){ // 292 é = para atribuição
			cout << "= ";
			tok = gera_token();
			Exps();
		}else{
			erro(4);
		}
	}else if(tok.nome_token == 258){ // 258 é o código do do
		cout << "do ";
		tok = gera_token();
		block();
		if(tok.nome_token == 261){ // 261 é o código do end
			cout << "end ";
		}
	}else if(tok.nome_token == 276){ // 276 código do while
		cout << "while ";
		tok = gera_token();
		Exp();
		if(tok.nome_token == 258){ // 258 é o código do do
			cout << "do ";
			tok = gera_token();
			block();
			tok = gera_token();
			if(tok.nome_token == 261){ // 261 é o código do end
				cout << "end ";
			}
		}
	}
}

void block(){
begin:
	tok = gera_token();
	if(code[cont_sim_lido] == '\0'){
		exit(0);
	}
	if(tok.nome_token == 40){
		cout << "( "; // 40 é o ascii do (
		Stmt();
		tok = gera_token();
		if(tok.nome_token == 59){ // 59 é o ascii do ;
			cout << ";";
			tok = gera_token();
			if(tok.nome_token == 41){ // 41 é o ascii do )
				cout << " )";
				goto begin;
			}else{
				erro(3);
			}
		}else{
			erro(2);
		}
	}else{
		erro(1);
	}
}