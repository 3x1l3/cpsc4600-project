PROG
2
1
DEFARG
2
4
DEFADDR
1
VARIABLE
0
3
CONSTANT
1
ASSIGN
1
VARIABLE
0
4
CONSTANT
2
VARIABLE
0
3
VALUE
MULTIPLY
ASSIGN
1
VARIABLE
0
5
CONSTANT
15
VARIABLE
0
4
VALUE
MODULO
ASSIGN							
1								
DEFADDR				First instruction of if construct Label: 3
3
VARIABLE			Evaluate Boolean expression
0
5
VALUE
VARIABLE
0
3
VALUE
EQUAL
CONSTANT			This is the constant t evaluated to true = 1
1
OR
ARROW				Label 5 (the Boolean is false) to represent next Boolean exp 
5
VARIABLE
0
6
VARIABLE
0
5
VALUE
ASSIGN
1
BAR				Label 4 (the first instruction after if construct) 
4
DEFADDR				This is the address of label 5
5
VARIABLE
0
5
VALUE
VARIABLE
0
3
VALUE
LESS
ARROW			Label 6 (the Boolean is false) to represent next Boolean exp		
6
VARIABLE
0
6
VARIABLE
0
4
VALUE
VARIABLE
0
5
VALUE
ADD
ASSIGN
1
BAR			Label 4 (the first instruction after if construct)		
4
DEFADDR			This is the address of label 6		
6
VARIABLE
0
5
VALUE
VARIABLE
0
3
VALUE
GREATER
ARROW			Label 7 (the Boolean is false) to represent next Boolean/fi
7			
VARIABLE
0
6
VARIABLE
0
4
VALUE
ASSIGN
1
BAR			Label 4 (the first instruction after if construct)
4			
DEFADDR			This is the address of label 7 (address of fi)		
7
FI			If we end up here the if instr. is incorrect
13			Line number
DEFADDR			This is the address of label 4
4
VARIABLE
0
3
VALUE
VARIABLE
0
4
VALUE
VARIABLE
0
5
VALUE
VARIABLE
0
6
VALUE
WRITE
4
ENDPROG
