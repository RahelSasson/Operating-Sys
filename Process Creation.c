//BS'D 

//calling on the fork() function to create multiple child processes 
//Proper labeling of the Parent, Child and Grandchild process
//As well as proper output of each individual Process ID 

//each new process is indenticle to the original process 
//fork() takes no parameters 
//the return value for a child process is '0'
//the return value for the parent process is the child process ID 
//fork() returns a negative value if there was an issue creating a child process 

//utilizing the unistd.h header file to access fork() 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
   
    int pid1;
    int pid2;
    pid1 = fork();
    pid2 = fork();
    
    sleep(1);
    
    //Parent Process 
    if (pid1 == 0) {
        
        //Granchild Process
        if (pid2 == 0) {
            printf("\nI am the Grandchild Process: PID: %d", pid2);
        }
        else {
            printf("\nI am the Parent Process: PID: %d", pid1);
        }
    }
    
    else {
        
        //we are in the second child process 
        //which was created by the Parent Process when we called fork() a second time
        //We don't want to print anything
       if (pid2 == 0) {
        printf("");
        }
        
        //Grandparent Process
         else { 
            printf("I am the Grandparent Process: PID: %d", pid1);
        }
    }
    

    exit(0);
    
}