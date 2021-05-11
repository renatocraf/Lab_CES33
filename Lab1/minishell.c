#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true  1
#define false 0

// #define MAX_LENGHT_PROG 1024
// #define MAX_QUANT_LINES 64


#define PIPE 124

#include "parser.h"
#include "parser.c"

#include "exec.h"
#include "exec.c"


void start_cmd()
{

	char *line = NULL;
	while (true)
	{
		
		printf("Cmd> ");

		//variaveis do bash
		size_t len = 0;
		ssize_t nread;
				
		//pegando a linha
		nread = getline(&line, &len, stdin);
		//NREAD == -1 -> ERRO!! TRATAR

		//tirando o \n do final
		line[nread-1]='\0';
		
		//realizando comandos

		//verificando se tem pipe e separando eles:
		char **tokens;
		int qnt_tokens;
		tokens = separar_por_char(&qnt_tokens,line,PIPE);
				
		comando *p,*inicio;

		inicio = NULL;
		p = NULL;

		int i = 0;
		while(tokens[i] != NULL)
		{
			//separando o token em 3: comando com parametros, in e out
			char **process = separar_prog_in_out(tokens[i]);
			char *prog = process[0];
			char **v;
			int qtd_parametros;
			// separando o comando com parametros e um vetor "v", onde v[0] é o comando e o restante são os argumentos
			v = separar_por_espaco(&qtd_parametros,prog);

			if(inicio == NULL)
			{
				p = (comando*)malloc(sizeof(comando));
				inicio = p;
			}			
			p->v = v;
			p->in = process[1];
			p->out = process[2];

			if(tokens[i+1] != NULL)			
				p->filho = (comando*)malloc(sizeof(comando));		
			else
				p->filho = NULL;			
			p = p->filho;
			i++;
		}

		launch_job(inicio);
		line = NULL;
	}
}

int main()
{	
	printf("CES-33 Minishell Project\n");
	printf("Aluno: Carlos Renato de Andrade Figueiredo - T22\n\n");	
	start_cmd();	
	return 0;
}
