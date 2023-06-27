# The Missing Semester of Your CS Education

## Lecture 1: Course overview + the shell

> Create Time: 2023.06.23   Update Time:2023.06.23

Because I'm a little familiar with the shell, so I decide to pass the main contents and only to record something I'm not familiar with.

- In Linux, the `sysfs` file system is mounted under `/sys`, and `sysfs` exposes a number of kernel parameters as files. For example, the brightness of your laptop's screen is exposed through a file called `brightness` under `/sys/class/backlight`
- Operations like `|`, `>` and `<` are done by *the shell*, not by *the individual program*. So when you try to use command `sudo echo 3 > brightness` for changing the brightness, you'll get a `Permission denied` error. We can use command `echo 3 | sudo tee brightness` to achieve the goal as `tee` is an individual program.

And still, as we all know, **the `man` program is our friend**.

## Lecture 2: Shell Tools and Scripting

> Create Time: 2023.06.24   Update Time:2023.06.24

> Actually, in my [Notes repo](https://github.com/MirageTurtle/Notes), I plan to study shell scripting, :)

### Basic 

- Space is special in the shell, which is used to seperate parameters.
- `$something` stands for a variable, or a value sometimes.
  - `$foo` is for the variable `foo`.
  - *`$0`*, `$1`, `$2` are for *the executed command* or *the name of the script*, the 1st parameter, the 2nd parameter, etc.
  - `$@` is for all the arguments.
  - `$#` is for the number of arguments.
  - `$_` is for the lastest parameter.
  - `$?` is for the code returned by the previous command.
  - `$$` is for the Process Identification Number(PID) of the current script/command.
- The variables quoted by `"` will be substituted into variable values whereas `'` delimiter will not.
- `&&`(and operator) and `||`(or operator) are short-circuiting operators. `;` will not short-circuit.
- `$(CMD)` will execute `CMD`, get the output of the command `CMD` and substitute it in place.
- Process substitution: `<(CMD)` will execute `CMD` and place the output in a temporary file and substitute the `<()`with that file's name. This is useful when the commands expect file instead of STDIN.

### Control Flow

```shell
#!/usr/bin/env bash

echo "Starting program at $(date)"  # " delimiter

echo "Runing program $0 with $# arguments with pid $$"  # " delimiter

for file in "$@"; do
    # When pattern is not found, grep has exit status 1
    # We redirect STDOUT(1> or >) and STDERR(2>) to a null register(/dev/null)
    grep foobar "$file" > /dev/null 2> /dev/null
    if [[ $? -ne 0 ]]; then  # [[]] instead of []
        echo "File $file does not have any foobar, adding one..."
        echo "# foobar" >> "$file"  # >> instead of >
    fi
done
```

See comments for details. Among those to be highlighted are:

- It is a good practice to write shebang lines using the `env` command that will resolve to wherever the command lives in the system, increasing the portability of your scripts.
- When performing comparisons in bash, try to use double brackets `[[]]` in favor of simple brackets `[]`. (I remember I know there are some differences between these two symbols, but I forget them.)
- `-ne` means not equal, and a detailed list can be find in the manpage for command `test`(I don't know this ever before).

### Globbing

- Wildcards. You can use `?` to match one character, and use `*` to match any amount of characters.
- Curly braces. You can use `{}` to merge a common substring in a series of commands. You can use `,` for the separate different parts in the commands(e.g., `{a,c,f,z}`, `{0,4,9}`), and use `..` for the consecutive different parts(e.g., `{a..z}`, `{0..9}`).

### Shell Tools and Skills

- [shellcheck](https://github.com/koalaman/shellcheck)

- [tldr pages](https://tldr.sh/) and [tldr InBrowser.App](https://tldr.inbrowser.app/)
- [fd](https://github.com/sharkdp/fd) for better `find`
- [ack](https://github.com/beyondgrep/ack3), [ag](https://github.com/ggreer/the_silver_searcher) and [rg](https://github.com/BurntSushi/ripgrep) for better `grep`
- In most shells, you can use `C+R` to perform backwards search through your history. Or use [fzf](https://github.com/junegunn/fzf) for a better fuzzy find.
- `tree` and [broot](https://github.com/Canop/broot)

> Besides all above, I know I have many commands to study, e.g., `awk` and `xargs`. I used some commands for some temporary tasks based on some online information, but I don't understand them actually. I will study them in my future shell works, and update this note if need.

