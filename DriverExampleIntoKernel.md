# 添加代码、Kconfig和Makefile #

在linux-2.6.31/drivers/char目录下建立子目录：
```
bhsong@bhsong-laptop:~/develop/svn/ldd6410/linux-2.6.31/drivers/char$ mkdir driver_examples
```
将三个驱动hello.c、globalmem.c、globalfifo.c拷入driver\_examples目录：
```
cd driver_examples/
cp ../../../../training/kernel/drivers/hello/hello.c ./
cp ../../../../training/kernel/drivers/globalmem/globalmem.c ./
cp ../../../../training/kernel/drivers/globalmem/globalfifo.c ./
```
修改drivers/char下面的Kconfig和Makefile导入driver\_examples目录：
  * 在drivers/char/Kconfig中添加：
```
source "drivers/char/driver_examples/Kconfig"
```
  * 添加drivers/char/driver\_examples/Kconfig文件：
```
#
# driver examples configuration
#

menuconfig DRIVER_EXAMPLE
    tristate "driver examples in 'Explain Linux Device Drivers in detail'"
    ---help---
      say Yes to build-in hello world, globalmem, globalfifo, say M to get 
      those kernel modules

if DRIVER_EXAMPLE

config HELLO_WORLD
    tristate "Hello World"
    ---help---
      To compile this driver as a module, choose M here; the module will be
      called hello.mem

config GLOBALMEM
    tristate "globalmem"
    ---help---
      To  compile this driver as a module, choose M here; the module will be
      called globalmem.

config GLOBALFIFO
    tristate "globalfifo"
    ---help---
      To  compile this driver as a module, choose M here; the module will be
      called globalfifo.

endif # DRIVER_EXAMPLE
```
这将形成如下菜单：
```
 driver examples in 'Explain Linux Device Drivers in detail' ─────────────────────────────┐
  │  Arrow keys navigate the menu.  <Enter> selects submenus --->.  Highlighted letters are hotkeys.  Pressing <Y>        │  
  │  includes, <N> excludes, <M> modularizes features.  Press <Esc><Esc> to exit, <?> for Help, </> for Search.  Legend:  │  
  │  [*] built-in  [ ] excluded  <M> module  < > module capable                                                           │  
  │                                                                                                                       │  
  │ ┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐ │  
  │ │                      --- driver examples in 'Explain Linux Device Drivers in detail'                              │ │  
  │ │                      < >   Hello World (NEW)                                                                      │ │  
  │ │                      < >   globalmem (NEW)                                                                        │ │  
  │ │                      < >   globalfifo (NEW)                                                                       │ │  
  │ │                                                                                                                   │ │  
  │ │                                                                                                                   │ │  
  │ │                                                        
```
  * 在drivers/char/Makefile中添加：
```
obj-$(CONFIG_DRIVER_EXAMPLE)        += driver_examples/
```
  * 添加drivers/char/driver\_examples/Makefile文件：
```
obj-$(CONFIG_HELLO_WORLD)       += hello.o
obj-$(CONFIG_GLOBALMEM)         += globalmem.o
obj-$(CONFIG_GLOBALFIFO)        += globalfifo.o
```
相关的Kconfig和Makefile patch如下：
```
Index: drivers/char/Kconfig
===================================================================
--- drivers/char/Kconfig	(revision 87)
+++ drivers/char/Kconfig	(working copy)
@@ -1110,5 +1110,7 @@
 
 source "drivers/s390/char/Kconfig"
 
+source "drivers/char/driver_examples/Kconfig"
+
 endmenu
 
Index: drivers/char/Makefile
===================================================================
--- drivers/char/Makefile	(revision 87)
+++ drivers/char/Makefile	(working copy)
@@ -111,6 +111,8 @@
 obj-$(CONFIG_JS_RTC)		+= js-rtc.o
 js-rtc-y = rtc.o
 
+obj-$(CONFIG_DRIVER_EXAMPLE)		+= driver_examples/
+
 # Files generated that shall be removed upon make clean
 clean-files := consolemap_deftbl.c defkeymap.c
 
Index: drivers/char/driver_examples/Kconfig
===================================================================
--- drivers/char/driver_examples/Kconfig	(revision 0)
+++ drivers/char/driver_examples/Kconfig	(revision 0)
@@ -0,0 +1,31 @@
+#
+# driver examples configuration
+#
+
+menuconfig DRIVER_EXAMPLE
+	tristate "driver examples in 'Explain Linux Device Drivers in detail'"
+	---help---
+	  say Yes to build-in hello world, globalmem, globalfifo, say M to get
+	  those kernel modules
+
+if DRIVER_EXAMPLE
+
+config HELLO_WORLD
+	tristate "Hello World"
+	---help---
+	  To compile this driver as a module, choose M here; the module will be
+	  called hello.
+
+config GLOBALMEM
+	tristate "globalmem"
+	---help---
+	  To  compile this driver as a module, choose M here; the module will be
+	  called globalmem.
+
+config GLOBALFIFO
+	tristate "globalfifo"
+	---help---
+	  To  compile this driver as a module, choose M here; the module will be
+	  called globalfifo.
+
+endif # DRIVER_EXAMPLE
Index: drivers/char/driver_examples/Makefile
===================================================================
--- drivers/char/driver_examples/Makefile	(revision 0)
+++ drivers/char/driver_examples/Makefile	(revision 0)
@@ -0,0 +1,3 @@
+obj-$(CONFIG_HELLO_WORLD)		+= hello.o
+obj-$(CONFIG_GLOBALMEM)		    += globalmem.o
+obj-$(CONFIG_GLOBALFIFO)		+= globalfifo.o
```

