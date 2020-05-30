#!/usr/bin/python3
from configparser import ConfigParser
import sys


CONFIG_FILE = "config.conf"
CONFIG_SECTION = "field-properties"
BORDER_WIDTH = 1

def generate_field(args):
    """
    Generate field with given function on borders and constant values inside.
    :param args: dictionary with arguments parsed from .conf file
    """

    def border(row, col):
        """
        Check if current point is in field border.
        :param row: x-coordinate of point
        :param col: t-coordinate of point
        :return: bool
        """
        return (row < int(BORDER_WIDTH) or row > int(args["height"]) - int(BORDER_WIDTH) - 1) or \
               (col < int(BORDER_WIDTH) or col > int(args["width"]) - int(BORDER_WIDTH) - 1)

    with open(args["output_file"], 'w+') as field:
        print(args["width"], file=field)
        print(args["height"], file=field)

        border_val = float(args["func_arg"])
        if args["func"] == "sqr":
            border_val = border_val ** 2

        for row in range(int(args["height"])):
            for col in range(int(args["width"])):
                if border(row, col):
                    print(border_val, end=' ', file=field)
                else:
                    print(args["temp"], end=' ', file=field)
            print('', file=field)

            border_val = getattr(sys.modules[__name__], args["func"])(args, border_val)


# functions to be used for field borders values generation
# with same signature
def const(args, cur_val):
    """
    Constant function.
    :param args: dictionary with field generation arguments
    :param cur_val: value of border in previous row
    """
    return cur_val


def lin(args, cur_val):
    """
    Linear function.
    """
    return cur_val * float(args["func_arg"]) + float(args["func_arg2"])


def sqr(args, cur_val):
    """
    y = x^2 function.
    """
    return (cur_val ** 0.5 + 1) ** 2


def exp(args, cur_val):
    """
    Exponential function.
    """
    return cur_val * float(args["func_arg"])


if __name__ == "__main__":
    config = ConfigParser()
    config.read(CONFIG_FILE)
    args = dict()
    for opt in config.options(CONFIG_SECTION):
        args[opt] = config.get(CONFIG_SECTION, opt)
    generate_field(args)
