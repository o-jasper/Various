
class Param:
    text = ""
    default = None
    
    def choose_parse(self, string):
        return self.choose(eval(string))  # TODO too trustful.

    def tell(self):
        print(self.text + ("" if (self.default is None) else "(" + self.default + ")"))

class ParamBasic(Param):
    def __init__(self, name, type, default=None):
        self.name = name
        self.type = type
        self.default = default

    def choose(self, value=None):
        assert type(value) is self.type
        return (value, 0)

class ParamListBox(Param):
    def __init__(self, name, list, default=None):
        self.name = name
        self.list = list
        self.default = default

    def choose(self, value=None):
        if value is None:
            value = self.default
        for i in range(len(self.list)):
            if list[i] == value:
                return (value, i)
        return None

    def tell(self):
        text = ("options:" if (self.text == "") else self.text)
        print(text + ("" if (self.default is None) else "(" + self.default + ")"))
        for el in self.list:
            print(el)

class ParamIntSep(Param):
    def __init__(self, name, default=0):
        self.name    = name
        self.default = default

    def choose(self, value=None):
        if value is None:
            return (self.default, self.default)
        assert type(value) is int
        return (value, value)

    def parse(self, string):
        return self.choose(int(string))

class ParamValueSep(Param):  # Single-value
    def __init__(self, name, threshhold=0, default=0):
        self.name       = name
        self.default    = default
        self.threshhold = threshhold

    def choose(self, value=None):
        if value is None:
            value = self.default
        assert type(value) is float or type(value) is int
        return (value, 0 if (value < self.threshhold) else 1)

    def parse(self, string):
        return self.choose(float(string))

class ParamSelect:
# Chain of choices if one entry is a list, it is expected the former one was a
# selector.
    def __init__(self, chain, values={}):
        self.chain    = chain
        self.in_chain = chain
        self.values   = values

    def next(self, ni=0):  # Note: might want to keep track of the chosen path directly.
        self.in_chain = self.in_chain[1:]
        if type(self.in_chain[0]) is list:
            self.in_chain = self.in_chain[0][ni]

    def choose(self, value=None):
        assert len(self.in_chain) > 0
        print(string(value) + " + " + string(type(value)))
        cur = self.in_chain[0]
        value, ni = cur.choose(value)
        self.values[cur.name] = value # Set the value.
        self.next(ni)

    def choose_parse(self, string):
        return self.choose(eval(string))  # TODO too trustful.

    def tell(self):
        cur = self.in_chain[0]
        cur.tell()
