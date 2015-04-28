本页讲解如何建立S3C6410的开发环境。

所需要的交叉编译工具链：`S3C6410X-ToolChain4.2.2-EABI-V0.0-cross-4.2.2-eabi.tar`
下载地址：
http://ldd6410.googlecode.com/files/cross-4.2.2-eabi.tar.bz2

  1. 解压上述工具链获得文件夹：4.2.2-eabi/
  1. 在/usr/local/下面创建目录arm/ (注意，最好是放到这个目录，不然在以后的编译过程中可能出现一些错误)
  1. 将目录4.2.2-eabi/移动到/usr/local/arm/下面
  1. 设置环境变量：
> > 编辑/etc/profile文件，在文件末尾添加：
```
PATH="$PATH:/usr/local/arm/4.2.2-eabi/usr/bin"
export PATH
```
> > 使环境变量生效，在终端输入命令：
```
source /etc/profile
```
> > 另外，也可以通过修改home目录的.bashrc来将`/usr/local/arm/4.2.2-eabi/usr/bin`添加到PATH：
```
export PATH=/usr/local/arm/4.2.2-eabi/usr/bin/:$PATH
```
  1. 测试环境变量是否设置成功：
> > 在终端输入：echo $PATH，如果输出的路径中包含了/usr/local/arm/4.2.2-eabi/usr/bin则说明环境变量设置成功
  1. 测试交叉编译工具链
> > 在终端输入：arm-linux-gcc -v
> > 显示如下：
```
Using built-in specs.
Target: arm-unknown-linux-gnueabi
Configured with: /home/scsuh/workplace/coffee/buildroot-20071011/toolchain_build_arm
/gcc-4.2.2/configure --prefix=/usr --build=i386-pc-linux-gnu --host=i386-pc-linux-gnu 
--target=arm-unknown-linux-gnueabi --enable-languages=c,c++ --with-sysroot=/usr/local
/arm/4.2.2-eabi/ --with-build-time-tools=/usr/local/arm/4.2.2-eabi//usr/arm-unknown-linux-
gnueabi/bin --disable-__cxa_atexit --enable-target-optspace --with-gnu-ld --enable-shared 
--with-gmp=/usr/local/arm/4.2.2-eabi//gmp --with-mpfr=/usr/local/arm/4.2.2-eabi//mpfr 
--disable-nls --enable-threads --disable-multilib --disable-largefile --with-arch=armv4t 
--with-float=soft --enable-cxx-flags=-msoft-float
Thread model: posix
gcc version 4.2.2
```
> > 说明交叉编译工具链已经安装成功。