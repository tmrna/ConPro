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
#define BUFF_SIZE sizeof(char)

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
    int fd = shm_open(BUFFER, O_CREAT | O_RDONLY, S_IRWXU | S_IRWXG); /* read write execute permissions (goup and owner),
                                                                     readonly with o-flag, create if not present */
    ftruncate(fd, BUFF_SIZE); // limit file size for buffer.

    //int* buffer = mapMem(fd);
    float* buffer = (float*)mmap(NULL, BUFF_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    


    int dataA = 1;
    int dataB = 2;

   //sem_t* writer = openWrite(SEM_WRITER),
    //*reader = openRead(SEM_READER);

    sem_t* writer = sem_open(SEM_WRITER, O_CREAT, S_IRWXU, 1);
    sem_t* reader = sem_open(SEM_READER,O_CREAT, S_IRWXU, 0);
    int tmp;
    while(1){

    bug;
    sem_wait(reader); // block
    //consume(reader, writer, buff);
    for(int i = 0; i < 2; i++){
        printf("Consuming %i\n", i);
    }
    sem_getvalue(writer, &tmp);
    printf("Value retrieved is %d\n", tmp);
    while(tmp > 0){ // making sure seamaphore is binary
        sem_wait(reader);
        sem_getvalue(reader, &tmp);
        printf("Current val reader %i\n", tmp);
    }
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
    return (float*)mmap(NULL, BUFF_SIZE, PROT_READ, MAP_SHARED, fd, 0);
}

sem_t* openWrite(const char* name){
    bug;
    return sem_open(name, O_CREAT, S_IRWXU, 1);
}

sem_t* openRead(const char* name){
    return sem_open(name, O_CREAT, S_IRWXU, 0);
}

void consume(sem_t* reader, sem_t* writer, int* seg){
    bug;

}