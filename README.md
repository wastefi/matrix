# matrix
2025 NJUST Cpp Course Design Assignment

This is just a simple matrix template. It's my Cpp Course Design Assignment.

It supports addition, subtraction, and multiplication, but does not include determinants or inverses.

In Competitive Programming, we usually need `modint`, maybe you could fill it in directly.

(Nobody cares your stupid code)

There is also a simple user interaction system. The following is the help document.

```txt
Usage:
    new <name> <row_size> <col_size> [-p] [-t] [-o]
    new <name> <size> [-p] [-t] [-o]
    copy <from_name> <to_name>
    add <name1> <name2> [-p] [-s]
    sub <name1> <name2> [-p] [-s]
    mul <name1> <name2> [-p] [-s]
    pow <name> <power> [-p] [-s]
    print <name>
    delete <name>
    quit
Options:
    -p              print the matrix after generating
    -t <type>       set the data type of matrix [default: int]
        int, i          integer
        double, d       double precision floating-point number
        modint          (to be done)
    -o <option>     set the option for generating matrix [default: 0]
        0               zero matrix
        1               1s matrix
        I               identity matrix
        R               random matrix in range [-10, 10]
        r               read from input
    -s <new_name>   store the result in "new_name"

```

