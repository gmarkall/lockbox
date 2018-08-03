CC = riscv32-unknown-elf-gcc
CXX = riscv32-unknown-elf-g++
LD = riscv32-unknown-elf-g++
OPENOCD = /home/sarah/SECURE/freedom-e-sdk/work/build/openocd/prefix/bin/openocd
GDB = /home/sarah/SECURE/freedom-e-sdk/work/build/riscv-gnu-toolchain/riscv64-unknown-elf/prefix/bin/riscv64-unknown-elf-gdb

CPPFLAGS = -Iinclude -DF_CPU=16000000LL -DFREEDOM_E300_HIFIVE1

CCFLAGS = -c -O2 -march=rv32imac -fpeel-loops -ffreestanding -g \
          -ffunction-sections -fdata-sections -Wall -Werror -Wextra

CXXFLAGS = ${CCFLAGS} -fno-rtti -fno-exceptions

LDFLAGS = -T hifive1.lds -nostartfiles -Wl,-N -Wl,--gc-sections -nostdlib -Wl,--wrap=malloc -Wl,--wrap=free -Wl,--wrap=sbrk

OPENOCD_ARGS = -f /home/sarah/SECURE/freedom-e-sdk/bsp/env/freedom-e300-hifive1/openocd.cfg

all: lockbox.exe


clean:
	rm -f *.o lockbox.exe

%.o: %.c
	${CC} ${CPPFLAGS} ${CCFLAGS} $<

%.o: %.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} $<

lockbox.exe: lockbox.o LiquidCrystal.o core.a
	${LD} ${LDFLAGS} lockbox.o LiquidCrystal.o -Wl,--start-group core.a -lm -lstdc++ -lc -lgloss -Wl,--end-group -lgcc -o lockbox.exe

upload: lockbox.exe
	$(OPENOCD) $(OPENOCD_ARGS) & \
            $(GDB) lockbox.exe --batch -ex "set remotetimeout 240" -ex "target extended-remote localhost:3333" -ex "monitor reset halt" -ex "monitor flash protect 0 64 last off" -ex "load" -ex "monitor resume" -ex "monitor shutdown" -ex "quit" && echo Done
