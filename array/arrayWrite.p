begin
  const c = 5; integer array S[c];
  
  integer i,j; 

  i := 100;
  S[1] := 5;
  
  write i;

  i := S[1]; 
  write S[1], i;

  S[2] := i;
  write S[2];

end.