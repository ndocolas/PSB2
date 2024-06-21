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

    void *alloc1 = mymemory_alloc(m, 1);
    void *alloc2 = mymemory_alloc(m, 1);
    void *alloc3 = mymemory_alloc(m, 1);
    void *alloc4 = mymemory_alloc(m, 10);
    void *alloc5 = mymemory_alloc(m, 1);

    mymemory_free(m, alloc4);

    void *alloc6 = mymemory_alloc(m, 5);

    mymemory_display(m);
    return 0;
}