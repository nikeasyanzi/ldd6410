本页讲解如何使用LDD6410工程的代码在skyeye中运行uboot、2.6.31内核及文件系统。由于目前skyeye还不支持s3c6410，所以我们生成的映像都是s3c2410的。
支持如何功能:
  * cs8900网卡
  * 320\*240 16bpp LCD
  * tftp
  * nfs
  * u-boot
  * linux-2.6.31
  * busybox
  * 从u-boot引导linux-2.6.31
# 主机端配置 #
## 安装skyeye ##
在主机端执行：
```
  sudo apt-get install skyeye
```

## 安装配置tftp服务器 ##
> 在主机端执行：
```
  sudo apt-get install tftpd-hpa
  sudo mkdir /home/tftp
  sudo chmod 777 /home/tftp
```
> sudo vim /etc/default/tftpd-hpa修改文件内容为:
```
  RUN_DAEMON="yes"
  OPTIONS="-l -c -s /home/tftp"
```
> 开启tftp服务：
```
  sudo /etc/init.d/tftpd-hpa start
  Starting HPA's tftpd: in.tftpd.
```

> skyeye target从主机下载文件的方式是：
```
  tftp -r file -g 10.0.0.1
```

## 安装配置NFS服务器 ##
> 在主机端执行：
```
  apt-get install nfs-kernel-server
  cd /home
  sudo ln -s tftp nfs
```
> sudo vim /etc/exports修改该文件内容为：
```
  /home/nfs *(sync,rw)
```

> 运行exportfs -rv
> 开启NFS服务：
```
   /etc/init.d/nfs-kernel-server restart
```
> 在skyeye target上mount nfs的方法是：
```
   mount -t nfs -o nolock 10.0.0.1:/home/nfs /mnt
```
> 由于skyeye模拟的Ethernet稳定性不太好，最好用如下参数来mount nfs:
```
   mount -t nfs -o nolock,soft,timeo=6000,retrans=100,rsize=16,wsize=16 10.0.0.1:/home/nfs /mnt
```
否则，容易出现网络无反应的现象。
# 编译uboot、内核和文件系统 #

## 编译u-boot ##
> 进入工程源代码目录ldd6410/s3c-u-boot-1.1.6，运行make smdk2410\_config，接着运行make得到u-boot映像。
> 如果要使得u-boot支持bootargs、bootcmd，可将ldd6410项目ldd6410/patches/2410-skyeye-uboot.patch拷贝到u-boot源代码目录，打上patch：
```
  patch -p0 < 2410-skyeye-uboot.patch
```

## 编译内核 ##
> 将ldd6410项目ldd6410/patches/2410-skyeye-kernel.patch拷贝到linux 2.6.31源代码目录，打patch:
```
  patch -p0 < 2410-skyeye-kernel.patch
```
> 接着运行：
> make smdk2410\_skyeye\_defconfig配置内核
> make vmlinux得到内核的vmlinux映像。

