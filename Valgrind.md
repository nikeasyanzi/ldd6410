下面的程序犯有错误：
```
#include <string.h>

int foo(int x)
{
	if (x < 0)
		printf("%d ", x);

	return 0;
}

int main(int argc, char *argv[])
{
	char *ptr, *ptr1;
	int x, y[10];

	/* malloc without free */
	ptr = (char*) malloc(10);
	strcpy(ptr, "01234567890");

	/* not initialized memory */
	foo(x);
	
	/* more free */
	ptr1 = (char*) malloc(10);
	ptr1[0] = '\0';
	free(ptr1);
	free(ptr1);
	
	/* array overflow */
	y[11] = 0;

	return 0;
}    
```
使用Valgrind运行它：
```
valgrind --tool=memcheck --leak-check=yes ./test
```
得到如下结果：
```
==21412== Memcheck, a memory error detector.
==21412== Copyright (C) 2002-2007, and GNU GPL'd, by Julian Seward et al.
==21412== Using LibVEX rev 1854, a library for dynamic binary translation.
==21412== Copyright (C) 2004-2007, and GNU GPL'd, by OpenWorks LLP.
==21412== Using valgrind-3.3.1-Debian, a dynamic binary instrumentation framework.
==21412== Copyright (C) 2000-2007, and GNU GPL'd, by Julian Seward et al.
==21412== For more details, rerun with: -v
==21412== 
==21412== Invalid write of size 1
==21412==    at 0x4026A08: memcpy (mc_replace_strmem.c:402)
==21412==    by 0x80484C4: main (test.c:18)
==21412==  Address 0x419d032 is 0 bytes after a block of size 10 alloc'd
==21412==    at 0x4025D2E: malloc (vg_replace_malloc.c:207)
==21412==    by 0x80484A6: main (test.c:17)
==21412== 
==21412== Invalid write of size 1
==21412==    at 0x4026A11: memcpy (mc_replace_strmem.c:402)
==21412==    by 0x80484C4: main (test.c:18)
==21412==  Address 0x419d033 is 1 bytes after a block of size 10 alloc'd
==21412==    at 0x4025D2E: malloc (vg_replace_malloc.c:207)
==21412==    by 0x80484A6: main (test.c:17)
==21412== 
==21412== Conditional jump or move depends on uninitialised value(s)
==21412==    at 0x804846E: foo (test.c:5)
==21412==    by 0x80484CF: main (test.c:21)
==21412== 
==21412== Use of uninitialised value of size 4
==21412==    at 0x407D7B6: (within /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x4081380: vfprintf (in /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x4088C3F: printf (in /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x8048482: foo (test.c:6)
==21412==    by 0x80484CF: main (test.c:21)
==21412== 
==21412== Conditional jump or move depends on uninitialised value(s)
==21412==    at 0x407D7BE: (within /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x4081380: vfprintf (in /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x4088C3F: printf (in /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x8048482: foo (test.c:6)
==21412==    by 0x80484CF: main (test.c:21)
==21412== 
==21412== Conditional jump or move depends on uninitialised value(s)
==21412==    at 0x407F76E: vfprintf (in /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x4088C3F: printf (in /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x8048482: foo (test.c:6)
==21412==    by 0x80484CF: main (test.c:21)
==21412== 
==21412== Conditional jump or move depends on uninitialised value(s)
==21412==    at 0x4081C5E: vfprintf (in /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x4088C3F: printf (in /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x8048482: foo (test.c:6)
==21412==    by 0x80484CF: main (test.c:21)
==21412== 
==21412== Conditional jump or move depends on uninitialised value(s)
==21412==    at 0x407F818: vfprintf (in /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x4088C3F: printf (in /lib/tls/i686/cmov/libc-2.8.90.so)
==21412==    by 0x8048482: foo (test.c:6)
==21412==    by 0x80484CF: main (test.c:21)
==21412== 
==21412== Invalid free() / delete / delete[]
==21412==    at 0x4024B4A: free (vg_replace_malloc.c:323)
==21412==    by 0x80484FA: main (test.c:27)
==21412==  Address 0x419d068 is 0 bytes inside a block of size 10 free'd
==21412==    at 0x4024B4A: free (vg_replace_malloc.c:323)
==21412==    by 0x80484EF: main (test.c:26)
-1091741928 ==21412== 
==21412== ERROR SUMMARY: 27 errors from 9 contexts (suppressed: 11 from 1)
==21412== malloc/free: in use at exit: 10 bytes in 1 blocks.
==21412== malloc/free: 2 allocs, 2 frees, 20 bytes allocated.
==21412== For counts of detected errors, rerun with: -v
==21412== searching for pointers to 1 not-freed blocks.
==21412== checked 51,468 bytes.
==21412== 
==21412== 
==21412== 10 bytes in 1 blocks are definitely lost in loss record 1 of 1
==21412==    at 0x4025D2E: malloc (vg_replace_malloc.c:207)
==21412==    by 0x80484A6: main (test.c:17)
==21412== 
==21412== LEAK SUMMARY:
==21412==    definitely lost: 10 bytes in 1 blocks.
==21412==      possibly lost: 0 bytes in 0 blocks.
==21412==    still reachable: 0 bytes in 0 blocks.
==21412==         suppressed: 0 bytes in 0 blocks.
```