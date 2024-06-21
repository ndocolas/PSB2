#include "mymemory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

mymemroy_t *mymemory_init(size_t size)
{
    //Tenta o alocar espaco na memoria
    //Usa malloc para alocar a memoria
    mymemroy_t *memory = (mymemroy_t *)malloc(sizeof(mymemroy_t));

    if(memory == NULL) return NULL;//Se a memoria nao tiver sido alocada, retorna null

    //Caso a memoria tiver sido devidaemnte aloca
    //aloca o pool de memoria usando o malloc
    memory->pool = malloc(size);
    if(memory->pool == NULL) //caso o pool nao tiver sido alocado
    {
        free(memory);
        return NULL; //retorna nulo caso nao tiver alocado com exito
    }

    //caso tenha alocado corretaente o pool
    memory->total_size = size; //define o tamanho do pool
    memory->head = NULL; //define o inicio da lista como nulo pois nada deve ter sido alocado ate entao

    //alocou tudo com exito e retorna o ponteiro para o programa
    return memory;    
}