$simple recursive file
$prints out 1 through 10, after the control var 112, and before the controlvar 224
$uses recursion, a calls a and so on.

begin

const p = 112;
integer x;
integer y;

proc a
begin 
  integer temp;
  write x;
  temp := x;
  x := temp + 1;
  if x < 10 -> 
      call a;
  [] ~(x > 10) -> y:=224;
  fi;
end;

x:=0;
call a;

write p, x, y;
end.