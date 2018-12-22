#!/bin/bash
try() {
    expected="$1"
    input="$2"

    ./macc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$expected expected, but got $actual"
        exit 1
    fi
}
try 41 '12 + 34 - 5'
try 0 0
try 42 42
try 18 '5+20-7'
echo OK
