I want to introduce `3` tips of CPU:  

(1) Endianness

The CPU has `2` kind of endianness (The images are from [Wikipedia](https://en.wikipedia.org/wiki/Endianness)):   
Big-endian(E.g., `SPARC`):  
![image](https://upload.wikimedia.org/wikipedia/commons/thumb/5/54/Big-Endian.svg/400px-Big-Endian.svg.png)  
little-endian(E.g.,`X86`):  
![image](https://upload.wikimedia.org/wikipedia/commons/thumb/e/ed/Little-Endian.svg/400px-Little-Endian.svg.png)  

Take the following program as a demonstration:  

	int main(void)
	{
		int a = 0x0A0B0C0D;
		return 0;
	}

Compile and use `gdb` to check the endianness of the CPU:  

	(gdb) start
	Temporary breakpoint 1 at 0x63e: file endian.c, line 3.
	Starting program: /root/Project/cup-tips/endian
	
	Temporary breakpoint 1, main () at endian.c:3
	warning: Source file is more recent than executable.
	3               int a = 0x0A0B0C0D;
	(gdb) n
	4               return 0;
	(gdb) p &a
	$2 = (int *) 0x7fffffffea8c
	(gdb) x/16xb 0x7fffffffea8c
	0x7fffffffea8c: 0x0d    0x0c    0x0b    0x0a    0x50    0x06    0x00    0x00
	0x7fffffffea94: 0x01    0x00    0x00    0x00    0xca    0x54    0xbd    0x70
	......

(2) Byte-alignment

Some CPUs require strict byte-alignment(Mostly many `RISC` architectures, e.g., `SPARC`) while some not(E.g., `X86`).  Take the following program as a demonstration:  

	#include <stdlib.h>

	int main(void)
	{
		char *p1 = calloc(10, sizeof(char));
		int *p2 = (int *)(p1 + 1);
		*p2 = 0x0A0B0C0D;
		return 0;
	}

Compile and use `gdb` to check the memory address of the `0x0A0B0C0D`:  

	5               char *p1 = calloc(10, sizeof(char));
	(gdb) n
	6               int *p2 = (int *)(p1 + 1);
	(gdb)
	7               *p2 = 0x0A0B0C0D;
	(gdb)
	8               return 0;
	(gdb) p p1
	$1 = 0x100202010 ""
	(gdb) x/16xb 0x100202010
	0x100202010:    0x00    0x0d    0x0c    0x0b    0x0a    0x00    0x00    0x00
	0x100202018:    0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00

The address of `0x0A0B0C0D` is not `byte-alignment`, i.e., the multiples of `sizeof(int)`. On `X86` machine, this can still work, but on `SPARC`, this will cause "`bus error`".   

(3) Stack growth.

Most CPUs' stack growth is from higher address to lower. Take `X86_64` architecture as an example (image is from [here](http://www.eecg.toronto.edu/~ashvin/courses/ece344/current/stack.png)):  
![image](http://www.eecg.toronto.edu/~ashvin/courses/ece344/current/stack.png)  

Take following program as an demonstration:  

	#include <stdio.h>
	
	void fun1(void)
	{
		double a = 111.111;
		return;
	}
	
	void fun2(void)
	{
		fun1();
	}
	
	int main(void)
	{
		fun2();
		return 0;
	}

Use `gdb` to check the calling stack:  
![image](https://github.com/NanXiao/cpu-tips/blob/master/image/call-stack.jpg)

`0x7fffffffea68` is `a`'s address; `0x7fffffffea70` is the caller function's "stack frame", and `0x7fffffffea78` is the return address.