#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define VECTOR_SIZE 1000
#define NUM_PROCESSES 4

int main() {
    int vector[VECTOR_SIZE];
    int pipes[NUM_PROCESSES][2];
    int chunk_size = VECTOR_SIZE / NUM_PROCESSES;

    for (int i = 0; i < VECTOR_SIZE; i++)
        vector[i] = 1;

    // Inicia os pipes
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            close(pipes[i][0]);
            for (int j = 0; j < NUM_PROCESSES; j++) {
                if (j != i) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }

            int start = i * chunk_size;
            int end =  (i + 1) * chunk_size;

            double local_sum = 0.0;
            for (int k = start; k < end; k++)
                local_sum += vector[k];
            
            write(pipes[i][1], &local_sum, sizeof(double));
            close(pipes[i][1]);
            exit(0); 
        }else
            close(pipes[i][1]);
    }

    double total_sum = 0.0;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        double partial_sum;
        read(pipes[i][0], &partial_sum, sizeof(double));
        
        total_sum += partial_sum;
        close(pipes[i][0]);
    }

    for (int i = 0; i < NUM_PROCESSES; i++)
        wait(NULL);

    printf("Soma: %.0f\n", total_sum);
    return 0;
}
