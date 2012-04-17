PROG
2
1
DEFARG
2
2
DEFADDR
1
VARIABLE
0
3
CONSTANT
1
ASSIGN
1
DEFADDR				Do loop start label (4)
4
DEFADDR				Do loop jump destination (3) labels 3 and 4 are same!
3
VARIABLE
0
3
VALUE
CONSTANT
10
LESS
ARROW				Label for the next Boolean exp.
5
VARIABLE
0
4
VARIABLE
0
4
VALUE
CONSTANT
1
ADD
ASSIGN
1
VARIABLE
0
3
VARIABLE
0
3
VALUE
CONSTANT
1
ADD
ASSIGN
1
BAR				unconditional jump to label 4/3
4
DEFADDR				Label 5 is the address of next Boolean	
5
VARIABLE			
0
3
VALUE
CONSTANT
9
EQUAL
ARROW				label (6) for the next Boolean exp
6
VARIABLE
0
3
VALUE
VARIABLE
0
4
VALUE
WRITE
2
BAR				unconditional jump to label 4/3			
4
DEFADDR				Label 6 is the address of the next Boolean
6
VARIABLE
0
3
VALUE
CONSTANT
1
LESS
ARROW				Label(7) for the next Boolean
7
VARIABLE
0
3
CONSTANT
10
ASSIGN
1
BAR				unconditional jump to label 4/3				
4
DEFADDR				address of label 7 (no more boolean exp)
7
VARIABLE
0
3
VALUE
VARIABLE
0
4
VALUE
WRITE
2
ENDPROG
