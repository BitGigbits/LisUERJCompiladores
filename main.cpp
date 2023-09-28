/*
 * ESTE CÓDIGO IMPLEMENTA UM ANALIZADOR LÉXICO PARA O EXEMPLO DE FRAGMENTO DE LINGUAGEM APRESENTADO EM SALA DE AULA (VEJA OS SLIDES DA AULA 4: ANÁLISE LÉXICA: PARTE 2)
 * E PODERÁ SER UTILIZADO COMO PONTO DE PARTIDA PARA IMPLEMENTAÇÃO DO ANALISADOR LÉXICO PARA LINGUAGEM ADOTADA NO TRABALHO PROPOSTO.
 * */

#include <iostream>
#include <vector>

using namespace std;

//NOME TOKENS
#define AND  256;
#define BREAK  257;
#define DO  258;
#define ELSE  259;
#define ELSEIF  260;
#define END  261;
#define FALSE  262;
#define FOR  263;
#define FUNCTION  264;
#define IF  265;
#define IN  266;
#define LOCAL  267;
#define NIL  268;
#define NOT  269;
#define OR  270;
#define REPEAT  271;
#define RETURN  272;
#define THEN  273;
#define UNTIL  274;
#define TRUE  275;
#define WHILE  276;


#define ID  277;
#define NUM  278;

#define RELOP  279;

//ATRIBUTOS DE RELOP ~= | <= | >= | < | > | ==

#define LT 280; // <
#define LE 281; // <=
#define EQ 282; // ==
#define NE 283; // ~=
#define GT 284; // >
#define GE 285; // >=

// DELIM  ( | ) | { | } | [ | ] | ; | : | , | . | ..
 
struct Token{
 int nome_token;
 int atributo;
};


int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
char *code;

vector<string> reservados = {"and", "break", "do", "else", "elseif", "end", "false", "for", "function"
"if", "in", "local", "nil", "not", "or", "repeat", "return", "then", "until", "true", "while"};
int count_tabela = 0;
vector<string> tabela;
string identifier;


char *readFile(char *fileName){
	FILE *file = fopen(fileName, "r");
	char *code;
	int n = 0;
	int c;

	if(file == NULL) return NULL;

	fseek(file, 0, SEEK_END);
	long f_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	code = new char (f_size);

	while ((c = fgetc(file))!= EOF)	{
		code[n++] = (char) c;
	}

	code[n] = '\0';
	return code;
}

int falhar(){
	switch(estado){

		case 0: partida = 12; break;

		case 9: partida = 12; break;

		case 12: partida = 20; break;

		case 20: partida = 25; break;

		case 25:
			//retornar msg de erro
			printf("Erro encontrado no código\n");
			cont_sim_lido++;
			break;

		default: printf("Erro do compilador");
	}

	return(partida);
}

Token proximo_token(){
	Token token;
	char c;
	while(code[cont_sim_lido] != EOF)
	{
		switch(estado)
		{
			case 0:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n'))
				{
					estado = 0;
					cont_sim_lido++;
				}
				else if(c == '<') estado = 1;
				else if(c == '=') estado = 4;
				else if(c == '~') estado = 7;
				else if(c == '>') estado = 9;
				else
					{
					 estado = falhar();
					}
				break;

			case 1:
				cont_sim_lido++;
				c = code[cont_sim_lido];

				if((c == ' ')||(c == '\n'))
					{
						cont_sim_lido++;
						c = code[cont_sim_lido];
					}

				if(c == '=') estado = 2;
				else if(c == '>') estado = 8;
				else estado = 4;
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
				// cont_sim_lido++;
				printf("<relop, LT>\n");
				token.nome_token = RELOP;
				token.atributo = LT;
				estado = 0;
				return(token);
				break;

            case 4:
				cont_sim_lido++;
				c = code[cont_sim_lido];

				if((c == ' ')||(c == '\n'))
					{
						cont_sim_lido++;
						c = code[cont_sim_lido];
					}

				if(c == '=') estado = 6;
				else estado = 5;
				break;    

			case 5: 
				// cont_sim_lido++;
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

				if((c == ' ')||(c == '\n'))
					{
						cont_sim_lido++;
						c = code[cont_sim_lido];
					}

				if(c == '=') estado = 8;
				else falhar();

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

				if((c == ' ')||(c == '\n'))
					{
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
				bool is_res = false;
				int i = 0;
				for(; i < reservados.size(); i++){
					// Neste for percorreremos as palavras reservadas vendo se é uma ou não
					if(identifier == reservados[i]){
						is_res = true;
						break;
					}
				}
				if(is_res){
					printf("<%s, >", identifier);					// Exibe o token da palavra reservada
					token.nome_token = i + 256;			// Como está ordenado de acordo com as definições de cada palavra reservada, estou somando
					token.atributo = ' ';				// o índice que parou no for com 256 para obter o código da palavra reservada correta
				}else{
					tabela.push_back(identifier);		// Coloca o identificador na tabela
					printf("<ID, %d>", count_tabela);	// Exibe o token na tela
					token.nome_token = ID;				
					token.atributo = count_tabela;
					count_tabela++;						// Incrementa a variável que controla quantidade de identificadores
				}
				identifier = "";						// Retorna a variável que guarda identificador para valor vazio
				estado = 0;
				return(token);
				break;                    
		}

	}
	token.nome_token = EOF;
	token.atributo = -1;
	return(token);
}

int main (){
	Token token;
    char t[] = "programa.txt";
    code = readFile(t);
    token = proximo_token();
    token = proximo_token();
    //...


}
