# tryC - a small interpreter written by C

tryC是一个用c语言手搓的非常简单的解释器，大约500+行：

采用递归下降法进行语法分析，不显式构建语法树，不生成中间代码或目标代码，在语法分析的同时进行解释执行（所以可能速度实在很慢）；

- 使用弱类型定义变量，支持的数据类型：双精度浮点数、字符型、字符串、浮点数数组
- 支持函数和变量的定义、函数的递归调用、嵌套作用域

    build:
        gcc -o tryc tryC.c

    usage: tryc [-d] filename

[用c语言手搓一个600行的类c语言解释器: 给编程初学者的解释器教程（1）- 目标和前言](https://blog.csdn.net/qq_42779423/article/details/105938297)

[用c语言手搓一个600行的类c语言解释器: 给编程初学者的解释器教程（2）- 简介和设计](https://blog.csdn.net/qq_42779423/article/details/105939788)