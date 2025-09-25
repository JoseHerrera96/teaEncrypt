#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern void tea_decrypt(uint32_t v[2], uint32_t key[4]);

void tea_decrypt_handler(unsigned char *encr_chain, unsigned char *decr_chain, size_t pad_len, uint32_t key[4]);
void tea_decrypt_handler(unsigned char *encr_chain, unsigned char *decr_chain, size_t pad_len, uint32_t key[4])
{
    uint32_t v[2];

    for(size_t offset = 0; offset < pad_len; offset += 8){
        memcpy(&v[0], encr_chain + offset, 4);
        memcpy(&v[1], encr_chain + offset + 4, 4);
        
        tea_decrypt(v, key);

        memcpy(decr_chain + offset, &v[0], 4);
        memcpy(decr_chain + offset + 4, &v[1], 4);
    }
}