## 制作根文件系统initrd.img ##
> 进入ldd6410工程代码的如下目录：`ldd6410/utils/busybox-1.15.1`
> 运行`make menuconfig`选择必要的项目，然后运行`make`，`make install`。`make install`运行的结果是会生成`_install`,busybox对应的`linuxrc`, `/bin`, `/sbin`, `/usr`就在该目录了。
> 有了 busybox 后制作 `initrd.img` 就容易多了,只是说起来比较烦琐。以命令演示如下:
> 创建映像文件并挂到 initrd 目录
```
  mkdir initrd
  dd if=/dev/zero of=initrd.img bs=1k count=4096
  mke2fs -F -v initrd.img
  mount -o loop initrd.img initrd
```
> 将添加 busybox 到此映像文件
```
  cd initrd
  cp -r ../_install/* .
```
> 创建必要的目录
```
  mkdir proc lib etc dev root home var tmp
  chmod 777 tmp
```
> 建立设备文件
```
  cd dev
  mknod -m 644 console c 5 1
  mknod -m 644 null c 1 3
  mknod -m 640 ram b 1 1
  mknod -m 644 mem c 1 1
  cd ..
```
> 创建脚本文件 etc/inittab, 内容如下:
```
  ::sysinit:/etc/init.d/rcS
  ::askfirst:-/bin/sh
  ::restart:/sbin/init
  ::ctrlaltdel:/sbin/reboot
  ::shutdown:/bin/umount -a -r
  ::shutdown:/sbin/swapoff -a
```
> 设置此脚本执行权限
```
  chmod 644 etc/inittab
```
> 创建脚本文件 etc/init.d/rcS, 内容如下:
```
  #!/bin/sh
  /bin/mount -t proc none /proc
  /sbin/ifconfig lo 127.0.0.1 up
  /sbin/ifconfig eth0 10.0.0.2 netmask 255.0.0.0 up
  hostname skyeye
  mkdir /var/tmp
  mkdir /var/log
  mkdir /var/run
  mkdir /var/lock
  /bin/ash
```
> 设置此脚本执行权限
```
  chmod 755 etc/init.d/rcS
```
> 最后一步,执行实际的写入操作,生成 initrd.img
```
  cd ..
  umount initrd
```

