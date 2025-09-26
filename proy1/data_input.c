#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libbase/uart.h>

unsigned char* data_input(void);
unsigned char* data_input(void)
{
    unsigned char input_chain[255];
    unsigned char *temp_chain;
    int spc_count = 0;
    int msg_len = 0;
    
    printf("\nInput data to cipher:\n ");
    fflush(stdout);
    
    // Leer caracteres con manejo de backspace
    int c;
    size_t pos = 0;
    
    while (pos < sizeof(input_chain) - 1) {
        if(readchar_nonblock()) {
            c = getchar();
            
            switch(c) {
                case 0x7f:  // DEL
                case 0x08:  // Backspace
                    if(pos > 0) {
                        pos--;
                        fputs("\x08 \x08", stdout);  // Borrar carácter en pantalla
                        fflush(stdout);
                    }
                    break;
                case '\r':
                case '\n':
                    goto input_complete;
                case 0x07:  // Bell - ignorar
                    break;
                default:
                    input_chain[pos++] = c;
                    putchar(c);  // Mostrar carácter
                    fflush(stdout);
                    break;
            }
        }
    }
    
input_complete:
    input_chain[pos] = '\0';
    
    // Calcular longitud del mensaje
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