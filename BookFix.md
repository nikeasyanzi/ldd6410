
```
1.
代码清单7.5第27行globalfifo_devp应该为globalmem_devp；
7.8节总结上面最后一行的globalfifo_devp也应该为globalmem_devp；
8.1.2节第一段中出现的globalfifo都改为globalmem，接下来第2段第1个globalfifo也改为globalmem；
2. 
代码清单8.6上一段的上一段文字中出现的所有__add_wait_queue()都改为__set_current_state()；
3. 
140页的倒数第6行的：
spin_lock_irqsave() = spin_unlock() + local_irq_save()
这一行有错误，应该是:
spin_lock_irqsave() = spin_lock() + local_irq_save()
4.
书中第53页倒数第11行，有个拼写错误，原文是“因此，需在/etc/modprobe.conf配置文件中进行响应修改”，这里的“响应”应该是“相应”；
5. 
代码清单6.18第38行中globalmem_setup_cdev(globalmem_devp[0],0);应该改为globalmem_setup_cdev(&globalmem_devp[0],0);
6. 
代码清单6.18第42行中fail_malloc:unregister_chrdev_region(devno,1);应该改为fail_malloc:unregister_chrdev_region(devno,2);
7. 
书中140页上第4行中spinlock_t  spin ;应该是spinlock_t  lock;而后的spin_lock(lock)改为spin_lock(&lock)；spin_trylock(lock)改为spin_trylock(&lock)；spin_unlock(lock)改为spin_unlock(&lock).
8.
代码清单1.4 第31行“/*写设备：可以不需要*/ ”注释应改为“/*读写设备：可以不需要*/。
9. 
代码清单6.8和代码清单6.17的第17行中的static globalmem_major宜改为static int globalmem_major。
10. 
14页第135行light_init();改为light_gpio_init();
11.
383页，图15.10下面文字的SAA7113H读写地址反了，应该是“以0x4B地址可读..........，以0x4A可写....”。原文正好相反。
12.
第202页，48行注释中的“globalfifo”应该删除。 
13.
第127页最顶一行幻数为“T”，改为幻数是0（注意0上面不加引号）。
14. 
第245页代码清单12.6第11行 中的&button_irq改为s3c2410_eint_key（注意也要删除&）;代码清单12.7第9行中的button_irq改为s3c2410_eint_key。
代码清单12.8第6行的注释“状态为按下”改为“状态为DOWNX，即不确定是否为按下”。
15.
代码清单8.9第13和73行的if改成while。
16.
图17.3中间的字样“AC编解码”改为“AC97编解码”。
17.
第142页，最后一个代码段上一段的上一段，“在对共享资源进行读取之前，应该先调用写锁定函数”，应该改为“在对共享资源进行写之前，应该先调用写锁定函数”。
18.
第131页第1个代码段下面第1行:“除了在globalmem_open()函数中通过filp->private_data=globalmem_devp语句(见第31行)”中的"第31行"应为"第30行"。
19.
第147页右边的第22行：
call_rcu(&e->rcu,audit_free_rule,e);
函数去掉第3个参数e； 即改为：
call_rcu(&e->rcu,audit_free_rule);
第148页右边涂黑的
call_rcu(&e->rcu,audit_free_rule,e);
也改为：
call_rcu(&e->rcu,audit_free_rule);
20.
第145页小标题“4.挂接回调”下的第4段“call_ruc_bh()函数的功能几乎与...”改为“call_rcu_bh()函数的功能几乎与...”
21.
第175页，signal原型声明有错误，原来为 
void(*signal(int signum,void(*handler))(int))(int) 
正确的声明应该是
void(*signal(int signum,void(*handler)(int)))(int) 。
22.
第490页图18.5错，表头的中文翻译不变，表体中请按照下图画红圈的位置进行修改：
 
第491页第1行页也相应进行如下修正：
upper_margin = 216，lower_margin = 2，hsync_len = 28，vsync_len = 2。即upper_margin 为16而不是原文的2，hsync_len 为8而不是原文的2。
23.
代码清单9.10第3行删除，第4、5、6、7行的行号减1。
24.
第124页顶部代码清单第14行，dev->mem应该为dev.mem
25.
192页倒数第6行，“返回-INVAL”改为“返回-EINVAL”
26.
118页代码清单6.5上一段“工程师通常习惯将设备定义为”改为“工程师通常习惯为设备定义”
```