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

    if(memory == NULL) return NULL;//Se a memoria nao tiver sido alocada, retorna NULL

    /*
    Caso a memoria tiver sido devidaemnte alocada
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
    memory->head = NULL; //define o inicio da lista como nulo pois a lista inicia vazia

    //alocou tudo com exito e retorna o ponteiro da lista para o programa
    return memory;    
}

void* mymemory_alloc(mymemory_t *memory, size_t size)
{
    /*Corrige o alocar_size incluindo os valores da struct
    Por conta dos ponteros e dos outros atributos contidos na struct 'allocation'
    faz-se esta correcao para adicionar todas as informacoes do nodo
    */
    size_t alocar_size = size + sizeof(allocation_t);

    //variaveis para iterar sobre a lista
    allocation_t *atual = memory->head;
    allocation_t *prev = NULL;

    void *atual_memoria = memory->pool;
    while (atual != NULL) //inicia a iteracao da lista
    {
        //verifica a diferenca entre os espacos para ver se tem disponibilidade para inserir
        if (((char *) atual->start - (char *)atual_memoria) >= alocar_size) break;
        
        /*
        1 - atual
        2 - atual_memoria                

        INICIO |------------1---------2------| FIM
                             |-----|           =====> quanto quer inserir
                
                Visualmente demonstrando, percebe-se que é possivel inserir. Caso isso ocorra,
                o if é valido e quebra o while seguindo para a alocação
    
        caso o espaco entre as alocacoes nao seja maior, entao altera os valores do iterador para passar
        para o proximo espaco
        

        adiciona o valor da nodo para pular para o fim dela, recebendo o valor do inicio da proxima nodo
        que é o fim desta */
        atual_memoria = (char *)atual->start + atual->size;
        //preserva a nodo atual para a proxima comparacao
        prev = atual;
        //incrementa para a proxima atual
        atual = atual->next;
    }

    /*
    ao sair do while, verifica overflow. caso a iteracao tenha ido ate o fim
    e nao tenha encontrado nenhum espaco disponivel, verifica caso o fim tenha espaco disponivel
    caso nao tenha retorna NULL
    */
    if(((char *)atual_memoria - ( (char *) memory->pool + memory->total_size ) ) < alocar_size) {printf("Overflow\n\n"); return NULL;}
    /*      retorna o lugar         _           retorna o fim da memoria     |\   caso menor
           atual da iteracao                                                 |/  retorna NULL
      
         |---------------------*------|
                                ------  ==> tamanho até o fim
                                ----    ==> quanto quer iserir

                                é menor entao da para inserir. se fosse maior nao retornaria NULL

    */
    //define os valores da nova nodo
    allocation_t *nodo = (allocation_t *)((char *)atual_memoria);

    nodo->start = (char *)atual_memoria + sizeof(allocation_t);
    nodo->size = size;
    nodo->next = atual;
    //apos definir os valores da nodo inicia a insercao na lista

    //caso nao tenha anterior, ou seja, caso for a primeira insercao define como o head
    if(prev == NULL) memory->head = nodo; 
    else prev->next = nodo; //caso nao seja, insere no final do ponto da iteracao

    //retorna o ponteiro do inicio da novo nodo alocado
    printf("Memoria de tamanho %zu alocada com sucesso\n", size + sizeof(allocation_t));
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
            if(prev == NULL) memory->head = alloc->next;
                /*
                caso o prev seja NULL, o que significa que o nodo que quer remover é a head.

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
            
            else prev->next = alloc->next;
                /*
                caso o prev nao seja NULL, significa que o nodo que se deseja remover é um nodo do meio.

                basta apontar o NEXT do nodo anterior para o next do nodo que se deseja excluir, removendo-o da lista

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
            break;
        }
        //pula para a proxima iteracao, (significa que alloc->start != part)
        prev = alloc;
        alloc = alloc->next;
    }
}

void mymemory_cleanup(mymemory_t *memory)
{
    free(memory->pool);
    memory->pool = NULL;
    memory->head = NULL;
    memory->total_size = 0;

    free(memory);
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
        printf("  Tamanho: %zu\n", nodo->size);
        printf("  Tamanho total: %zu (contando outros atributos da struct)\n", sizeof(allocation_t) + nodo->size);

        nodo = nodo->next;
    }

    printf("\nNúmero total de alocações: %d\n\n", count);
}

void mymemory_stats(mymemory_t *memory)//meu
{
    size_t qtd_alocacoes = 0,
    memoria_alocada = 0,
    memoria_disponivel = memory->total_size, 
    maior_fragmento = 0,
    fragmentos = 0;

    allocation_t *nodo = memory->head;
    void *atual_memoria = memory->pool;

    //caso nao tenha nenhum nodo a memoria inteira esta livre, logo o maior bloco livre
    //é o proprio tamanho alocado da memoria
    if(nodo == NULL) maior_fragmento = memory->total_size;

    while(nodo != NULL)
    {
        qtd_alocacoes++;//acrescenta 1 para o numero de allocations
        memoria_alocada += nodo->size + sizeof(allocation_t);//acrescenta o tamanho da allocation para o tamanho total

        //verifica se ha espaco entre os nodos, no caso de um free ter acontecido 
        //e nenhuma nodo ter preenchido o espaco
        size_t memoria_livre = (size_t) ((char *)nodo->start - (char *)atual_memoria - sizeof(allocation_t));
        if(memoria_livre > 0)
        {
            //caso haja espaco livre acrescenta para o numero de fragmentos
            fragmentos++;
            //verifica se o fragmento atual é maior que o maior até agora lido
            //caso for altera
            if(memoria_livre > maior_fragmento) maior_fragmento = memoria_livre;
        }
        //retorna caso haja
        printf("Espaco livre antes do bloco %u: %u bytes\n", (unsigned) qtd_alocacoes, (unsigned) memoria_livre);

        //altera os valores para a proxima iteracao
        atual_memoria = (char*) nodo + sizeof(allocation_t) + nodo->size;
        nodo = nodo->next;
    }

    size_t memoria_final = (size_t)((char *)memory->pool + memory->total_size - (char *)atual_memoria);
    
    if(memoria_final > 0)
    {
        //acrescenta para o numero de fragmentos
        fragmentos++;
        //verifica se o final é o maior fragmento
        if(memoria_final > maior_fragmento) maior_fragmento = memoria_final;
        printf("Espaco livre no fim da pool: %u\n", (unsigned)memoria_final);
    }

    memoria_disponivel -= memoria_alocada;

    printf("\nStatus da memoria:\n");
    printf("Numero de alocacoes: %u\n", (unsigned)qtd_alocacoes);
    printf("Memoria alocada: %u\n", (unsigned)memoria_alocada);
    printf("Memoria livre: %u\n",(unsigned)memoria_disponivel);
    printf("Maior fragmento: %u\n", (unsigned)maior_fragmento);
    printf("Quantidade de fragmentos: %u\n", (unsigned)fragmentos);
}