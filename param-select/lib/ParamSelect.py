
class ParamBasic():
    def __init__(name, type, default=None):
        self.name = name
        self.type = type
        self.default = default

    def choose(value=none):
        assert type(value) is self.type
        return (value, 0)

class ParamListBox():
    def __init__(name, list, default=None):
        self.name = name
        self.list = list
        self.default = default

    def choose(value=None):
        if value is None:
            value = self.default
        for i in range(len(self.list)):
            if list[i] == value:
                return (value, i)
        return None

class ParamIntSep():
    def __init__(name, default=0):
        self.name    = name
        self.default = default

    def choose(value=None):
        if value is None:
            return (self.default, self.default)
        assert type(value) is IntType
        return (value, value)

class ParamValueSep():  # Single-value
    def __init__(name, threshhold=0, default=0):
        self.name       = name
        self.default    = default
        self.threshhold = threshhold

    def choose(value=None):
        if value is None:
            value = self.default
        assert type(value) is FloatType or type(value) is IntType or
               type(value) is LongType
        return (value, 0 if (value < self.threshhold) else 1)

class ParamSelect:
# Chain of choices if one entry is a list, it is expected the former one was a
# selector.
    def __init(chain, values={}):
        self.chain    = chain
        self.in_chain = chain
        self.values   = values

    def next(ni=0) #Note: might want to keep track of the chosen path directly.
        self.in_chain = self.in_chain[1:]
        if type(self.in_chain[0]) is ListType:
            self.in_chain = self.in_chain[0][ni]

    def choose(value=None):
        assert len(self.in_chain) > 0
        cur = self.in_chain[0]
        value, ni = cur.choose(value)
        self.values[cur.name] = value # Set the value.
        self.next(ni)
