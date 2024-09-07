#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  if [ $? -ne 0 ]; then
    echo "compile failed: $input"
    exit 1
  fi
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert_link() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  if [ $? -ne 0 ]; then
    echo "compile failed: $input"
    exit 1
  fi
  cc -c tmp.s
  cc -o tmp tmp.o foo.o
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 "0;"
assert 42 "42;"
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5 ;"
assert 47 "5+6*7;"
assert 12 "16-8/2;"
assert 15 "5*(9-6);"
assert 4 "(3+5)/2;"
assert 10 "-10+20;"
assert 10 "+20-10;"
assert 1 "1==1;"
assert 0 "1==2;"
assert 0 "1!=1;"
assert 1 "1!=2;"

assert 1 "1<2;"
assert 0 "2<1;"
assert 0 "1<1;"
assert 1 "1<=2;"
assert 0 "2<=1;"
assert 1 "1<=1;"

assert 0 "1>2;"
assert 1 "2>1;"
assert 0 "1>1;"
assert 0 "1>=2;"
assert 1 "2>=1;"
assert 1 "1>=1;"
assert 14 "a=3;b=5*6-8;a+b/2;"
assert 1 "a=1;a;"
assert 1 "foo=1;foo;"
assert 6 "foo = 1;bar = 2 + 3; foo + bar;"
assert 14 "a=3;b=5*6-8;return a+b/2;"
assert 5 "return 5; return 8;"
assert 42 "if ( 1 ) return 42; return 43;"
assert 43 "if ( 0 ) return 42; else return 43;"
assert 6 "a=0;while(a<5) a=a+2;a;"
assert 15 "i=0;a=0;for(i=0;i<6;i=i+1)a=a+i;a;"
assert 4 "{a=1;b=3;return a+b;}"
assert_link 0 "foo1();0;"
assert_link 3 "foo_int();"

echo OK
