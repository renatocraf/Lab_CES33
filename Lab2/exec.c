
struct comando
{
	char **v;
	char *in;
	char *out;
	comando *filho;

};

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

void launch_process(char **v, char *in, char *out, int infile,int outfile,int errfile)
{
	
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
	if(in != NULL)
	{					
		mudar_stdin(in);
	}
	if(out != NULL)
	{	
		mudar_stdout(out);
	}			
	execve(v[0],v,0);	
	perror("execve");
}

void launch_job(comando *inicio)
{

	pid_t pid,
		  wpid;

	int mypipe[2], 
		infile  = 0, 
		outfile = 1,
		errfile = 2;

	int status = 0;

	comando *p;
	p = inicio;

	while(p != NULL)
	{
		if(p->filho != NULL)//se existir proximo token, fazemos um pipe
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
		//se for o filho, manda executar
		if(pid == 0)
			launch_process(p->v,p->in,p->out, infile,outfile,errfile);
		// se der erro
		else if(pid < 0)
		{
			perror("fork");
			exit(1);
		}
		//se for o pai
		else 
		{
			//printf("pai esta aguardando\n");
			while ((wpid = wait(&status)) > 0);
			//printf("pai parou de aguardar\n");				
		}
		//limpando os pipes
		if (infile != 0)
	        close (infile);
	    if (outfile != 1)
	        close (outfile);
	    infile = mypipe[0];
	    p = p->filho;
	}
		

}