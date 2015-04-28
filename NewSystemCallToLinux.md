例子中的2个系统调用一个获取进程真实的PID，一个获取进程动态优先级。

# 为i386平台新增系统调用 #
## i386平台内核修改 ##

修改内核系统调用表：
```
--- a/arch/x86/kernel/syscall_table_32.S
+++ b/arch/x86/kernel/syscall_table_32.S
@@ -336,3 +336,7 @@ ENTRY(sys_call_table)
    .long sys_pwritev
    .long sys_rt_tgsigqueueinfo /* 335 */
    .long sys_perf_event_open
+
+   /* new system for study, hack by Barry Song */
+   .long sys_getrpid
+   .long sys_getprpriority
```

修改系统调用号定义：
```
--- a/arch/x86/include/asm/unistd_32.h
+++ b/arch/x86/include/asm/unistd_32.h
@@ -343,9 +343,15 @@
 #define __NR_rt_tgsigqueueinfo 335
 #define __NR_perf_event_open   336

+/* new system call for return real pid and priority 
+ * hack by Baohua Song
+ */  
+#define __NR_getrpid            337
+#define __NR_getprpriority      338
+
 #ifdef __KERNEL__
 
-#define NR_syscalls 337
+#define NR_syscalls 339
 
 #define __ARCH_WANT_IPC_PARSE_VERSION
 #define __ARCH_WANT_OLD_READDIR
```
实现系统调用：
```
--- a/kernel/sys.c
+++ b/kernel/sys.c
@@ -205,6 +205,16 @@ out:
 }

 /*
+ * return dynamic prio of current process
+ * hack by Barry Song
+ */
+SYSCALL_DEFINE0(getprpriority)
+{
+   printk("%s, current static prio: %d, dynamic prio: %d\n", __func__, current->static_prio, current->prio);
+   return (20 - (current->prio - MAX_RT_PRIO - 20));
+}
+
+/*
  * Ugh. To avoid negative return values, "getpriority()" will
  * not return the normal nice-value, but a negated value that
  * has been offset by 20 (ie it returns 40..1 instead of -20..19)
```

```
--- a/kernel/timer.c
+++ b/kernel/timer.c
@@ -1276,6 +1276,15 @@ SYSCALL_DEFINE0(getpid)
    return task_tgid_vnr(current);
 }

+/** 
+ * sys_getrpid - return the pid of the current process
+ * hack by Barry Song
+ */  
+SYSCALL_DEFINE0(sys_getrpid)
+{
+   return current->pid;    
+}
+
 /*
  * Accessing ->real_parent is not SMP-safe, it could
  * change from under us. However, we can use a stale
```

早先的内核系统调用新增方法有些不一样，如Linux 2.6.15的同样功能用如下方法实现：
```
kernel/sys.c

 379 /*
 380  * return dynamic prio of current process
 381  * hack by Baohua Song
 382  */
 383 asmlinkage long sys_getprpriority(void)
 384 {
 385         printk("%s, current static prio: %d, dynamic prio: %d sleep_avg: %ld\n", __func__, current->static_prio, current->prio, current->sleep_avg);
 386         return (20 - (current->prio - MAX_RT_PRIO - 20));
 387 }

kernel/timer.c

 999 /**
1000  * sys_getrpid - return the pid of the current process
1001  * hack by Baohua Song
1002  */
1003 asmlinkage long sys_getrpid(void)
1004 {
1005         return current->pid;
1006 }

include/asm-i386/unistd.h

317 #define __NR_ppoll              309
318 #define __NR_unshare            310
320 /* 
321  * new system call for return real pid and priority 
322  * hack by Baohua Song
323  */
324 #define __NR_getrpid            311
325 #define __NR_getprpriority              312
326 #define NR_syscalls 313 /* 311->313 */

arch/i386/kernel/syscall_table.S
312         .long sys_unshare               /* 310 */
313         .long sys_getrpid               /* 311: hack by Baohua Song */
314         .long sys_getprpriority         /* 312: hack by Baohua Song */ 
```
体现出Linux 2.6内核的代码在小版本之间都存在较大的变化。

## i386平台用户空间修改 ##
```
#define __syscall_return(type, res) \
do { \
        if ((unsigned long)(res) >= (unsigned long)(-(128 + 1))) { \
                res = -1; \
        } \
        return (type) (res); \
} while (0)

#define _syscall0(type,name) \
type name(void) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
        : "=a" (__res) \
        : "0" (__NR_##name)); \
__syscall_return(type,__res); \
}

#define __NR_getprpriority 338
#define __NR_getrpid       337
_syscall0(int, getprpriority)
_syscall0(int, getrpid)
```

# 为arm平台新增系统调用 #
## arm平台内核修改 ##
## arm平台用户空间修改 ##