#Android如何编译特定product.

# 定制新设备 #

# 一、Android如何编译特定product #

> 首先来看看Android Makefile是如何解析环境变量TARGET\_PRODUCT的。
> Android Makefile 的引用关系是这样的：
```
   Makefile  -> build/core/main.mk -> build/core/config.mk ->build/core/envsetup.mk
   -> build/core/product_config.mk
```
> 在build/core/product\_config.mk 中编译系统首先调用 `build/core/product.mk`中定义的函数  `get-all-product-makefiles`，来遍历整个device 的子目录， 找到device下所有的  `AndroidProducts.mk`, 不同子目录下的`AndroidProducts.mk `中定义了不同的` PRODUCT_NAME`,` PRODUCT_DEVICE `等信息，（我们也可以通过 打开`build/core/product_config.mk`中的`#$(dump-products)`语句使控制台编译的时候输出所有product 的信息），接着 `build/core/product_config.mk `会调用`resolve-short-product-name`将TARGET\_PRODUCT匹配的`AndroidProducts.mk` 中定义的 PRODUCT\_DEVICE 赋值给TARGET\_DEVICE。有了这个TARGET\_DEVICE, 再回到 `build/core/config.mk`，会`include $(TARGET_DEVCIE)/BoardConfig.mk`
```
     board_config_mk := \
       $(strip $(wildcard \
       $(SRC_TARGET_DIR)/board/$(TARGET_DEVICE)/BoardConfig.mk \
       vendor/*/$(TARGET_DEVICE)/BoardConfig.mk \
      ))
     include $(board_config_mk)
```
> 而这个配置文件`BoardConfig.mk `决定了目标系统编译属性，比如使用ALSA还是不是 GENERIC\_AUDIO等等, 另外在这里TARGET\_DEVICE 宏也决定了TARGET\_DEVICE\_DIR， 因为TARGET\_DEVICE\_DIR 取的是上面提到的`BoardConfig.mk` 的路径。
```
     TARGET_DEVICE_DIR := $(patsubst %/,%,$(dir $(board_config_mk)))
```
> 当然Android 的Ob目标输出也是由TARGET\_DEVICE决定，见`build/core/envsetup.mk`
```
     TARGET_OUT_ROOT_release := $(OUT_DIR)/target
     TARGET_OUT_ROOT_debug := $(DEBUG_OUT_DIR)/target
     TARGET_OUT_ROOT := $(TARGET_OUT_ROOT_$(TARGET_BUILD_TYPE))
     TARGET_PRODUCT_OUT_ROOT := $(TARGET_OUT_ROOT)/product
     PRODUCT_OUT := $(TARGET_PRODUCT_OUT_ROOT)/$(TARGET_DEVICE)
```
> 再回到 `build/core/main.mk`, 编译系统接着做的一个件事情是，遍历所有字目录，找到所有 `Android.mk`文件，并将这些`Android.mk`文件include 进来
```
     #
     # Typical build; include any Android.mk files we can find.
     #
     subdir_makefiles := \
      $(shell build/tools/findleaves.py --prune=out --prune=.repo --prune=.git $(subdirs) Android.mk)
     include $(subdir_makefiles)
```
> 再来看其中的`./build/target/board/Android.mk`，对了它引用了
```
     include $(TARGET_DEVICE_DIR)/AndroidBoard.mk
```
> 由上面TARGET\_DEVICE\_DIR的定义，这下又进入了device下TARGET\_DEVICE指向的目录了，这个mk文件中定义了特定Product需要编译和安装的app 和 script.

# 二、创建新设备的配置架构 #
```
  device-|
         |-<company_name>-|
                          |-<board_name>-|
                                         |-AndroidBoard.mk
                                         |-BoardConfig.mk                      
                          |-products-|
                                     |-AndroidProducts.mk
                                     |-<first_product_name>.mk
                                     |-<second_product_name>.mk
```

# 三、具体配置分析 #

  * 1、`AndroidProducts.mk`
> > 在`AndroidProducts.mk`中只会包括PRODUCT\_MAKEFILES变量，一般格式如下：
```
    PRODUCT_MAKEFILES := \
      $(LOCAL_DIR)/<first_product_name>.mk
```
  * 2、`<first_product_name>.mk`
> > 在`<first_product_name>.mk`中一般包括如下配置：
```
    $(call inherit-product, &(SRC_TARGET_DIR)/product/languages_full.mk)
      
    PRODUCT_BRAND := freescale
    PRODUCT_MANUFACTUER := freescale
    PRODUCT_NAME := <first_product_name>
    PRODUCT_DEVICE := <first_product_name>
    PRODUCT_COPY_FILE += \
          device/mine/mine_mx53/init.rc:root/init.freescale.rc
```
> > 如果选择通用package的话，添加`generic.mk`:
```
    $(call inherit-product, &(SRC_TARGET_DIR)/product/generic.mk)
```
> > 如果要重新选择安装的package,需要重定义
```
      PRODUCT_PACKAGES := \
          Music \
          Email \
          ...
```
  * 3、`AndroidBoard.mk`
> > 一般会在此添加Keypad的定义：
```
    LOCAL_PATH := $(call my-dir)
    file := $(TARGET_OUT_KEYLAYOUT)/mxckpd.kl
    ALL_PREBUILT += $(file)
    $(file): $(LOCAL_PATH)/mxckpd.kl | $(ACP)
         $(transform-prebuilt-to-target)
    include $(CLEAR_VARS)
    LOCAL_SRC_FILES := mxckpd.kcm
    include $(BUILD_KEY_CHAR_MAP)
```
  * 4、`BoardConfig.mk`
> > 一般在此定义board的特性，例如：
```
    TARGET_CPU_ABI := armeabi
    TARGET_NO_BOOTLOADER := true
    TARGET_NO_KERNEL := true
    TARGET_PROVIDES_INIT_RC := true
    BOARD_HAVE_WIFI := true
    BOARD_HAVE_MODEM := true
    BOARD_USES_ALSA_AUDIO := true
    ...
```