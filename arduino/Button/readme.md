# Simple button with (non-interrupt)wait debouncing
Thats all, trivial, of course, but imo 'sketches' are more nicely written if
written more modularly. (Probably no performance impact other than `digitalRead` 
being slower than direct use.)

## Usage
Create it with the constructor `Button button(pin);`, then use it with 
`downp(&button,wait)`, where `wait` is the number of times `downp` finds it
`HIGH`/`LOW` before it considers the button to be high/low.
(maximum and default is `126`)

`downp` needs to be called a lot, but not too much that it will all measure on a
down moment in bouncing. Maybe once in 0.1ms or something.
(depends on `wait`)

## Reference

`Button`, the class that represents the button.

`Button(uchar pin)` creates one *and* sets the pin `HIGH` and `INPUT`, which is
pull-up mode. 

`downp(&button,wait=126)` see if the bottom is down.(`p` stands for 'predicate')

`down(&button,wait=126)` is *negative* if the button is down, and *positive* if it
is up.(`p(&button,wait)==(down(&button,edge)>0)`) specifically `±3` indicates it
is consistant, and `±1` means bouncing..

`button.state` is the counter used, `button.state>0` indicates if it is up,
however, `down`(or `downp`) need to be called regularly.
