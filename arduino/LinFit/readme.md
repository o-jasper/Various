# Linear fit and averages
Averages and linear fits can be calculated by keeping track of some sums.
This allows for that, and using those sums to calculate averages/fit info.

Sums are kept in a `int32_t`, so that has limitations. Use
`divide_down(LinFit*,int16_t)` to instead have a running average/fit that
deweights older information put in.

## Usage
For just an average, `AvFit` for fits(and two averages) use `LinfFit`.

`reset(&thing)` resets either, its the same as the constructor

`add_to_fit(&avfit,x,w=1)` adds a data point, analogously `add_to_fit(&linfit,x,y,w=1)` 
adds two. (`w` is the weight, must be integer)

`divide_down(&avfit,divisor)` takes weight off older input relative to newer
input.(newer input weights `divisor` more) Can be used to make a running average.

#### Calculating results

`f_calc_av()`, `f_calc_errorsqr(..)` calculates the average, and error
squared of the x axis. 

`f_calc_A`, `f_calc_B` calculate the fit.

`l_` versions do the same, but with integer aritmetic, expect overflow..

## More
There is also `PartAvFit`, which doesnt have the counter build-in. The idea is
that you can use multiple averages, with a single counter. Any calculation takes
the count as an argument.

## TODO
Clearly needs more testing.
