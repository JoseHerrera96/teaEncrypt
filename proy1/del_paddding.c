#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

size_t del_pkcs7(unsigned char *pad_chain, size_t pad_len, size_t block_size);
size_t del_pkcs7(unsigned char *pad_chain, size_t pad_len, size_t block_size){
    unsigned char sub_len = pad_chain[pad_len - 1];
    return pad_len - sub_len;
}
