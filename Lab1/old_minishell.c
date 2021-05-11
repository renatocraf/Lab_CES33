#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

// #define MAX_LENGHT_PROG 1024
// #define MAX_QUANT_LINES 64
#define MAX_QUANT_TOKENS 10

#define PIPE 124
#define MAIOR_QUE 62
#define MENOR_QUE 60
#define ESPACO 32
#define ASPAS_DUPLAS 34
#define ASPAS_SIMPLES 39

char **separar_por_char(int *valor,char *line,char c)
{
	
	char **tokens;
	int quant_tokens = MAX_QUANT_TOKENS;
	tokens = malloc(quant_tokens * sizeof(char*));

	char *str, *token;
	char *saveptr;

	int i;
	for(i = 1, str = line;;i++,str = NULL)
	{
		token = strtok_r(str, &c, &saveptr);		
		if(i > quant_tokens)
		{
			quant_tokens += MAX_QUANT_TOKENS;
			tokens = realloc(tokens,quant_tokens);
		}
		if (token == NULL)
			break;
		tokens[i-1] = token;	
	}
	*valor = i-1;
	return tokens;
}

char **separar_por_espaco(int *valor,char *line)
{

	//a diferença daqui eh que temos q considerar  um programa com aspas
	char **tokens;
	int quant_tokens = MAX_QUANT_TOKENS;
	tokens = malloc(quant_tokens * sizeof(char*));

	//separando primeiro as aspas
	int i = 0; //iteraçao para tokens
	int j = 0; //iteração para token
	int cont = 0; //iteração para line

	int len =0;
	while(line[len]!= '\0'){ len++;} //mais 1 pra o /0

	char c = line[cont];
	char *token;
	token = malloc(len*sizeof(char));

	int aspas_sim,aspas_dup;

	//checando se inicia com aspas
	if(c == ASPAS_DUPLAS)
	{
		aspas_dup = true;
	}
	else
	{
		aspas_dup = false;
	}
	if(c == ASPAS_SIMPLES)
	{
		aspas_sim = true;
	}
	else
	{
		aspas_sim = false;
	}
	

	while(cont < len)
	{
		if ((c == ASPAS_DUPLAS && aspas_dup == false )||(c == ASPAS_SIMPLES && aspas_sim == false))
		//acaba palavra sem aspas, entao verifica espacos e cria vetor
		{
			if(c == ASPAS_DUPLAS)
				aspas_dup = true;
			else
				aspas_sim = true;

			token[j] = '\0';
			char **grupo1;
			//usar o separar por char
			int qtd_grupo1;
			grupo1 = separar_por_char(&qtd_grupo1,token, ESPACO);
			int k = 0;
			while(grupo1[k]!=NULL)
			{
				if(i > quant_tokens)
				{
					quant_tokens += MAX_QUANT_TOKENS;
					tokens = realloc(tokens,quant_tokens * sizeof(char*));
				}

				tokens[i] = grupo1[k];
				i++;
				k++;
			}
			//reseta token
			j = 0;
			token = NULL;
			token = malloc(len*sizeof(char));

		}
		else if((c == ASPAS_DUPLAS && aspas_dup == true)||(c == ASPAS_SIMPLES && aspas_sim == true))
		//se for fechamento de aspas, ja insere o token de vez
		{
			if(c == ASPAS_DUPLAS)
				aspas_dup = false;
			else
				aspas_sim = false;

			if(i > quant_tokens)
				{
					quant_tokens += MAX_QUANT_TOKENS;
					tokens = realloc(tokens,quant_tokens);
				}
			token[j] = '\0';
			tokens[i] = token;
			i++;
			//reseta token
			j = 0;
			token = NULL;
			token = malloc(len*sizeof(char));
			
		}
		else
		{
			token[j]= c;
			j++;
		}

		cont++;
		c = line[cont];
		if(c == '\0' || len == cont)
		{
			token[j] = '\0';
			char **grupo1;
			//usar o separar por char
			int qtd_grupo1;
			grupo1 = separar_por_char(&qtd_grupo1,token, ESPACO);
			int k = 0;
			while(grupo1[k]!=NULL)
			{
				if(i > quant_tokens)
				{
					quant_tokens += MAX_QUANT_TOKENS;
					tokens = realloc(tokens,quant_tokens * sizeof(char*));
				}

				tokens[i] = grupo1[k];
				i++;
				k++;

			}
			//reseta token
			j = 0;
			token = NULL;
		}
	}	
	
	return tokens;
}

void mudar_stdin(char in[])
{
	int entrada = open(in, O_RDONLY);
	if (entrada <0)
	{
		perror("open");
		exit(2);
	}	
	dup2(entrada, STDIN_FILENO);
	close(entrada);
}