# 编译和安装 #
选中三个模块：
```
─── driver examples in 'Explain Linux Device Drivers in detail' ─────────────────────────────┐
  │  Arrow keys navigate the menu.  <Enter> selects submenus --->.  Highlighted letters are hotkeys.  Pressing <Y>        │  
  │  includes, <N> excludes, <M> modularizes features.  Press <Esc><Esc> to exit, <?> for Help, </> for Search.  Legend:  │  
  │  [*] built-in  [ ] excluded  <M> module  < > module capable                                                           │  
  │                                                                                                                       │  
  │ ┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐ │  
  │ │                      --- driver examples in 'Explain Linux Device Drivers in detail'                              │ │  
  │ │                      <M>   Hello World                                                                            │ │  
  │ │                      <M>   globalmem                                                                              │ │  
  │ │                      <M>   globalfifo                                                                             │ │  
  │ │                                                                                                                   │ │ 
```
在linux-2.6.31下运行：
```
bhsong@bhsong-laptop:~/develop/svn/ldd6410/linux-2.6.31$ make modules
```
以loopback方式mount根文件系统，譬如对于skyeye的initrd.img运行如下命令：
```
...skyeye/training-simulation$ sudo mount -o loop initrd.img initrd
```
将所有模块安装到根文件系统：
```
make INSTALL_MOD_PATH=/home/bhsong/develop/training/skyeye/training-simulation/initrd modules_install
```
请根据你的路径，正确的设置INSTALL\_MOD\_PATH。安装之后，所有模块以及模块间依赖关系进入根文件系统：
```
initrd/lib/modules$ tree
.
`-- 2.6.31.6-svn79
    |-- build -> /home/bhsong/develop/svn/ldd6410/linux-2.6.31
    |-- kernel
    |   |-- drivers
    |   |   |-- cdrom
    |   |   |   `-- cdrom.ko
    |   |   |-- char
    |   |   |   `-- driver_examples
    |   |   |       |-- globalfifo.ko
    |   |   |       |-- globalmem.ko
    |   |   |       `-- hello.ko
    |   |   |-- input
    |   |   |   `-- gameport
    |   |   |       `-- gameport.ko
    |   |   `-- scsi
    |   |       |-- ch.ko
    |   |       |-- scsi_wait_scan.ko
    |   |       |-- sr_mod.ko
    |   |       `-- st.ko
    |   |-- lib
    |   |   |-- ts_bm.ko
    |   |   |-- ts_fsm.ko
    |   |   `-- ts_kmp.ko
    |   `-- net
    |       |-- ipv4
    |       |   |-- netfilter
    |       |   |   |-- arp_tables.ko
    |       |   |   |-- arpt_mangle.ko
    |       |   |   |-- arptable_filter.ko
    |       |   |   |-- ip_queue.ko
    |       |   |   |-- ip_tables.ko
    |       |   |   |-- ipt_ECN.ko
    |       |   |   |-- ipt_LOG.ko
    |       |   |   |-- ipt_MASQUERADE.ko
    |       |   |   |-- ipt_NETMAP.ko
    |       |   |   |-- ipt_REDIRECT.ko
    |       |   |   |-- ipt_REJECT.ko
    |       |   |   |-- ipt_ULOG.ko
    |       |   |   |-- ipt_addrtype.ko
    |       |   |   |-- ipt_ah.ko
    |       |   |   |-- ipt_ecn.ko
    |       |   |   |-- iptable_filter.ko
    |       |   |   |-- iptable_mangle.ko
    |       |   |   |-- iptable_nat.ko
    |       |   |   |-- iptable_raw.ko
    |       |   |   |-- nf_conntrack_ipv4.ko
    |       |   |   |-- nf_defrag_ipv4.ko
    |       |   |   |-- nf_nat.ko
    |       |   |   |-- nf_nat_amanda.ko
    |       |   |   |-- nf_nat_ftp.ko
    |       |   |   |-- nf_nat_h323.ko
    |       |   |   |-- nf_nat_irc.ko
    |       |   |   |-- nf_nat_pptp.ko
    |       |   |   |-- nf_nat_proto_gre.ko
    |       |   |   |-- nf_nat_proto_udplite.ko
    |       |   |   |-- nf_nat_sip.ko
    |       |   |   |-- nf_nat_snmp_basic.ko
    |       |   |   `-- nf_nat_tftp.ko
    |       |   |-- tcp_bic.ko
    |       |   |-- tcp_htcp.ko
    |       |   `-- tcp_westwood.ko
    |       `-- netfilter
    |           |-- ipvs
    |           |   `-- ip_vs.ko
    |           |-- nf_conntrack.ko
    |           |-- nf_conntrack_amanda.ko
    |           |-- nf_conntrack_ftp.ko
    |           |-- nf_conntrack_h323.ko
    |           |-- nf_conntrack_irc.ko
    |           |-- nf_conntrack_netbios_ns.ko
    |           |-- nf_conntrack_netlink.ko
    |           |-- nf_conntrack_pptp.ko
    |           |-- nf_conntrack_proto_gre.ko
    |           |-- nf_conntrack_proto_udplite.ko
    |           |-- nf_conntrack_sip.ko
    |           |-- nf_conntrack_tftp.ko
    |           |-- nfnetlink.ko
    |           |-- nfnetlink_log.ko
    |           |-- nfnetlink_queue.ko
    |           |-- x_tables.ko
    |           |-- xt_CLASSIFY.ko
    |           |-- xt_CONNMARK.ko
    |           |-- xt_HL.ko
    |           |-- xt_MARK.ko
    |           |-- xt_NFLOG.ko
    |           |-- xt_NFQUEUE.ko
    |           |-- xt_RATEEST.ko
    |           |-- xt_TCPMSS.ko
    |           |-- xt_cluster.ko
    |           |-- xt_comment.ko
    |           |-- xt_connbytes.ko
    |           |-- xt_connlimit.ko
    |           |-- xt_connmark.ko
    |           |-- xt_conntrack.ko
    |           |-- xt_dccp.ko
    |           |-- xt_dscp.ko
    |           |-- xt_esp.ko
    |           |-- xt_hashlimit.ko
    |           |-- xt_helper.ko
    |           |-- xt_hl.ko
    |           |-- xt_iprange.ko
    |           |-- xt_length.ko
    |           |-- xt_limit.ko
    |           |-- xt_mac.ko
    |           |-- xt_mark.ko
    |           |-- xt_multiport.ko
    |           |-- xt_owner.ko
    |           |-- xt_pkttype.ko
    |           |-- xt_policy.ko
    |           |-- xt_quota.ko
    |           |-- xt_rateest.ko
    |           |-- xt_realm.ko
    |           |-- xt_recent.ko
    |           |-- xt_state.ko
    |           |-- xt_statistic.ko
    |           |-- xt_string.ko
    |           |-- xt_tcpmss.ko
    |           |-- xt_tcpudp.ko
    |           |-- xt_time.ko
    |           `-- xt_u32.ko
    |-- modules.alias
    |-- modules.ccwmap
    |-- modules.dep
    |-- modules.ieee1394map
    |-- modules.inputmap
    |-- modules.isapnpmap
    |-- modules.ofmap
    |-- modules.order
    |-- modules.pcimap
    |-- modules.seriomap
    |-- modules.symbols
    |-- modules.usbmap
    `-- source -> /home/bhsong/develop/svn/ldd6410/linux-2.6.31

17 directories, 116 files
```

