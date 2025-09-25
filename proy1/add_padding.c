#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

size_t add_pkcs7(unsigned char *chain, size_t chain_len, size_t block_size, unsigned char **pad_chain);
size_t add_pkcs7(unsigned char *chain, size_t chain_len, size_t block_size, unsigned char **pad_chain){
    size_t pad_len = block_size - (chain_len % block_size);
    if (pad_len == 8) {
        *pad_chain = malloc(chain_len);
        memcpy(*pad_chain, chain, chain_len);
        return chain_len;
    }
    else {
        *pad_chain = malloc(chain_len + pad_len);
        memcpy(*pad_chain, chain, chain_len);
        memset(*pad_chain + chain_len, pad_len, pad_len);
        return chain_len + pad_len;
    }
}