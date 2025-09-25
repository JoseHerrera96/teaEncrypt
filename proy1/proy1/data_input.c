#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned char* data_input(void);
unsigned char* data_input(void)
{
    unsigned char input_chain[255];
    unsigned char *temp_chain;
    int spc_count = 0;
    int msg_len = 0;

    printf("Input data to cipher:\n");
    fgets(input_chain, sizeof(input_chain) , stdin);

    for (size_t i = 0; input_chain[i] != '\0'; i++)
    {
        if (input_chain[i] == ' ') {
            spc_count++;
            if (spc_count == 2) {
                msg_len--;
                break;
            }            
        }
        else {spc_count = 0;}
        msg_len++;
    }
    
    temp_chain = malloc(msg_len+1);
    memcpy(temp_chain, input_chain, msg_len);
    temp_chain[msg_len] = '\0';

    return temp_chain;
}