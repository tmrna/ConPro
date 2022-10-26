/*
Consumer
Rob Deans
os fall 2022
*/

#include <semaphore.h> // for semaphores
#include <stdio.h> // debugging / output to console
#include <sys/mman.h> // mapping mem
#include <fcntl.h> // o flags
#include <unistd.h>
#include <sys/types.h>

// for ouir buffer, pts a, b
#define BUFFER "/buffer"

// names for semaphores
#define SEM_WRITER "/sem_writer"
#define SEM_READER "/sem_reader"

// size of our buffer
#define BUFF_SIZE sizeof(int) * 3

// debugger
#define bugMode 0
#define bug if(bugMode) printf("We are in file %s in function %s at line %d\n", __FILE__,  __FUNCTION__, __LINE__)

//////////////////////////////////////////////////   FUNCTIONS  /////////////////////////////////////////////////////////////////////////

// unlink any existing shared mem
void clean();

//consume contents of buffer
void consume(sem_t*, sem_t*, int*);

/////////////////////////////////////////////////   END FUNCTIONS /////////////////////////////////////////////////////////////////// 
int main(){
    bug;

    clean();

    int fd = shm_open(BUFFER, O_CREAT | O_RDWR, 0600); /* read write execute permissions (goup and owner),
                                                                     readyonl with o-flag, create if not present */
    if(fd < 0) return -35;
    ftruncate(fd, BUFF_SIZE); // limit file size for buffer.


    int* buffer = (int*)mmap(0, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    

    sem_t* writer = sem_open(SEM_WRITER, O_CREAT, S_IRWXU, 1);
    sem_t* reader = sem_open(SEM_READER,O_CREAT, S_IRWXU, 0);
    int tmp;
    while(1){
    sem_getvalue(reader, &tmp);
    bug;
    sem_wait(reader); // block
    //while(tmp > 0){ // making sure seamaphore is binary
    //   sem_wait(reader);
    //    sem_getvalue(reader, &tmp);
   // }

    consume(reader, writer, buffer);
    

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

void consume(sem_t* reader, sem_t* writer, int* buffer){
    printf("Consuming buffer[0]: %i\n", buffer[0]);
    buffer[0] = 0;
    printf("Consuming buffer[1]: %i\n", buffer[1]);
    buffer[1] = 0;
    bug;

}