// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <irq.h>
#include <libbase/uart.h>
#include <libbase/console.h>
#include <generated/csr.h>

/*-----------------------------------------------------------------------*/
/* Uart                                                                  */
/*-----------------------------------------------------------------------*/

static char *readstr(void)
{
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = getchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					fputs("\x08 \x08", stdout);
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				fputs("\n", stdout);
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				fputs(c, stdout);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}

	return NULL;
}

static char *get_token(char **str)
{
	char *c, *d;

	c = (char *)strchr(*str, ' ');
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

static void prompt(void)
{
	printf("\e[92;1mproy1\e[0m> ");
}

/*-----------------------------------------------------------------------*/
/* Help                                                                  */
/*-----------------------------------------------------------------------*/

static void help(void)
{
	puts("\nLiteX minimal - Proyecto 1 "__DATE__" "__TIME__"\n");
	puts("Available commands:");
	puts("help               - Show this command");
	puts("reboot             - Reboot CPU");
	puts("helloc             - Hello C");
	puts("add                - Adds two integer values");
	puts("encrypt            - Proceso completo de encriptacion (data->padding->key->encrypt)");
	puts("decrypt            - Proceso completo de desencriptacion (data->key->decrypt->unpadding)");
}

/*-----------------------------------------------------------------------*/
/* Commands                                                              */
/*-----------------------------------------------------------------------*/

static void reboot_cmd(void)
{
	ctrl_reset_write(1);
}

extern void helloc(void);

static void helloc_cmd(void)
{
	printf("Hello C demo...\n");
	helloc();
}

extern int add(int a, int b);
static void add_cmd(void)
{
	int a = 2;
	int b = 3;
	printf("Adding %d + %d is = %d \r\n", a, b, add(a,b));
}

//
static uint32_t key[4];
static unsigned char *chain;
static unsigned char *pad_chain;
static size_t block_size;
static size_t pad_len;
static size_t unpad_len;
static unsigned char *encr_chain;
static unsigned char *decr_chain;

static void data_init(void)
{
	key[0] = 0x12345678;
	key[1] = 0x9ABCDEF0;
	key[2] = 0xFEDCBA98;
	key[3] = 0x86543210;

	unsigned char temp_chain[] = "HOLA1234";
	chain = malloc(sizeof(temp_chain));
	memcpy(chain, temp_chain, sizeof(temp_chain));
	//chain = (unsigned char *)strdup(temp_chain);
	
	block_size = 8;
}

extern unsigned char* data_input(void);
static void data_cmd(void)
{
	if (chain) {free(chain);}
	chain = data_input();
}

extern void sel_key(uint32_t key[4]);
static void key_cmd(void)
{
	printf("%s %08X %08X %08X %08X\n", "Current Key: ", key[3], key[2], key[1], key[0]);
	sel_key(key);
	printf("%s %08X %08X %08X %08X\n", "New Key: ", key[3], key[2], key[1], key[0]);
}

extern size_t add_pkcs7(unsigned char *chain, size_t chain_len, size_t block_size, unsigned char **pad_chain);
static void add_pad_cmd(void)
{
	pad_len = add_pkcs7(chain, strlen((char*)chain), block_size, &pad_chain);
    printf("%s\n", chain);
    printf("Padding:\n");
    for (size_t i = 0; i < pad_len; i++) {
        printf("%02X ", pad_chain[i]);
        if ((i+1) % 8 == 0){printf("\n");}
    }
    printf("\n");
	
}

extern size_t del_pkcs7(unsigned char *pad_chain, size_t pad_len, size_t block_size);
static void del_pad_cmd(void)
{
	unpad_len = del_pkcs7(decr_chain, pad_len, block_size);
    decr_chain[unpad_len] = '\0';
    printf("Padding deleted: %s", decr_chain);
	free(pad_chain);
	free(chain);
	free(encr_chain);
	free(decr_chain);
}

extern void tea_encrypt_handler(unsigned char *pad_chain, unsigned char *encr_chain, size_t pad_len, uint32_t key[4]);
static void encrypt_cmd(void)
{
	// Flujo completo: data->padding->key->encryptation->mostrar resultado
	printf("=== PROCESO DE ENCRIPTACION ===\n");
	
	// 1. Obtener datos
	printf("1. Ingrese los datos a cifrar:\n");
	if (chain) {free(chain);}
	chain = data_input();
	printf("Datos ingresados: %s\n", chain);
	
	// 2. Aplicar padding
	printf("\n2. Aplicando padding PKCS#7...\n");
	if (pad_chain) {free(pad_chain);}
	pad_len = add_pkcs7(chain, strlen((char*)chain), block_size, &pad_chain);
	printf("Datos con padding:\n");
	for (size_t i = 0; i < pad_len; i++) {
		printf("%02X ", pad_chain[i]);
		if ((i+1) % 8 == 0){printf("\n");}
	}
	printf("\n");
	
	// 3. Configurar clave
	printf("\n3. Configuracion de clave:\n");
	printf("Clave actual: %08X %08X %08X %08X\n", key[3], key[2], key[1], key[0]);
	printf("¿Desea cambiar la clave? (y/n): ");
	fflush(stdout);
	char response = getchar();
	if (response == 'y' || response == 'Y') {
		sel_key(key);
		printf("Nueva clave: %08X %08X %08X %08X\n", key[3], key[2], key[1], key[0]);
	}
	
	// 4. Encriptar
	printf("\n4. Encriptando datos...\n");
	if (encr_chain) {free(encr_chain);}
	encr_chain = malloc(pad_len);
	tea_encrypt_handler(pad_chain, encr_chain, pad_len, key);
	
	// 5. Mostrar resultado
	printf("\n=== RESULTADO DE ENCRIPTACION ===\n");
	printf("Datos cifrados (hex):\n");
	for (size_t i = 0; i < pad_len; i++) {
		printf("%02X ", encr_chain[i]);
		if ((i+1) % 8 == 0){printf("\n");}
	}
	printf("\nEncriptacion completada exitosamente!\n");
}

extern void tea_decrypt_handler(unsigned char *encr_chain, unsigned char *decr_chain, size_t pad_len, uint32_t key[4]);
static void decrypt_cmd(void)
{
	// Flujo completo: data->key->decryptation->unpadding->mostrar resultado
	printf("=== PROCESO DE DESENCRIPTACION ===\n");
	
	// Verificar que hay datos encriptados
	if (!encr_chain) {
		printf("Error: No hay datos encriptados disponibles.\n");
		printf("Primero debe ejecutar el comando 'encrypt'.\n");
		return;
	}
	
	// 1. Mostrar datos encriptados
	printf("1. Datos encriptados a descifrar:\n");
	for (size_t i = 0; i < pad_len; i++) {
		printf("%02X ", encr_chain[i]);
		if ((i+1) % 8 == 0){printf("\n");}
	}
	printf("\n");
	
	// 2. Configurar clave
	printf("\n2. Configuracion de clave:\n");
	printf("Clave actual: %08X %08X %08X %08X\n", key[3], key[2], key[1], key[0]);
	printf("¿Desea cambiar la clave? (y/n): ");
	fflush(stdout);
	char response = getchar();
	if (response == 'y' || response == 'Y') {
		sel_key(key);
		printf("Nueva clave: %08X %08X %08X %08X\n", key[3], key[2], key[1], key[0]);
	}
	
	// 3. Desencriptar
	printf("\n3. Desencriptando datos...\n");
	if (decr_chain) {free(decr_chain);}
	decr_chain = malloc(pad_len);
	tea_decrypt_handler(encr_chain, decr_chain, pad_len, key);
	
	// 4. Remover padding
	printf("\n4. Removiendo padding PKCS#7...\n");
	unpad_len = del_pkcs7(decr_chain, pad_len, block_size);
	decr_chain[unpad_len] = '\0';
	
	// 5. Mostrar resultado
	printf("\n=== RESULTADO DE DESENCRIPTACION ===\n");
	printf("Datos descifrados: %s\n", decr_chain);
	printf("Desencriptacion completada exitosamente!\n");
}

//

/*-----------------------------------------------------------------------*/
/* Console service / Main                                                */
/*-----------------------------------------------------------------------*/

static void console_service(void)
{
	char *str;
	char *token;

	str = readstr();
	if(str == NULL) return;

	token = get_token(&str);

	if(strcmp(token, "help") == 0)
		help();
	else if(strcmp(token, "reboot") == 0)
		reboot_cmd();
	else if(strcmp(token, "helloc") == 0)
		helloc_cmd();
	else if(strcmp(token, "add") == 0)
		add_cmd();
	else if(strcmp(token, "encrypt") == 0)
		encrypt_cmd();
	else if(strcmp(token, "decrypt") == 0)
		decrypt_cmd();

	prompt();
}

int main(void)
{
#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
#endif
	uart_init();
	
	data_init();

	help();
	prompt();

	while(1) {
		console_service();
	}

	return 0;
}
