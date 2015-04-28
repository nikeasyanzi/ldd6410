# 程序 #
```
#include "stdio.h"
#include "stdlib.h"

void a() 
{
    volatile int i=20000000;
    while(i--);
}


void c() 
{
    volatile int i=40000000;
    while(i--);
}

void b() 
{
    volatile int i = 5000000;
    while(i--);
    a();
    c();
}

int main()
{
    printf(" main() function()\n");
    b();
}
```
# 编译 #
编译的时候应该指定`-pg`选项：
```
gcc -pg test.c -o test
```
这将使得目标代码中添加了gprof相关的内容，譬如查看a()、b()、c()的反汇编：
```
08048504 <a>:
 8048504:   55                      push   %ebp
 8048505:   89 e5                   mov    %esp,%ebp
 8048507:   83 ec 10                sub    $0x10,%esp
 804850a:   e8 c9 fe ff ff          call   80483d8 <mcount@plt>
 804850f:   c7 45 fc 00 2d 31 01    movl   $0x1312d00,-0x4(%ebp)
 8048516:   8b 45 fc                mov    -0x4(%ebp),%eax
 8048519:   85 c0                   test   %eax,%eax
 804851b:   0f 95 c2                setne  %dl
 804851e:   83 e8 01                sub    $0x1,%eax
 8048521:   89 45 fc                mov    %eax,-0x4(%ebp)
 8048524:   84 d2                   test   %dl,%dl
 8048526:   75 ee                   jne    8048516 <a+0x12>
 8048528:   c9                      leave
 8048529:   c3                      ret    

0804852a <c>:
 804852a:   55                      push   %ebp
 804852b:   89 e5                   mov    %esp,%ebp
 804852d:   83 ec 10                sub    $0x10,%esp
 8048530:   e8 a3 fe ff ff          call   80483d8 <mcount@plt>
 8048535:   c7 45 fc 00 5a 62 02    movl   $0x2625a00,-0x4(%ebp)
 804853c:   8b 45 fc                mov    -0x4(%ebp),%eax
 804853f:   85 c0                   test   %eax,%eax
 8048541:   0f 95 c2                setne  %dl 
 8048544:   83 e8 01                sub    $0x1,%eax
 8048547:   89 45 fc                mov    %eax,-0x4(%ebp)
...
```
也就算在每个函数的入口处调用了
```
void _mcount()
```
# 运行 #
```
./test
```
# 查看结果 #
编译运行结束后，目录中会多出一个gmon.out文件，查看结果：
```
bhsong@bhsong-laptop:~/develop/svn/ldd6410/training/debug/gprof$ gprof -b test gmon.out
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 58.33      0.14     0.14        1   140.00   140.00  c
 29.17      0.21     0.07        1    70.00    70.00  a
 12.50      0.24     0.03        1    30.00   240.00  b


			Call graph


granularity: each sample hit covers 4 byte(s) for 4.17% of 0.24 seconds

index % time    self  children    called     name
                0.03    0.21       1/1           main [2]
[1]    100.0    0.03    0.21       1         b [1]
                0.14    0.00       1/1           c [3]
                0.07    0.00       1/1           a [4]
-----------------------------------------------
                                                 <spontaneous>
[2]    100.0    0.00    0.24                 main [2]
                0.03    0.21       1/1           b [1]
-----------------------------------------------
                0.14    0.00       1/1           b [1]
[3]     58.3    0.14    0.00       1         c [3]
-----------------------------------------------
                0.07    0.00       1/1           b [1]
[4]     29.2    0.07    0.00       1         a [4]
-----------------------------------------------


Index by function name

   [4] a                       [1] b                       [3] c
```
bhsong@bhsong-laptop:~/develop/svn/ldd6410/training/debug/gprof$ gprof -b test gmon.out
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 58.33      0.14     0.14        1   140.00   140.00  c
 29.17      0.21     0.07        1    70.00    70.00  a
 12.50      0.24     0.03        1    30.00   240.00  b


			Call graph


granularity: each sample hit covers 4 byte(s) for 4.17% of 0.24 seconds

index % time    self  children    called     name
                0.03    0.21       1/1           main [2]
[1]    100.0    0.03    0.21       1         b [1]
                0.14    0.00       1/1           c [3]
                0.07    0.00       1/1           a [4]
-----------------------------------------------
                                                 <spontaneous>
[2]    100.0    0.00    0.24                 main [2]
                0.03    0.21       1/1           b [1]
-----------------------------------------------
                0.14    0.00       1/1           b [1]
[3]     58.3    0.14    0.00       1         c [3]
-----------------------------------------------
                0.07    0.00       1/1           b [1]
[4]     29.2    0.07    0.00       1         a [4]
-----------------------------------------------


Index by function name

   [4] a                       [1] b                       [3] c
}}}```