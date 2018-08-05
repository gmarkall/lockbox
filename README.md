Lockbox + stack erase
=====================

Goals:

- To create an exploit for the lockbox application that steals the secret, K,
  from a location on the stack.
- To demonstrate that when the GCC Stack Erase feature is used when compiling
  the lockbox application, the exploit fails to steal the secret. (E.g.
  the exploit will still execute, but will read zeroes instead of the secret).


Lockbox application
-------------------

The lockbox application is representative of an embedded program which holds a
secret value that we wish to protect. Its operation is as follows:

- Upon reset, the device waits for a button to be pressed.
- Once the button is pressed, the device generates a secret, seeded using the
  time until the button press.
  - Note that a real application must have a better way of generating the
    secret, as the random() function from Arduino is not a CSPRNG.
  - It would also need a better way of seeding randomness (there is not much
    entopy in the time until a button press).
  - Nevertheless, this provides a sufficient level of randomness for
    demonstrating stack erase, in that the secret is not the same on every
    execution.
- The secret is displayed on the LCD screen. This is for our convenience, so we
  can see what the secret is for testing this unlock mechanism, or to compare
  with the secret stolen by the exploit.
- The application then prompts for the code over the serial port.
- Serial input is read in a loop.
- Code entries (terminated by a newline) are checked for correctness.
  - If the correct code is entered, "UNLOCKED" is displayed on the screen.
  - If the code is incorrect, the code is prompted-for again.

Vulnerability
-------------

The doSerial function has a buffer for holding characters received on the serial
port:

```
char recvBuf[9];
```

which can hold eight characters and a newline. Given that unlock codes are eight
digits, this provides enough space for normal operation. However, the loop that
places characters into this buffer will carry on doing so indefinitely, as long
as a newline is not received.

Therefore, if more than nine characters are received, the input will start
overwriting other locations on the stack. This (hopefully!) provides an
exploitable vulnerability, in that executable code can be stored into the buffer
and the program counter returned to the buffer location by overwriting the
return address.


Exploit requirements
--------------------

The code injected into the buffer should exfiltrate the secret from an old stack
frame. The function `correctInput` has been written such that the key is loaded
into its stack frame (compiled without optimisation, this should hopefully be
reflected in the lockbox binary). So, the exploit code needs to load the value
from this location and transmit it back to a program on the host that is
listening for the value. This could be done with a call to the Serial.print()
function.

Ideally the exploit would then return execution back to the beginning of the
doSerial() function, so that unlocking using the now-stolen secret can be
demonstrated. Alternatively, the device may just crash or end up otherwise
unresponsive after the exfiltration, which would be OK to demonstrate the stack
erase feature, but not quite as cool a demo.


Developing the exploit
======================

The following section outlines a plan for becoming familiar with the application
and outlines the issues that need to be solved to complete development of the
exploit.


Building and running the application
------------------------------------

The application uses a Makefile to build. Simply type:

```
make
```

to build the lockbox application.

To run on the HiFive1 board, use:

```
make upload
```

It will be helpful to debug the application, to understand what happens as
execution proceeds. To do this requires two terminals. In the first, launch
OpenOCD:

```
make run_openocd
```

In the second, you can launch GDB, the GNU debugger:

```
make run_gdb
```

Some useful commands:

```
tui enable
layout asm
layout src
next
step
nexti
stepi
info registers
info frame
print <expression>
```


Interacting with the application through the serial port
--------------------------------------------------------

Open up the Arduino Serial monitor (set it to newline) to see the prompt for
codes and to try entering codes. Experiment with entering the correct, and
incorrect code.

Eventually you will want to interact with the application over the serial port
programmatically - you should use whatever tool / language you feel comfortable
with for doing this (e.g. python and pyserial, etc.)

The port settings are (to be checked): 9600 baud, 8 bits, no parity, 1 stop bit.


Understanding the stack layout
------------------------------

This is crucial to developing the exploit. You need to work out the addresses of
at least `recvBuf` and `K_correct` on the stack. It will probably be helpful to
draw out what the stack looks like starting from the doSerial() function during
the execution of the correctInput() function. 

To help with this, there is a description of the general stack layout on RISC-V,
taken from the GCC source:

