.section .text
.global tea_decrypt
.type tea_decrypt, @function


# ------------------------------------------------------------------
# void tea_decrypt(uint32_t v[2], const uint32_t key[4])
# con prologue/epilogue igual
# ------------------------------------------------------------------
tea_decrypt:
    # prologue
    addi  sp, sp, -16
    sw    ra, 12(sp)

    # cargar v0, v1
    lw    t0, 0(a0)        # t0 = v0
    lw    t1, 4(a0)        # t1 = v1

    # cargar clave
    lw    t2, 0(a1)        # t2 = key[0]
    lw    t3, 4(a1)        # t3 = key[1]
    lw    t4, 8(a1)        # t4 = key[2]
    lw    t5, 12(a1)       # t5 = key[3]

    li    t6, 0x9E3779B9   # delta
    # sum = delta * 32 = 0xC6EF3720
    li    a2, 0xC6EF3720   # sum
    li    a3, 32           # i = 32

decrypt_loop:
    # v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3)
    slli   a4, t0, 4
    add   a4, a4, t4
    add   a5, t0, a2
    srli   a6, t0, 5
    add   a6, a6, t5
    xor   a4, a4, a5
    xor   a4, a4, a6
    sub   t1, t1, a4       # v1 -= tmp

    # v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1)
    slli   a4, t1, 4
    add   a4, a4, t2
    add   a5, t1, a2
    srli   a6, t1, 5
    add   a6, a6, t3
    xor   a4, a4, a5
    xor   a4, a4, a6
    sub   t0, t0, a4       # v0 -= tmp

    sub   a2, a2, t6       # sum -= delta
    addi  a3, a3, -1
    bne   a3, zero, decrypt_loop

    # almacenar resultado
    sw    t0, 0(a0)
    sw    t1, 4(a0)

    # epilogue
    lw    ra, 12(sp)
    addi  sp, sp, 16
    jalr x0, ra, 0

