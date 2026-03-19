#!/bin/bash
assert(){
	expected="$1"
	input="$2"

	./mcc "$input" > tmp.s
	cc -o tmp tmp.s test_func.c
	./tmp
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo "$input => $expected expexted, but got $actual"
		exit 1
	fi
}

assert 0 'main(){0;}'
assert 42 'main(){42;}'
assert 42 'main(){21+21-10+10;}'
assert 47 'main(){5+6*7;}'
assert 15 'main(){5*(9-6);}'
assert 4 'main(){(3+5)/2;}'
assert 4 'main(){-15+(-3+3)+19;}'
assert 32 'main(){+4*+8;}'
assert 0 'main(){0==1;}'
assert 1 'main(){12==12;}'
assert 14 'main(){(12==12)+13;}'
assert 1 'main(){0<9;}'
assert 0 'main(){12<10;}'
assert 0 'main(){12>12;}'
assert 1 'main(){12>=12;}'
assert 0 'main(){12   <  11;}'
assert 2 'main(){12-12;13-11;}'
assert 11 'main(){a=12;a-1;}'
assert 12 'main(){a=15;b=a-3;b;}'
assert 10 'main(){a = 11; b = 1; a-b;}'
assert 10 'main(){abc = 23; def = 10; def = 13; abc - def;}'
assert 23 'main(){abc = 23; def = 10; def = 13; abc;}'
assert 13 'main(){abc = 23; def = 10; def = 13; def;}'
assert 14 'main(){
abc = 23;
def = 10;
def = 234 - 220;
}'

assert 19 'main(){
abc = 19;
name = 111;
return abc;
return name;
}'

assert 11 'main(){
a = 10;
b = 0;
if (a == b)
	return b+3;
else if (a != b + 10)
	return a;
else
	return 11;
}'

assert 3 'main(){
a = 0;
b = 1;
while (a <= 2)
	a = a+1;
a;
}'


assert 8 'main(){
i=0;
a=0;
for ( i=0; i<4; i= i+1)
	a = a + 2;
a;
}'

assert 40 'main(){
a=0;
b=0;
for (i=0; i<4; i= i+1){
	a = a+10;
	b = b+11;
}
a;
}'

assert 44 'main(){
a=0;
b=0;
for (i=0; i<4; i= i+1){
	a = a+10;
	b = b+11;
}
b;
}'

assert 40 'main(){
a=0;
b=0;
for (i=0; i<4; i= i+1){
	a = a+10;
	b = b+11;
	if (a > b){
		a = a + 1;
	} else {
	        b = b + 1;
        }
}

a;
}'
assert 48 'main(){
a=0;
b=0;
for (i=0; i<4; i= i+1){
	a = a+10;
	b = b+11;
	if (a > b){
		a = a + 1;
	} else {
	        b = b + 1;
        }
}

b;
}'

assert 33 'main(){foo();}'
assert 5 'main(){bar();}'
assert 10 'main(){add_ten();}'
assert 38 'main(){bar() + foo();}'
assert 15 'main(){a = add_ten(); a + bar();}'
assert 36 'main(){f_with_arg(3, 33);}'

assert 46 '
mizuno(yu, ki){
    return 2*23;
}

main(){
	c = mizuno(yu,ki);
	c;
}
'

echo OK
