$PL test program for if statements
begin
const n = 10;
integer i,sum;

i := 1;
do (i < n) -> sum := sum + 1;
              i := i+1;[]
   (i = 9) -> write i,sum; [] 
   (i < 1) -> i := 10;
od;
write i,sum;
end.
 
