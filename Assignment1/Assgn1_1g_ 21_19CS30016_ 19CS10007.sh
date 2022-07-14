#!/bin/bash
read filename
read col
read str

FILE=$filename
if test -f "$FILE"; then
   rm $FILE
fi

for i in {1..150}
do
   foo=""
   flg=0
   for j in {1..10}
   do
      x=$(($RANDOM))
      if [ $flg -eq 0 ]
      then
         flg=1
         foo="$x"
      else   
         foo="$foo,$x"
      fi   
   done      
   #echo $foo
   echo "$foo" >> $filename
done

flg=0

while read line; do 
   cnt=1
   foo=$line
   temp=""

   for (( i=0; i<${#foo}; i++ )); do   
      if [ $cnt -gt $col ]
      then
         break
      fi
         
      if [ $cnt -eq  $col ] && ! [ "${foo:$i:1}" == "," ]
      then
         temp="$temp${foo:$i:1}"
      fi   

      if [ "${foo:$i:1}" == "," ]
      then
         cnt=$(($cnt + 1))
      fi           
            
      #echo "${foo:$i:1}"
   done

   x="`echo $temp | grep $str`"
   # echo $x
   if ! [ -z $x ]
   then
      flg=1
      break
   fi   
done < $filename 

if [ $flg -eq 1 ]
then
   echo "YES"
else
   echo "NO"
fi

      
