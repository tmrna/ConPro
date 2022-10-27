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
#include <errno.h> // for errors when debugging

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

// produce to buffer
void produce(int*, int, int);

/////////////////////////////////////////////////   END FUNCTIONS /////////////////////////////////////////////////////////////////// 

int main(){
    bug;

    // clean all existing
    clean();

    int fd = shm_open(BUFFER, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR); /* create mem if not already existing, read/write permissions
                                                                        for process and usr */

                                                                     
    if(bugMode) perror("fd:ERROR");
    if(fd < 0) return -35;
    ftruncate(fd, BUFF_SIZE); // limit file size for buffer.
    int buff[ELEMENT_CT] = {20, 20};
    //write(fd, buff, BUFF_SIZE);
    int* buffer = (int*)mmap(0, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t* writer = sem_open(SEM_WRITER, O_CREAT, S_IRWXU, 1); // open create sem, read/write/execute
    sem_t* reader = sem_open(SEM_READER,O_CREAT, S_IRWXU, 0);


    int tmp; // checking if binary sem is 1

    sem_getvalue(writer, &tmp);
    if(tmp == 0){
        sem_post(writer);
    }
    bug;
    int i = 0;
    while(1){
        sem_wait(writer); // block
        sem_getvalue(writer, &tmp); // load in val for tmp to make sure it stays binary

        produce(buffer, 0, ++i);
        produce(buffer, 1, ++i);
        
        if( i > 1000) i =0; // preventing overflow
        while(tmp > 0)  sem_wait(writer); sem_getvalue(writer, &tmp); // making sure sem val is 0 for binary

        sem_post(reader); // reader++, unblock
    }
}


///////////////////////////////////////////   FUNCTION DEF ////////////////////////////////////////////////////////////////////////////

// unlink all existing
void clean(){ 
    bug;
    shm_unlink(BUFFER);

    shm_unlink(SEM_WRITER);

    shm_unlink(SEM_READER);

}

// write the given product to the buffer in the place spot
void produce(int* buffer, int spot, int product){
    printf("Producing buffer[%i]: %i\n",spot,  product);
    buffer[spot] = product;
    bug;

}