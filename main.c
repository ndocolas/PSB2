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
    mymemory_display(m);
    void *alloc1 = mymemory_alloc(m, 100);
    void *alloc2 = mymemory_alloc(m, 200);
    void *alloc3 = mymemory_alloc(m, 50);
    mymemory_display(m);

    mymemory_free(m, alloc2);
    mymemory_display(m);
    return 0;
}