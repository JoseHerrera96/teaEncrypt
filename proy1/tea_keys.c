#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void sel_key(uint32_t key[4]);
void sel_key(uint32_t key[4]){
    for (size_t i = 0; i < 4; i++)
    {
        printf("Enter new key: ");
        fflush(stdout);
        scanf("%x", &key[i]);
    }
}