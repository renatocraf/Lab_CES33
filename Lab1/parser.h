
#define MAIOR_QUE 62
#define MENOR_QUE 60
#define ESPACO 32
#define ASPAS_DUPLAS 34
#define ASPAS_SIMPLES 39

#define MAX_QUANT_TOKENS 10

#define true  1
#define false 0


char **separar_por_char(int *valor,char *line,char c);
char **separar_por_espaco(int *valor,char *line);

char **separar_prog_in_out(char *palavra);