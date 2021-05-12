/********************************************************
*	Instituto Tecnológico de Aeronáutica				*
*	CES-33 Sistemas Operacionais						*
*	Lab - Criação de um Minishell - Milestone 2			*
*	Prof: Lourenço A Pereira							*
*	Aluno: Carlos Renato de Andrade Figueiredo - T22	*
*********************************************************/


#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true  1
#define false 0
#define PIPE 124 //corresponde ao valor ASCII do character pipe

#include "parser.h"
#include "parser.c"
#include "exec.h"
#include "exec.c"

/*
	Essa funcao gerencia diretamente o cmd
	- Inicialmente criamos um loop que para apenas quando o usuário interromper
	- Dentro desse loop temos a palavra 'Cmd>' escrita e após isso o programa 
	aguarda o recebimento de uma linha.
	- Após receber a linha escrita pelo usuário será feito o parser:
		- Primeiro a string é separada em tokens como o critério de 
		separação sendo o PIPE (isso significa que está sendo pego um comando
		(com todos os seus parametros, entradas e saidas) por token)
		- Após isso, cada token é observado, sendo separado o comando e seus 
		parametros num vetor, a entrada em um ponteiro e a saída também em um 
		ponteiro. Toda essa estrutura é guardada numa struct. 
		- Com isso é formada uma lista encadeada, onde é considerado que o próximo 
		token contém um comando "filho" do token anterior.
	- Com a lista encadeada formada podemos pedir a execução do comando contido 
	no ponteiro 'inicio', e a parti daí os demais comando serão executados em 
	sequência, obedecendo seus pipes corretamente.
	(Obs: essa função chama funções que correspondem tanto ao 'parser' quanto ao 
	'exec', portanto preferi deixar ela no meu arquivo principal)
*/
void start_cmd();


int main()
{	
	printf("CES-33 Minishell Project\n");
	printf("Aluno: Carlos Renato de Andrade Figueiredo - T22\n\n");	
	start_cmd();	
	return 0;
}

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
			// separando o comando com parametros e um vetor "v", 
			//onde v[0] é o comando e o restante são os argumentos
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