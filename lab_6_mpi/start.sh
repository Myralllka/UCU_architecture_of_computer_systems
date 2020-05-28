#!/bin/bash
config_filename=config.conf
while true; do
  case $1 in
    -f|--file)
      config_filename=$2
      shift 2
    ;;
    -D|--debug-build)
      debug_build=true;
      shift
    ;;
    -d|--debug)
      debug=true;
      shift
    ;;
    -O|--optimize-build)
      optimize_build=true;
      shift
    ;;
    -h|--help)
      echo "Usage: ./start.sh [options]
  Options:
    -O   --optimize-build  Compile with optimization before executing
    -D   --debug-build     Compile with debug options
    -d   --debug           Run executable with debug symbols
    -f    --file           Config filename
    -h    --help           Show help message"
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

mkdir -p ./res;


if [[ "$debug_build" = true ]]; then
  mkdir -p ./cmake-build-debug;
  pushd ./cmake-build-debug  > /dev/null || exit 1
  echo Compiling...
  cmake -DCMAKE_BUILD_TYPE=Debug -G"Unix Makefiles" ..
  make
  popd
fi;

if [[ "$optimize_build" = true ]]; then
  mkdir -p ./cmake-build-release;
  pushd ./cmake-build-release  > /dev/null || exit 1
  echo Compiling...
  cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ..
  make
  popd
fi;

if [[ "$debug_build" = true ]]; then
    mpirun -np 4 ./cmake-build-debug/mpi_heat_transfer "$config_filename" || exit 1
else
    mpirun -np 4 ./cmake-build-release/mpi_heat_transfer "$config_filename" || exit 1
fi
