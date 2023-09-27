/*
 * ESTE CÓDIGO IMPLEMENTA UM ANALIZADOR LÉXICO PARA O EXEMPLO DE FRAGMENTO DE LINGUAGEM APRESENTADO EM SALA DE AULA (VEJA OS SLIDES DA AULA 4: ANÁLISE LÉXICA: PARTE 2)
 * E PODERÁ SER UTILIZADO COMO PONTO DE PARTIDA PARA IMPLEMENTAÇÃO DO ANALISADOR LÉXICO PARA LINGUAGEM ADOTADA NO TRABALHO PROPOSTO.
 * */

#include <stdio.h>


//NOME TOKENS
#define IF  256;
#define THEN  257;
#define ELSE  258;

#define ELSEIF  259;
#define FOR  260;
#define WHILE  261;
#define AND  262;
#define BREAK  263;
#define DO  264;
#define END  265;
#define FUNCTION  266;
#define IN  267;
#define LOCAL  268;
#define NILL  269;
#define NOT  270;
#define OR  271;
#define REPEAT  272;
#define RETURN  273;
#define TRUE  274;
#define FALSE  275;
#define UNTIL  276;


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


char *readFile(char *fileName)
{
	FILE *file = fopen(fileName, "r");
	char *code;
	int n = 0;
	int c;

	if(file == NULL) return NULL;

	fseek(file, 0, SEEK_END);
	long f_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	code = new char (f_size);

	while ((c = fgetc(file))!= EOF)
	{
		code[n++] = (char) c;
	}
	code[n] = '\0';
	return code;
}




int falhar()
{
	switch(estado)
	{
	case 0: partida = 12; break;

	// case 9: partida = 12; break;

	// case 12: partida = 20; break;

	// case 20: partida = 25; break;

	// case 25:
	// 	//retornar msg de erro
	// 	printf("Erro encontrado no código\n");
	// 	cont_sim_lido++;
	// 	break;

	// default: printf("Erro do compilador");
	// }
	// return(partida);
}

Token proximo_token()
{
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
				token.atributo = '';
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
				//else estado = ?;
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
				// cont_sim_lido++;
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
				if((c == ' ')||(c == '\n'))
				{
					estado = 0;
					cont_sim_lido++;
				}
				else if(isalpha(c) || c == '_') estado = 13;
				

				else
					{
					 estado = falhar();
					}
				break;

			case 13:
				cont_sim_lido++;
				c = code[cont_sim_lido];

                if(isalpha(c) || isdigit(c) || c == '_' ) estado = 13;
                else estado = 14;

            case 14:
				printf("<ID, GT>\n");
				token.nome_token = ID;
				token.atributo = GT;
				estado = 0;
				return(token);
				break;                    



		}

	}
	token.nome_token = EOF;
	token.atributo = -1;
	return(token);
}




int main ()
{
	Token token;
    char t[] = "programa.txt";
    code = readFile(t);
    token = proximo_token();
    token = proximo_token();
    //...


}