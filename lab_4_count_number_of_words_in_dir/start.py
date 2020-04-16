#!/bin/python3
import os
import re
import sys

outA, outN = "", ''


def build(d, r):
    """
    :param d: debug
    :param r: release
    """
    print("..building project")
    os.system("mkdir -p cmake-build-debug")
    os.chdir("cmake-build-debug")
    cflags = ''
    if d:
        cflags = "-DCMAKE_BUILD_TYPE=Debug"
    elif r:
        cflags = "-DCMAKE_BUILD_TYPE=Release"
    else:
        cflags = "-G \"Unix Makefiles\""
    os.system("cmake  {} .. >> ../build_logs.txt".format(cflags))
    os.system("make >> ../build_logs.txt")
    os.chdir("../")
    os.system("rm build_logs.txt")


def run(n, conf):
    durations = list()
    analyzing = list()
    loadings = list()
    print("..running program")
    # os.chdir("cmake-build-debug")
    for _ in range(n):
        print("...running {} time".format(_ + 1))
        os.system("./cmake-build-debug/count_number_of_all_words ./{} >> durations.txt".format(conf))
        with open("durations.txt", 'r') as dur_file:
            analyzing.append(float(re.findall(r"Analyzing: \d+", dur_file.read())[-1].split()[1]))
        with open("durations.txt", 'r') as dur_file:
            durations.append(float(re.findall(r"Total: \d+", dur_file.read())[-1].split()[1]))
        with open("durations.txt", 'r') as dur_file:
            loadings.append(float(re.findall(r"Loading: \d+", dur_file.read())[-1].split()[1]))
    os.system("rm durations.txt")
    print("> minimum total runtime {} us\n> minimum total analyzing {} us, \n> minimum total loading {} us"
          .format(min(durations), min(analyzing), min(loadings)))


def check_results(config_file):
    print("..checking results. running program in one thread for etalon results...")
    with open("./{}".format(config_file), 'r') as main_config:
        tmp = main_config.readlines()
        # print(tmp)
        outA = tmp[1].split()[-1].split('./')[-1]
        outN = tmp[2].split()[-1].split('./')[-1]
        # print(outA, outN)
    # run in 1 thread
    # create new config file
    with open("./{}".format(config_file), 'r') as main_config:
        infile = main_config.readline()[9:]
    with open("test_config.conf", 'w') as test_config:
        test_config.write("""infile = {}
out_by_a = test_res_a.txt
out_by_n = test_res_n.txt
threads = 1""".format(infile))
    run(1, "test_config.conf")
    # check files
    if os.system("cmp {} ./test_res_a.txt".format(outA)):
        print("different results sorted by alpha! see error folder")
        os.system("mkdir -p error")
        os.system("cp {} ./error/res_a.txt".format(outA))
        os.system("cp ././test_res_a.txt ./error/./test_res_a.txt")
    if os.system("cmp {} ./test_res_n.txt".format(outN)):
        print("different results sorted by number! see error folder")
        os.system("mkdir -p error")
        os.system("cp {} ./error/res_n.txt".format(outN))
        os.system("cp ././test_res_n.txt ./error/./test_res_n.txt")
    os.system("rm test_config.conf")
    os.system("rm ./test_res_a.txt")
    os.system("rm ./test_res_n.txt")


def heelp():
    print("Usage:\n\t./start.py <path_to_config_file> [options]\n"
          "Options:\n\t-n <number>\tNumber of cycles\n"
          "\t-b\t\t(re)Build the program\n"
          "\t-d\t\tAdd debug Building option\n"
          "\t-o\t\tAdd release Building option\n"
          "\t-h\t\tHelp message (this message)")
    exit(1)


# ./start.py config.dat -n 100 -d
# config.dat configuration file
# -n 100 to repeat program 100 times
# -d debug build type
if __name__ == "__main__":
    try:
        os.system("rm -rf ./error")
        if len(sys.argv) < 2:
            heelp()
        config_file = sys.argv[1]
        arguments = ' '.join(sys.argv)
        n = 1
        if re.search("-n \d+", arguments) is not None:
            n = int(re.search("-n \d+", arguments).group(0)[2:])
        debug, release = False, False
        if re.search("-d", arguments) is not None:
            debug = True
        if re.search("-o", arguments) is not None:
            release = True
        if re.search("-b", arguments) is not None:
            build(debug, release)
        if re.search("-h", arguments) is not None:
            heelp()
        run(n, config_file)
        check_results(config_file)
    except Exception as e:
        print(e)
        os.system("rm -rf test_config.conf ./test_res_a.txt ./test_res_n.txt ./error durations.txt 2> /dev/null")
        exit(1)
