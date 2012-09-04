# Laplace solver by Relaxation method

Uses relaxation method to solve the 2d Laplace equation numerically. It is 
really easy, i am sure first-year physics students often get it as an 
exercise. (And as example of numerical simulation.)

Uses relaxing and then doubling grid number repeated to speed it up.
(approx 1 second for 1024x1024 on this computer,
so similar for 100x100x100 if the same method were used in 3d.)

Made it because Electrodynamics can be stated as a 4-dimensional wave 
equation. And the wave equation can be stated in the Laplace equation, when 
you pose the field as superposition of φ(ω,x)exp(i ω*t). I haven't actually 
used it that way, though.

Not really knowledgeable of the subject, but unsure about 'over' relaxation 
method; don't know exactly in what ranges that converges properly. Seems 
likely that for big grids doubling with interpolation already gave much more 
speed; over-relaxation can still do one pixel/turn at maximum.

## Compilation and use 

See `examples/ex1.sh` (how) to compile, running it will compile that example.
There is also a makefil.

To use just run the executable. To change sources/constraints, see the 
example, you can replace sources and constrains there, with mk_fixed_Rect to 
specify the areas as argument of 'source' and 'constraint'.

Only dependency is base SDL. Note that this mostly just example..

## Files
<!--markup doesnt have this??--> 
<dl>
<dt>pre-main.c</dt> <dd>Some stuff to not have to put code for each case.</dd>
<dt>main.c</dt>		<dd>Ties things to show result with SDL.</dd>
<dt>ex1.c</dt>		<dd>Example how to use (see compilation and use.)</dd>
<dt>field.c</dt>    <dd>Code for field, drawing, doubling of size of field, 
	                    sources and constraints.</dd>
<dt>relax.c</dt>    <dd>Relaxation algoritm. (It is very simple.)</dd>
</dl>

### Maybe todo

* Check result of 'relax_source_ness' to be consistent.
* Implement other algorithms(overrelaxation)
* Stuff to help with wavefunction solutions φ(ω,x)exp(ω*t), and stuff to help
  with those solutions in electromagnetism.
* Other output/input mechanisms.
* CL cffi package.

## License
Author: Jasper den Ouden, placed in public domain. 
