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

    def upper_border(row):
        """
        Check if current point is in upper field border.
        :param row: x-coordinate of point / number of row
        """
        return row < BORDER_WIDTH

    def bottom_border(row):
        """
        Check if current point is in bottom field border.
        :param row: x-coordinate of point / number of row
        """
        return row > int(args["height"]) - BORDER_WIDTH - 1

    def left_border(col):
        """
        Check if current point is in left field border.
        :param col: y-coordinate of point / number of column
        """
        return col < BORDER_WIDTH

    def right_border(col):
        """
        Check if current point is in right field border.
        :param col: y-coordinate of point / number of column
        """
        return col > int(args["height"]) - BORDER_WIDTH - 1

    with open(args["output_file"], 'w+') as field:
        print(args["width"], file=field)
        print(args["height"], file=field)

        border_vals = dict()
        for key in ["up", "bottom", "left", "right"]:
            border_vals[key] = float(args[key + "_func_arg"])

        for key in border_vals:
            if args[key + "_func"] == "sqr":
                border_vals[key] = border_vals[key] ** 2

        for row in range(int(args["height"])):
            for col in range(int(args["width"])):
                if upper_border(row):
                    to_print = border_vals["up"]
                elif bottom_border(row):
                    to_print = border_vals["bottom"]
                elif left_border(col):
                    to_print = border_vals["left"]
                elif right_border(col):
                    to_print = border_vals["right"]
                else:
                    to_print = args["temp"]
                print(to_print, end=' ', file=field)
            print('', file=field)

            for key in border_vals:
                border_vals[key] = getattr(sys.modules[__name__], args[key + "_func"])(args, key, border_vals[key])


# functions to be used for field borders values generation
# with same signature
def const(args, key, cur_val):
    """
    Constant function.
    :param args: dictionary with field generation arguments
    :param cur_val: value of border in previous row
    """
    return cur_val


def lin(args, key, cur_val):
    """
    Linear function.
    """
    return cur_val * float(args[key + "_func_arg"]) + float(args[key + "_func_arg2"])


def sqr(args, key, cur_val):
    """
    y = x^2 function.
    """
    return (cur_val ** 0.5 + 1) ** 2


def exp(args, key, cur_val):
    """
    Exponential function.
    """
    return cur_val * float(args[key + "_func_arg"])


if __name__ == "__main__":
    config = ConfigParser()
    config.read(CONFIG_FILE)
    args = dict()
    for opt in config.options(CONFIG_SECTION):
        args[opt] = config.get(CONFIG_SECTION, opt)
    generate_field(args)
