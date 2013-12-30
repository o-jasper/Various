# Tiny capacitence sensor by ~&mu;s emptying/filling time

You have a thing you want to determine the tiny capacitance of.(Which may
depend on objects/fingers nearby) This is connected to the drive pin via a
large resistor, basically the lower, the larger, from 100K**&Omega;**
100M**&Omega;** or so, also connect it to a sense pin, possibly with a 
protecting resistor. (~k**&Omega;**)

Currently the sensing method is the 'disengaging' one. The other two may have
uses though, and they're largely compatible, so using multiple might reveal 
more information that just using one.

There is also a cross-capacitive sensing charlieplex like code.

None of them use interrupts, interrupts may.. interrupt these things and make
the counters count too few ticks.

## disengaging_capsense

Senses using only one pin and a voltage sufficient to make a pin go `HIGH`.
The pin is connected via the protective resistor, and to high via a large 
resistor.

The pin must start in `LOW`, `OUTPUT` mode, and then have time to actually be
low.(If the 'lowness' varies the result will aswel) When it is time to 
measure, `disengaging_capsense(pin)` is called which sets the pin to `INPUT` 
mode('disengaging it') and the time is measured untill it is pulled up by the
high side via the high resistor. 

Note that the value `LOW` and `HIGH` cannot be swapped here due to the pullup
resistor when in `HIGH`, `OUTPUT`.  The up voltage going lower means it will
take a bit longer. Which is good as long as it eventually goes to `HIGH`.

This is currently the 'main method' used.

### Functions

    void disengaging_mode(int sense_pin);
    void disengaging_mode_with_drive(int sense_pin, int drive_pin); //Compatible with drive pin, if other modes also used on same thing.
    int disengaging_capsense(int pin); //Needs LOW, leaves LOW
    int drive_capsense_up(int drive_pin, int sense_pin); //Needs LOW, leaves HIGH.

## 'Crosscap' sensing'
By having one driver and senser, basically the thing to capacitive sense on
can be chosen by choosing the driver. (Charlieplex-like) It basically uses the


Basically its a grid, lets say, vertically wires of the sensers and horizontally
the drivers. At each crossing there is a capacitive plate where the the sensers
connect with diodes and the drivers with a high resistor.

Initially everything is `OUTPUT`,`LOW`, a pair is selected by setting the desired
driver `HIGH`, and the desired measuring `INPUT`, basically disconnecting it from
the `LOW` there.('disengaging') The capacitive pad then starts to go `HIGH` through
current through the resistor, taking time depending on the current capacitance.
It is not held `LOW` by other senses that are still in `OUTPUT` due to the diodes.

**TODO** an image says it better..

### Usage
Kindah limited to using the source yourself, otherwise a bit limited i reckon.

`crosscap_sense_from` and `crosscap_sense_from` are `#define`d integers indicate
from which pin numbers driving/sensing starts. `inline int cap_sensor(int)`
actually provides pin numbers, however.(It is somewhat annoying that these are build-in)

`crosscap_prepare(drive_cnt,sense_cnt)` prepares pins, setting the drivers and sensers
all to `OUTPUT` and `LOW`. 

`crosscap_measure_1_drive(uint16_t* sums, drive_nr,sense_cnt, sense_times)` measures 
a single row.(Selecting it by driving the voltage `HIGH` on that row.) The number of
ticks it was high is *added* to sums. `sense_times` is the maximum number of ticks.

`crosscap_measure(uint16_t* sums, drive_nr,sense_cnt, sense_times)` measures 
everything

To measure a single pad, `disengaging_capsense(pin)` can be used directly after
`crosscap_select_driver`,assuming only one is selected.

## Other uses
Currently an idea(i think others have done similar) is to basically use this
directly on reverse-biased leds (possibly nearly dark)optotransitors for light
intensity sensing.(at frequencies depending on the led/optotransistor) If two
pins are used, or used 'Charlieplex-like' The leds would be usable as light 
emitters alwel!

# Other methods
NOTE: barely tried these, and now, i doubt all of it makes sense.

## Driving capsensing

Two pins are used, one to drive, and the other just senses. The one is always
in `OUTPUT` and the other `INPUT`. The drive pin functions as the
'outside pin', but in this case it can be `LOW` aswel as high. 

Then `drive_capsense_up(drive_pin,sense_pin)` assumes the sense and drive
starts `LOW` and sets `drive_pin` to `HIGH` and then counts until `sense_pin`
is also `HIGH`, `drive_capsense_down` does the opposite.

`drive_capsense_ensure(drive_pin,which)` tries to get the sense and drive pin
up and/or down. But that takes a while.

#### (dis)Advantages

A disadvantage is the two pins needed. Sharing drive pins means they can 
affect each other and ensuring it is sufficiently low/high before
taking the corresponding measurement takes time. 

An advantage is that it measures both going low and high.

## 'Edge driving'
Uses the same layout as the driving capsense. But instead of expecting the
voltage to be completely down/up, it tries to get it right on the edge. 
The idea is that it latches a little, or something.
**(To be honest i am not sure it makes sense, now)**

It starts at a voltage at the edge of `LOW` and `HIGH`, but at that point
being `HIGH`. If initially finds the sense pin `LOW`, it will drive it `HIGH`
until it is `HIGH` and then pull it `LOW` again to find the edge. 
If it is initally `HIGH` it will just pull it `LOW` until the edge.

After it is on the edge and `LOW`,(according to `digitalRead`) it pulls it 
`HIGH` and counts until is actually is, putting the result in `result[0]`.
Then it is on the `HIGH` side of the edge and pulls it `LOW`, timing again,
and denoting that time in `result[1]`.

### Functions
    
    void drive_mode(int drive_pin, int sense_pin);
    int drive_capsense_up(int drive_pin, int sense_pin); //Needs LOW, leaves HIGH.
    int drive_capsense_down(int drive_pin, int sense_pin); //Needs HIGH, leaves LOW.


#### (dis)Advantages

Not sure yet, have seen some strange effect where `result[0]` goes up and
simultaniously `result[1]` goes down. Suspect it might be contact conduction?
One reason to think it might have been contact is that the plots of the
result are very plateau-ed.

There is the potential that doing this might measure something else than the
first two. Maybe it produces a bigger dI/dt and there is more of a 
selfinductance component to it. If things make different measurements, it 
might be useful to use multiple measurement methods.

### Functions

    void edge_mode(int drive_pin,int sense_pin);
    void edgedrive_capsense(int drive_pin, int sense_pin, int* result);

# TODO (well maybe)

* Tell what is in what files.

* More about cross-capsense stuff

* implement versions that don't use `digitalWrite`, and one that uses 
  interrupts.
  
* Explore the use of draining of backwardly placed LEDs.(should be possible
  to both turn them on and use them for light sensing)

* Example sketch.
