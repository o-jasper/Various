#!/usr/bin/env python

import sys, os
from types import *

sys.path.append(os.path.join(os.path.dirname(__file__), 'lib'))

from ParamSelect import ParamSelect, ParamBasic, ParamListBox

ps = ParamSelect([ParamBasic("float", float), ParamBasic("int", int),
                  ParamBasic("string",str)])

for line in sys.stdin:
    v = eval(line)
    print(string(v) + " tp " + string(type(v)))
#    ps.tell()
#    ps.choose_parse(line)