```
   RISC-V stack frames grown downward.  High addresses are at the top.

	+-------------------------------+
	|                               |
	|  incoming stack arguments     |
	|                               |
	+-------------------------------+ <-- incoming stack pointer
	|                               |
	|  callee-allocated save area   |
	|  for arguments that are       |
	|  split between registers and  |
	|  the stack                    |
	|                               |
	+-------------------------------+ <-- arg_pointer_rtx
	|                               |
	|  callee-allocated save area   |
	|  for register varargs         |
	|                               |
	+-------------------------------+ <-- hard_frame_pointer_rtx;
	|                               |     stack_pointer_rtx + gp_sp_offset
	|  GPR save area                |       + UNITS_PER_WORD
	|                               |
	+-------------------------------+ <-- stack_pointer_rtx + fp_sp_offset
	|                               |       + UNITS_PER_HWVALUE
	|  FPR save area                |
	|                               |
	+-------------------------------+ <-- frame_pointer_rtx (virtual)
	|                               |
	|  local variables              |
	|                               |
      P +-------------------------------+
	|                               |
	|  outgoing stack arguments     |
	|                               |
	+-------------------------------+ <-- stack_pointer_rtx
```


Determining how to overwrite the program counter
------------------------------------------------

When the `ret` instruction is executed, the contents of the `ra` instruction are
placed in the program counter. Usually the `ra` register is saved to the stack
in most functions (because its contents would otherwise be overwritten by
child function calls). This provides a vector for controlling the program
counter - if we overwrite the saved ra location, then when it is reloaded, the
address we return to is the one that we specified.

Check that the doSerial() function saves the return address and eventually
returns - if not, you may need to modify it to return.

Next, try to work out how many characters you need to send down the serial port
for the return address to be overwritten. You will probably be able to get a
rough feel for this by experimenting with the Arduino serial monitor - if you
enter the character "A" repeatedly, the device should try to jump to address
0x41414141 at some point - you may even see OpenOCD complain on the console that
the device has trapped due to branching to this illegal address.

Once you have observed this behaviour, try to automate inducing it - you will
probably want to write a program that sends a set number of characters down the
serial port (to be determined) followed by characters that correspond to the
address you want to jump to.

Once you have this working, set the address that is returned-to to the start of
the bufer `recvBuf` - this gives us the starting point for executing our own
code within the lockbox application.


Writing shellcode
-----------------

Shellcode is the code that is executed on the device. To write our shellcode, we
need to find:

- The address of the `K_correct` variable
- The address of Serial.print
- The address of somewhere suitable to return to (e.g. the start of the while
  loop in doSerial)

Once we have these things we need to write code that:

- Loads the address of `K_correct` into an argument register
- Calls the address of Serial.print
- Places the return address in `ra`
- Executes `ret`

To work out what this looks like, we can try:

- Writing a function in C that does something similar to the above (e.g. calling
  Serial.print with an argument
- Compiling it without assembling or linking:

```
riscv32-unknown-elf-gcc -S <filename>
```

We can inspect the generated assembly and try to modify it to do what we want
before assembling. To assemble, we can call the compiler on the assembly file
with the `-c` option:

```
riscv32-unknown-elf-gcc -c <asm or C file>
```

(if this is used with a C file, it will compile and assemble).

We can then look at the opcodes used for the assembled code using objdump:

```
riscv32-unknown-elf-objdump -dr <object file>
```

This will be important - the opcodes are what we will have to write into the
receive buffer.

If there are relocations in the code (you will see things like `R_RISCV_*` mixed
in with the assembly) then the opcodes are not "final" - they are yet to be
sorted out by the linker. If we want to resolve them, we can also compile and
link:

```
riscv32-unknown-elf-gcc -nostdlib -nostartfiles <file>
```

This won't result in a workable program because there will be no entry point,
but for our purposes (seeing what our function gets assembled to) this doesn't
matter.


Delivering the shellcode
------------------------

Once we have the exact opcodes for the functions that we want, we need to get
them put into `recvBuf`. Modify your program for sending code down the serial
line so that instead of putting garbage into the receive buffer, it puts your
opcodes into it instead. Then, when the return address is set, our shell code
will be executed.

This might be a bit fiddly to get right!


Overall exploit flow
--------------------

Now that we can execute code to read the secret and send it back, we need to
orchestrate everything on the host. The host program needs to:

- Send one "guess" down the serial (e.g. 12345678) so that correctInput is
  called at least once, to leave the `K_correct` value somewhere on an old stack
  frame.
- Send the exploit code (basically the program we already wrote)
- Listen on the serial port to receive the exfiltrated secret.
- Print out the secret.

If this is all working, we should have a complete demo!


Potential pitfalls
------------------

- Maybe there is no ret instruction in doSerial() due to the infinite loop. If
  this is the case, the function may need to be modified so that it returns.
  - Additionally, this may require that the overwrite of the stack with code
    also overwrites a local variable such that control flow in the function ends
    up in the block containing that ret.

- Maybe the value dead `K_correct` value from an old stack frame will be
  overwritten by another function call before we can read it.
  - If this is the case, maybe we can move it further up the stack - for
    example, by introducing a large array to push the location of `K_correct`
    further.

- Calling the Serial.print function may be more complicated than we hoped, due
  to extra (default) parameters, macros used in the source, inheritance, or some
  other issue. 
