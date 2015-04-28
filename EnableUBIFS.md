讲解ubifs内核配置、映像制作、烧录与挂载的过程。

# 内核配置 #
配置内核选中UBIFS
```
Device Drivers   --->
  Memory Technology Device(MTD) support  --->
    UBI - Unsorted block images --->
      <*> Enable UBI
      <*> MTD devices emulation driver(gluebi)(NEW)
File systems   --->
  Miscellaneous filesystems  --->
    <*> UBIFS file system support
```
# 制作映像 #
在PC上运行如下命令得到UBI image
```
mkfs.ubifs -r rootfs -m 2048 -e 129024 -c 4096 -o ubifs.img
ubinize -o ubi.img -m 2048 -s 512 -p 128KiB ubifs.conf
```
对应的flash的page size为2048，subpage size为512,eraseblock size为128KB。rootfs为要制作的根文件系统的目录

# 烧录和运行UBIFS #
## 烧录 ##
```
root:/> ubiformat /dev/mtd1 -s 512 -f ubi.img
ubiformat: mtd1 (NAND), size 130023424 bytes (124.0 MiB), 131072 eraseblocks of 131072 bytes (128.0 KiB), min. I/O size 2048 bytes
libscan: scanning eraseblock 991 -- 100 % complete  
ubiformat: 992 eraseblocks are supposedly empty
ubiformat: flashing eraseblock 15 -- 100 % complete  
ubiformat: formatting eraseblock 991 -- 100 % complete
```
## attach ##
```
root:/> ubiattach /dev/ubi_ctrl -m 1
UBI: attaching mtd1 to ubi0
UBI: physical eraseblock size:   131072 bytes (128 KiB)
UBI: logical eraseblock size:    129024 bytes
UBI: smallest flash I/O unit:    2048
UBI: sub-page size:              512
UBI: VID header offset:          512 (aligned 512)
UBI: data offset:                2048
UBI: volume 0 ("rootfs") re-sized from 17 to 979 LEBs
UBI: attached mtd1 to ubi0
UBI: MTD device name:            "file system(nand)"
UBI: MTD device size:            124 MiB
UBI: number of good PEBs:        992
UBI: number of bad PEBs:         0
UBI: max. allowed volumes:       128
UBI: wear-leveling threshold:    4096
UBI: number of internal volumes: 1
UBI: number of user volumes:     1
UBI: available PEBs:             0
UBI: total number of reserved PEBs: 992
UBI: number of PEBs reserved for bad PEB handling: 9
UBI: max/mean erase counter: 0/0
UBI: image sequence number: 0
UBI: background thread "ubi_bgt0d" started, PID 179
UBI device number 0, total 992 LEBs (127991808 bytes, 122.1 MiB), available 0 LEBs (0 bytes), LEB size 129024 bytes (126.0 KiB)
```
## 挂载 ##
```
root:/> mount -t ubifs ubi0:rootfs /mnt
UBIFS: mounted UBI device 0, volume 0, name "rootfs"
UBIFS: file system size:   124895232 bytes (121968 KiB, 119 MiB, 968 LEBs)
UBIFS: journal size:       9033728 bytes (8822 KiB, 8 MiB, 71 LEBs)
UBIFS: media format:       w4/r0 (latest is w4/r0)
UBIFS: default compressor: lzo
UBIFS: reserved for root:  0 bytes (0 KiB)
```
## 检查mount结果 ##
```
root:/> mount
rootfs on / type rootfs (rw)
proc on /proc type proc (rw,nosuid,nodev,noexec,relatime)
sysfs on /sys type sysfs (rw,nosuid,nodev,noexec,relatime)
...
ubi0:rootfs on /mnt type ubifs (rw,relatime)
```

```
root:/mnt> ubinfo
UBI version:                    1
Count of UBI devices:           1
UBI control device major/minor: 10:63
Present UBI devices:            ubi0
```