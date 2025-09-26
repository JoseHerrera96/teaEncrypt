#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libbase/uart.h>

void sel_key(uint32_t key[4]);
void sel_key(uint32_t key[4]){
    char buffer[20];
    int c;
    for (size_t i = 0; i < 4; i++){
        size_t pos = 0;
        
        printf("\nEnter new key part %zu (hex):\n ", i+1);
        fflush(stdout);
        
        // Limpiar el buffer
        memset(buffer, 0, sizeof(buffer));
        
        // Leer caracteres con manejo de backspace
        while (pos < sizeof(buffer) - 1) {
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
                        goto key_input_complete;
                    case 0x07:  // Bell - ignorar
                        break;
                    default:
                        buffer[pos++] = c;
                        putchar(c);  // Mostrar carácter
                        fflush(stdout);
                        break;
                }
            }
        }
        
key_input_complete:
        buffer[pos] = '\0';
        
        // Convertir a valor hexadecimal
        key[i] = strtoul(buffer, NULL, 16);
    }
}