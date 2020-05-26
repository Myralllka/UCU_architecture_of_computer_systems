#!/bin/bash
config_filename=config.conf
while true; do
  case $1 in
    -f|--file)
      config_filename=$2
      shift 2
    ;;
    -d|--debug)
      debug=true;
      shift
    ;;
    -o|--optimize)
      optimize=true;
      shift
    ;;
    -h|--help)
      echo "Usage: ./start.sh [options]
  Options:
    -co   --compile       Compile with O3 optimization before executing
    -cd   --debug         Compile with debug options
    -f    --file          Config filename
    -h    --help          Show help message"
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
mkdir -p ./res;
pushd ./cmake-build-debug  > /dev/null || exit 1

if [[ "$debug" = true ]]; then
  echo Compiling...
  cmake -DCMAKE_BUILD_TYPE=Debug -G"Unix Makefiles" ..;
  make;
fi;

if [[ "$optimize" = true ]]; then
  echo Compiling...
  cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ..;
  make;
fi;

./cmake-build-debug/lab_6 "$config_filename"
