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

// get fd
int openMem(const char*);

// map mem segment
float* mapMem(int);

// open read semaphore
sem_t* openRead(const char*);

// open write semaphore
sem_t* openWrite(const char*);

//consume contents of buffer
void consume(sem_t*, sem_t*, int*);

/////////////////////////////////////////////////   END FUNCTIONS /////////////////////////////////////////////////////////////////// 
int main(){
    bug;

    clean();

    //int fd = openMem(BUFFER);
    int fd = shm_open(BUFFER, O_CREAT | O_RDWR | O_EXCL, 0600); /* read write execute permissions (goup and owner),
                                                                     readonly with o-flag, create if not present */
    //ftruncate(fd, BUFF_SIZE); // limit file size for buffer.

    //int* buffer = mapMem(fd);
    int* buffer = (int*)mmap(NULL, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

   //sem_t* writer = openWrite(SEM_WRITER),
    //*reader = openRead(SEM_READER);

    sem_t* writer = sem_open(SEM_WRITER, O_CREAT, S_IRWXU, 1);
    sem_t* reader = sem_open(SEM_READER,O_CREAT, S_IRWXU, 0);
    int tmp;
    while(1){
    sem_getvalue(reader, &tmp);
    bug;
    sem_wait(reader); // block
    //    while(tmp > 0){ // making sure seamaphore is binary
    //    sem_wait(reader);
    //    sem_getvalue(reader, &tmp);
    //}

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


int openMem(const char* name){
    bug;
    return shm_open(name, O_CREAT | O_RDONLY, S_IRWXU | S_IRWXG); /* read write execute permissions (goup and owner),
                                                                     readonly with o-flag, create if not present */
}

float* mapMem(int fd){
    bug;
    return (float*)mmap(0, BUFF_SIZE, PROT_READ, MAP_SHARED, fd, 0);
}

sem_t* openWrite(const char* name){
    bug;
    return sem_open(name, O_CREAT, S_IRWXU, 1);
}

sem_t* openRead(const char* name){
    return sem_open(name, O_CREAT, S_IRWXU, 0);
}

void consume(sem_t* reader, sem_t* writer, int* buffer){
    printf("Consuming buffer[0]: %i\n", buffer[0]);
    buffer[0] = 0;
    printf("Consuming buffer[1]: %i\n", buffer[1]);
    buffer[1] = 0;
    bug;

}