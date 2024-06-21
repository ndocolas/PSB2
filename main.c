#include "mymemory.h"
#include <stdio.h>

int main()
{
    printf("Iniciando...\n");
    size_t tamanho = 500;
    mymemory_t *memory = mymemory_init(tamanho);

    if(memory == NULL)
    {
        printf("Falhou!\n ");
        return 1;
    }

    printf("Sucesso!\n");
    void *aloc1 = mymemory_alloc(memory, 100);
    void *aloc2 = mymemory_alloc(memory, 200);
    void *aloc3 = mymemory_alloc(memory, 50);
    printf("Alocou com sucesso!\n");
    return 0;
}