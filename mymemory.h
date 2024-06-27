#ifndef MYMEMORY_H
#define MYMEMORY_H

#include <stddef.h>

typedef struct allocation //nodo
{
    void *start; //ponteiro para o inicio da alocacao
    size_t size; //tamanho da alocacao
    struct allocation *next; //ponteiro para a proxima alocacao. caso null, nao tem proximo
} allocation_t; 

typedef struct //lista
{
    void *pool; //ponteiro para o bloco de memoria real
    size_t total_size; //tamanho total da lista
    allocation_t *head; //inicio da lista. aponta para o HEAD
} mymemory_t;

mymemory_t* mymemory_init(size_t size);

void* mymemory_alloc(mymemory_t *memory, size_t size);
void mymemory_free(mymemory_t *memory, void *part);
void mymemory_display(mymemory_t *memory);
void mymemory_cleanup(mymemory_t *memory);
void mymemory_stats(mymemory_t *memory);

#endif