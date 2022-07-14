#!/bin/bash
mkdir files_mod
for file in ./temp/*.txt
do 
  touch /home/akanksha/OSLAB/demo/files_mod/$(basename "$file")  #path of folder
  file2=/home/akanksha/OSLAB/demo/files_mod/$(basename "$file")
  sed '=' $file | sed 'N; s/\n/ /' > $file2
  sed -i 's/ /,/g' $file2
done
