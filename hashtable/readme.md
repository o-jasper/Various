
# Hashtable and conser
If you are looking for things to to use as a library, i hope there are 
better-tested and thought-out ones. I wrote most of this -basically as
 exercise- quite a while ago now. I basically just cleaned it up just a bit.

It consists of two parts; an **hash table**;
<table>
<tr><td>hash4str.c</td> <td>Hash table</td></tr>
<tr><td>lookup3.c</td>  <td>hash functions by Bob Jenkins, in public domain</td></tr>
</table>
And beyond that, **a cons-cell parser**;
<table>
<tr><td>symbol.c</td> <td>Symbol stuff, including some namespacing.</td></tr>
<tr><td>cons.c</td>   <td>Various types in one object; integer, float, conscell, symbol.
implements reader that is tested test/cons.c
</td></tr>
</table>

There are also some **tests**, `make` compiles them, the two shellscripts are 
examples of running them.

`hash4str_test` counts words, `cons_test` conses a file, parsing s-expressions
and immediately outputting them.

## License
I placed everything i wrote under the public domain.
