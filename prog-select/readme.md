
# Prog-select

Very simple gtk program that selects from commands. Pressing tab, the commands
get listed according to the number of times they were chosen.(which is stored 
in  `~/.prog-select`.)

Better stuff available. Tab completion on typical terminal applications is
nearly as good! Hey, this is the 'random crap' folder.(it also produces a lot 
of warnings)

Screenshot, with just some random stuff i entered.
(because it didnt have anything in the lists because i dont use it)

<img src="doc/screen.png">

# Compilation
Have the dependencies, and type `make`. `pkg-config-store` is in the git repo incase
you dont have it. If it somehow insists on rewriting it, `git checkout pkg-config-store`
and remove the file from the `prog_select:` line in the `Makefile`.

(this is overkill no one is going to use this..)
