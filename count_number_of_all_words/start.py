#!venv/bin python3
import sys, os
import re


EXEC = "./main"


# ./start.py 1000 - number of times to repeat each method
def build(debug):
    print("..building project")
    os.system("mkdir -p build")
    os.chdir("build")
    if debug:
        os.system("cmake -DCMAKE_BUILD_TYPE=Debug .. >> ../build_logs.txt")
    else:
        os.system("cmake -G \"Unix Makefiles\" .. >> ../build_logs.txt")
    os.system("make >> ../build_logs.txt")
    os.chdir("../")
    os.system("rm build_logs.txt")


def run(n, config_file):
    print("..runing program")
    os.chdir("build")
    for i in range(n):

    for i in range(len(METHODS)):
        print("....{}".format(METHODS[i]))
        for _ in range(n):
            os.system(EXEC + " {} ../{} ../{} > ../data/result/time.txt".format(i + 1, input_file, result_file))
            with open("../data/result/time.txt", 'r') as durations_file:
                data = durations_file.readlines()
            duration = int(data[0].split()[-2])
            durations[i].append(duration)
        DURATION[i] = min(durations[i])
        with open("../{}".format(result_file), 'r') as read_file:
            data = read_file.readlines()
            RESULTS[i] = (int(data[0]), float(data[1]))
    os.chdir("../")
    os.system("rm data/result/time.txt")


def check_results():
    print("..checking results")
    # run in 1 thread
    # create new config file
    # check files

#./start.py config.conf -n 100 -d
# config.conf configuration file
# -n 100 to repeat program 100 times
# -d debug build type
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("! invalid number of arguments")
    else:
        config_file = sys.argv[1]
        arguments = ' '.join(sys.argv)
        n = 1
        if re.search("-n \d+ ", arguments) is not None:
            n = int(re.search("-n \d+ ", arguments).group(0)[2:-1])
        debug = False
        if re.search("-d", arguments) is not None:
            debug = True

        build(debug)
        run(n, config_file)
        check_results(config_file)
