# Do stuff after some time has passed
Does *not* handle, the clock going right (and fairly useless for that reason)

**TODO** fix that before any sort of release.

## Reference
`DoAfter timer(some_period,time)` would create and start the timer.

`peek_do_now(&timer,time)` returns if the time has expired, but does not reset. 
`do_now_p(&timer,time,max_frac)` returns whether it expired and does reset.

`coerce_do_now_p(&timer,time)` resets, and returns if has expired.
(before resetting it of course)

`time` may be left out at any point, the default is `millis()`.
