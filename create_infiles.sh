if [ "$#" -ne 5 ];
	then echo Wrong number of parameters
	exit
fi
countryFile=$1
diseaseFile=$2
input_dir=$3
filesPerDir=$4
recsPerFile=$5
if [ -e $input_dir ]
	then echo $input_dir exists
	exit
else
	echo $input_dir will be created
	mkdir $input_dir
fi
cd $input_dir
currentFiles=0
currentRecs=0
id=0
day=1
month=1
year=2010
count=0
totalDiseases=0
diseases=""
event=("EXIT" "ENTER")
while IFS= read -r line
do
    diseases="$diseases ${line//[$'\t\r\n ']}"
    ((totalDiseases++))
done < ../$diseaseFile
diseaseArr=($diseases)

while IFS= read -d '\n'  -r line || [[ -n "$line" ]]; do
	mkdir $line
	for  dir in */; do 			#diladi gia 7 fores
	# echo $dir
	currentFiles=0
	day=1
	month=1
	year=2010
    while [ $currentFiles -ne $filesPerDir ]
	do
	while [ $currentRecs -ne $recsPerFile ]; do
			((id++))
			# echo STH >> rec.txt
				if [[ ( $day -lt 10 && $month -lt 10 ) ]]; then
					echo $id ${event[$RANDOM % ${#event[@]}]} $(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w $((3 + RANDOM % 9)) | head -n 1) $(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w $((3 + RANDOM % 9)) | head -n 1) ${diseaseArr[$((RANDOM%$totalDiseases))]} $((1 + RANDOM % 120)) >> $dir/0$day-0$month-$year.txt
				elif [[ ( $day -lt 10 && $month -gt 10 ) ]]; then
					echo $id ${event[$RANDOM % ${#event[@]}]} $(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w $((3 + RANDOM % 9)) | head -n 1) $(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w $((3 + RANDOM % 9)) | head -n 1) ${diseaseArr[$((RANDOM%$totalDiseases))]} $((1 + RANDOM % 120))>> $dir/0$day-$month-$year.txt
				elif [[ ( $day -gt 10 && $month -lt 10 ) ]];  then
					echo $id ${event[$RANDOM % ${#event[@]}]} $(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w $((3 + RANDOM % 9)) | head -n 1) $(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w $((3 + RANDOM % 9)) | head -n 1) ${diseaseArr[$((RANDOM%$totalDiseases))]} $((1 + RANDOM % 120))>> $dir/$day-0$month-$year.txt
				else 
					echo $id ${event[$RANDOM % ${#event[@]}]} $(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w $((3 + RANDOM % 9)) | head -n 1) $(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w $((3 + RANDOM % 9)) | head -n 1) ${diseaseArr[$((RANDOM%$totalDiseases))]} $((1 + RANDOM % 120))>> $dir/$day-$month-$year.txt
				fi
			((currentRecs++))
		done
		currentRecs=0
   
	((day++))
	
    if [ $day -eq 31 ]; then
        ((month++))
        day=1
    fi
    if [ $month -eq 12 ]; then
        ((year++))
        month=1
    fi

	((currentFiles++))
	done
	done
done <  ../$countryFile
