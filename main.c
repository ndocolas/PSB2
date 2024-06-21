#include "mymemory.h"
#include <stdio.h>

int main()
{
    printf("Iniciando...\n");
    size_t tamanho = 500;
    mymemory_t *m = mymemory_init(tamanho);

    if(m == NULL)
    {
        printf("Falhou!\n ");
        return 1;
    }

    printf("Sucesso!\n");
    void *alloc1 = mymemory_alloc(m, 100);
    void *alloc2 = mymemory_alloc(m, 200);
    void *alloc3 = mymemory_alloc(m, 50);
    printf("Alocou com sucesso!\n");

    mymemory_free(m, alloc2);
    printf("Limpou com sucesso\n");
    return 0;
}