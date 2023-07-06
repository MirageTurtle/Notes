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

>  Create Time: 2023.06.28   Update Time:2023.07.03

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
- [`jq`](https://github.com/jqlang/jq) for JSON.

### Regular expressions

**!!! Complicated Mention!**

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
  - `<PREFIX> "` and `<PREFIX %` Split a window into two panes horizontally and vertically.
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

## Lecture 6: Version Control(Git)

> Create Time: 2023.06.28   Update Time:2023.06.29

> I like the content of this lecture, since it shows me the beautiful ideas underlying git. I have been a git user since about 3 years ago, but I don't understand what git does when I press enter after the command.

### The well-though-out model

#### Element model

- A *tree* is a folder and a *blob* is a file.
- Git uses a directed acyclic graph to model history. Every state points back to which state preceded it.

#### Data structure

```
type blob = array<byte>
type tree = map<string, blob|tree>
type commit = struct {
	parents: array<commit>,
	author: string,
	message: string,
	snapshot: tree
}
type object = blob|tree|commit
type references = map<string, string>  # readable string to id(SHA-1)
objects = map<string ,object>  # the string is id by SHA-1
```

All things in a commit are pointers. All objects can be addressed by their hash.

> Since I use git for a long time, I still don't note so many things about the using about git but the model things. Sorry about that.

### Extented Reading

- [Pro Git](https://git-scm.com/book/en/v2)

## Lecture 7: Debugging and Profiling

> Create Time: 2023.06.29   Update Time:2023.06.29

### Debugging

> There's a simple and effecient debugging method, i.e., print debugging, and I also use it often. Emphasis here is on logging and debugger.

#### Logging

logging is very useful for debugging systems, whether software systems which you built or operation systems that you are using.

- Color can make logs more readable.
  - [ANSI escape codes](https://en.wikipedia.org/wiki/ANSI_escape_code)
  - [true color](https://github.com/termstandard/colors#truecolor-support-in-output-devices)
- System logging
  - under `/var/log`
  - `systemd` places the logs under `/var/log/journal`
    - You can use `journalctl` to read the logs.
    - The corresponding logs are under `/var/log/system.log` and the corresponding command is `log show`.
  - On most UNIX systems, you can use `dmesg` command to access kernel log.
  - You can use `logger` shell program to log under the system logs.
  - `lnav` provides an improved presentation and navigation for log files.

#### Debugger

Many programming languages come with some form of debugger. For Python, it's `pdb`, which `ipdb` is imporved from.

For more low level programming, `gdb` (and its quality of life modification `pwndbg`) and `lldb` are good choices. (Actually, I always thought they were debugger for C-like language. And by the way, I failed to install gdb on my macOS.)

#### Specialized Tools

If you are trying to trace the **system calls** your program makes, you can use `strace` in Linux and `dtrace` in macOS and BSD (`dtruss` provides an interface more similar to `strace`).

If you need to look at the network packets to figure out the issues in your program. Tools like `tcpdump` and `Wireshark` are network packet analyzers the let you read the contents of network packets and filter them based on different criteria.

#### Static Analysis

Static analysis programs take source code as input and analyze it using coding rules to reason about its correctness. Now, many IDEs and editors have the plug-ins for static analysis, and they support displaying the output of these tools within the editor itself, highlighting the locations of warnings and errors. This is often called **code linting**.

For Python, `pyflakes` and `mypy` are good choices for statcic analysis, `pylint` and `pep8` are examples of stylistic linters and `bandit` is a tool designed to find common security issues. Others are like `black` for Python, `gofmt` for Go, `rustfmt` for Rust or `prettier` for JavaScript, HTML and CSS.

### Profiling

#### Timing

It is common for tools to make a distinction between *Real*, *User* and *Sys* time.

- *Real* - Wall clock elapsed time from start to finish of the program, including the time taken by other processes and time taken while blocked (e.g. waiting for I/O or network)
- *User* - Amount of time spent in the CPU running user code
- *Sys* - Amount of time spent in the CPU running kernel code

`time` command and `time` Python module.

#### CPU

There are two main types of CPU profilers: *tracing* and *sampling* profilers. Tracing profilers keep a record of every function call your program makes whereas sampling profilers probe your program periodically (commonly every millisecond) and record the program's stack. They use these records to present aggregate statistics of what your program spent the most time doing. There's another type of profilers: *line* profiler. Its profiling information includes the time taken per line of code.

`cProfile` Python module and `line_profiler` Python module.

#### Memory

[Valgrind](https://valgrind.org/) and `memory_profiler` Python module.

#### Event

`perf` command.

#### Visualization

You can use `pycallgraph` Python module to generate a flame graph.

#### Resource Monitoring

- **General Monitoring** - Probably the most popular is [`htop`](https://htop.dev/), which is an improved version of [`top`](https://www.man7.org/linux/man-pages/man1/top.1.html). See also [`glances`](https://nicolargo.github.io/glances/) for similar implementation with a great UI. For getting aggregate measures across all processes, [`dstat`](http://dag.wiee.rs/home-made/dstat/) is another nifty tool that computes real-time resource metrics for lots of different subsystems like I/O, networking, CPU utilization, context switches, &c.

- **I/O Operations** - [`iotop`](https://www.man7.org/linux/man-pages/man8/iotop.8.html) displays live I/O usage information and is handy to check if a process is doing heavy I/O disk operations.

- **Disk Usage** - [`df`](https://www.man7.org/linux/man-pages/man1/df.1.html) displays metrics per partitions and [`du`](http://man7.org/linux/man-pages/man1/du.1.html) displays **d**isk **u**sage per file for the current directory. A more interactive version of `du` is [`ncdu`](https://dev.yorhel.nl/ncdu) which lets you navigate folders and delete files and folders as you navigate.

- **Memory Usage** - [`free`](https://www.man7.org/linux/man-pages/man1/free.1.html) displays the total amount of free and used memory in the system. Memory is also displayed in tools like `htop`.

- **Open Files** - [`lsof`](https://www.man7.org/linux/man-pages/man8/lsof.8.html) lists file information about files opened by processes. It can be quite useful for checking which process has opened a specific file.

- **Network Connections and Config** - [`ss`](https://www.man7.org/linux/man-pages/man8/ss.8.html) lets you monitor incoming and outgoing network packets statistics as well as interface statistics. A common use case of `ss` is figuring out what process is using a given port in a machine. For displaying routing, network devices and interfaces you can use [`ip`](http://man7.org/linux/man-pages/man8/ip.8.html). Note that `netstat` and `ifconfig` have been deprecated in favor of the former tools respectively.

- **Network Usage** - [`nethogs`](https://github.com/raboof/nethogs) and [`iftop`](http://www.ex-parrot.com/pdw/iftop/) are good interactive CLI tools for monitoring network usage.

#### Specialized tools

Tools like [`hyperfine`](https://github.com/sharkdp/hyperfine) let you quickly benchmark command line programs.

### Extented Reading

- [Strace -- The Sysadmin's Microscope](https://blogs.oracle.com/linux/strace-the-sysadmins-microscope-v2)
- [strace-zine-unfolded.pdf](https://jvns.ca/strace-zine-unfolded.pdf)
- [Awesome Static Analysis](https://github.com/analysis-tools-dev/static-analysis)
- [Awesome Linters](https://github.com/caramelomartins/awesome-linters)
- [Premature Optimization](http://wiki.c2.com/?PrematureOptimization)
- [How do Ruby & Python profilers work?](https://jvns.ca/blog/2017/12/17/how-do-ruby---python-profilers-work-/)

## Lecture 8: Metaprogramming

> Create Time: 2023.07.04   Update Time:2023.07.04

### Build System

For most projects, whether they contain code or not, there is a "build process'. You need to do some sequences of operations for the task, e.g., some dependencies, some targets and some rules. So you can use so-called "build system" to help you. It find all the transitive dependencies, and then apply the rules to produce intermediate targets all the way until the final target has been produced.

#### `make`

Here we focus on `make` tool, which is one of the most common build systems out there. It has its warts, but works quite well for simple-to-moderate projects.

When you run `make`, it consults a file called `Makefile` in the current directory. This file defines all the targets, their dependencies, and the rules.

Here's a straightforward simple case.

```makefile
paper.pdf: paper.tex plot-data.png
	pdflatex paper.tex

plot-%.png: %.dat plot.py
	./plot.py -i $*.dat -o $@
```

`%` is a wildcard character. `$*` will be substituted by the characters captured by `%`, and `$@` will be substituted by the target string.

I was going to finish this case on my machine, but unfortunately, I deleted LaTeX from my machine and moved my works to Overleaf six months ago. So I just note that, and no files in my repo. You can get the codes on the official websites.

### Dependency Management

There's a lot of tools for dependency managements, but here Jon want to tell about the version number. One relatively common standard is [semantic versioning](https://semver.org/). With semantic versioning, every version number is of the form: `major.minor.patch`. The rules are:

- If a new release does not change the API, increase the patch version.
- If you add to your API in a backwards-compatible way, increase the minor version.
- If you change the API in a non-backwards-compatible way, increase the major version.

When working with dependency management systems, you may also come across the notion of *lock file*. A lock file is simply a file that lists the exact version you are currently depending on of each dependency. An extreme version of this kind of dependency locking is *vendering*, which is where you copy all the code of your dependencies into your own project.

Thank Jon for letting me know about the version number which I've been trying to know about for a long time.

### Continuous Integration System

Continuous integration, or CI, is an umbrella term of "stuff that runs whether your code changes". It's like an event-trigger. When the event triggers, the CI provider spins up a virtual machines (or more), runs the commands in your "*recipe*", and then usually notes down the results somewhere. GitHub Pages is a good example of a CI system.

#### a brief aside on testing

Most large software projects come with a "test suite", and they often use CI for testing. Here're some testing terminology that you may encounter:

- Test suite: a collective term for all the tests.
- Unit test: a "micro-test" that tests a specific feature in isolation
- Integration test: a "macro-test" that runs a larger part of the system to check that different features or components work together.
- Regression test: a test that implements a particular pattern that previously caused a bug to ensure that the bug does not resurface.
- Mocking: to replace a function, module, or type with a fake implementation to avoid testing unrelated functionality. For example, you can mock a network or mock a disk.

## Lecture 9: Security and Cryptography

### Entropy

[Entropy](https://en.wikipedia.org/wiki/Entropy_(information_theory)) is measured in *bits*, and when selecting uniformly at random from a set of possible outcomes, the entropy is equal to `log_2(# of possibilities)`.

### Hash Function

A [cryptographic hash function](https://en.wikipedia.org/wiki/Cryptographic_hash_function) maps data of arbitrary size to a fixed size, e.g., [SHA-1](https://en.wikipedia.org/wiki/SHA-1) and [MD5](https://en.wikipedia.org/wiki/MD5). A rough specification of a hash function is as follows:

```
hash(value: array<byte>) -> vector<byte, N>  (for some fixed N)
```

A hash function has some special properties:

- Deterministic: the same input always generates the same output.
- Non-invertible: it is hard to find an input `m` such that `hash(m) = h` for some desired output `h`.
- Target collision resistant: given an input `m_1`, it's hard to find a different input `m_2` such that `hash(m_1) = hash(m_2)`.
- Collision resistant: it's hard to find two inputs `m_1` and `m_2` such that `m_1` and `m_2` such that `hash(m_1) = hash(m_2)`.

Note that while it may work for certain purposes, SHA-1 is [no longer](https://shattered.io/) considered a strong cryptographic hash function. We can find about lifetimes of cryptographic hash functions for [this website](https://valerieaurora.org/hash.html).

#### Applications

- Content-addressed storage of git.
- Summary of the contents of a file. It's a litte like signature, and you can check the hash number after downloading a file.
- [Commitment schemes](https://en.wikipedia.org/wiki/Commitment_scheme).

### Key Derivation Function

In [cryptography](https://en.wikipedia.org/wiki/Cryptography), a [key derivation function](https://en.wikipedia.org/wiki/Key_derivation_function) (KDF) is a cryptographic algorithm that derives one or more [secret keys](https://en.wikipedia.org/wiki/Key_(cryptography)) from a secret value such as a master key, a [password](https://en.wikipedia.org/wiki/Password), or a [passphrase](https://en.wikipedia.org/wiki/Passphrase) using a [pseudorandom function](https://en.wikipedia.org/wiki/Pseudorandom_function) (which typically uses a [cryptographic hash function](https://en.wikipedia.org/wiki/Cryptographic_hash_function) or [block cipher](https://en.wikipedia.org/wiki/Block_cipher)). (I copy this from wikipedia.)

Applications

- Producing keys from passphrases for use in other cryptographic algorithms (e.g., sysmmetric cryptography).
- Storing login credentials. Storing plaintext passwords is bad; the right approach is to generate and store a random [salt](https://en.wikipedia.org/wiki/Salt_(cryptography)) for each user, store `KDF(password + salt)`, and verify login attempts by re-computing the KDF given the entered password and the stored salt.
  - The parameter passed for KDF not must be `password + salt`. Actually, that can be any format generated by password and salt, e.g., password is string `password` and salt is string `salt`, the parameter can be `psaaslstword`. I get this thought from the wikipedia of salt with Chinese version.

### Symmetric Cryptography

Symmetric cryptography accomplishes this with the following set of functionality:

```
keygen() -> key  (this function is randomized)

encrypt(plaintext: array<byte>, key) -> array<byte>  (the ciphertext)
decrypt(ciphertext: array<byte>, key) -> array<byte>  (the plaintext)
```

The encrypt function has the property that given the output (ciphertext), it’s hard to determine the input (plaintext) without the key. The decrypt function has the obvious correctness property, that `decrypt(encrypt(m, k), k) = m`.

An example of a symmetric cryptosystem in wide use today is [AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard).

### Asymmetric Cryptography

The term “asymmetric” refers to there being two keys, with two different roles. A private key, as its name implies, is meant to be kept private, while the public key can be publicly shared and it won’t affect security (unlike sharing the key in a symmetric cryptosystem). Asymmetric cryptosystems provide the following set of functionality, to encrypt/decrypt and to sign/verify:

```
keygen() -> (public key, private key)  (this function is randomized)

encrypt(plaintext: array<byte>, public key) -> array<byte>  (the ciphertext)
decrypt(ciphertext: array<byte>, private key) -> array<byte>  (the plaintext)

sign(message: array<byte>, private key) -> array<byte>  (the signature)
verify(message: array<byte>, signature: array<byte>, public key) -> bool  (whether or not the signature is valid)
```

The encrypt/decrypt functions have properties similar to their analogs from symmetric cryptosystems. A message can be encrypted using the *public* key. Given the output (ciphertext), it’s hard to determine the input (plaintext) without the *private* key. The decrypt function has the obvious correctness property, that `decrypt(encrypt(m, public key), private key) = m`.

#### Applications

- [PGP email encryption](https://en.wikipedia.org/wiki/Pretty_Good_Privacy). People can have their public keys posted online (e.g. in a PGP keyserver, or on [Keybase](https://keybase.io/)). Anyone can send them encrypted email.
- Private messaging. Apps like [Signal](https://signal.org/) and [Keybase](https://keybase.io/) use asymmetric keys to establish private communication channels.
- Signing software. Git can have GPG-signed commits and tags. With a posted public key, anyone can verify the authenticity of downloaded software.

Asymmetric-key cryptography is wonderful, but it has a big challenge of distributing public keys / mapping public keys to real-world identities. We call this problem as **key distribution**. There are many solutions to this problem.

In fact, we use **hybrid cryptography** in real world. Person A send `symmetric_encrypt(message, symmetric_key)` and `asymmetric_encrypt(public_key, symmetric_key)`. Person B can use `private_key` to get `symmetric_key` and then decrypt for `message`. In this way, the asymmetric cryptography is used for transmitting the symmetric key actually.

### Case studies

> I almost copy the lecture notes for reading.

#### Password managers

This is an essential tool that everyone should try to use (e.g. [KeePassXC](https://keepassxc.org/), [pass](https://www.passwordstore.org/), and [1Password](https://1password.com/)). Password managers make it convenient to use unique, randomly generated high-entropy passwords for all your logins, and they save all your passwords in one place, encrypted with a symmetric cipher with a key produced from a passphrase using a KDF.

Using a password manager lets you avoid password reuse (so you’re less impacted when websites get compromised), use high-entropy passwords (so you’re less likely to get compromised), and only need to remember a single high-entropy password.

#### Two-factor authentication

[Two-factor authentication](https://en.wikipedia.org/wiki/Multi-factor_authentication) (2FA) requires you to use a passphrase (“something you know”) along with a 2FA authenticator (like a [YubiKey](https://www.yubico.com/), “something you have”) in order to protect against stolen passwords and [phishing](https://en.wikipedia.org/wiki/Phishing) attacks.

#### Full disk encryption

Keeping your laptop’s entire disk encrypted is an easy way to protect your data in the case that your laptop is stolen. You can use [cryptsetup + LUKS](https://wiki.archlinux.org/index.php/Dm-crypt/Encrypting_a_non-root_file_system) on Linux, [BitLocker](https://fossbytes.com/enable-full-disk-encryption-windows-10/) on Windows, or [FileVault](https://support.apple.com/en-us/HT204837) on macOS. This encrypts the entire disk with a symmetric cipher, with a key protected by a passphrase.

#### Private messaging

Use [Signal](https://signal.org/) or [Keybase](https://keybase.io/). End-to-end security is bootstrapped from asymmetric-key encryption. Obtaining your contacts’ public keys is the critical step here. If you want good security, you need to authenticate public keys out-of-band (with Signal or Keybase), or trust social proofs (with Keybase).

#### SSH

When you run `ssh-keygen`, it generates an asymmetric keypair, `public_key, private_key`. This is generated randomly, using entropy provided by the operating system (collected from hardware events, etc.). The public key is stored as-is (it’s public, so keeping it a secret is not important), but at rest, the private key should be encrypted on disk. The `ssh-keygen` program prompts the user for a passphrase, and this is fed through a key derivation function to produce a key, which is then used to encrypt the private key with a symmetric cipher.

In use, once the server knows the client’s public key (stored in the `.ssh/authorized_keys` file), a connecting client can prove its identity using asymmetric signatures. This is done through [challenge-response](https://en.wikipedia.org/wiki/Challenge–response_authentication). At a high level, the server picks a random number and sends it to the client. The client then signs this message and sends the signature back to the server, which checks the signature against the public key on record. This effectively proves that the client is in possession of the private key corresponding to the public key that’s in the server’s `.ssh/authorized_keys` file, so the server can allow the client to log in.

## Lecture 9: Potpourri

> I may copy something basic from [the lecture notes](https://missing.csail.mit.edu/2020/potpourri/, because I'm a little familiar with most things of this lecture.

### Keyboard Remapping

For many Vim or Emacs users, I think they remap the Caps Lock to Escape or Ctrl. So do I. But I get a very smart remapping here: **Caps Lock is remapped to Escape if you tap it quickly and to Ctrl if you hold it**. I think it's very good, because there's almost nothing that needs tap Ctrl quickly.

### Daemons

Most computers have a series of processes that are always running in the background rather than waiting for a user to launch them and interact with them. These processes are called daemons and the programs that run as daemons often end with a `d` to indicate so. (Copied from lecture notes.)

Also, if you just want to run some program with a given frequency there is no need to build a custom daemon, you can use [`cron`](https://www.man7.org/linux/man-pages/man8/cron.8.html), a daemon your system already runs to perform scheduled tasks. (Copied from lecture notes.) And if you just want to execute something simple and want to config a user daemon, you also can use cron. `@reboot` can config a job for executing once when system starts.

