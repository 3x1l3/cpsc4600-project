PROG  				Instruction to set up stack frame for program
2				label for storage: program variables 
1				label for address: address of the first prog instr
DEFARG				storage size is 7 ( 5 + 1 + 1)
2
7
DEFADDR				This is the address of the first executable instr.
1
VARIABLE			i is located at offset 8 at relative level 0
0
8				j is located at offset 9 at relative level 0
VARIABLE			
0
9
CONSTANT			expression evaluates to constant 2
2
CONSTANT			expression evaluates to constant 3
3
ASSIGN				parallel assignment of size 2
2
VARIABLE			array access: S 
0
3				S is located beginning at offset 3
CONSTANT
1				index of the first element of S
INDEX				check the array index and bring the element 
5
8
VARIABLE			array access: S 			
0
3				S is located beginning at offset 3			
CONSTANT			index of the first element of S
2
INDEX				check the array index and bring the element
5
8
VARIABLE
0
3
CONSTANT
3
INDEX
5
8
VARIABLE
0
3
CONSTANT
4
INDEX
5
8
VARIABLE			Evaluate the first expression (i)	
0
8
VALUE
VARIABLE			Evaluate the second expression (j)			
0
9
VALUE
VARIABLE			Evaluate the third expression (i+j):i			
0
8
VALUE
VARIABLE			Evaluate the third expression (i+j):j			
0
9
VALUE
ADD				Evaluate the third expression (i+j):i+j
VARIABLE
0
8
VALUE
CONSTANT
2
VARIABLE
0
9
VALUE
MULTIPLY
ADD
ASSIGN				Parallel assignment of length 4
4
ENDPROG				Clean up and exit
