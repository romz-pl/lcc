Cited from the book Dave Hanson and Christopher W. Fraser 
"A Retargetable C Compiler: Design and Implementation" 
(Addison-Wesley, 1995, ISBN 0-8053-1670-1)

Strings
=======
Strings are created for identifiers, constants, registers, and so on. Strings
are compared often; for example, when a symbol table is searched for
an identifier.

The most common uses of strings are provided by the functions 
exported by **string** library:
```c++
char *string (char *str);
char *stringn (char *str, int len);
char *stringd (int n);
```

Each of these functions returns a pointer to a permanently allocated
string. The function **string** makes a copy of the null-terminated string **str**, 
the function **stringn**
makes a copy of the **len** bytes in **str**, and the function **stringd** converts **n** to its
decimal representation and returns that string.


These functions save exactly one copy of each distinct string, so two
strings returned by these functions can be compared for equality by 
comparing their addresses. These semantics simplify comparisons and save
space, and **stringn** can handle strings with embedded null characters.
