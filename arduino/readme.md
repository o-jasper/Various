# Stuff i wrote for some arduino sketches
Note that it isnt particularly well-tested. It is nothing special, mostly
trivial stuff, but writing it this way makes sketches better organized.

There is also (completely untried)
[OptoCom](https://github.com/o-jasper/OptoComm), that i didnt include here.
Here is:

* The capacitive sensing bit is probably the most interesting to bit, also has
  'cross capacitive sensing'(charlieplex-like)
* Debouncing buttons.
* Averaging and linear fitting stuff.(latter not tried in actual sketch either)

I didnt use member functions, opting to use function overloading instead.

## Level of testing
Dont feel it is that good, would-be-nice to at least see that it compiles,
or 'logic-of-the-situation' tests.
