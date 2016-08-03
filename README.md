# Nexus Communication Infrastructure

Nexus infrastructure tries to ease communication stack development in cross platform embedded projects.
The design is simple completely OO, giving a great baseline for the comm infrastructure to build on.

## Getting Started

Easiest way to compile / cross-compile this library is to run:
```
cmake ./
make 
```

Or use the cmake UI on Windows

### Windows

When using the Socket module you need to link your project with the Ws2_32.lib. See http://msdn.microsoft.com/en-us/library/windows/desktop/ms737629(v=vs.85).aspx


### Linux
If you don't like cmake - Use the makefile that comes with this library (Simply call `make`)

In order to cross-compile for different architectures (for example, arm), use 'make CXX=arm-linux-gnueabi-g++'
