/*
 * ESTE CÓDIGO IMPLEMENTA UM ANALIZADOR LÉXICO PARA O EXEMPLO DE FRAGMENTO DE LINGUAGEM APRESENTADO EM SALA DE AULA (VEJA OS SLIDES DA AULA 4: ANÁLISE LÉXICA: PARTE 2)
 * E PODERÁ SER UTILIZADO COMO PONTO DE PARTIDA PARA IMPLEMENTAÇÃO DO ANALISADOR LÉXICO PARA LINGUAGEM ADOTADA NO TRABALHO PROPOSTO.
 * */

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

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

#define RELOP  279

//ATRIBUTOS DE RELOP ~= | <= | >= | < | > | ==

#define LT 280 // <
#define LE 281 // <=
#define EQ 282 // ==
#define NE 283 // ~=
#define GT 284 // >
#define GE 285 // >=

// DELIM  ( | ) | { | } | [ | ] | ; | : | , | . | ..
 
struct Token{
 int nome_token;
 int atributo;
};


int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
int is_float = false;
int cont_tam = 0;
bool is_res = false;
unsigned int i = 0;
string code;

vector<string> reservados = {"and", "break", "do", "else", "elseif", "end", "false", "for", "function",
"if", "in", "local", "nil", "not", "or", "repeat", "return", "then", "until", "true", "while"};
int count_tabela = 0;
vector<string> tabela;
string identifier = "";
string numeral = "";

void readFile(){
	string nomeArquivo = "programa.txt";
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()){
        cout << "Não foi possível abrir o arquivo: " << nomeArquivo << "\n";
    }

    string linha;

    while (std::getline(arquivo, linha)) {
        code += linha + '\n';
    }

    arquivo.close();
}

int falhar(){
	switch(estado){

		case 0: partida = 12; break;

		case 12: partida = 15; break;

		default: printf("Erro do compilador");
	}

	return(partida);
}

Token proximo_token(){
	Token token;
	char c;
	long long int tam = code.length();
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
				token.atributo = LE;
				estado = 0;
				return(token);
				break;
			case 3:
				printf("<relop, LT>\n");
				token.nome_token = RELOP;
				token.atributo = LT;
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
				token.nome_token = '=';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			case 6:
				cont_sim_lido++;
				printf("<relop, EQ>\n");
				token.nome_token = RELOP;
				token.atributo = EQ;
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
				token.atributo = NE;
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
				cont_sim_lido++;
				printf("<relop, GT>\n");
				token.nome_token = RELOP;
				token.atributo = GT;
				estado = 0;
				return(token);
				break;
			case 11:
				cont_sim_lido++;
				printf("<relop, GE>\n");
				token.nome_token = RELOP;
				token.atributo = GE;
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
					token.atributo = count_tabela;
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
					falhar();
				}
				break;
			case 16:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(isdigit(c)){
					numeral += c;
				}else if(c == '.'){	
					estado = 17;
				}else{
					estado = 19;
				}
				break;
			case 17:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				is_float = true;
				if(isdigit(c)){
					numeral += c;
					estado = 18;
				}else{
					printf("Erro no numero decimal\n");
				}
				break;
			case 18:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(isdigit(c)){	
					numeral += c;
				}else{
					estado = 19;
				}
				break;
			case 19:
				printf("<numero, %s>\n", numeral.c_str());
				token.nome_token = NUM;
				if(is_float){
					token.atributo = stof(numeral);
				}
				token.atributo = stoi(numeral);
				numeral = "";
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