#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void sel_key(uint32_t key[4]);
void sel_key(uint32_t key[4]){
    char buffer[20];
    int c;
    for (size_t i = 0; i < 4; i++){
        size_t pos = 0;
        
        printf("Enter new key part %zu (hex):\n ", i+1);
        fflush(stdout);
        
        // Limpiar el buffer
        memset(buffer, 0, sizeof(buffer));
        
        // Leer caracteres hasta Enter o fin de buffer
        while (pos < sizeof(buffer) - 1) {
            c = getchar();
            if (c == '\n' || c == EOF)
                break;
            
            buffer[pos++] = c;
            // Imprimir el carácter para mostrar feedback
            putchar(c);
            fflush(stdout);
        }
        buffer[pos] = '\0';
        
        // Convertir a valor hexadecimal
        key[i] = strtoul(buffer, NULL, 8);
    }
}