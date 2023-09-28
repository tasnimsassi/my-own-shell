#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <assert.h>


#define MAX_LEN 1024
#define MAX_HISTORY 10
#define clear() printf("\033[H\033[J")


char history[MAX_HISTORY][MAX_LEN];
int history_index = 0;


void add_to_history(char* command) {
    strcpy(history[history_index], command);
    history_index = (history_index + 1) % MAX_HISTORY;
}

void print_history() {
    for (int i = history_index; i < MAX_HISTORY; i++) {
        printf("%d. %s\n", i, history[i]);
    }
    for (int i = 0; i < history_index; i++) {
        printf("%d. %s\n", i, history[i]);
    }
}



int takeInput(char* str)
{
	char* buf;

	buf = readline("\n % ");
	if (strlen(buf) != 0) {
		add_history(buf);
		strcpy(str, buf);
		return 0;
	} else {
		return 1;
	}
}


void printDir()
{
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Dir: %s\n", cwd);
    } else {
        printf("Error getting current working directory\n");
    }
}


void init_shell()
{
	clear();
	printf("\n\n\n\n******************"
		"************************");
	printf("\n\n\n\t****MY SHELL****");
	printf("\n\n\t-sassi tasnim 1ere ingenieur grp 3-");
	printf("\n\n\n\n******isi-ariana*************"
		"***********************");
	char* username = getenv("USER");
	printf("\n\n\nUSER is: @%s", username);
	printf("\n");
	sleep(1);
	clear();
}



void executeCommand (char* command  )
 {
    
    char* commands[MAX_LEN];
    int i = 0;
     commands[i] = strtok(command, " ");
     while (commands[i] != NULL) {
        commands[++i] = strtok(NULL, " ");
    }
    
   
    if(strcmp(commands[0], "cd") == 0) {
        if(commands[1] != NULL) { 
            if(chdir(commands[1]) != 0) { 
                printf("Error: specified directory not found\n");
            }
        } else {
            chdir(getenv("HOME")); 
        }
    }
    

      if (strcmp(commands[0], "history") == 0) {
        add_to_history( command);
        return;
    }
     
    
    if ((strcasecmp(commands[0], "quit") == 0) || (strcasecmp(commands[0], "eof") == 0) ) {
     printf(" goodbye beautiful \n");
    exit(0);
     }

    
    while (commands[i] != NULL) {
        commands[++i] = strtok(NULL, " ");
    }

    int status = 0;
    pid_t pid = fork();
    
	
    if (pid == 0) {
          int is_success = 1;
         for (int j = 0; commands[j] != NULL; j++) {
         if (strcmp(commands[j], "&&") == 0) {
                char* cmd1[j+1];
                for (int k = 0; k < j; k++) {
                    cmd1[k] = commands[k];
                }
                cmd1[j] = NULL;
                execvp(cmd1[0], cmd1);
                printf("Error: command not found\n");
                
            } 
            else if (strcmp(commands[j], "||") == 0) {
                char* cmd1[j+1];
                for (int k = 0; k < j; k++) {
                    cmd1[k] = commands[k];
                }
                cmd1[j] = NULL;
                execvp(cmd1[0], cmd1);
                printf("Error: command not found\n");
            } else if (strcmp(commands[j], ";") == 0) {
                char* cmd1[j+1];
                char* cmd2[MAX_LEN - j];
                for (int k = 0; k < j; k++) {
                    cmd1[k] = commands[k];
                }
                for (int k = j + 1; k < MAX_LEN; k++) {
                   cmd2[k - j - 1] = commands[k];
                }
                cmd1[j] = NULL;

                pid_t pid = fork();
                if (pid == 0) {
                    if (execvp(cmd1[0], cmd1) == -1) {
                        printf("Error: command not found\n");
                    }
                } else {
                    wait(NULL);
                        if (execvp(cmd2[0], cmd2) == -1) {
                            printf("Error: command not found\n");
                        }
                }
            }

            } 
		
        
    } else {
        wait(&status);
         
        if (WEXITSTATUS(status) == 0) {
    for (int j = 0; commands[j] != NULL; j++) {
        if (strcmp(commands[j], "&&") == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                char* cmd2[MAX_LEN - j];
                for (int k = j + 1; commands[k] != NULL; k++) {
                    cmd2[k - j - 1] = commands[k];
                }
                cmd2[MAX_LEN - j - 1] = NULL;
                execvp(cmd2[0], cmd2);
                printf("Error: command not found\n");
                exit(EXIT_FAILURE);
            } else {
                wait(NULL);
            }
            break;
        }
    } }
         if (WEXITSTATUS(status) != 0) {
               for (int j = 0; commands[j] != NULL; j++) {
                  if (strcmp(commands[j], "||") == 0) {
                      char* cmd2[MAX_LEN - j];
              for (int k = j + 1; commands[k] != NULL; k++) {
                cmd2[k - j - 1] = commands[k];
            }
            cmd2[MAX_LEN - j - 1] = NULL;
            execvp(cmd2[0], cmd2);
            printf("Error: command not found\n");
           break;
        }
    }
}
                 
    }
    
if (pid == 0) {
for (int j = 0; commands[j] != NULL; j++) {
            
           if (strcmp(commands[j], "|") == 0) {
    commands[j] = NULL;
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    if (pid == 0) {
        if (dup2(fd[1], 1) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        if (close(fd[0]) == -1 || close(fd[1]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        if (execvp(commands[0], commands) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        if (wait(NULL) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd[0], 0) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        if (close(fd[0]) == -1 || close(fd[1]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        if (execvp(commands[j + 1], &commands[j + 1]) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
} else if (strcmp(commands[j], ">") == 0) {
    commands[j] = NULL;
    int fd = open(commands[j + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Error: Failed to open or create file %s\n", commands[j + 1]);
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, 1) == -1) {
        printf("Error: Failed to redirect output to file %s\n", commands[j + 1]);
        exit(EXIT_FAILURE);
    }
    close(fd);
    if (execvp(commands[0], commands) == -1) {
        printf("Error: Failed to execute command %s\n", commands[0]);
        exit(EXIT_FAILURE);
    }
    exit(0);
}

            else if (strcmp(commands[j], ">>") == 0) {
                commands[j] = NULL;
                int fd = open(commands[j + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
                dup2(fd, 1);
                close(fd);
                execvp(commands[0], commands);
            }
        }
        execvp(commands[0], commands);
        if (execvp(commands[0], commands) < 0) {
        fprintf(stderr, "Error: command not found or can't be executed \n");
         exit(1);
       }
    } else {
         wait(&status);
    }  
}

void runBatchFile(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        printf("Error: Failed to open batch file %s\n", fileName);
        return;
    }
    char command[MAX_LEN];
    while (fgets(command, MAX_LEN, file)) {
        command[strcspn(command, "\n")] = 0;
        if (strcmp(command, "quit") == 0) {
            printf("Exiting the shell...\n");
            exit(0);
        }
        executeCommand(command);
        add_to_history(command);
    }
    fclose(file);
}



int main(int argc, char* argv[]) {
    init_shell();
         if (argc == 2) {
        runBatchFile(argv[1]);
        return 0;
    }
    
    else if (argc > 2) {
        printf("Error: Incorrect number of arguments. Usage: %s [batch_file]\n", argv[0]);
    }
    else {
        while (1) {
            char command[MAX_LEN];
            char* commands[MAX_LEN];
            printDir();
            if (takeInput(command))
                continue;
            executeCommand(command);
            add_to_history(command);
            if (strcmp(command, "history") == 0) {
                print_history();
            }
        }
    
}
    return 0;
}