#!/bin/sh

_V=0             # logging is off; make equal to 1 to turn on; 0 to turn on

log_var="NONE"

function log () {
    if [[ $_V -eq 1 ]]; then
        echo $log_var
    fi
}

#point 1:- setting environment variable
export REQ_HEADERS=Accept,traceparent,Host
# point 2:- saving file from given url to current directory
curl -o example.html https://www.example.com/

#point3:- 
  #printing IP address
curl http://ip.jsontest.com/
  #printing response headers
curl --head http://ip.jsontest.com/

#point4:-
   #sending GET request
curl http://headers.jsontest.com/
str2="$REQ_HEADERS"
sed -e 's/^"//' -e 's/"$//' <<<"$str2" 
IFS=',' read -r -a str <<< "$str2"

for item in "${str[@]}"
do
 
 echo $item 
 curl -s http://headers.jsontest.com/ | jq -r ".$item"
 printf "\n"
 done

# export REQ_HEADERS=”Connection,Keep-Alive”

# curl "https://www.example.com/" -o example.html

# curl --head "http://ip.jsontest.com/"
# curl "http://ip.jsontest.com/"

# curl "http://headers.jsontest.com/" | jq
### COMMENT EVERYTHING BELOW TO SEE PART 1 TO 4. FOR JSON FILES SEGREGATION CODE BELOW
for a in 0 1 2 3 4 5 6 7 8 9
do
	flag=0
	flag2=0

	lines=$(~/Downloads/jq-win64.exe -c '.[]' $a.json)

	log_var="lines: "$lines
	log

	while read i; do
		flag2=1

		log_var="flag2 in: "$flag2
		log
		log_var="-----------------"
		log

		r='http://validate.jsontest.com/?json='
		r2="$r$i"

		log_var="r22222:- "
		log
		log_var= $r2
		log

		/c/Program\ Files\ \(x86\)/GnuWin32/bin/wget.exe -O - "$r2" > outt
		v=$(~/Downloads/jq-win64.exe .validate outt)

		log_var="v: $v"
		log

		pp=$PWD
		pp2="$pp/outt"
		if [ "$v" == "false" ] || [ ! -f $pp2 ]; then
			log_var="Error"
			log
			flag=1
			break
		fi	
		rm outt
	done <<< $lines

	log_var="flag: $flag"
	log
	log_var="flag2: $flag2"
	log

	if [ $flag -eq 0 ] && [ $flag2 -eq 1 ]; then
		log_var="good"
		log
		log_var=$a
		log
		log_var="###############################################"
		log
		echo $a.json >> valid.txt
	else
		log_var="bad"
		log
		log_var=$a
		log
		log_var="###############################################"
		log
		echo $a.json >> invalid.txt
	fi
done
	