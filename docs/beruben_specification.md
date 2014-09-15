Beruben Language Specification v1.0
===================================

In this specification, 'a' is the topmost item on the stack,
'b' is the next item, ...

Basics
------

Beruben is a 2D language.
Each character is a single instruction that does something.

```
> 0             v  Write comments in places that will never be run.
v "hello world" <  Add "hello world" to the stack.
> , v              Printing out loop
    :
^   _ @
```

Modules
-------

Upper case letters are reserved for use by modules.

Here is an example of importing a module in beruben.
The import lines MUST be at the top of the file.
The import lines are included as part of the row count,
but navigating to an import line will result in an error.

```
v Import "test.rb" as A
> 25*: A
```

Instructions
============

Control
-------
>	Right
<	Left
v	Down
^	Up
#	Jump over next command
[	Skip til next ]
]	End skip
_	If a is 0, go right, else go left
|	If a is 0, go up, else go down
g	Gets value from code at (b, a)
p	Puts value c to code at (b, a)
t	Start new thread (see threading)
m	Mutex at code position (b, a) (see threading)
n	Release mutex at code position (b, a) (see threading)
@	Exit thread

Operations
----------
-	b-a
+	b+a
/	b/a
*	b*a
%	b%a
=	b==a, adds 0(true) or 1(false) to stack
!	Not. [if(a>0) then 0, else 1;]
:	Duplicate a

Input Output
------------
1	puts number on stack
"str"	puts a string on the stack
i	Input Character
n	Input Number
, 	Output Character
. 	Output Number

If file is open:
r	Read character from file
y	Read number from file
w	Write character to file
e	Write number to file
c	Close file

If file is closed:
o	Open file for reading (filename is taken off the stack until 0 is reached)
a	Open file for writing

Please note that once you read or write from a file, the mode is set.

Threading
=========

When a script starts, a thread is automatically created at positioned at (0, 0)
When a thread encounters the 't' instruction, it duplicates. A new thread goes right,
the old thread goes left. The stack is copied to the new thread.

```
 v
vt                        11m 11n @

  A thread mutexes below, blocking
  the above thread from ending
  until it releases the mutex.

> 11m                             v
v                                 <
> 11n @   Release mutex and end
```

Examples
========

File IO
-------
```
v                     Set up program
0
> ~:v                 Accept filepath from user, and count length
^   _  o > r : v      Print characters in file
         ^ ,   _  @
```
             
