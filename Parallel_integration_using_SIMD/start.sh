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

unique_values() {
  typeset i
  for i do
    [ "$1" = "$i" ] || return 1
  done
  return 0
}
n=2
config_filename=../config.conf
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
    -f|--file)
      config_filename=$2
      shift 2
    ;;
    -n|--number)
      test -z "${OPTARG//[0-9]}"  || (echo "$1 is not a number" >&2 && exit 1)
      n=$2
      shift 2
      ;;
    -d|--debug)
      debug=true;
      shift
    ;;
    -h|--help)
      echo "Usage: ./start.sh [options]
  Options:
    -c    --compile       Compile before executing
    -h    --help          Show help message
    -n    --number        Number of cycles for execution
    -t    --threads       Number of threads
    -d    --debug         Debuging mode
    -f    --file          Path to the configuration file"
#          "--build_graph   Build the graph on the result (running [number] times on 1-8 threads)"
      exit 0;
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

if [ -z "$n" ] && [ -z "$graph" ]; then
  echo "Error: use -n [number] to set number of iterations."; exit 1;
fi
## Making working directories
## Compiling (if needed)
##

mkdir -p ./cmake-build-debug || exit 1
cd ./cmake-build-debug || exit 1;
mkdir -p tmp;

if [ "$comp" = true ]; then
  echo Compiling...
  cmake -G"Unix Makefiles" .. || exit 1
  make || exit 1
fi;

if ! test -f "$config_filename" ; then
  echo "No conf file: use -f [filename] to identify it."; exit 1;
fi

if [ ! -z "$threads" ]; then
  sed -i "s/flow_num...*/flow_num = $threads/g" "$config_filename";
else
  sed -i "s/flow_num...*/flow_num = 1/g" "$config_filename";
fi
counter=0
if [ "$graph" = true ]; then
  cores=$(grep -c ^processor /proc/cpuinfo) || exit 1
  sed -i "s/flow_num...*/flow_num = 1/g" "$config_filename" || exit 1
else
  cores=1;
fi
echo ------
echo "Starting integrating $n times"
echo "Using $threads threads"
echo ------
for i in $(seq 1 "$cores"); do
  for j in $(seq 1 "$n"); do
    pwd
    ./lab_2_parallel_integration "$config_filename" > ./tmp/result.txt || exit 1
    if [ "$debug" = true ]; then
      cat ./tmp/result.txt
    fi;
    cat tmp/result.txt | grep Result | sed -n 's/\w*//p'| cut -c 4- >> tmp/array_result || exit 1
    cat tmp/result.txt | grep Time | sed -n 's/\w*//p'| cut -c 4- >> tmp/array_time || exit 1
    ((counter++))
    if [ -z "$debug" ] && [ -z "$graph" ]; then
      ProgressBar ${counter} ${n}
    else
      ProgressBar ${counter} ${((n*cores))}
    fi;
  done;
  if [ "$graph" = true ]; then
    sed -i "s/flow_num...*/flow_num = $((cores+1))/g" "$config_filename" || exit 1
    echo tmp/array_time | sort -n | head -n 1 >> tmp/python_time|| exit 1
  fi
done;

echo
echo "Minimal time: $(cat tmp/array_time | sort -n | head -n 1)"
## check all results
a=($(cat tmp/array_result))
if unique_values "${a[@]}"; then
  echo "All results are equal."
else
  echo "Error: results are not equal!"; exit 1;
fi

if [ -z "$debug" ]; then
  echo "Result is: $(cat tmp/array_result | head -n 1)"
fi;
echo Finished!
rm -rf tmp/*
