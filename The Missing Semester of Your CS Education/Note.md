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

## Lecture 3: Editors(Vim)

> Create Time: 2023.06.28   Update Time:2023.06.28

Even though I'm an Emacser, I listened to this lecture. Because I was a Vimer, and I believe there're some commonalities between the editors. Actually, Vim and Emacs are both powerful for me. But when I first learned Vim, I did't know much about CS and I didn't have the ability to study Vim very well. After that, I started using VS Code, one of the mose popular programming editor now. Now I'm still a Emacs beginner, and I can't do some quick operations which Vim can do easily by one or two keys. But I think I have the ability to study Emacs, and I can do some quick operations which I couldn't do by Vim(I'm not familiar with Vim actually). I'm not saying Emacs is better Vim, but I think it's more suitable for me. **Wish everyone finds his editors.**

## Lecture 4: Data Wrangling

>  Create Time: 2023.06.28   Update Time:2023.06.28

Data wrangling means changing data with one format into some different formats. When you want to analyse some data or get some sepcific informations from too many informations, it's very useful.

### Some useful tools maybe

#### Basic

- **Pipeline `|`**
- **`grep`**
- **`sed`**: a stream editor, based on `ed` which is an old editor.
- Regular expressions
- `wc`
- `sort`
- `uniq`
- **`awk`**: a column-based stream processor.
- `paste`
- `xargs`

#### Advanced

- `bc`
- Some advanced program language scripts or programs
- `gnuplot`
- `ffmpeg`
- `feh`
- `pup` for HTML.
- `jq` for JSON.

### Regular expressions

!!! Complicated Mention!

#### Basic

- `.` is for any characters, and you need to use `\.` for period.
- `*` is for any times(0 or more), `?` is for 0 or 1 times, `+` is for 1 or more times.
- `^` is for the begin of a line, while `$` is for the end.
- `[]` can list all character candidates, and you can use range syntax in it.
- `{}` is for repetition times.
- `^` is for not. For an instance, `[abc]` means all candidates are a, b, and c, `[^abc]` means not a, not b, and not c.
- The default mode is greedy match, you can use non-greedy match by suffix a `*` or a `+` with a `?`.
- `\d` means any digit, while `\D` means any non-digit character.
- `\w` means any alphanumeric character, while `\W` means any non-alphanumeric character.
- `\s` means any whitespace, while `\S` means any non-whitespace character.

#### Capture group

- Use `()` capture some informations, which we call capture group.
- The capture group is useful because you can use the content of these groups by index if need.
- Actually, I think capture groups are sometimes wasteful. If you want use `*`, `?` or `+` for some words or some character groups, you need to use syntax like `(some words)?`, and this capture group is wasted if you don't care about it.

#### Some helpers

- Shot interactive regex tutorial: [RegexOne](https://regexone.com/)

- Debugger: [regular expressions 101](https://regex101.com/)

## Lecture 5: Command-line Environment

> Create Time: 2023.06.28   Update Time:2023.06.28

### Job Control

- Unix communication mechanism: signal. You can use `man signal` for details.
  - `C-C` is for interrupting the current program(`SIGINT`).
  - `C-\` is for quiting the current program(`SIGQUIT`).
  - `C-Z` is for stopping the current program(`SIGSTOP`).
  - Some signals can not be captured by program, e.g., `SIGKILL`.
- You can use `jobs` command for your all jobs.
- Using `kill` command to send a signal to a job.
- Closing the terminal when a program is running will send `SIGHUP` to the program.
- You can use `nohup` command for a program to no `SIGHUP` signal.
- `bg` and `fg` command is for continuing the paused job in the *background* and *foreground* respectively.

### Terminal Multiplexers

> Now I have another dotfile to configure…

Hierarchy:

- **Sessions**: A session is an independent workspace with one or more windows.
- **Windows**: Equivalent to tabs in editors or browsers, they are visually separate parts of the same session.
  - `<PREFIX> c` *Create* a new window.
  - `<PREFIX> p` and `<PREFIX n` *Previous* and *next* window respectively.
  - `<PREFIX> ,` Rename the current window.
  - `<PREFIX> w` List all windows of the current session.
- **Panes**: A pane is corresponding a shell.
  - `<PREFIX> ,"` and `<PREFIX %` Split a window into two panes horizontally and vertically.
  - `<PREFIX> <DIRECTION>` Move to the pane in the specified direction.
  - `<PREFIX> z` Toggle zoom for the current pane.

Tmux is like Emacs, you can use commands for all things, and you can use shortcuts for some commands.

Many people remapped the prefix to `C-A` instead of the default `C-B`.

```conf
unbind-key C-b
set-option -g prefix C-a
bind-key C-a send-prefix
```

You can use `C-d` Terminate the current shell.

#### Tutorial for tmux

- [A Quick and Easy Guide to tmux](https://www.hamvocke.com/blog/a-quick-and-easy-guide-to-tmux/)
- [Linux Command Line Adventure: Terminal Multiplexers](http://linuxcommand.org/lc3_adv_termmux.php)

### Dotfiles and Remote Machine

> I'm familiar with these two parts. So I didn't note anything for this. Sorry about this.
