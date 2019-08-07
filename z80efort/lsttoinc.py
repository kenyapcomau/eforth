#!/usr/bin/env python3

"""Extract code lines from asxxxx .lst file and output as DB declarations
This relies on the listing format of the asxxxx assemblers"""

#
# Lines without line number or out of sequence number are ignored
#   these are headers and blank lines
# Lines with opcodes are output as DB declarations
#   with original code as comment
# Lines starting with ;! have rest of line copied to output
#   these are for Forth code, labels, or literal DB/DWs
# Else lines starting with ; are copied verbatim
# Else a ; is prepended and the line is output
#   these are non-code generating lines like .ifdef, .equ, etc
#


import sys
import re


def hexify(h_s):
    """Print h_s in the form xxh with leading zero if first char is a-fA-F"""
    return "0" + h_s + "h" if re.match("[a-fA-F]+", h_s) else h_s + "h"


def main():
    """Extract code from .lst to make DB declarations"""
    linecount = 1
    for line in sys.stdin:
        line = line.rstrip("\r\n")
        if len(line) < 31:
            # invalid lines
            continue
        op1 = line[8:10].strip()
        op2 = line[11:13].strip()
        op3 = line[14:16].strip()
        lineno = line[27:31].strip()
        code = line[32:]
        if not re.match("[0-9]+", lineno):
            # ignore if no line number (headers...)
            continue
        if int(lineno) != linecount:
            # lost sync or EOF
            break
        linecount += 1
        if op1:
            ops = [hexify(op1)]
            if op2:
                ops.append(hexify(op2))
            if op3:
                ops.append(hexify(op3))
            # line up ;s
            tabs = "\t" if op2 else "\t\t"
            print("DB %s%s;%s\r\n" % (",".join(ops), tabs, code), end='')
        else:
            if len(code) >= 2 and code[0:2] == ";!":
                # remove ;! and print remainder of line
                print("%s\r\n" % code[2:], end='')
            else:
                # print as comment
                firstchar = "" if not code or re.match("^\\s*;", code) else ";"
                print("%s%s\r\n" % (firstchar, code), end='')


if __name__ == '__main__':
    main()
