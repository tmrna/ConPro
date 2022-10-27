# Programming assignment 1

Rob Deans Consumer Prodcer assignment os 2022.

# Consumer Producer

Two binary semaphores are used to manage read/write processes. 

The producer creates a buffer and two semaphores (read write), then decraments the writer and prints two integers before writing them to the buffer. The integers are managed by a counter prior to writing. A reset on the counter was implemented after the writing process to prevent overflow. After writing, the producer ensures that the writer is zeroed it then incraments the reader and waits for the writer to be incramented. 


The consumer fetches the buffer and waits for the reader to be incramented. Once it is the contents of the buffer are read and printed to the console. After that the consumer ensures the reader is zeroed and incraments the writer.

This back and forth continues indefinitely.

# Compiler
Executables were compiled using gcc 11.3.0 with command pthread and lrt flags. 

# Features to implement
As of right now there is no killswitch