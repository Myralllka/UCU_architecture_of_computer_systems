#!/usr/bin/python3
from argparse import ArgumentParser, RawTextHelpFormatter
from os import stat

PICTURE_OUTPUT_FILE = "../include/display/pure_pixel_back_image.h"

option_parser = ArgumentParser(prog="field_generator.py",
                               formatter_class=RawTextHelpFormatter,
                               description="Generate the field for mpi_heat_transfer prgramm. First three lines are\n"
                                           "height and width without the border (represents the environment\n"
                                           "temperature) around the rectangilear, and the third line is the border\n"
                                           "width. Then gos the field where columns are separated with spaces and\n"
                                           "ros with '\\n'.",
                               usage="%(prog)s [options]")

option_parser.add_argument("inner_temperature", type=float, nargs=1,
                           help="Temperature of the object (field).")
option_parser.add_argument("--output_file", "-of", default="field.txt", type=str,
                           help="Output file with the header and field generated.")
option_parser.add_argument("--width", "-W", type=int, default=1000,
                           help="Width of the field. The border is not added to the Width.")
option_parser.add_argument("--height", "-H", type=int, default=1000,
                           help="Height of the field. The border is not added to the height.")
option_parser.add_argument("--border", "-B", type=int, default=1,
                           help="The border width. Go around the field rectangular and represents the outer\n"
                                "environment. This border is not counted in the and parameters.")
option_parser.add_argument("--outer_t", "-T", type=float, default=20.0,
                           help="Temperature of the environment (border).")


def generate_field(args_f, out_file):
    """
    Generate the field with specified border width and temperature,
    and inner object field width, height and temperature,
    :param args_f: The parsed params from ArgumentParser.
    :param out_file: Opened file to write the field
    """

    def _pad_border():
        for _ in range(args_f.border):
            print(args_f.outer_t, end=' ', file=out_file)

    def _border_side():
        for __ in range(args_f.border):
            for _ in range(args_f.width + 2 * args_f.border):
                print(args_f.outer_t, end=' ', file=out_file)
            print(file=out_file)

    _border_side()
    for _row in range(args_f.height):
        _pad_border()
        for _col in range(args_f.width):
            print(args_f.inner_temperature[0], end=' ', file=out_file)
        _pad_border()
        print(file=out_file)
    _border_side()


if __name__ == "__main__":
    args = option_parser.parse_args()
    with open(args.output_file, "w+", encoding="utf-8") as f:
        print(f"{args.width}\n{args.height}\n{args.border}", file=f)
        generate_field(args, f)
    print(f"Result file {args.output_file} of size {round(stat(args.output_file).st_size / (1024 * 1024), 2)} MiB")
