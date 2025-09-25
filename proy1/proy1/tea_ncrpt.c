#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern void tea_encrypt(uint32_t v[2], uint32_t key[4]);

void tea_encrypt_handler(unsigned char *pad_chain, unsigned char *encr_chain, size_t pad_len, uint32_t key[4]);
void tea_encrypt_handler(unsigned char *pad_chain, unsigned char *encr_chain, size_t pad_len, uint32_t key[4])
{
    uint32_t v[2];

    for(size_t offset = 0; offset < pad_len; offset += 8){
        memcpy(&v[0], pad_chain + offset, 4);
        memcpy(&v[1], pad_chain + offset + 4, 4);
        
        tea_encrypt(v, key);

        memcpy(encr_chain + offset, &v[0], 4);
        memcpy(encr_chain + offset + 4, &v[1], 4);
    }
}
