 # include <stdio.h>
 #include <stdlib.h>
 # include <unistd.h>
 # include <sys/wait.h>

 int main(){
    pid_t pid;
    int qtd = 4;

    for(int i = 0; i < qtd; i++){
        pid = fork();

        if ( pid < 0) { // Erro
            fprintf (stderr , " Fork falhou !\n");
            return 1;
        } else if (pid == 0) { // Processo Filho
            printf ("Eu sou o filho! Meu PID e %d, meu pai e %d.\n", getpid () , getppid ());
            exit(0);
        }
    }

    for(int i = 0; i < qtd; i++){
        wait(NULL);
    }

    printf ("Meu filho terminou, agora eu posso terminar.\n");
    return 0;
 }