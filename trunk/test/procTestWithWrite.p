$PL test program for procedure
$definition and call
begin
 integer i,j,k,r;
 proc P
  begin 
   integer i;
   i := 10;
   j := 15;
   write i, j, k, r;
 end;

 proc Q
   begin
    integer k;
    proc R
    begin
     integer j;
     j := 0;
     write i, j, k, r;
   end;
   k := 0;
   write i,j,k,r;
   call R;
 end;
 
 i := 1;
 j := 1;
 k := i+j;
 r := j+k;
 call P;
 call Q; 
 
 i := 9999;
 write i,j,k,r;
end.
 
