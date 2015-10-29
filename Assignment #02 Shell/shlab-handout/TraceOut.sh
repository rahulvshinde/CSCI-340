#!/bin/sh
clear
for file in ~/shlab-handout/*.txt;
do echo "$file" >>My_trace.out;
echo "$file" >>Ref_trace.out;
echo "\nRunning your traces\nshortly output will be in My_trace.out for ./tsh traces & Ref_trace.out for ./tshref traces"
./sdriver.pl -t trace01.txt -s ./tsh -a "-p" > My_trace.out &&  ./sdriver.pl -t trace01.txt -s ./tshref -a "-p" > Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;

 ./sdriver.pl -t trace02.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace02.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;

 ./sdriver.pl -t trace03.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace03.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;

 ./sdriver.pl -t trace04.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace04.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;

 ./sdriver.pl -t trace05.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace05.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;

 ./sdriver.pl -t trace06.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace06.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;
echo "6 traces completed";

./sdriver.pl -t trace07.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace07.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;

 ./sdriver.pl -t trace08.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace08.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;
echo "8 traces completed"

 ./sdriver.pl -t trace09.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace09.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;

 ./sdriver.pl -t trace10.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace10.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;
echo "10 traces completed"

 ./sdriver.pl -t trace11.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace11.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;

 ./sdriver.pl -t trace12.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace12.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;

echo "12 traces completed"
 ./sdriver.pl -t trace13.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace13.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;

 ./sdriver.pl -t trace14.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace14.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;
echo "14 traces completed\nRunning last 2 traces\nmaximize terminal to see vimdiff in full screen."

 ./sdriver.pl -t trace15.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace15.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;

 ./sdriver.pl -t trace16.txt -s ./tsh -a "-p" >>My_trace.out &&  ./sdriver.pl -t trace16.txt -s ./tshref -a "-p" >>Ref_trace.out
echo "\n\n" >>My_trace.out;
echo "\n\n" >>Ref_trace.out;
echo "Script by- Akshay Joshi\n"
vimdiff My_trace.out Ref_trace.out
done

echo<< EOF

"This is script for tinyshell put this file in the folder shlab-handout.Output files are My_trace.out and Ref_trace.out.
The script runs all 16 traces and it takes time so run it by using screen command. Run the script give command
vimdiff My_trace.out Ref_trace.out. and enjoy
Author: Akshay Joshi"
