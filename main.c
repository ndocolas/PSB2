#include "mymemory.h"
#include <stdio.h>

int main()
{
    size_t tamanho = 100;
    mymemory_t *m = mymemory_init(tamanho);

    if(m == NULL)
    {
        printf("Falhou!\n ");
        return 1;
    }

    void *alloc1 = mymemory_alloc(m, 5);
    void *alloc2 = mymemory_alloc(m, 5);
    void *alloc3 = mymemory_alloc(m, 5);
    mymemory_stats(m);

    mymemory_free(m, alloc1);
    printf("\n\n\n--==--=--=-=Depois-==-=-=-=-==-\n\n\n");
    void *alloc4 = mymemory_alloc(m, 2);
    printf("\n");
    mymemory_stats(m);

    mymemory_cleanup(m);
    return 0;
}