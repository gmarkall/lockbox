CXX = riscv32-unknown-elf-g++

CXXFLAGS = -c -O2 -march=rv32imac -fpeel-loops -ffreestanding \
	   -ffunction-sections -fdata-sections -Wall -Werror -Wextra \
	   -fno-rtti -fno-exceptions -g -include sys/cdefs.h

LDFLAGS = -T hifive1.ld -nostartfiles -Wl,-N -Wl,--gc-sections

all: lockbox.exe

lockbox.exe: lockbox.o

lockbox.o: lockbox.cpp
	${CXX} ${CXXFLAGS} lockbox.cpp
