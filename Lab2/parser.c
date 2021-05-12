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