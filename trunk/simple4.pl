begin

integer i;
integer j;
integer k, l, m, n;

proc Q
begin
integer a, i;
integer j;
integer k, l, m, n;
a := 0;
i := 5;
j := i;
m := i * j;
l := 50;
write a, i, j, m, l;
end;	

call Q

i := 5;
j := i;
m := i * j;
l := 50;
write i, j, m, l;

end.