void mudar_stdout(char out[])
{	
	int saida = open(out, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
	dup2(saida,STDOUT_FILENO);
	close(saida);
}

char **separar_prog_in_out(char *palavra)
{
	char **subtokens_menor;
	char **subtokens_maior;
	
	int qnt_subtokens_menor;
	int qnt_subtokens_maior;

	char *prog,*in,*out;

	//verificar se tem menor
	subtokens_menor = separar_por_char(&qnt_subtokens_menor,palavra,MENOR_QUE);
	qnt_subtokens_menor--;

	//se tiver menor
	if(qnt_subtokens_menor)
	{
		//verificando se tem maior na esquerda
		subtokens_maior = separar_por_char(&qnt_subtokens_maior,subtokens_menor[0],MAIOR_QUE);
		qnt_subtokens_maior--;
		if(qnt_subtokens_maior)
		{
			prog = subtokens_maior[0];
			in = subtokens_menor[1];
			out = subtokens_maior[1];
		}
		else
		{
			//verificando se tem maior na direita
			subtokens_maior = separar_por_char(&qnt_subtokens_maior,subtokens_menor[1],MAIOR_QUE);
			qnt_subtokens_maior--;
			if(qnt_subtokens_maior)
			{
				prog = subtokens_menor[0];
				in = subtokens_maior[0];
				out = subtokens_maior[1];
			}
			else
			{	
				//senao, nao tem maior
				prog = subtokens_menor[0];
				in = subtokens_menor[1];
				out = NULL;
			}
		}
	}
	else{
		//se nao tiver menor, verifica o maior no tokens
		subtokens_maior = separar_por_char(&qnt_subtokens_maior,palavra,MAIOR_QUE);
		qnt_subtokens_maior--;
		if(qnt_subtokens_maior)
		{
			prog = subtokens_maior[0];
			in = NULL;
			out = subtokens_maior[1];
		}
		else
		{
			prog = palavra;
			in = NULL;
			out = NULL;
		}
	}	
	//eliminando espaço do in e out
	char **esp1,**esp2;
	int qtd_esp1,qtd_esp2;	
	if(in != NULL)
	{					
		esp1 = separar_por_espaco(&qtd_esp1,in);					
		in = esp1[0];
	}
	if(out != NULL)
	{					
		esp2 = separar_por_espaco(&qtd_esp2,out);	
		out = esp2[0];
	}

	char **process;
	process = malloc(3*sizeof(char*));
	process[0] = prog;
	process[1] = in;
	process[2] = out;

	return process;	
}

void launch_process(char *prog, char *in, char *out, int infile,int outfile,int errfile)
{
	char **v;
	int qtd_parametros;
	//linkando os pipes
	if (infile != STDIN_FILENO)
    {
      	dup2 (infile, STDIN_FILENO);
      	close (infile);
    }
  	if (outfile != STDOUT_FILENO)
    {
      	dup2 (outfile, STDOUT_FILENO);
      	close (outfile);
    }
  	if (errfile != STDERR_FILENO)
    {
      	dup2 (errfile, STDERR_FILENO);
      	close (errfile);
    }
    //executar o processo
    v = separar_por_espaco(&qtd_parametros,prog);
	if(in != NULL)
	{					
		mudar_stdin(in);
	}
	if(out != NULL)
	{	
		mudar_stdout(out);
	}				
	//pid_t filho = getpid();
	// printf("pid filho: %d\n",filho);
	// int ret = execve(v[0],v,NULL);
	execve(v[0],v,0);
	//execve certo, nao roda o "erro"
	perror("execve");
	printf("erro do execve\n");
}

int main()
{
		
	//main valendo:
	
	//variaveis
	pid_t parent_pid = getpid();
	

	printf("CES-33 Minishell Project\n");
	printf("Aluno: Carlos Renato de Andrade Figueiredo - T22\n\n");
	//printf("pid da execução(pai): %d\n",getpid());


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

		//verificando se tem pipe:
		char **tokens;
		int qnt_tokens;
		tokens = separar_por_char(&qnt_tokens,line,PIPE);
		
		pid_t pid;
		int mypipe[2], infile=0, outfile=1,errfile = 2;
		pid_t wpid;
		int status = 0;
		int i = 0;
		while(tokens[i] != NULL)
		{			
			char **process = separar_prog_in_out(tokens[i]);

			char *prog, *in,*out;
			prog = process[0];
			in = process[1];
			out = process[2];


			if(tokens[i+1] != NULL)//se existir proximo token, fazemos um pipe
			{
				if(pipe(mypipe) < 0)
				{
					perror("pipe");
					exit(1);
				}
				outfile = mypipe[1];
			}
			else
				outfile = 1;

			//Fork para executar processo

			pid = fork();
			if(pid == 0)//se for o filho, manda executar
			{
				launch_process(prog,in,out, infile,outfile,errfile);
			}
			else if(pid < 0)
			{
				perror("fork");
				exit(1);
			}
			else //se for o pai
			{
				while ((wpid = wait(&status)) > 0);
				//wait(&status);
			}
			//limpando os pipes
			if (infile != 0)
		        close (infile);
		    if (outfile != 1)
		        close (outfile);
		    infile = mypipe[0];			
			i++;
		}
		line = NULL;
		
	}
	return 0;
}

	

