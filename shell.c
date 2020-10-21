#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> // tipo pid_t
#include <sys/wait.h> // waitpid

// estrutura para separar as matrizes de argumentos
typedef struct {
    char **argv;
} fullCom;


// obtem a quantidade de pipes
int get_pipe_qt(int argc, char **argv) {
    int i, qt = 0;
    for (i = 0; i < argc; i++) {
        if (*argv[i] == '|') {
            qt++;
        }
    }
    return qt;
}

// dada a matriz de argumentos, calcula a posicao do proximo pipe
int get_pipe_pos(char **argv) {
	int i=0;
	while (argv[i]) {
		if (*argv[i] == '|') return i;
		i++;
	}
	return -1;
}

int main(int argc, char ** argv){
    
    // se o programa nao receber nenhum argumento -> alerta o usuario
    if (argc == 1){
        printf("Uso %s <comando> <p_1> <p_2> ... <p_n> \n", argv[0]);
        return 0;
    }

    char **comando;
    comando = &argv[1]; // "comando" recebe o endereço de memoria do primeiro comando e vai ate o ultimo

    int numCom = get_pipe_qt(argc, argv) + 1; // "numCom" recebe o numero de comandos
    

    fullCom listaCom[numCom]; // cria-se um vetor com cada posicao representando uma matriz de argumentos
    
    int i, posPipe;
    int fd[numCom][2]; // cria um descritor de arquivos para cada comando
    pid_t filho; // declaracao do filho

    // criacao dos pipes de cada comando
    for(i=0; i<numCom; i++) {
        pipe(fd[i]);
    }

    for(i = 0; i < numCom; i++){

        // Separar o argv por matriz de argumentos
        if(i == 0) { // na primeira interacao do laco
            posPipe = get_pipe_pos(comando); // obtem a posicao do primeiro pipe
            listaCom[i].argv = &comando[0]; // na primeira posicao da estrutura recebe o primeiro comando ...
            listaCom[i].argv[posPipe] = NULL; // ... ate a posicao do primeiro pipe
        }
        else { // a partir das proximas iteracoes
            comando = &comando[posPipe+1]; // o novo comando aponta para o proximo endereco apos o pipe
            posPipe = get_pipe_pos(comando); // calcula o proximo pipe
            listaCom[i].argv = &comando[0]; // na posicao "i" da estrutura recebe o comando ...
            listaCom[i].argv[posPipe] = NULL; // ... ate a posicao do pipe calculado
        }
        // filho está executando
        filho = fork(); // cria o processo filho
        if(filho == 0) { // o processo filho executa
            if(i != numCom-1) { // se nao for o ultimo comando
                close(fd[i][0]); // nao le o pipe atual
                dup2(fd[i][1], STDOUT_FILENO); // escreve no pipe atual
                close(fd[i][1]);
            }
            if(i != 0) { // se nao for o primeiro comando
                close(fd[i-1][1]);  // nao escreve no pipe no pipe anterior
                dup2(fd[i-1][0], STDIN_FILENO); // le do pipe anterior
                close(fd[i-1][0]);
            }
            execvp(listaCom[i].argv[0], listaCom[i].argv); // executa o seu comando
        } else if(filho > 0) { // o processo pai esta executando
            if(i != numCom-1) { // se nao for o ultimo comando
                close(fd[i][1]); // fecha a escrita no pipe
            }
            if(i != 0) { // se nao for o primeiro comando
                close(fd[i-1][0]); // fecha a leitura no pipe
            }
            waitpid(filho, NULL, 0); // aguarda a execucao do processo filho
        }
    }
    return 0;
}