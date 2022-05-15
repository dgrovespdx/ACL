=== ACLdesign.txt 2022/05/05 ========================================================
/*
 * Copyright (c) 2022 Don Groves
 *
 * Permission to use, copy, modify, and distribute this document for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

Why ACL?
========
ACL is designed to be the programming language I would most like to use. it is 
primarily based upon: 
(1) things I learned during 40+ years of professional programming. 
(2) things I've learned in studying functional language theory. 
(3) My experience using Forth. Forth was the original CL (Concatenative Language) 
    developed by Charles Moore in the 1970s. I used Forth on a project to develop 
    an operating system for a U. S. Navy salvage vessel. I found myself developing 
    working code in less time than with any other language I had previously used. 
    It was a revelation! I attribute this increased efficiency to the interactive, 
    write small functions, and test frequently nature of CLs.
    
ACL is a concatenative language. CLs are described at www.concatenative.org.
CLs use postfix syntax which eliminates the need for parentheses to establish
operator precedence. In this regard, CLs are sometimes referred to as "Lisp without
parentheses."
 
Here are some of the things that may differentiate ACL from other languages:

- ACL is functional. Everything is a function and computation is based on the 
  composition of functions represented as sequences of operations. Some operations 
  have side effects, such as I/O, so ACL is not a pure functional language.

- ACL is homoiconic. Functions and data are distinguished only by how they are used, 
  not by their form, ala Lisp.
  
- ACL code is designed to be as self-documenting as possible. For example, predicates
  (operations which result in true or false) are ended with a ? to show intent.
  Thus, the ACL test for equality is =?, for inequality is !=?, etc.
  
  Any ACL term may be aliased, so if the user would rather use eq, neq, ..., for
  predicates, simply add [eq] alias =?, etc. to ACLlib.txt. In fact, the entire ACL
  vocabulary may be altered to suit the user.
  
- Operator overloading. Primitive operations are overloaded as appropriate. 
  For example, + adds numbers, adds vectors and matricies, and concatenates strings.
  * multiplies numbers and matricies and computes the cross product of vectors.
  
- Type checking: ACL uses dynamic (runtime) type checking. For each primitive, both
  the number and type(s) of stack arguments are tested and error messages are printed
  it they are not as required. 
    
- ACL is interactive. Any function can be entered and run interactively. 
  This promotes a bottom-up, well-factored, implementation strategy that I have 
  found to be the most effective software development paradigm of any I have worked 
  with. In ACL, one never has to write code that cannot be immediately tested and 
  immediate testing promotes bug-free code. The best way to avoid bugs is to not
  include them in the first place.

- ACL has high-level semantics: functions may operate on other functions.
							    anonymous functions (quotations).
								Lambda calculus grammar is supported.
								Combinatorial grammar is supported.
								
The Name
--------
ACL stands for "A Compositional Language." My gratitude to Christopher Diggins for 
suggesting that name. ACL is compositional because sequential ordering of functions
denotes function composition, e.g., the ACL form f g is semantically equivalent to 
the mathematical form g(f).

Alphabet
--------
Symbols in ACL may contain any Unicode character. For example, the value 3.14...  
may be named π and less-than-or-equal-to may named ≤. Any name in ACL may be aliased, 
so using strictly ASCII symbols is always an option.

ACL is case sensitive.

The result of executing a function is shown by a stack diagram;
[S] f => [f(S)] where S represents the stack contents.
				      f represens a function.
				      => is read as "becomes" or "results in."
				      f(S) represents the result of applying f to S.  

EBNF Grammar
------------ 
Notation:	:= denotes “is defined as”
			|  denotes "any one of."
     		{} denotes "one or more of" the bracketed item(s).
			[] denotes the stack with the top element to the right.
			[...]  the ellipsis denotes the "rest of the stack." (that part of the 
			       stack unaffected by the function)
			       
expression	:= term | {term}

term		:= primitive | literal | function 

primitive	:= nullary | unary | binary | tertiary 

literal 	:= number | string | boolean | ⋋ | list | set | vector | matrix 

number  	:= integer | float | rational | complex
			   			   	
integer		:= signed 64-bit (long int)				;numeric types
float 		:= 64-bit IEEE floating point (double)
rational	:= integer/integer						;numerator/denominator			
complex		:= float,float							;real part,imaginary part

string 		:= char | {char}						;strings are enclosed in "".
char		:= ASCII | utf-8 encoded Unicode
boolean     := T|F									;T means true, F means false.

aggr types  := list | set | vector | matrix | ⋋
list 		:= () | [{term}]						; aggregate types
set			:= {} | {{term}}
vector		:= <number | {number}>					;vectors may be math objects or
													;may be tuples, depending on
													;how they are used.
matrix		:= <vector | {vector}>
⋋			:= [term | {term}] 						;a ⋋ can be an annonymous 
													;function or a quotation 
													;(deferred execution)
function 	:= primitive | ⋋ | named ⋋ 
named ⋋		:= ⋋ defun name 						;creates a symbol table entry 
													;binding name to ⋋
													
nullary		:= push | T | F	| empty					;primitive operations
unary		:= drop | dup | uncons | size | B
binary		:= swap | + | - | * | / | mod | cons | append | if
tertiary    := rotup | rotdn | swapXZ | swapYZ | ifelse
defining	:= defun | defcon						;defun defines a function
													;defcon defines a constant)
									
													;nullary (no stack arguments)
push(x)		:= [...] push(x) => [... x]				;push a value on the stack
T			:= [...] T => [... T]					;boolean 
F			:= [...] F => [... F]
empty		:= [...] empty => []					;empty the stack
					
pop			:= [... X] pop => [...]					;unary (one stack argument)		
drop		:= [... X] drop => [...]				
dup			:= [... X] dup => [... X X]
sin			:= [... ∠] sin => [... sin(∠)]			;∠ in radians
cos			:= [... ∠] cos => [... cos(∠)]
tan			:= [... ∠] tan => [... tan(∠)]
d2r			:= [... ∠deg] d2r => [... ∠rad]
r2d			:= [... ∠rad] r2d => [... ∠deg]			;sorry, no r2d2 ;-)
size		:= [... string|aggr] size => [... n]  	;n = length of object
print		:= [... X] print => [...] with value of X printed 	

swap		:= [... Y X] swap => [... X Y]			;binary (two stack arguments) 
+			:= [... Y X] + => [... Y+X]
-			:= [... Y X] - => [... Y-X]
*			:= [... Y X] * => [... Y*X]
/			:= [... Y X] / => [... Y/X]
^			:= [... Y X] ^ => [... Y^X]
%			:= [... Y X] % => [... Y%X]
.			;= [... v1 v2] . = [... v1.v2]			;dot product of vectors
predop		:= [... Y X] op => [... T|F]			;predicate operators
			;where predop is one of =?, !=?, >?, >=?, <?. <=?. T = true, F = false.
ensure		:= [⋋ <depth xtyp ytyp ztyp>] ensure	;⋋ executed if conditions met,
													;error mag and ⋋ dropped if not.
													;used for type checking by user 
													;defined functions.
											   	
rotup		:= [... Z Y X] => [... X Y Z]			;tertiary (three stack arguments)
rotdn		:= [... Z Y X] => [... Y X Z]
swapYZ		:= [... Z Y X] => [... Y Z X]
B			:= [... Z Y ⋋] B => [...⋋(Y Z)] 		;the B combinator
													;operations on lists
append		:= [...(a b ...) n] append => [.... (a b ... n)]
uncons		:= [... (a b ...)] uncons => [... (b c ...) a]
cons		:= [... (b c ...) a] cons => [... (a b c ...)]			
													;program control functions
if			:= [... Z ⋋ T] if => [... ⋋(Z)]			;⋋ executed if T
			:= [... Z ⋋ F] if => [...]				;ignored if F  	
ifelse		:= [... W ⋋2 ⋋1 T] ifelse => [... ⋋1(W)] ;W is any valid expression
			:= [... W ⋋2 ⋋1 F] ifelse => [... ⋋2(W)]
for			:= [... X ⋋ n] for => [... ⋋(Z)] 		;⋋ executed n times
while		:= [... Z Y X] while => [... Y(Z) X] 	;while X is true.
								 => [... Y(Z)]      ;when X becomes false.

													;defining functions
defcon		:= [... number] defcon name => [...]	;defines a constant 
defun		:= [... ⋋) defun name => [...]			;defines a function
													;both result in symbol table entry
													;binding name to value.
Alternative grammars
--------------------
Lambda calculus: (brief explanation goes here...)
Examples
[... X] ⋋x.x*x => [... X²] 					;the form ⋋x.x*x is the square function.				 
[... Y X] ⋋xy.x+y => [... X+Y]				;add function.

Combinatorial algebra: an alternative to lambda calculus using combinators. 
Combinators are defined as : Ix = x			;identity function (a noop)	
							 Bxyz = x(yz)	;execute x
							 B'xyz = y(xz)	;execute y (dip in other CLs)	
							 Kxy = x		;equivalent to drop
							 Wxy = xyy		;equivalent to dup
							 Cxyz = xzy		;equivalent to swap
							 
The following combinators are defined in ACL:							 
[... Z Y ⋋] B => [... ⋋(Y Z)] 			 
[... Y X] K => [... Y]     
[... Y X] W => [... Y X X]  
[... Y X] C => [... X Y]

Reserved words
--------------
Primitive names.
#define names in ACL.h
All single capital letters.

Operational Overview
--------------------
A function in ACL is a sequence of terms, representing a mapping from stacks to stacks, 
with the following syntax:

[S] f => [f(S)] where f is a function and S is the stack contents. 

Function terms are evaluated left to right according to the following two rules:

1 - Literal terms are pushed onto the stack. Some examples:
	[...] 1 => [... 1]
	[...] string => [... string]
	[...] (1 2 3) => [... (1 2 3)]

2 - Functions are executed sequentially according to their definitions. For example:
	[...] 5 dup * => [... 5] dup * => [... 5 5] * => [... 25]
	[...]"Hello!" print => [... "Hello!"] print => [...] with Hello! printed.

An ACL program is a function p which, when applied to an empty stack, produces a 
desired result: [] p => [] with appropriate side effects. Programs may be saved
to a file using saveprog and executed by main(filename).

Notice that all communication between functions is via the stack. ACL functions 
communicating via stacks can be thought of as being similar to Unix shell scripts 
communicating via pipes. Gluing ACL functions together via concatenation produces 
programs in the same sense as gluing shell scripts together.

Data types and typing
---------------------
ACL data types are either simple (numbers and strings) or aggregates (lambdas, 
lists, sets, vectors, matricies). Aggregates are represented by a beginning and 
ending symbol pair that serves to bind the symbols between them into a single 
entity. Lambdas (⋋) are bound by [], sets by {}, vectors and matricies by <>. 

Rational and complex numbers are tuples with special identifying separators: 
/ for rational numbers (fractions) and , for complex numbers.
ACL integers are rational numbers with a denominator of one.
ACL floating point numbers are complex numbers with a zero imaginary part.

Dynamic (run-time) type checking is performed for each operator. Both the number and 
type of stack arguments are checked and error messages are printed if they are not as 
expected. Dynamic type checking allows dynamic (run time) creation and execution of 
functions, thus the collection of primitive functions built-in to ACL may be extended
indefinitely.

Input tokens
------------
A token is one or more Unicode characters delimited by whitespace (space|newline) 
or by the beginning and ending symbol of an aggregate. Aggregate tokens and strings
may include whitespace, but numbers and function names may not.

Comments are indicated by the semicolon ; the remainder of the input line is ignored. 

Symbol table
------------
ACL uses a symbol table for user created functions. If a function is not found by the 
compiler, the symbol table is searched, thus user created functions are first class,
i.e., treated the same as built-in functions.

The stack
---------
The ACL stack is a linked list of term structures. The upper limit on stack size 
depends on the amount of heap memory allocated to the ACL process.
The stack contents shown as the ACL interactive prompt: [S]: where S is a sequence of
stack terms with the top term to the left.

Interpreter operation
---------------------
Literals: pushed onto the stack.
Primitives: C code is immediately executed.
Named functions: the associated lambda is executed.

ACLlib.txt
----------
; ACL standard library 2022/05/13
; uncomment silent to see compiler messages
;
;silent
;                         
;math constants
3.1415926535 defcon pi
2.7182818284 defcon eps
1.4142135623 defcon √2
;
;math functions
[dup *] defun ^2
[dup dup * *] defun ^3
[^2 pi *] defun pir^2
[<1 0 0>] defun uvx
[<0 1 0>] defun uvy
[<0 0 1>] defun uvz
[swap dup ! rotdn swap - ! /] defun nPr
[dup ! rotdn nPr swap /] defun nCr
;
;aliases
[eq] alias =?
[neq] alias !=?
[gt] alias >?
[gte] alias >=?
[lt] alias <?
[lte] alias <=?
[dot] alias .
;
;combinators
;---------------
;gen									; execute a generator
[uncons dup swapYZ I rotdn cons swap
] defun gen

;I/O functions
;-------------
[print get] defun ask					; [... "prompt"] => [... response]				

;list functions
;--------------
;reverse a list							; [... (a b .. y z)] => [... )z  y .. b a)]
[size pred								; [... (a b .. y z) size-1]
 dup swapYZ								; [... size-1 (a b .. y z) size-1]
 [uncons swap swapYZ] swap
 times									; [... a b .. y size-1 (z)]
 swap									; [... a b .. y (z) size-1]
 [swap append] swap
 times	 								; [... (z y .. b a)] 
] defun reverse	
;
;generators
;----------
;generate an integer sequence from n.
[uncons dup succ 
 rotdn swap cons
] defun Ngen
;
[[Ngen] 0] def N0						; initiate an integer sequence from 0
[[Ngen] 1] def N						; initiate an integer sequence from 1
;
;flipgen: [n] -> ¬n [¬n]				; generate an oscillator: 1,0,1,0,...
[uncons ¬ dup rotup cons
] defun flipgen
;
[[flipgen] 0] def flipflop				; initiate an oscillator
;
;end of file

=== end ACLdesign.t==================================================================









