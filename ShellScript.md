# 演示程序 #
```
#!/bin/sh -x
#由反引号括起来的也是一条命令，Shell先执行该命令，然后将输出结果立刻代换到当前命令行中。例如定义一个变量存放date命令的输出：
echo `date`
#命令代换也可以用$()表示：
echo $(date)

#如果一个变量叫做VARNAME，用${VARNAME}可以表示它的值，在不引起歧义的情况下也可以用$VARNAME表示它的值。通过以下例子比较这两种表示法的不同：
echo $SHELL
echo $SHELLabc
echo ${SHELL}abc

#单引号用于保持引号内所有字符的字面值，即使引号内的\和回车也不例外，但是字符串中不能出现单引号
echo '$SHELL'
echo "$SHELL"
echo '$SHELL
	  hello world'
#双引号用于保持引号内所有字符的字面值（回车也不例外），除以下情况外：
#$加变量名可以取变量的值
#反引号仍表示命令替换
#\$表示$的字面值
#\`表示`的字面值
#\"表示"的字面值
#\\表示\的字面值
echo "$SHELL
	  hello world"
echo "\$SHELL
	  hello world"

#命令test或[可以测试一个条件是否成立，如果测试结果为真，则该命令的Exit Status为0，如果测试结果为假，则命令的Exit Status为1
VAR=2
test $VAR -gt 1
echo $?

test $VAR -gt 3
echo $?

[ $VAR -gt 3 ]
echo $?
#存在Desktop目录且VAR等于abc
VAR=abc
[ -d Desktop -a $VAR = 'abc' ]
echo $?

#在Shell中用if、then、elif、else、fi这几条命令实现分支控制
echo "Is it morning? Please answer yes or no."
read YES_OR_NO
if [ "$YES_OR_NO" = "yes" ]; then
	echo "Good morning!"
elif [ "$YES_OR_NO" = "no" ]; then
	echo "Good afternoon!"
else
	echo "Sorry, $YES_OR_NO not recognized. Enter yes or no."
	exit 1
fi

#C语言的case只能匹配整型或字符型常量表达式，而Shell脚本的case可以匹配字符串和Wildcard，每个匹配分支可以有若干条命令，末尾为;;
echo "Is it morning? Please answer yes or no."
read YES_OR_NO
case "$YES_OR_NO" in
	yes|y|Yes|YES)
	echo "Good Morning!";;
	[nN]*)
	echo "Good Afternoon!";;
	*)
	echo "Sorry, $YES_OR_NO not recognized. Enter yes or no."
	exit 1;;
esac

#FRUIT是一个循环变量，第一次循环$FRUIT的取值是apple，第二次取值是banana，第三次取值是pear
for FRUIT in apple banana pear; do
	  echo "I like $FRUIT"
done

#while的用法和C语言类似
echo "Enter password:"
read TRY
while [ "$TRY" != "secret" ]; do
	echo "Sorry, try again"
	read TRY
done

COUNTER=1
while [ "$COUNTER" -lt 10 ]; do
	echo "COUNTER is $COUNTER"
	COUNTER=$(($COUNTER+1))
done

#$0	相当于C语言main函数的argv[0]
echo "The program $0 is now running"
#$1、$2...	这些称为位置参数（Positional Parameter），相当于C语言main函数的argv[1]、argv[2]...
echo "The first parameter is $1"
echo "The second parameter is $2"
$@	表示参数列表"$1" "$2" ...，例如可以用在for循环中的in后面
for PARAM in $@; do
	echo "PARAM $PARAM"
done

echo "The parameter list is $@"
#位置参数可以用shift命令左移
shift
echo "The first parameter is $1"
echo "The second parameter is $2"
echo "The parameter list is $@"

#数就像是迷你脚本，调用函数时可以传任意个参数，在函数内同样是用$0、$1、$2等变量来提取参数
#函数中的位置参数相当于函数的局部变量，改变这些变量并不会影响函数外面的$0、$1、$2等变量
#函数中可以用return命令返回，如果return后面跟一个数字则表示函数的Exit Status

