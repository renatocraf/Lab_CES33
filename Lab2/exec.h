
/* Essa struct representa o comando
	- Possui um vetor 'v' que representa o argv, com argv[0] sendo 
	o comando e a partir do indice 1 representa os parametros do comando
	- Caso haja uma entrada de um arquivo, o diretorio dele estará 
	representado no ponteiro 'in'
	- Caso haja uma saída para um arquivo, o diretorio dele estará 
	representado no ponteiro 'out'
	- Caso haja um Pipe depois do comando, significa que o stdout desse 
	comando irá pra o stdin	do comando após o pipe. Para fazer organizar 
	uma série de comandos foi utilizado uma lista encadeada,
	portanto o próximo comando está representado como 'filho' desse comando.

*/
typedef	struct comando comando;

/* Essa função serve para mudar a entrada do stdin*/
void mudar_stdin(char in[]);

/* Essa função serve para mudar a saída do stdout*/
void mudar_stdout(char out[]);

/* Essa função executa o processo propriamente dito.
   Após receber os parâmetros, ele verifica e faz o link com o pipe e em 
   seguida manda executar o comando contido no vetor 'v'
	- Caso o infile seja diferente do Stdin, ele faz o link(mudando o stdin) e 
	depois fecha o infile
	- Caso o outfile seja diferente do Stdout, ele faz o link(mudando o stdout) 
	e depois fecha o outfile
	- Caso o errfile seja diferente do Stderror, ele faz o link(mudando o stderror) 
	e depois fecha o errfile
	Após isso, verifica se existe foi chamado alguma entrada ou saída pela linha 
	de comando (caso haja, os ponteiros 'in' e 'out' serão diferentes de NULL) e, 
	caso haja, muda o stdin ou stdout novamente.
	- Por fim, após essas verificações executa o comando através do 'execve'.
*/
void launch_process(char **v, char *in, char *out, int infile,int outfile,int errfile);


/* Essa função gerencia o comandos.
	- Inicialmente o ponteiro 'p' aponta para o 'inicio', a partir dai
	entramos no loop que vai rodar toda a lista encadeada criada.
	- Caso o comando possua "filhos"(não confundir comando 'filho' com 
	processo filho!!!), fazemos um pipe('mypipe' recebe os valores do pipe) e 
	atribuimos o valor do Stdout ao 'outfile'.
	- Senão, atribuímos o valor 1 ao outfile.
	- Após isso o processo é clonado através do 'fork()':
		- Para o processo filho criado é mandado executar(através do launch_process) 
		o comando de 'p'.
		- Para o processo pai, deve-se aguardar o processo filho terminar 
		sua execução.
	- Após o 'infile' recebe o conteúdo do 'mypipe[0]'
	- O próximo comando é chamado até que todos sejam executados.
*/
void launch_job(comando *inicio);