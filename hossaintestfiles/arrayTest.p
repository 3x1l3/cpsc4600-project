$PL test program for array definition and access
begin
  const c = 5; integer array S[c];
  const t = true; const f = false;
  integer i,j; 

  i, j := 2,3; 
  S[1], S[2], S[3], S[4] := i, j, i+j, i+2*j;
end.  
