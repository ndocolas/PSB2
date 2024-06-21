#ifndef MYMEMORY_H
#define MYMEMORY_H

#include <stddef.h>

typedef struct allocation
{
    void *start; //ponteiro para o inicio da alocacao
    size_t size; //tamanho da alocacao
    struct allocation *next; //ponteiro para a proxima alocacao. caso null, nao tem proximo
} allocation_t; 

typedef struct 
{
    void *pool; //ponteiro para o bloco de memoria real
    size_t total_size; //tamanho total da lista
    allocation_t *head; //inicio da lista. aponta para o HEAD
} mymemroy_t;

mymemroy_t* mymemory_init(size_t size);

void* mymemory_alloc(mymemroy_t *memory, size_t size);
void mymemory_free(mymemroy_t *memory, void *prt);
void mymemory_display(mymemroy_t *memory);




#endif