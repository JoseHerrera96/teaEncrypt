PACKAGES=libc libcompiler_rt libbase libfatfs liblitespi liblitedram libliteeth liblitesdcard liblitesata bios
PACKAGE_DIRS=/root/litex/litex/litex/soc/software/libc /root/litex/litex/litex/soc/software/libcompiler_rt /root/litex/litex/litex/soc/software/libbase /root/litex/litex/litex/soc/software/libfatfs /root/litex/litex/litex/soc/software/liblitespi /root/litex/litex/litex/soc/software/liblitedram /root/litex/litex/litex/soc/software/libliteeth /root/litex/litex/litex/soc/software/liblitesdcard /root/litex/litex/litex/soc/software/liblitesata /root/litex/litex/litex/soc/software/bios
LIBS=libc libcompiler_rt libbase libfatfs liblitespi liblitedram libliteeth liblitesdcard liblitesata
TRIPLE=riscv64-linux-gnu
CPU=vexriscv
CPUFAMILY=riscv
CPUFLAGS=-march=rv32i2p0_m     -mabi=ilp32 -D__vexriscv__
CPUENDIANNESS=little
CLANG=0
CPU_DIRECTORY=/root/litex/litex/litex/soc/cores/cpu/vexriscv
SOC_DIRECTORY=/root/litex/litex/litex/soc
PICOLIBC_DIRECTORY=/root/litex/pythondata-software-picolibc/pythondata_software_picolibc/data
PICOLIBC_FORMAT=integer
COMPILER_RT_DIRECTORY=/root/litex/pythondata-software-compiler_rt/pythondata_software_compiler_rt/data
export BUILDINC_DIRECTORY
BUILDINC_DIRECTORY=/workspace/build/sim/software/include
LIBC_DIRECTORY=/root/litex/litex/litex/soc/software/libc
LIBCOMPILER_RT_DIRECTORY=/root/litex/litex/litex/soc/software/libcompiler_rt
LIBBASE_DIRECTORY=/root/litex/litex/litex/soc/software/libbase
LIBFATFS_DIRECTORY=/root/litex/litex/litex/soc/software/libfatfs
LIBLITESPI_DIRECTORY=/root/litex/litex/litex/soc/software/liblitespi
LIBLITEDRAM_DIRECTORY=/root/litex/litex/litex/soc/software/liblitedram
LIBLITEETH_DIRECTORY=/root/litex/litex/litex/soc/software/libliteeth
LIBLITESDCARD_DIRECTORY=/root/litex/litex/litex/soc/software/liblitesdcard
LIBLITESATA_DIRECTORY=/root/litex/litex/litex/soc/software/liblitesata
BIOS_DIRECTORY=/root/litex/litex/litex/soc/software/bios
LTO=0
BIOS_CONSOLE_FULL=1