#!/usr/bin/bash

function ProgressBar {
    ## https://github.com/fearside/ProgressBar/blob/master/progressbar.sh
      ((_progress=((($(($1*10000/$2)))/100))))
      ((_done=$(((_progress*4)/10))))
      ((_left=40-_done))
      _done=$(printf "%${_done}s")
      _left=$(printf "%${_left}s")
      printf "\rProgress : [${_done// /#}${_left// /-}] ${_progress}%%"
}

while true; do
  case $1 in
    -c|--compile)
      comp=true;
      shift
      ;;
    -t|--threads)
      test -z "${OPTARG//[0-9]}"  || (echo "$1 is not a number" >&2 && exit 1)
      threads=$2
      shift 2
      ;;
    -n|--number)
      test -z "${OPTARG//[0-9]}"  || (echo "$1 is not a number" >&2 && exit 1)
      n=$2
      shift 2
      ;;
    -h|--help)
      echo "Usage: ./start.sh [options]
  Options:
    -c    --compile   Compile before executing
    -h    --help      Show help message
    -n    --number    Number of cycles for execution
    -t    --threads   Number of threads
    -d    --debug     Debuging mode"
      exit 0;
    ;;
    -d|--debug)
    debug=true;
    shift
    ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1 ;;
    :)
      echo "Option -$OPTARG requires an numerical argument." >&2
      exit 1 ;;
    *)
      break
      ;;
  esac
done

if [ -z "$n" ]; then
  echo "Error: use -n [number] to set number of iterations."; exit 1;
fi
if [ -z "$threads" ]; then
  sed -i "s/flow_num...*/flow_num = $threads/g" ../execution.conf;
else
  sed -i "s/flow_num...*/flow_num = 1/g" ../execution.conf;
fi

## Making working directories
## Compiling (if needed)
##

mkdir -p ./cmake-build-debug;
cd ./cmake-build-debug || exit;
mkdir -p tmp;
if [ "$comp" = true ]; then
  echo Compiling...
  cmake -G"Unix Makefiles" ..;
  make;
fi;

sed -i "s/flow_num...*/flow_num = $threads/g" ../execution.conf
counter=0
echo
echo ----
echo "Starting integrating $n times"
echo "Using $threads threads"
echo ----
if [ -z "$debug" ]; then
   ProgressBar ${counter} ${n}
fi;
for i in $(seq 1 $n); do
  ./lab_2_parallel_integration ../execution.conf > ./tmp/result.txt
  if [ "$debug" = true ]; then
    ./lab_2_parallel_integration ../execution.conf
  fi;
  cat tmp/result.txt | grep Result | sed -n 's/\w*//p'| cut -c 4- >> tmp/array_result
  cat tmp/result.txt | grep Time | sed -n 's/\w*//p'| cut -c 4- >> tmp/array_time
  ((counter++))
  if [ -z "$debug" ]; then
    ProgressBar ${counter} ${n}
  fi;
done;
echo
echo "Minimal time: $(cat tmp/array_time | sort | head -n 1)"
if [ -z "$debug" ]; then
  cat tmp/array_result
fi;

echo Finished!
rm -rf tmp/*
