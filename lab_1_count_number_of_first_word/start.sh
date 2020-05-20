mkdir ./cmake-build-debug
cd ./cmake-build-debug || exit;
cmake -G"Unix Makefiles" ..;
make;
mkdir -p tmp;
results_array=()
> ../$3
experiment=("reading word by word from the file and counting"
            "reading all words in memory and then count"
            "reading all words in memory and counting using boost regex")
echo "--------------"
echo "processing...."
echo "--------------"
for number in 1 2 3; do
  for i in $(seq 1 "$1"); do
    ./lab2 $number ../"$2" ../"$3" >> ./tmp/result.txt
  done;
  result_array+=$(tail ../"$3" -n 1);
  if [ "${#result_array[@]}" -gt 0 ] && [ $(printf "%s\000" "${result_array[@]}" | LC_ALL=C sort -z -u | grep -z -c .) -eq 1 ] ; then
    echo results are still equal;
  else
    echo "-------------------"
    echo "Different results!!"
    echo "-------------------"
    exit
  fi
  sort -o ./tmp/result.txt ./tmp/result.txt
  echo ${experiment[number-1]} "experiment. min time: $(tail ./tmp/result.txt -n 1)"
  rm ./tmp/result.txt
done;
rm -rf tmp
echo "---------------------------------"
echo "Perfect. All results are the same"
echo "---------------------------------"
