/*
Producer
Rob Deans
os fall 2022
*/

#include <semaphore.h> // for semaphores
#include <stdio.h> // debugging / output to console
#include <sys/mman.h> // mapping mem
#include <fcntl.h> // o flags
#include <sys/types.h> // truncate
#include <unistd.h> // truncate
#include <errno.h>

// for ouir buffer, pts a, b
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

void produce(sem_t*, sem_t*, int*, int, int);

/////////////////////////////////////////////////   END FUNCTIONS /////////////////////////////////////////////////////////////////// 

int main(){
    bug;

    clean();

    int fd = shm_open(BUFFER, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR | S_IXUSR); /* read write execute permissions (goup and owner),
                                                                     readyonl with o-flag, create if not present */
    int fd2 = shm_open(BUFFER, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR | S_IXUSR); /* read write execute permissions (goup and owner),
                                                                     readyonl with o-flag, create if not present */
                                                                     
    perror("fd2:ERROR");
    if(fd < 0) return -35;
    ftruncate(fd, BUFF_SIZE); // limit file size for buffer.
    int buff[ELEMENT_CT] = {20, 20};
    //write(fd, buff, BUFF_SIZE);
    int* buffer = (int*)mmap(0, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t* writer = sem_open(SEM_WRITER, O_CREAT, S_IRWXU, 1);
    sem_t* reader = sem_open(SEM_READER,O_CREAT, S_IRWXU, 0);


    int tmp; // checking if binary sem is 1

    sem_getvalue(writer, &tmp);
    if(tmp == 0){
        sem_post(writer);
    }
    bug;
    int i = 0;
    while(1){
        sem_wait(writer);
        sem_getvalue(writer, &tmp);

        produce(reader, writer, buffer, 0, ++i);
        produce(reader, writer, buffer, 1, ++i);
        
        if( i > 1000) i =0;
        while(tmp > 0)  sem_wait(writer); sem_getvalue(writer, &tmp);

        sem_post(reader);
    }
}


///////////////////////////////////////////   FUNCTION DEF ////////////////////////////////////////////////////////////////////////////
void clean(){ // unlink all existing
    bug;
    shm_unlink(BUFFER);

    shm_unlink(SEM_WRITER);

    shm_unlink(SEM_READER);

}

void produce(sem_t* reader, sem_t*  writer, int* buffer, int spot, int product){
    printf("Producing buffer[%i]: %i\n",spot,  product);
    buffer[spot] = product;
    bug;

}