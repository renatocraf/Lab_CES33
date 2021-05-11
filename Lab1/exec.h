typedef	struct comando comando;

void mudar_stdin(char in[]);

void mudar_stdout(char out[]);

void launch_process(char **v, char *in, char *out, int infile,int outfile,int errfile);

void launch_job(comando *inicio);