#include "mymemory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

mymemroy_t *mymemory_init(size_t size)
{
    /*
    Tenta o alocar espaco na memoria
    Usa malloc para alocar a memoria
    */
    mymemroy_t *memory = (mymemroy_t *)malloc(sizeof(mymemroy_t));

    if(memory == NULL) return NULL;//Se a memoria nao tiver sido alocada, retorna null

    /*
    Caso a memoria tiver sido devidaemnte aloca
    aloca o pool de memoria usando o malloc
    */
    memory->pool = malloc(size);
    if(memory->pool == NULL) //caso o pool nao tiver sido alocado
    {
        free(memory);
        return NULL; //retorna nulo caso nao tiver alocado com exito
    }

    //caso tenha alocado corretaente o pool
    memory->total_size = size; //define o alocar_size do pool
    memory->head = NULL; //define o inicio da lista como nulo pois nada deve ter sido alocado ate entao

    //alocou tudo com exito e retorna o ponteiro para o programa
    return memory;    
}

void* mymemory_alloc(mymemroy_t *memory, size_t size)
{
    //Corrige o alocar_size incluindo o alocar_size da struct
    size_t alocar_size = size + sizeof(allocation_t);

    //variaveis para iterar sobre a lista
    allocation_t *atual = memory->head;
    allocation_t *prev = NULL;

    void *atual_memoria = memory->pool;
    while (atual != NULL ) //inicia a iteracao da lista
    {
        //verifica a diferenca entre os espacos para ver se tem disponibilidade para inserir
        if (((char *) atual->start - (char *)atual_memoria) > alocar_size) break;
        /*
        caso o if retorne true, ou seja, caso o espaco na memoria entre as alocacoes
        seja maior do que o alocar_size que se quer alocar, entao quebra o while e pula alocacao
    
        caso o espaco entre as alocacoes nao seja maior, entao altera os valores do iterador para passar
        para o proximo espaco
        
        adiciona o valor da alocacao para pular para o fim dela, recebendo o valor do inicio da proxima alocacao
        que é o fim desta
        */
        atual_memoria = (char *)atual->start + atual->size;
        
        //preserva a alocacao atual para a proxima comparacao
        prev = atual;
        //incrementa para a proxima atual
        atual = atual->next;
    }

    /*
    ao sair do while, verifica overflow. caso a iteracao tenha ido ate o fim
    e nao tenha encontrado nenhum espaco disponivel, verifica caso o fim tenha espaco disponivel
    caso nao tenha retorna null
    */
    if((char *) memory->pool + memory->total_size - (char *)atual_memoria < alocar_size)
    {
        printf("Nao há espaço para alocar.\n");
        return NULL;//retorna nulo caso nao tenha espaco no fim
    }

    //define os valores da nova alocacao
    allocation_t *nodo = (allocation_t *)((char *)atual_memoria);

    nodo->start = (char *)atual_memoria + sizeof(allocation_t);
    nodo->size = size;
    nodo->next = atual;

    //apos definir os valores da alocacao inicia a insercao na lista
    if(prev == NULL) memory->head = nodo; //caso nao tenha anterior, ou seja, caso for a primeira insercao define como head
    else prev->next = nodo; //caso nao seja, insere no final do ponto de iteracao

    //retorna o ponteiro do inicio da novo nodo alocado
    return nodo->start;
}