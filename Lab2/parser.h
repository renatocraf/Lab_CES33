
#define MAIOR_QUE 62		//corresponde ao valor ASCII do character >
#define MENOR_QUE 60		//corresponde ao valor ASCII do character <
#define ESPACO 32			//corresponde ao valor ASCII do character espaco
#define ASPAS_DUPLAS 34 	//corresponde ao valor ASCII do character "
#define ASPAS_SIMPLES 39 	//corresponde ao valor ASCII do character '

#define MAX_QUANT_TOKENS 10  //valor de tokens que vai alocar no malloc

#define true  1
#define false 0

/*
	Essa função serve para dividir uma strings em varias
	strings menores considerando o critério de divisão
	o character 'c'.
	A função utilizada ajuda da função strtok. 
	Inicialmente a função aloca cria o vetor baseado no
	MAX_QUANT_TOKENS e, caso o valor seja extrapolado,
	é dado um realloc no vetor para um valor somando mais
	MAX_QUANT_TOKENS à quantidade de tokens

*/
char **separar_por_char(int *valor, char *line,char c);

/*
	Essa função faz algo semelhante a função separar_por_char
	considerando o character 'espaco' como divisor.
	A diferença dessa função é que não queremos que uma string
	que possua aspas (duplas ou simples) tenha divididas( ou seja,
	tudo que está entre as aspas é atômico, não sendo dividido)

*/
char **separar_por_espaco(int *valor, char *line);

/*
	Essa função serve para verificar se a string possui 
	entradas(in) e saidas(out).
	Após fazer a separação, ela irá retornar um vetor 
	contendo 'prog', que é o comando a ser executado(ainda 
	com os parametros), 'in', que é a entrada(caso a string 
	possua o character '<') e 'out', que representa a saida
	(caso a string possua o character '>').
	Lembrando que, caso nao haja entradas ou saidas, os valores
	de 'in' e 'out' serão NULL, respectivamente.
*/
char **separar_prog_in_out(char *palavra);