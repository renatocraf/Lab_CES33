#include <sys/types.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

#define MAX_LENGHT_PROG 1024
#define MAX_QUANT_LINES 64
#define MAX_QUANT_TOKENS 10

#define PIPE 124
#define MAIOR_QUE 62
#define MENOR_QUE 60
#define ESPACO 32
#define ASPAS_DUPLAS 34
#define ASPAS_SIMPLES 39



/*
CRIAR UMA FUNÇAO PARA GERAR OS PROGRAMAS
*/

//getline
char **get_lines()
{
	//capturar linhas
	
	size_t len = 0;
	ssize_t nread;
	char *line = NULL;

	char **v;

	int quant_lines = MAX_QUANT_LINES;

	v = malloc(quant_lines*sizeof(char*));
	int i = 0 ;

	while ((nread = getline(&line, &len, stdin)) != -1 && i < 1) 
	{
	   //printf("Retrieved line of length %zu:\n", nread);
		//tirando o \n do final
		line[nread-1]='\0';

	   //fwrite(line, nread, 1, stdout);
		if(feof(stdin))
		{
			return v;
		}

		if(i == quant_lines)
		{
			quant_lines += MAX_QUANT_LINES;
			v = realloc(v,quant_lines);
		}		
		v[i] = line;
		
		//printf("valor %d\n",i );
		line = NULL;
		i++;
	}
	/*
	fwrite(v[0], nread,1,stdout);
	fwrite(v[1], nread,1,stdout);
	if(v[2]== NULL)
	{
		printf("V2 eh null\n");
	}
	*/

	free(line);
	return v;

}

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
	//printf("c: %c\n",c);

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
		//printf("iteraçao: %d c: %d\n", cont,c );
		if ((c == ASPAS_DUPLAS && aspas_dup == false )||(c == ASPAS_SIMPLES && aspas_sim == false))
		//acaba palavra sem aspas, entao verifica espacos e cria vetor
		{
			//printf("abriu aspas\n");
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
				//printf("token encontrado: %s\n",tokens[i]);
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
			//printf("fechou aspas\n");
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
			//printf("token encontrado: %s\n",tokens[i] );
			i++;
			//reseta token
			j = 0;
			token = NULL;
			token = malloc(len*sizeof(char));
			
		}
		else
		{
			//printf("nao foi aspas\n");
			token[j]= c;
			//printf("%s\n",token );
			j++;
		}

		cont++;
		c = line[cont];
		//printf("c: %c c_num:%d\n",c,c );
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
					//printf("%d\n",quant_tokens );
					tokens = realloc(tokens,quant_tokens * sizeof(char*));
				}

				tokens[i] = grupo1[k];
				//printf("token encontrado: %s\n",tokens[i]);
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

void mudar_stdin(char **in)
{
	int entrada = open(in[0], O_RDONLY);	
	dup2(entrada, STDIN_FILENO);
	close(entrada);
}

void mudar_stdout(char **out)
{
	
	int saida = open(out[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
	dup2(saida,STDOUT_FILENO);
	close(saida);
}

int main()
{
		
	//main valendo:
	
	//variaveis
	pid_t parent_pid = getpid();
	pid_t wpid;
	int status = 0;

	printf("Welcome to bash 2.0\n");
	printf("pid da execução(pai): %d\n",getpid());

	

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
		if(qnt_tokens > 1)
		{
			printf("teremos que fazer pipe\n");
			
		}
		
		//procurando por < e > para mudar o fd

		int i = 0;
		while(tokens[i] != NULL)
		{
			char **subtokens_menor;
			char **subtokens_maior;
			
			int qnt_subtokens_menor;
			int qnt_subtokens_maior;

			char *prog,*in,*out;


			//verificar se tem menor
			subtokens_menor = separar_por_char(&qnt_subtokens_menor,tokens[i],MENOR_QUE);
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
				subtokens_maior = separar_por_char(&qnt_subtokens_maior,tokens[i],MAIOR_QUE);
				qnt_subtokens_maior--;
				if(qnt_subtokens_maior)
				{
					prog = subtokens_maior[0];
					in = NULL;
					out = subtokens_maior[1];
				}
				else
				{
					prog = tokens[i];
					in = NULL;
					out = NULL;
				}
			}
			//tirando os espaços			
			char **v;

			int qtd_parametros;

			pid_t child_pid = fork();
		
			if(child_pid == 0) //quer dizer que é o filho
			{
				v = separar_por_espaco(&qtd_parametros,prog);
				char **esp1,**esp2;
				int qtd_esp1,qtd_esp2;
				if(in != NULL)
				{
					
					esp1 = separar_por_espaco(&qtd_esp1,in);					
					mudar_stdin(esp1);
				}
				if(out != NULL)
				{
					
					esp2 = separar_por_espaco(&qtd_esp2,out);	
					mudar_stdout(esp2);
				}				
				pid_t filho = getpid();
				// printf("pid filho: %d\n",filho);
				// int ret = execve(v[0],v,NULL);
				execve(v[0],v,0);
				//execve certo, nao roda o "erro"
				perror("execve");
				printf("erro do execve\n");
				//close(filho);			
			}
			// getchar();
			while ((wpid = wait(&status)) > 0);

			i++;
		}



		//printf("tokens: %d\n",qnt_tokens);



		//
		line = NULL;
		
	}


	return 0;
}

	

