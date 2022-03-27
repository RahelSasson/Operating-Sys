 //BS'D 
 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

//Semaphore is a synchronization tool that provides a way for processes to synchronize their activities
//it contains an integer value 

 //To write a program that uses semaphores to simulate mediated access to computer resources:
 //5 printers 
 //6 plotters 
 //4 scanners 

// declaration of constants for resource sempahores
#define PRINTER_ID 1
#define PLOTTER_ID 2
#define SCANNER_ID 3

#define PRINTER_COUNT 5
#define PLOTTER_COUNT 6
#define SCANNER_COUNT 4


//random number generator 
//generates a random number in range from 'low' to 'high'
//range is passed into parameters 'low' and 'high'
//this function will determine which resource to request 
int random_resource(int low, int high) {
    return random() % (high - low + 1) + low;
}

int main(int argc, char** argv) {
    

    //variable initializations 
    int rid, status;
    int pid;
    sem_t* resources[4];
    const char resource_name[4][10] = {"", "printer", "plotter", "scanner" };
    
    //sem_open creates a new semaphore
    //initialize semaphores to appropriate values 
    resources[PRINTER_ID] = sem_open(resource_name[PRINTER_ID], O_CREAT, 1, PRINTER_COUNT);
    resources[PLOTTER_ID] = sem_open(resource_name[PLOTTER_ID], O_CREAT, 1, PLOTTER_COUNT);
    resources[SCANNER_ID] = sem_open(resource_name[SCANNER_ID], O_CREAT, 1, SCANNER_COUNT);
    
    
    //seeds random sequence of numbers 
    srandom(time(NULL));
    
    //routine that loops through the sequence 4 times 
    //in each iteration the process : 
    //forks a child 
    //sleeps for a random time between 1-3 seconds 
    for (int i = 0; i < 4; i++) {
        
        pid = fork();
        
        //fork() returns a negative value if there was an issue creating a child process
        //exit failure
        if (pid == -1) {
            printf("ERROR! Failed forking a child process.\n");
            exit(1);
            }
            //fork() returns '0' as value for a child process
            //if we are in a child process choose a resource 
            else if (pid == 0) { 
                rid = random_resource(PRINTER_ID, SCANNER_ID);
        
                //while semaphore value is positive 
                //acquire resource 
                //sem_trywait() decrements (locks) the semaphore
                while (1) {
                    status = sem_trywait(resources[rid]);
                    
                    //output process ID, and requested resource
                    printf("Process %d requests %s.\n", getpid(), resource_name[rid]);
                    //output process id and request success status
                    printf("Process %d request %s.\n", getpid(), (status ? "failed" : "succeeded"));
                    
                    //if resource is unavailable 
                    //sleep between 2 to 4 seconds 
                    if (status != 0) { 
                        sleep(random_resource(2, 4));
                    }
                    
                        //if resource is available 
                        //sleep between 1 and 3 seconds 
                        //release the resource 
                        //sem_post() increments (unlocks) the semaphore 
                        //then break to exit loop and terminate
                        else { 
                            sleep(random_resource(1, 3));
                            sem_post(resources[rid]);
                            break;
                        }
                }
                    
                //exit success
                exit(0);
            }       
            //else we are in the parent process
            //sleep for a random time between 1-3 seconds
            else { 
            sleep(random_resource(1, 3));
            }
        }
            
            //loops and calls wait() to block the parent process until its child processes finish execution
            //after child process terminates, parent continues execution
            for (int i = 0; i < 4; i++) {
             wait(NULL);
            }
            
            
    //sem_unlink() removes the named semaphore referred to by name
    //marks the semaphore to be destroyed once all processes cease using it
    //in this case, immediately 
    sem_unlink(resource_name[PRINTER_ID]);
    sem_unlink(resource_name[PLOTTER_ID]);
    sem_unlink(resource_name[SCANNER_ID]);
                    
    exit(0);
}
