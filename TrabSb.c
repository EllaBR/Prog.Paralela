#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/file.h>


int main(){

    FILE * buffer;
    FILE * buffer_txt_lock;
    FILE * apagar;

    buffer = fopen("buffer.txt", "w");
    for(int i = 0; i < 10; i++){
        fprintf(buffer,"%d\n",i);
    }
    fclose(buffer);

    int num_consumidor, lixo;
    int n = 1;
    int pid = fork();
    srand(time(NULL)); //usa como semente de valor aleatório o tempo em segundos
    while(n=1){
    if (pid != 0){  //filho - produtor
        sleep(2);
        if (access("buffer.txt.lock", F_OK) != 0) { // verifica se o arquivo existe
            buffer_txt_lock = fopen("buffer.txt.lock", "a"); // se não existe, cria
            //for(int i = 0; i < 3; i++) {
                buffer = fopen("buffer.txt", "a");
                int random = rand()%99; // cria um valor aleatório entre 0 e 99
                fprintf(buffer, "%d\n", random); // coloca no final do arquivo
                printf("[PRODUTOR]: %d\n", random);
                int s = 1 + (rand()%3); // nuemro aleatório de 1-3
                sleep(s);
                fclose(buffer);
            //}
            remove("buffer.txt.lock");
        }
        }else{ //pai - consumidor
        sleep(2);
            if (access("buffer.txt.lock", F_OK) != 0) { // verifica se o arquivo existe
                buffer_txt_lock = fopen("buffer.txt.lock", "a"); // se não existe, cria
                sleep(1);
                buffer = fopen("buffer.txt", "r");
                apagar = fopen("apagar.txt", "w");
                fscanf(buffer, "%d", &lixo);
                int linhas_consumidor = 0;
                while(fscanf(buffer, "%d", &num_consumidor) == 1){ //lê as linhas do arquivo e copia tudo que o consumidor não usou para outro arquivo
                    if(lixo == num_consumidor){
                        continue;
                    }else{
                       fprintf(apagar, "%d\n", num_consumidor);   
                    }
                }
                remove("buffer.txt");
        
                char nome_velho[] = "apagar.txt";
                char nome_novo[] = "buffer.txt";
                
                rename(nome_velho, nome_novo);
                
                fclose(apagar);
                
                printf("[CONSUMIDOR]: %d\n", lixo);
                int s = 1 + (rand()%3); // nuemro aleatório de 1-3
                sleep(s);
                remove("buffer.txt.lock");                
            }  
            
        }
    }   
    return 0; 
}
        