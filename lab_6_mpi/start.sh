#!/bin/bash
config_filename=config.conf
number_of_processes=2
debug=true
image=true
while true; do
  case $1 in
    -f|--file)
      config_filename=$2
      shift 2
    ;;
    -i|--image)
      image=true
      shift
    ;;
    -np|--number_of_processes)
    if [ "$2" -eq "$2" ] 2>/dev/null; then
      number_of_processes=$2
      shift 2
    else
      echo "Option -np requires an numerical argument." >&2
      exit 1
    fi

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
    -h      --help                  Show help message.
    -O      --optimize-build        Compile with optimization before executing.
    -D      --debug-build           Compile with debug options.
    -d      --debug                 Run executable with debug symbols.
    -i      --image                 Save the images, not only gif, to res/ folder.

    -np     --number_of_processes   Number of processes for mpirun. Require second parameter. 2 by default.
    -f      --file                  Config filename. Require second parameter. ./config.conf by default."
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


rm -rf ./res;
mkdir ./res;

# generate a field
python3 field/field_generation.py "$config_filename" || exit 1

# change code control with respect to input flags
sed -i '/IMG/s/^\/\///g' includes/code_controle.h || exit 1
sed -i '/DEBUG/s/^\/\///g' includes/code_controle.h || exit 1
if [[ "$image" = false ]]; then
  sed -i '/IMG/s/^/\/\//g' includes/code_controle.h || exit 1
fi
if [[ "$debug" = false ]]; then
  sed -i '/DEBUG/s/^/\/\//g' includes/code_controle.h || exit 1
fi

if [[ "$debug_build" = true ]]; then
  mkdir -p ./cmake-build-debug;
  pushd ./cmake-build-debug  > /dev/null || exit 1
  echo Compiling...
  cmake -lpng -DCMAKE_BUILD_TYPE=Debug -G"Unix Makefiles" .. || exit 1
  make || exit 1
  popd
fi;

if [[ "$optimize_build" = true ]]; then
  mkdir -p ./cmake-build-release;
  pushd ./cmake-build-release  > /dev/null || exit 1
  echo Compiling...
  cmake -lpng -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" .. || exit 1
  make || exit 1
  popd
fi;

if [[ "$debug" = true ]]; then
    mpirun --allow-run-as-root -np "$number_of_processes" ./cmake-build-debug/mpi_heat_transfer "$config_filename" || exit 1
else
    mpirun --allow-run-as-root -np "$number_of_processes" ./cmake-build-release/mpi_heat_transfer "$config_filename" || exit 1
fi
