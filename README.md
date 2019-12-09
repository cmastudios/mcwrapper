mcwrapper
=========

MCWrapper is a very simple tool that wraps another program and translates
terminate and interrupt signals (Ctrl-C) into sending a "stop" message.
It was designed to safely stop Minecraft servers when running from a
service environment like systemd.

Building
--------
Requires CMake.
```
cmake .
make
make install
```

Example
-------
Check out `minecraft.service` for an example of a systemd unit file that uses
this wrapper correctly.
