# tryC - a small interpreter written by C

tryC是一个用c语言手搓的非常简单的解释器：

采用递归下降法进行语法分析，和语法制导的语义分析

- 支持的数据类型：双精度浮点数、字符型、字符串、数组
- 支持函数和变量的定义、函数的递归调用、嵌套作用域

    build:
        gcc -o tryc tryC.c

    usage: tryc [-d] {filename}