is_directory()
{
	DIR_NAME=$1
	if [ ! -d $DIR_NAME ]; then
		return 1
	else
		return 0
	fi
}

for DIR in "$@"; do
	if is_directory "$DIR"
	then :
	else
		echo "$DIR doesn't exist. Creating it now..."
		mkdir $DIR > /dev/null 2>&1
		if [ $? -ne 0 ]; then
			echo "Cannot create directory $DIR"
			exit 1
		fi
	fi
done
```
# sh与bash区别 #
在我们所使用的系统当中，使用sh调用执行脚本，相当于打开了bash的POSIX标准模式 （等效于bash的 --posix 参数）

一般的，sh是bash的“子集” （不是子集的部分，具体区别见下的“Things sh has that bash does not”）

例子：
```
[wwy@sf-watch test]$ cat t2.sh 
#!/bin/bash
diff <(echo xxx) <(echo yyy) # 此语法包含bash的特性，不属于sh的POSIX标准

    [wwy@sf-watch test]$ bash -x ./t2.sh # 使用bash 调用，不会出问题
+ diff /dev/fd/63 /dev/fd/62
++ echo xxx
++ echo yyy
1c1
< xxx
---
>    yyy
[wwy@sf-watch test]$ sh ./t2.sh    # 而用sh调用，报错如下
./t2.sh: line 3: syntax error near unexpected token `('
./t2.sh: line 3: `diff <(echo xxx) <(echo yyy)'
[wwy@sf-watch test]$ echo $? 
```
2

但是，在我们的linux系统中，sh是bash的一个软链接：

[wangweiyu@ComSeOp mon]$ which sh
/bin/sh
[wangweiyu@ComSeOp mon]$ ls -l /bin/sh
lrwxrwxrwx  1 root root 4 Mar 21  2007 /bin/sh -> bash

那为什么上面的例子中还会出现问题呢？原因在于： bash程序执行，当“$0”是“sh”的时候，则要求下面的代码遵循一定的规范，当不符合规范的语法存在时，则会报错， 所以可以这样理解， “sh”并不是一个程序，而是一种标准（POSIX），这种标准，在一定程度上保证了脚本的跨系统性（跨UNIX系统）

下面的内容详细的说明了bash与sh在语法等方面的具体差异：
```
Things bash has that sh does not:

   long invocation options
   [+-]O invocation option
   -l invocation option
   `!' reserved word to invert pipeline return value
   `time' reserved word to time pipelines and shell builtins
   the `function' reserved word
   the `select' compound command and reserved word
   arithmetic for command: for ((expr1 ; expr2; expr3 )); do list; done
   new $'...' and $"..." quoting
   the $(...) form of command substitution
   the $(<filename) form of command substitution, equivalent to
      $(cat filename)
   the ${#param} parameter value length operator
   the ${!param} indirect parameter expansion operator
   the ${!param*} prefix expansion operator
   the ${param:offset[:length]} parameter substring operator
   the ${param/pat[/string]} parameter pattern substitution operator
   expansions to perform substring removal (${p%[%]w}, ${p#[#]w})
   expansion of positional parameters beyond $9 with ${num}
   variables: BASH, BASH_VERSION, BASH_VERSINFO, UID, EUID, REPLY,
         TIMEFORMAT, PPID, PWD, OLDPWD, SHLVL, RANDOM, SECONDS,
         LINENO, HISTCMD, HOSTTYPE, OSTYPE, MACHTYPE, HOSTNAME,
         ENV, PS3, PS4, DIRSTACK, PIPESTATUS, HISTSIZE, HISTFILE,
         HISTFILESIZE, HISTCONTROL, HISTIGNORE, GLOBIGNORE, GROUPS,
         PROMPT_COMMAND, FCEDIT, FIGNORE, IGNOREEOF, INPUTRC,
         SHELLOPTS, OPTERR, HOSTFILE, TMOUT, FUNCNAME, histchars,
         auto_resume
   DEBUG trap
   ERR trap
   variable arrays with new compound assignment syntax
   redirections: <>, &>, >|, <<<, [n]<&word-, [n]>&word-
   prompt string special char translation and variable expansion
   auto-export of variables in initial environment
   command search finds functions before builtins
   bash return builtin will exit a file sourced with `.'
   builtins: cd -/-L/-P, exec -l/-c/-a, echo -e/-E, hash -d/-l/-p/-t.
        export -n/-f/-p/name=value, pwd -L/-P,
        read -e/-p/-a/-t/-n/-d/-s/-u,
        readonly -a/-f/name=value, trap -l, set +o,
        set -b/-m/-o option/-h/-p/-B/-C/-H/-P,
        unset -f/-v, ulimit -i/-m/-p/-q/-u/-x,
        type -a/-p/-t/-f/-P, suspend -f, kill -n,
        test -o optname/s1 == s2/s1 < s2/s1 > s2/-nt/-ot/-ef/-O/-G/-S
   bash reads ~/.bashrc for interactive shells, $ENV for non-interactive
   bash restricted shell mode is more extensive
   bash allows functions and variables with the same name
   brace expansion
   tilde expansion
   arithmetic expansion with $((...)) and `let' builtin
   the `[[...]]' extended conditional command
   process substitution
   aliases and alias/unalias builtins
   local variables in functions and `local' builtin
   readline and command-line editing with programmable completion
   command history and history/fc builtins
   csh-like history expansion
   other new bash builtins: bind, command, compgen, complete, builtin,
             declare/typeset, dirs, enable, fc, help,
             history, logout, popd, pushd, disown, shopt,
             printf
   exported functions
   filename generation when using output redirection (command >a*)
   POSIX.2-style globbing character classes
   POSIX.2-style globbing equivalence classes
   POSIX.2-style globbing collating symbols
   egrep-like extended pattern matching operators
   case-insensitive pattern matching and globbing
   variable assignments preceding commands affect only that command,
      even for builtins and functions
   posix mode and strict posix conformance
   redirection to /dev/fd/N, /dev/stdin, /dev/stdout, /dev/stderr,
      /dev/tcp/host/port, /dev/udp/host/port
   debugger support, including `caller' builtin and new variables
   RETURN trap
   the `+=' assignment operator

Things sh has that bash does not:
   uses variable SHACCT to do shell accounting
   includes `stop' builtin (bash can use alias stop='kill -s STOP')
   `newgrp' builtin
   turns on job control if called as `jsh'
   $TIMEOUT (like bash $TMOUT)
   `^' is a synonym for `|'
   new SVR4.2 sh builtins: mldmode, priv

Implementation differences:
   redirection to/from compound commands causes sh to create a subshell
   bash does not allow unbalanced quotes; sh silently inserts them at EOF
   bash does not mess with signal 11
   sh sets (euid, egid) to (uid, gid) if -p not supplied and uid < 100
   bash splits only the results of expansions on IFS, using POSIX.2
      field splitting rules; sh splits all words on IFS
   sh does not allow MAILCHECK to be unset (?)
   sh does not allow traps on SIGALRM or SIGCHLD
   bash allows multiple option arguments when invoked (e.g. -x -v);
      sh allows only a single option argument (`sh -x -v' attempts
      to open a file named `-v', and, on SunOS 4.1.4, dumps core.
      On Solaris 2.4 and earlier versions, sh goes into an infinite
      loop.)
   sh exits a script if any builtin fails; bash exits only if one of
      the POSIX.2 `special' builtins fails
```
调用相关：

在脚本的调用方面（interactive、login相关），bash与sh也是存在差异 以下是详细说明（假如被调用执行的脚本名字叫xxx.sh）

BASH：

1、   交互式的登录shell （bash –il xxx.sh）
载入的信息：
```
/etc/profile
~/.bash_profile（ ->  ~/.bashrc  ->  /etc/bashrc）
~/.bash_login
~/.profile
```
2、非交互式的登录shell （bash –l xxx.sh）
载入的信息：
```
/etc/profile
~/.bash_profile （ ->  ~/.bashrc  ->  /etc/bashrc）
~/.bash_login
~/.profile
$BASH_ENV
```
3、交互式的非登录shell （bash –i xxx.sh）
载入的信息：
```
~/.bashrc （ ->  /etc/bashrc）
```
4、非交互式的非登录shell （bash xxx.sh）
载入的信息：
```
$BASH_ENV
 
SH：
```
1、交互式的登录shell
载入的信息：
```
/etc/profile
~/.profile
```
2、非交互式的登录shell
载入的信息：
```
/etc/profile
~/.profile
```
3、交互式的非登录shell
载入的信息：
```
$ENV
```
4、非交互式的非登录shell
载入的信息：
nothing

由此可以看出，最主要的区别在于相关配置文件的是否载入， 而这些配置的是否载入，也就导致了很多默认选项的差异 （具体请仔细查看~/.bash\_profile 等文件） 如：
```
[wangweiyu@ComSeOp ~]$ grep ulimit /etc/profile    
ulimit -S -c unlimited > /dev/null 2>&1
```
即，如果/etc/profile没有被载入，则不会产生core dump

值得一提的是，使用ssh远程执行命令， 远端sshd进程通过“bash –c”的方式来执行命令（即“非交互式的非登录shell”） 所以这一点，和登录之后再在本地执行执行命令，就存在了一定的差异

如：
```
[wangweiyu@ComSeOp ~]$ ssh wangweiyu@127.0.0.1 'echo $-'
wangweiyu@127.0.0.1's password: 
hBc
[wangweiyu@ComSeOp ~]$ echo $-
himBH
[wangweiyu@ComSeOp ~]$ ssh wangweiyu@127.0.0.1 'echo $0'
wangweiyu@127.0.0.1's password: 
bash
[wangweiyu@ComSeOp ~]$ echo $0
-bash
```
注： “$-” 中含有“i”代表“交互式shell” “$0”的显示结果为“-bash”，bash前面多个“-”，代表“登录shell” 没有“i“和“-”的，是“非交互式的非登录shell”

另外还有一点，虽然ssh远程执行的命令是“非交互式的非登录shell”，但在执行命令之前，ssh的那一次登录本身是“交互式的登录shell”，所以其会先读一下“~/.bash\_profile”

如：
```
[wangweiyu@ComSeOp ~]$ cat .bashrc 
# .bashrc
# User specific aliases and functions
# Source global definitions
if [ -f /etc/bashrc ]; then
        . /etc/bashrc
fi
echo 'xxx'

[wangweiyu@ComSeOp ~]$ ssh wangweiyu@127.0.0.1 'echo $-'
wangweiyu@127.0.0.1's password: 
xxx
hBc
```
这一点，衍生出一个关于scp的问题，scp在传输数据之前，会先进行一次ssh登录， 而当.bashrc文件有输出的时候，则会导致scp失败！原因是解析返回的数据包出现混乱

如：
```
[wangweiyu@ComSeOp ~]$ cat .bashrc 
# .bashrc
# User specific aliases and functions
# Source global definitions
if [ -f /etc/bashrc ]; then
        . /etc/bashrc
fi
echo 'xxx'
[wangweiyu@ComSeOp ~]$ scp file wangweiyu@127.0.0.1:/tmp
wangweiyu@127.0.0.1's password: 
xxx
[wangweiyu@ComSeOp ~]$ echo $?
1
[wangweiyu@ComSeOp ~]$ ls /tmp/
[wangweiyu@ComSeOp ~]$
```