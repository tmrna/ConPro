/*
Consumer
Rob Deans
os fall 2022
*/

#include <semaphore.h> // for semaphores
#include <stdio.h> // debugging / output to console
#include <sys/mman.h> // mapping mem
#include <fcntl.h> // o flags
#include <errno.h> // for errors when debug
#include <unistd.h> // read

// for our buffer
#define BUFFER "buffer"

// names for semaphores
#define SEM_WRITER "/sem_writer"
#define SEM_READER "/sem_reader"

// size of our buffer
#define ELEMENT_CT 2
#define BUFF_SIZE sizeof(int) * ELEMENT_CT

// debugger
#define bugMode 0
#define bug if(bugMode) printf("We are in file %s in function %s at line %d\n", __FILE__,  __FUNCTION__, __LINE__)

//////////////////////////////////////////////////   FUNCTIONS  /////////////////////////////////////////////////////////////////////////

// unlink any existing shared mem
void clean();

//consume contents of buffer and display.
void consume(int*);

/////////////////////////////////////////////////   END FUNCTIONS /////////////////////////////////////////////////////////////////// 
int main(){
    int fd = shm_open(BUFFER, O_RDONLY, S_IRUSR); /* read only permissions for usr and process */

    if(bugMode)perror("shm_open():ERROR");

    int buff[ELEMENT_CT]; // make a buffer to read to 
    read(fd, buff, BUFF_SIZE); // read to it

    int* buffer = (int*)mmap(0, BUFF_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    bug;

    sem_t* writer = sem_open(SEM_WRITER, O_CREAT, S_IRWXU, 1);
    sem_t* reader = sem_open(SEM_READER,O_CREAT, S_IRWXU, 0);
    int tmp;
    while(1){
    bug;
    sem_wait(reader); // block
    /*  Crit sect  */
    sem_getvalue(reader, &tmp); // set val to ensure reader stays binary sem

    consume(buffer); // read from buffer

    while(tmp > 0 ) sem_wait(reader); sem_getvalue(reader, &tmp); // Ensures reader is set to 0

    sem_post(writer); // unblock
    }
}

///////////////////////////////////////////   FUNCTION DEF ////////////////////////////////////////////////////////////////////////////
void clean(){ // unlink all existing
    bug;
    shm_unlink(BUFFER);

    shm_unlink(SEM_WRITER);

    shm_unlink(SEM_READER);
}

// read from buffer and output to console.
void consume(int* buffer){
    printf("Consuming buffer[0]: %i\n", buffer[0]);
    
    printf("Consuming buffer[1]: %i\n", buffer[1]);

    bug;

}