# 运行 #
启动Linux，对于skyeye为：
```
sudo skyeye -e vmlinux -c skyeye-standalone.conf
...
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
加载模块：
```
modprobe hello
modprobe globalmem  globalmem_major=250
modprobe globalfifo globalfifo_major=251
```
skyeye对insmod的支持有些问题，加载模块时会出现：
```
# modprobe hello
Unable to handle kernel paging request at virtual address bf000000
pgd = c05f0000
[bf000000] *pgd=00000000
Internal error: Oops: 807 [#1]
Modules linked in:
CPU: 0    Not tainted  (2.6.31.6-svn79 #21)
PC is at __memzero+0x24/0x80
LR is at 0x0
pc : [<c0107404>]    lr : [<00000000>]    psr: 20000013
sp : c182bf0c  ip : 00000000  fp : 00000005
r10: c2807230  r9 : c2807000  r8 : c2807388
r7 : 00000004  r6 : c2807168  r5 : 00000012  r4 : bf000000
r3 : 00000000  r2 : 00000000  r1 : 00000464  r0 : bf000000
Flags: nzCv  IRQs on  FIQs on  Mode SVC_32  ISA ARM  Segment user
Control: 0000317f  Table: c05f0000  DAC: 00000015
Process modprobe (pid: 795, stack limit = 0xc182a270)
Stack: (0xc182bf0c to 0xc182c000)
bf00:                            c005fba0 00000979 00000000 c2807518 c2807608 
bf20: c28072dc c1139e60 c28078a0 00000010 000000c5 c2807388 c182a000 c2807630 
bf40: 00000000 c008c178 00000171 00000000 0015ec79 00000287 00000001 00000008 
bf60: 00000000 00000979 0015e2f0 00000000 0015e300 c002bf48 c182a000 00000000 
bf80: 00000000 c0060878 c1c1a2c0 c1c2f3a0 00000003 00000979 0015e2f0 0015dfa8 
bfa0: 00000080 c002bda0 00000979 0015e2f0 0015e300 00000979 0015e2f0 00000000 
bfc0: 00000979 0015e2f0 0015dfa8 00000080 0015e2a0 0015dfbc 00000001 00000000 
bfe0: bea65958 bea65948 000177f8 00009350 20000010 0015e300 ab081000 11000006 
[<c0107404>] (__memzero+0x24/0x80) from [<00000000>] (0x0)
Code: e52de004 e1a0c002 e1a0e002 e2511040 (a8a0500c) 
---[ end trace 1e19dcbb1e19dcba ]---
Segmentation fault
```
详见bug:
http://sourceforge.net/tracker/?func=detail&aid=2897905&group_id=85554&atid=576533
我们使用strace去寻找死在哪个系统调用：
```
# strace modprobe hello
execve("/sbin/modprobe", ["modprobe", "hello"], [/* 7 vars */]) = 0
uname({sys="Linux", node="lihacker", ...}) = 0
brk(0)                                  = 0x15d000
brk(0x15dd02)                           = 0x15dd02
set_tls(0x15d4a0, 0x1573c0, 0, 0x1, 0x15d4a0) = 0
brk(0x17ed02)                           = 0x17ed02
brk(0x17f000)                           = 0x17f000
getuid32()                              = 0
chdir("/lib/modules")                   = 0
uname({sys="Linux", node="lihacker", ...}) = 0
chdir("2.6.31.6-svn79")                 = 0
open("/proc/modules", O_RDONLY)         = 3
fstat64(3, {st_mode=S_IFREG|0444, st_size=0, ...}) = 0
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x40000000
read(3, ""..., 1024)                    = 0
close(3)                                = 0
munmap(0x40000000, 4096)                = 0
lstat64("/etc/modprobe.conf", 0xbe846880) = -1 ENOENT (No such file or directory)
lstat64("/etc/modprobe.d", 0xbe846880)  = -1 ENOENT (No such file or directory)
open("modules.dep", O_RDONLY)           = 3
fstat64(3, {st_mode=S_IFREG|0644, st_size=17141, ...}) = 0
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x40001000
read(3, "/lib/modules/2.6.31.6-svn79/kerne"..., 1024) = 1024
read(3, "nel/net/netfilter/x_tables.ko\n/li"..., 1024) = 1024
read(3, "/lib/modules/2.6.31.6-svn79/kerne"..., 1024) = 1024
read(3, "/net/netfilter/xt_esp.ko: /lib/mo"..., 1024) = 1024
read(3, "ables.ko /lib/modules/2.6.31.6-sv"..., 1024) = 1024
read(3, "es/2.6.31.6-svn79/kernel/net/netf"..., 1024) = 1024
read(3, ".6-svn79/kernel/net/netfilter/nf_"..., 1024) = 1024
read(3, "b/modules/2.6.31.6-svn79/kernel/n"..., 1024) = 1024
read(3, "etfilter/nf_conntrack_ipv4.ko /li"..., 1024) = 1024
read(3, ".6.31.6-svn79/kernel/net/netfilte"..., 1024) = 1024
read(3, "defrag_ipv4.ko\n/lib/modules/2.6.3"..., 1024) = 1024
read(3, "tfilter/nf_conntrack_ipv4.ko /lib"..., 1024) = 1024
read(3, ".6-svn79/kernel/net/ipv4/netfilte"..., 1024) = 1024
read(3, "1.6-svn79/kernel/net/netfilter/x_"..., 1024) = 1024
read(3, "ib/modules/2.6.31.6-svn79/kernel/"..., 1024) = 1024
read(3, "ib/modules/2.6.31.6-svn79/kernel/"..., 1024) = 1024
read(3, "o:\n/lib/modules/2.6.31.6-svn79/ke"..., 1024) = 757
close(3)                                = 0
munmap(0x40001000, 4096)                = 0
open("/lib/modules/2.6.31.6-svn79/kernel/drivers/char/driver_examples/hello.ko", O_RDONLY) = 3
fstat64(3, {st_mode=S_IFREG|0644, st_size=2425, ...}) = 0
read(3, "\177ELF\1\1\1\0\0\0\0\0\0\0\0\0\1\0(\0\1\0\0\0\0\0\0\0\0\0\0\0\210"..., 3072) = 2425
read(3, ""..., 647)                     = 0
close(3)                                = 0
init_module(0x15e300, 2425, ""Unable to handle kernel paging request at virtual address bf000000
pgd = c0620000
[bf000000] *pgd=00000000
Internal error: Oops: 807 [#1]
Modules linked in:
CPU: 0    Not tainted  (2.6.31.6-svn79 #21)
PC is at __memzero+0x24/0x80
LR is at 0x0
pc : [<c0107404>]    lr : [<00000000>]    psr: 20000013
sp : c069ff0c  ip : 00000000  fp : 00000005
r10: c2807230  r9 : c2807000  r8 : c2807388
r7 : 00000004  r6 : c2807168  r5 : 00000012  r4 : bf000000
r3 : 00000000  r2 : 00000000  r1 : 00000464  r0 : bf000000
Flags: nzCv  IRQs on  FIQs on  Mode SVC_32  ISA ARM  Segment user
Control: 0000317f  Table: c0620000  DAC: 00000015
Process modprobe (pid: 796, stack limit = 0xc069e270)
Stack: (0xc069ff0c to 0xc06a0000)
ff00:                            c005fba0 00000000 00000000 c2807518 c2807608 
ff20: c28072dc c1139240 c28078a0 00000010 00000000 c2807388 00000000 c2807630 
ff40: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 
ff60: 00000000 00000979 0015e2f0 00000000 0015e300 c002bf48 c069e000 00000000 
ff80: 00000000 c0060878 be846958 c002dbe0 00000000 00000979 0015e2f0 0015dfa8 
ffa0: 00000080 c002bf0c 00000979 0015e2f0 0015e300 00000979 0015e2f0 00000000 
ffc0: 00000979 0015e2f0 0015dfa8 00000080 0015e2a0 0015dfbc 00000001 00000000 
ffe0: be846958 be846948 000177f8 00009350 20000010 0015e300 ebff1fc9 e3500000 
[<c0107404>] (__memzero+0x24/0x80) from [<00000000>] (0x0)
Code: e52de004 e1a0c002 e1a0e002 e2511040 (a8a0500c) 
---[ end trace 1e19dcbb1e19dcba ]---
 <unfinished ...>
+++ killed by SIGSEGV +++
```
很明显，死在内核的linux-2.6.31/kernel/module.c的init\_module()函数中,加上几个printk:
```
2453 SYSCALL_DEFINE3(init_module, void __user *, umod,
2454         unsigned long, len, const char __user *, uargs)
2455 {
2456     struct module *mod;
2457     int ret = 0;
2458 
2459     printk("%s %d\n", __func__, __LINE__);
2460 
2461     /* Must have permission */
2462     if (!capable(CAP_SYS_MODULE) || modules_disabled)
2463         return -EPERM;
2464 
2465     +printk("%s %d\n", __func__, __LINE__);
2466     
2467     /* Only one module load at a time, please */
2468     if (mutex_lock_interruptible(&module_mutex) != 0)
2469         return -EINTR;
2470 
2471     +printk("%s %d\n", __func__, __LINE__);
2472     
2473     /* Do all the hard work */
2474     mod = load_module(umod, len, uargs);
2475     if (IS_ERR(mod)) {
2476         mutex_unlock(&module_mutex);
2477         return PTR_ERR(mod);
2478     }
2479 
2480     +printk("%s %d\n", __func__, __LINE__);
2481     
```
再次运行:
```
# modprobe hello
sys_init_module 2459
sys_init_module 2465
sys_init_module 2471
Unable to handle kernel paging request at virtual address bf000000
pgd = c05f0000
[bf000000] *pgd=00000000
Internal error: Oops: 807 [#1]
Modules linked in:
```
从打印信息可以看出，panic发生在load\_module函数，既然"sys\_init\_module 2471"被打印而"sys\_init\_module 2480"未打印。
继续跟进load\_module()，直到最后发现内核崩溃在`memset(ptr, 0, mod->core_size);`：
```
static noinline struct module *load_module(void __user *umod,
                   unsigned long len,
                   const char __user *uargs)
{
...
     ptr = module_alloc_update_bounds(mod->core_size);
...
     kmemleak_not_leak(ptr);
...
     memset(ptr, 0, mod->core_size);
```
一路跟踪源代码，发现ARM的Kernel会将模块的内存申请在3G-16M到3G的区域，而skyeye在内核空间处理3G以下地址的时候，会发生错误:
```
"Unable to handle kernel paging request at virtual address bf000000"
```
因此，workaround掉这个bug:
```
Index: arch/arm/kernel/module.c
===================================================================
--- arch/arm/kernel/module.c	(revision 87)
+++ arch/arm/kernel/module.c	(working copy)
@@ -38,7 +38,7 @@
 #ifdef CONFIG_MMU
 void *module_alloc(unsigned long size)
 {
-	struct vm_struct *area;
+	/*struct vm_struct *area;
 
 	size = PAGE_ALIGN(size);
 	if (!size)
@@ -49,6 +49,9 @@
 		return NULL;
 
 	return __vmalloc_area(area, GFP_KERNEL, PAGE_KERNEL_EXEC);
+*/
+
+	return size == 0 ? NULL : kmalloc(size, GFP_KERNEL);
 }
 #else /* CONFIG_MMU */
 void *module_alloc(unsigned long size)
@@ -59,7 +62,7 @@
 
 void module_free(struct module *module, void *region)
 {
-	vfree(region);
+	kfree(region);
 }
 
 int module_frob_arch_sections(Elf_Ehdr *hdr,
```
由于此workaround使用了kmalloc，如果内核模块很大，kmalloc申请的内存不够存放，则可以修改：
include/linux/slab.h中的KMALLOC\_SHIFT\_HIGH宏定义。另外，此workaround成立的条件是mem=32M或小于32M。
现在我们可以自由地加载模块了，看看结果：
```
加载模块：
# modprobe hello
Hello World enter
# modprobe globalmem  globalmem_major=250
# modprobe globalfifo globalfifo_major=251

查看加载的模块:
# lsmod
    Not tainted
globalmem 3356 0 - Live 0xc0f66000
hello 1188 0 - Live 0xc1128000
globalfifo 4292 0 - Live 0xc182a000

查看设备：
# cat /proc/devices 
Character devices:
  1 mem
  2 pty
  3 ttyp
  4 /dev/vc/0
  4 tty
  4 ttyS
  5 /dev/tty
  5 /dev/console
  5 /dev/ptmx
  7 vcs
 10 misc
 13 input
 21 sg
 29 fb
128 ptm
136 pts
204 s3c2410_serial
250 globalmem
251 globalfifo

建立结点：
# mknod /dev/globalmem c 250 0
# mknod /dev/globalfifo c 251 0
# 
读写设备文件：
# echo "hello, cisco" > /dev/globalmem
written 13 bytes(s) from 0
# cat /dev/globalmem 
read 4096 bytes(s) from 0
hello, cisco

# cat /dev/globalfifo &
# echo "hello,cisco, I love you" > /dev/globalfifo 
written 24 bytes(s),current_len:24
# read 24 bytes(s),current_len:0
hello,cisco, I love you
```

已经做好的包位于：
http://ldd6410.googlecode.com/files/ldd6410-skyeye-pack-with-module.tar.gz