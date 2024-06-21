#include "mymemory.h"
#include <stdio.h>

int main()
{
    printf("Iniciando...\n");
    size_t tamanho = 500;
    mymemroy_t *memory = mymemory_init(tamanho);

    if(memory == NULL)
    {
        printf("Falhou!\n ");
        return 1;
    }

    printf("Sucesso!\n");
    return 0;
}