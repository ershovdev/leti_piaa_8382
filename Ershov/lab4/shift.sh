#!/bin/bash

FILES=Tests/shift/*
for test in $FILES
do
	echo "INPUT:"
	cat $test
	echo "OUTPUT:"
	./lab4 < $test
	echo "========================"
done