$ A program to compute the nth fibonacci number. The sequence
$ goes 1, 1, 2, 3, 5, 8, 11, .... This shows that recursion does indeed 
$ work.
begin
integer temp;
proc fib
begin
       integer x, y, z;
       if temp > 1 -> 
               x := temp;
               temp := x - 1;
               call fib;
               y := temp;
               temp := x - 2;
               call fib;
               z := temp;
               temp := y + z;
       []  ~(temp > 1) -> skip;
       fi;
end;
read temp;
call fib;
write temp;
end.