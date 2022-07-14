#!/bin/sh
mkdir 1.b.files.out
for i in 1.b.files/*.txt;
 do
 	sort -n $i  > 1.b.files.out/$(basename "$i")
done
cd 1.b.files.out 
cat *.txt | sort -n > 1.b.out.txt

mv 1.b.out.txt "test.txt"
uniq -c test.txt 1.b.out.txt
#truncate -s 0 "test.txt"
#rm "test.txt"

mv 1.b.out.txt "test2.txt"
awk ' { t = $1; $1 = $2; $2 = t; print; } ' test2.txt > 1.b.out.txt
#truncate -s 0 "test2.txt"
cat /dev/null > test.txt
cat /dev/null > test2.txt
rm "test.txt" "test2.txt"
