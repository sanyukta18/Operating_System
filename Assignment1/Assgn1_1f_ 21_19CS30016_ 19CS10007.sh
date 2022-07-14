echo "Enter the File Name "
read var
echo "Enter the Column "
read col
# x.txt is a temporary file which will be deleted at later stage
out="1f_output_"
out+=$col
out+="_column.freq"
if [ $col == 1 ];
then    
        awk '{print $1}' $var > x.txt
fi
if [ $col == 2 ];
then
        awk '{print $2}' $var > x.txt
fi
if [ $col == 3 ];
then
        awk '{print $3}' $var > x.txt
fi
if [ $col == 4 ];
then
        awk '{print $4}' $var > x.txt
fi
tr '[:upper:]' '[:lower:]' < x.txt > y.txt
tr ' ' '\12' <y.txt| sort | uniq -c | sort -nr > x.txt
awk -F $' ' ' { t = $1; $1 = $2; $2 = t; print; } ' OFS=$' ' x.txt > $out
rm x.txt        #removing not needed files
rm y.txt        #removing not needed files