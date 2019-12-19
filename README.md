# Programming-Tests
A variety of programming tests in C, C++, and Python

See the top of the individual files for information on how they were compiled.


## add1

Write a function `int add1(int val)` that returns val+1 without using +'s or -'s anywhere in your code.

Shows the use of bitwise operators.


## parenPermutations
This was an actual test question from [Cryptic Studios](http://www.crypticstudios.com), circa 2004.

Write a function `void parenPermutations(char* expression)` that prints all the possible different results
you can get from different grouping of parentheses in the given expression.

The string 'expression' contains the operators '+', '-', '*' and positive integers
(or possibly no operators and just one integer).
The expression is entirely unparenthesized.
Your function should determine the result of every possible parenthesization of the expression
and print the distinct ones.

Don't worry about overflowing int or strange formatting of the expression.

Examples:

1. expression `1 + 2 - 3 * 4`

    (((1 + 2) - 3) * 4) = 0<br>
    ((1 + 2) - (3 * 4)) = -9<br>
    ((1 + (2 - 3)) * 4) = 0<br>
    (1 + ((2 - 3) * 4)) = -3<br>
    (1 + (2 - (3 * 4))) = -9

    There were five possible, but two were the same, so your function should print:<br>
    `3 unique { 0, -9, -3 }`

2. expression `1 - 1 + 1`

    ((1 - 1) + 1) = 1<br>
    (1 - (1 + 1)) = -1

    Your function should print:<br>
    `2 unique { 1, -1 }`

3. expression `10`

    `1 unique { 10 }`

4. expression `1 + 2 + 3 * 4 - 5 * 2`

    `18 unique { 38, 14, -12, -36, 20, 5, 17, 0, -3, -15, 32, 11, 31, -8, -9, -29, 19, -1 }`


## reverse_string
Write a function `reverse(char* string)` to reverse a string without allocating a temporary buffer on the heap.
Also no use of variable length array declarations, and no use of the **C++** standard library inside the function.

Shows the use of pointers and how they relate to the **[]** operator,
as well as an understanding of how **C** strings are stored in memory.


## shortest_string
Supplied in **C++** and **Python** versions.

Given an *input* string and an *alphabet*, find the shortest substring in *input* that contains all the characters
in *alphabet*.  The *alphabet* is allowed to contain duplicate characters, which means the substring must contain
at least the same number of duplicates.

Shows use of window search algorithm.

Examples:

* *input* = `this is a test string`  *alphabet* = `tist`

    Output is: `t stri`

* *input* = `geeksforgeeks`  *alphabet* = `ork`

    Output is: `ksfor`





