#include "mymemory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

mymemory_t *mymemory_init(size_t size)
{
    /*
    Tenta o alocar espaco na memoria
    Usa malloc para alocar a memoria
    */
    mymemory_t *memory = (mymemory_t *)malloc(sizeof(mymemory_t));

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

void* mymemory_alloc(mymemory_t *memory, size_t size)
{
    /*Corrige o alocar_size incluindo o alocar_size da struct
    Por conta dos ponteros e dos outros atributos contidos na struct 'allocation'
    faz-se esta correcao para adicionar todas as informacoes do nodo
    */
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
        caso o espaco entre as alocacoes seja maior do que o tamanho
        que se quer alocar, entao quebra o while e vai alocacao
    
        caso o espaco entre as alocacoes nao seja maior, entao altera os valores do iterador para passar
        para o proximo espaco
        */

        //adiciona o valor da alocacao para pular para o fim dela, recebendo o valor do inicio da proxima alocacao
        //que é o fim desta
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
    if(( (char *)atual_memoria - ( (char *) memory->pool + memory->total_size ) ) < alocar_size) return NULL;
    /*      retorna o lugar         _           retorna o fim da fila        |\   caso menor
           atual da iteracao                                                 |/  retorna null
      
         |---------------------*------|
                                ------  ==> tamanho até o fim
                                ----    ==> quanto quer iserir

                                é menor entao da para inserir. se fosse maior nao retornaria null

    */

    //define os valores da nova alocacao
    allocation_t *nodo = (allocation_t *)((char *)atual_memoria);

    nodo->start = (char *)atual_memoria + sizeof(allocation_t);
    nodo->size = size;
    nodo->next = atual;

    //apos definir os valores da alocacao inicia a insercao na lista

    //caso nao tenha anterior, ou seja, caso for a primeira insercao define como o head
    if(prev == NULL) memory->head = nodo; 
    else prev->next = nodo; //caso nao seja, insere no final do ponto da iteracao

    //retorna o ponteiro do inicio da novo nodo alocado
    return nodo->start;
}

void mymemory_free(mymemory_t *memory, void *part)
{
    if(part == NULL) return; //caso o valor recebido seja nulo, encerra o free

    //cria os atributos para iterar
    allocation_t *alloc = memory->head;
    allocation_t *prev = NULL;

    //inicia o iterador para tentar achar a parte enviada no parametro
    while(part != NULL)
    {
        if(alloc->start == part)//caso encontre o alloc do parametro
        {
            if(prev == NULL)
            {
                /*
                caso o prev seja null, o que significa que o nodo que quer remover é a head
                a head passa a apontar para o segundo nodo da lista, tornando-o o primeiro
                e removendo o desejado

                antes:          
                             ___________
                             |         |
                INICIO    REMOVER   SEGUNDO 
                  |          |
                  ------------

                depois:

                INICIO    REMOVER   SEGUNDO 
                  |                    |
                  ----------------------
                
                neste caso representei o INICIO como 'memory->head'
                */
                memory->head = alloc->next;
            }
            else
            {
                /*
                caso o prev nao seja null, significa que o nodo que se deseja remover é um nodo do meio
                basta apontar o NEXT do nodo anterior para o next do nodo que se deseja remover, tirando-o da lista

                antes:
                           ___________
                           |         |
                PREV    REMOVER     NEXT
                 |         |
                 -----------

                 depois:
                
                PREV    REMOVER     NEXT
                 |                   |
                 ---------------------
                */
                prev->next = alloc->next;
            }
            break;
        }
        //pula para a proxima iteracao, (significa que alloc->start != part)
        prev = alloc;
        alloc = alloc->next;
    }
}

void mymemory_display(mymemory_t *memory)
{
    if (memory->head == NULL)
    {
        printf("Nenhuma alocação salva\n\n");
        return;
    }
    
    printf("Alocações atuais:\n");
    allocation_t *nodo = memory->head;
    int count = 0;

    while (nodo != NULL)
    {
        printf("\nAlocação %d:\n", ++count);
        
        //recebe o valor dos ultimos 3 bits do endereco e transforma de hexadecimal para inteiro
        printf("  Início: %03lu\n", ((unsigned long)nodo->start)%1000);
        printf("  Tamanho: %zu bytes\n", sizeof(allocation_t) + nodo->size);
        nodo = nodo->next;
    }

    printf("\nNúmero total de alocações: %d\n\n", count);
}