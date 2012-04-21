$Simple recursive file that attempts to overflow the interpreter
$Should compile fine, but should loop indefinitely/until interp fails

begin

const multi = 5;
integer x;
const END = -1;

proc lol
begin 
  integer temp;
  write x;
  temp := x;
  x := temp + multi;
  call lol;
end;

x:=0;
call lol;

write x, END;
end.