# The Missing Semester of Your CS Education

## Lecture 1: Course overview + the shell

Because I'm a little familiar with the shell, so I decide to pass the main contents and only to record something I'm not familiar with.

- In Linux, the `sysfs` file system is mounted under `/sys`, and `sysfs` exposes a number of kernel parameters as files. For example, the brightness of your laptop's screen is exposed through a file called `brightness` under `/sys/class/backlight`
- Operations like `|`, `>` and `<` are done by *the shell*, not by *the individual program*. So when you try to use command `sudo echo 3 > brightness` for changing the brightness, you'll get a `Permission denied` error. We can use command `echo 3 | sudo tee brightness` to achieve the goal as `tee` is an individual program.

And still, as we all know, **the `man` program is our friend**.

