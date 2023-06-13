var b : bool;
if b do
    print "b0 is true\n";
else
    print "b0 is false\n";
end if;
b := true;
b := true & true;
if b do
    print "b1 is true\n";
else
    print "b1 is false\n";
end if;
var a : bool := false;
a := b & a;
if a do
    print "a0 is true\n";
else
    print "a0 is false\n";
end if;
var s : string := "str";
a := s = "str";
if a do
    print "a1 is true\n";
else
    print "a1 is false\n";
end if;
