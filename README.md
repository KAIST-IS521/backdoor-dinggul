# Description of Activity 3

### dinggul(Minjoon Park)

## What I made (required)
- [./interpreter/](https://github.com/KAIST-IS521/backdoor-dinggul/tree/master/interpreter)
There was already some skeleton code for VM implementation. My TODO was,
implementing functions for each instruction and setting context for those functions.
Finally, I checked boundary conditions for memory access and code access.

- [./login/login.mini](https://github.com/KAIST-IS521/backdoor-dinggul/blob/master/login/login.mini)
This program is an implementation of "Listing 1" in the spec docs.

- [./test/test.mini](https://github.com/KAIST-IS521/backdoor-dinggul/blob/master/test/test.mini)
I wrote this program for covering my VM implementation.

- [./test/test.md](https://github.com/KAIST-IS521/backdoor-dinggul/blob/master/test/test.md)
It contains pseudocode of `test.mini`, brief description, and sample inputs/outputs.

- [./backdoor/](https://github.com/KAIST-IS521/backdoor-dinggul/tree/master/backdoor)
Since it is almost impossible to identify my login program (except
copy and compare whole program, but it is not cool, so I don't want it),
I store program's last output by puts instruction. When it is "User: ",
I just check that input by gets is "superuser". This way is not a solid,
but I can identify all my classmates' login programs because my
classmates just following "Listing 1" at the spec doc.
That's the point why I implemented in this way.

- [./README.md](https://github.com/KAIST-IS521/backdoor-dinggul/blob/master/README.md)
THIS FILE

## Additional stuff
- [./compiler/good.py](https://github.com/KAIST-IS521/backdoor-dinggul/blob/master/compiler/good.py)
It is a just wrapper program of given compiler. Since I needed some syntatic
sugars to implement `test.mini` and `login.mini`, `good.py` contains some functions such as
label, comment, and some types(char, hex) for puti instruction.
> USAGE: `./good.py ../login/_login.mini`

## What I learned
- Through this activity, I learned how to write commit message.
I just learned convention of commit message at Activity 1, but it was just following, not becoming my skill.
I tried to make comment on source code during developing process all the time.
Repeated commit in developing process was really helpful to practice.

- During this activity, I tried to use new shortcuts of vim continuously.
Since I think I have had unnecessary key-downs for using vim, I learned
additional features of vim and practice it (such as mark).
