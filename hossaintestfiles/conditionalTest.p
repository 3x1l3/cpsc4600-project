$PL test program for if statements
begin
const t = true; 
const f = false;
integer i,j,k,r;

i := 1;
j := 2*i;
k := 15\j;
if ((k = i) | t) -> r := k;[]
   k < i -> r := j+k;[]
   k > i -> r := j;
fi;
write i,j,k,r;
end.
 
