#!/bin/bash
config_filename=config.conf
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
#    --build-graph)
#      echo "Building graph..."
#      graph=true;
#      shift
#    ;;
    -h|--help)
      echo "Usage: ./start.sh [options]
  Options:
    -c    --compile       Compile before executing
    -h    --help          Show help message
    -t    --threads       Number of threads"
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

mkdir -p ./cmake-build-debug;
pushd ./cmake-build-debug || exit;
#mkdir -p tmp;

if [ "$comp" = true ]; then
  echo Compiling...
  cmake -G"Unix Makefiles" ..;
  make;
fi;

if [ ! -z "$threads" ]; then
  sed -i "s/threads...*/threads = $threads/g" ../config.conf;
else
  sed -i "s/threads...*/threads = 1/g" ../config.conf;
fi
popd
#./cmake-build-debug/count_number_of_all_words "$config_filename"

