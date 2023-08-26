# Introduction to Computer Systems

## Lab-environment related

When I worked for labs of this lecture on **macOS**, I found that I cannot use the lab tools properly on macOS. So I plan to use docker for lab environment. However, because I use docker little on macOS, I found I cannot get `docker0` interface (more details on [there-is-no-docker0-bridge-on-the-host](https://docs.docker.com/desktop/networking/#there-is-no-docker0-bridge-on-the-host)), which means I cannot use ssh to connect the lab environment directly. (It's also ok if I just use linux in docker to compile something by `docker exec xxxx`, but I want to build a general image for others who want to finish these labs on macOS, even other platform.)

Finally, I use port-forwarding to figure it out.

### Get started

```
cd /path/to/directory/within/dockerfile_and_compose.yml
UID=$(id -u) GID=$(id -g) docker compose up -d
```

Now you can use `ssh root@127.0.0.1 -p 8022` with the password `csapplab` to login the environment. You may find that the user in docker is root, but don't worry about the permission of the compiled executable files. Feel free to use it.

If you're familiar with `docker` and `docker-compose`, you can also customize them for you.

If you only want the image, you can just use `docker pull mirage7/csapp_lab` to pull it.