# 运行 #
> 将编译得到的u-boot、vmlinux、initrd.img放入同一目录，将vmlinux、zImage拷入tftp服务器根目录。
> 可以直接运行Linux，使用http://ldd6410.googlecode.com/files/ldd6410-skyeye-pack.tar.gz 中的skyeye-standalone.conf配置文件，结合内核映像vmlinux:
```
sudo skyeye -e vmlinux -c skyeye-standalone.conf
```
> 也可以先启动u-boot,然后在uboot中下载vmlinux并启动它，使用http://ldd6410.googlecode.com/files/ldd6410-skyeye-pack.tar.gz 中的skyeye-with-uboot.conf 配置文件，结合先前编译得到的u-boot:
```
   sudo skyeye -e u-boot -c skyeye-with-uboot.conf -l 0x30000000,0xFFFFFF
```
> 在u-boot正常启动后，从tftp服务器下载vmlinux：
```
   tftp 0xc0000000 vmlinux
```
> 接着运行bootelf 0xc0000000，Linux内核开始启动。
> 打上2410-skyeye-kernel.patch的u-boot也支持zImage引导:
```
SMDK2410 # tftp 0xc0008000 zImage
TFTP from server 10.0.0.1; our IP address is 10.0.0.2
Filename 'zImage'.
Load address: 0xc0008000
Loading: #################################################################
	 #################################################################
	 #################################################################
	 #################################################################
	 #########
done
Bytes transferred = 1376828 (15023c hex)
SMDK2410 # bootm 0xc0008000
Boot with zImage

Starting kernel ...

Uncompressing Linux........................................................................................ done, booting the kernel.
Linux version 2.6.31.6-svn79 (bhsong@bhsong-laptop) (gcc version 4.2.2) #17 Tue Jan 5 22:00:50 CST 2010
CPU: ARM920T [41009200] revision 0 (ARMvundefined/unknown), cr=00003177
CPU: VIVT data cache, VIVT instruction cache
Machine: SMDK2410
Memory policy: ECC disabled, Data cache writeback
CPU S3C2410 (id 0x32410000)
S3C24XX Clocks, (c) 2004 Simtec Electronics
S3C2410: core 62.400 MHz, memory 31.200 MHz, peripheral 15.600 MHz
CLOCK: Slow mode (1.500 MHz), fast, MPLL on, UPLL on
Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 8128
Kernel command line: set bootargs mem=32M console=ttySAC0 root=/dev/ram initrd=0xc0800000,0x00800000 ramdisk_size=8192 rw
PID hash table entries: 128 (order: 7, 512 bytes)
Dentry cache hash table entries: 4096 (order: 2, 16384 bytes)
Inode-cache hash table entries: 2048 (order: 1, 8192 bytes)
Memory: 32MB = 32MB total
Memory: 21416KB available (2368K code, 251K data, 140K init, 0K highmem)
NR_IRQS:99
irq: clearing pending status 00004000
irq: clearing pending status 00008000
irq: clearing pending status 00800000
irq: clearing pending status 10000000
irq: clearing subpending status 00000093
Console: colour dummy device 80x30
console [ttySAC0] enabled
Calibrating delay loop... 12.67 BogoMIPS (lpj=31680)
Mount-cache hash table entries: 512
CPU: Testing write buffer coherency: ok
NET: Registered protocol family 16
S3C2410: Initialising architecture
bio: create slab <bio-0> at 0
SCSI subsystem initialized
NET: Registered protocol family 2
IP route cache hash table entries: 1024 (order: 0, 4096 bytes)
TCP established hash table entries: 1024 (order: 1, 8192 bytes)
TCP bind hash table entries: 1024 (order: 0, 4096 bytes)
TCP: Hash tables configured (established 1024 bind 1024)
TCP reno registered
NET: Registered protocol family 1
Trying to unpack rootfs image as initramfs...
rootfs image is not initramfs (junk in compressed archive); looks like an initrd
Freeing initrd memory: 8192K
msgmni has been set to 57
io scheduler noop registered (default)
ERROR: s3c2410x_io_write_word(0x4d000060) = 0x00000f82
ERROR: s3c2410x_io_write_word(0x4d000050) = 0x00000000
Console: switching to colour frame buffer device 40x30
fb0: s3c2410fb frame buffer device
Serial: 8250/16550 driver, 4 ports, IRQ sharing enabled
s3c2410-uart.0: s3c2410_serial0 at MMIO 0x50000000 (irq = 70) is a S3C2410
s3c2410-uart.1: s3c2410_serial1 at MMIO 0x50004000 (irq = 73) is a S3C2410
s3c2410-uart.2: s3c2410_serial2 at MMIO 0x50008000 (irq = 76) is a S3C2410
brd: module loaded
loop: module loaded
cs89x0:cs89x0_probe(0x0)
cs89x0.c: v2.4.3-pre1 Russell Nelson <nelson@crynwr.com>, Andrew Morton
eth0: cs8900 rev J found at 0xe0000300 eth0: Setting MAC address to c0:ff:ee:08:00:00.
[Cirrus EEPROM] 
cs89x0 media RJ-45, IRQ 53, programmed I/O, MAC c0:ff:ee:08:00:00
cs89x0_probe1() successful
mice: PS/2 mouse device common for all mice
S3C2410 Watchdog Timer, (c) 2004 Simtec Electronics
s3c2410-wdt s3c2410-wdt: watchdog inactive, reset disabled, irq enabled
TCP cubic registered
NET: Registered protocol family 17
RPC: Registered udp transport module.
RPC: Registered tcp transport module.
RAMDISK: ext2 filesystem found at block 0
RAMDISK: Loading 8192KiB [1 disk] into ram disk... done.
EXT2-fs warning: mounting unchecked fs, running e2fsck is recommended
VFS: Mounted root (ext2 filesystem) on device 1:0.
Freeing init memory: 140K
eth0: 10Base-T (RJ-45) has no cable
eth0: using half-duplex 10Base-T (RJ-45)
Welcome to
     _      _____      __   __    _      _
    / \    /  __ \    /  \_/  \  | |    |_|                 
   / _ \   | |  | |  / /\   /\ \ | |     _ ____  _   _  _  _ 
  / /_\ \  | |__| | / /  \_/  \ \| |    | |  _ \| | | |\ \/ /
 / /___\ \ | |__\ \ | |       | || |___ | | |_| | |_| |/    \
/_/	\_\| |   \_\|_|       |_||_____||_|_| |_|\____|\_/\_/
	   
ARMLinux for Lihacker LDD6410

For further information please check:
http://www.lihacker.com/
http://www.linuxdriver.cn/
For technical support, please subscribe mail list linuxdriver@googlegroups.com and post there.
The url is http://groups.google.com/group/linuxdriver.
# 
```

> http://ldd6410.googlecode.com/files/ldd6410-skyeye-pack.tar.gz 包含了已经做好的映像，可以直接运行。



