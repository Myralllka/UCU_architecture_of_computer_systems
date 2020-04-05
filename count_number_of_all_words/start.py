# !/bin/env python3
import sys, os
import re


# ./start.py 1000 - number of times to repeat each method
def build(debug):
    print("..building project")
    os.system("mkdir -p cmake-build-debug")
    os.chdir("cmake-build-debug")
    if debug:
        os.system("cmake -DCMAKE_BUILD_TYPE=Debug .. >> ../build_logs.txt")
    else:
        os.system("cmake -G \"Unix Makefiles\" .. >> ../build_logs.txt")
    os.system("make >> ../build_logs.txt")
    os.chdir("../")
    os.system("rm build_logs.txt")


def run(n, config_file):
    durations = list()
    print("..runing program")
    os.chdir("cmake-build-debug")
    for _ in range(n):
        os.system("./count_number_of_all_words ../{} >> durations.txt".format(config_file))
        with open("durations.txt", 'r') as dur_file:
            # print(re.match(r"Total: \d+", dur_file.readlines()[-1]).group(0).split()[1])
            durations.append(float(re.match(r"Total: \d+", dur_file.readlines()[-1]).group(0).split()[1]))
    os.system("rm durations.txt")
    os.chdir("../")
    print("> minimum runtime {} us".format(min(durations)))

def check_results(config_file):
    print("..checking results")
    # run in 1 thread
    # create new config file
    with open(config_file, 'r') as main_config:
        infile = main_config.readline()[9:]
    with open("test_config.conf", 'w') as test_config:
        test_config.write("""infile = {}
out_by_a = test_res_a.txt
out_by_n = test_res_n.txt
threads = 1""".format(infile))
    run(1, "test_config.conf")
    # check files
    if (os.system("cmp ./cmake-build-debug/res_a.txt ./cmake-build-debug/test_res_a.txt")):
        print("different results sorted by alpha!")
    if (os.system("cmp ./cmake-build-debug/res_n.txt ./cmake-build-debug/test_res_n.txt")):
        print("different results sorted by number!")
    os.system("rm test_config.conf")
    os.system("rm ./cmake-build-debug/test_res_a.txt")
    os.system("rm ./cmake-build-debug/test_res_n.txt")


#./start.py config.conf -n 100 -d
# config.conf configuration file
# -n 100 to repeat program 100 times
# -d debug build type
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage:\n\t./start.py <path_to_config_file>")
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

