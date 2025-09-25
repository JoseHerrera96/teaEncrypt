# TEA test para RARS
# Data: bloque v[0],v[1] y key[0..3]
.data
v:      .word 0x01234567, 0x89ABCDEF       # plaintext (v0, v1)
key:    .word 0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F

.text
.globl main
main:
    # cargar direcciones en a0, a1 (punteros a memoria)
    la    a0, v         # a0 -> &v
    la    a1, key       # a1 -> &key

    # llamamos a la función de cifrado
    call  tea_encrypt

    # aquí el bloque v[0], v[1] ya está cifrado en memoria
    # entra en bucle infinito para inspección en RARS

# -----------------------------------------
# tea_encrypt: implementacion (compatible RARS)
# usa registros t0..t6, a0..a6; prologue/epilogue simples
# -----------------------------------------
.globl tea_encrypt
tea_encrypt:
    addi  sp, sp, -16
    sw    ra, 12(sp)

    # cargar v0, v1 desde memoria usando a0 (puntero)
    lw    t0, 0(a0)        # t0 = v0
    lw    t1, 4(a0)        # t1 = v1

    # cargar clave desde memoria usando a1 (puntero)
    lw    t2, 0(a1)        # k0
    lw    t3, 4(a1)        # k1
    lw    t4, 8(a1)        # k2
    lw    t5, 12(a1)       # k3

    li    t6, 0x9E3779B9   # delta
    li    a2, 0            # sum = 0
    li    a3, 32           # rondas = 32

encrypt_loop:
    add   a2, a2, t6       # sum += delta

    # tmp = ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1)
    slli  a4, t1, 4
    add   a4, a4, t2
    add   a5, t1, a2
    srli  a6, t1, 5
    add   a6, a6, t3
    xor   a4, a4, a5
    xor   a4, a4, a6
    add   t0, t0, a4       # v0 += tmp

    # tmp2 = ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3)
    slli  a4, t0, 4
    add   a4, a4, t4
    add   a5, t0, a2
    srli  a6, t0, 5
    add   a6, a6, t5
    xor   a4, a4, a5
    xor   a4, a4, a6
    add   t1, t1, a4       # v1 += tmp2

    addi  a3, a3, -1
    bne   a3, zero, encrypt_loop

    # escribir resultado de vuelta en memoria (a0 apunta a v)
    sw    t0, 0(a0)
    sw    t1, 4(a0)

    lw    ra, 12(sp)
    addi  sp, sp, 16
