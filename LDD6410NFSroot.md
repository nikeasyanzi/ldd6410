# 内核配置 #

内核中可使能如下选项：
```
CONFIG_NETWORK_FILESYSTEMS=y
CONFIG_NFS_FS=y
CONFIG_NFS_V3=y
# CONFIG_NFS_V3_ACL is not set
CONFIG_NFS_V4=y
CONFIG_ROOT_NFS=y
# CONFIG_NFSD is not set
CONFIG_LOCKD=y
CONFIG_LOCKD_V4=y
CONFIG_NFS_COMMON=y
CONFIG_SUNRPC=y
CONFIG_SUNRPC_GSS=y
```

# u-boot配置和启动 #
启动配置信息如下:
```
root=/dev/nfs ip=192.168.1.20:192.168.1.111:192.168.1.1:255.255.255.0::eth0:off nfsroot=192.168.1.111:/home/nfs,nolock,proto=tcp console=ttySAC0,115200
```
其中192.168.1.20为目标板IP，192.168.1.111为主机IP，192.168.1.1是网关IP，255.255.255.0为子网掩码。/home/nfs是主机NFS根目录。
通过在uboot中运行如下命令完成：
```
LDD6410 # set bootargs root=/dev/nfs ip=192.168.1.20:192.168.1.111:192.168.1.1:255.255.255.0::eth0:off nfsroot=192.168.1.111:/home/nfs,nolock,proto=tcp console=ttySAC0,115200
LDD6410 # 
```
通过NFS启动的启动信息最后一步就是mount nfs，具体如下：
```
eth0: link up, 100Mbps, full-duplex, lpa 0x45E1
IP-Config: Complete:
     device=eth0, addr=192.168.1.20, mask=255.255.255.0, gw=192.168.1.1,
     host=192.168.1.20, domain=, nis-domain=(none),
     bootserver=192.168.1.111, rootserver=192.168.1.111, rootpath=
Looking up port of RPC 100003/2 on 192.168.1.111
Looking up port of RPC 100005/1 on 192.168.1.111
VFS: Mounted root (nfs filesystem).
Freeing init memory: 524K
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