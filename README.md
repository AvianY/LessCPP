# Added README

## Install

### Windows - Mingw
You will need Mingw with `make` and `g++` compiler installed. Once in Mingw-cli you can install those using pacman.
```
pacman -S make
pacman -S g++
```
Download and place appropriate [SFML](https://www.sfml-dev.org/) on project-root directory.

As a workaround `make` should be called from PowerShell! (LSHIFT-right click...)
Make sure `obj` directory exists; it gets deleted by make clean.
```
c:\path\to\make\make.exe
```

Copy .dll files from `SFML-2.5.1/bin` to project-root folder.
Done.


## TODO
Cross platform makefile

## Debate
### Cross platform compilation
https://stackoverflow.com/questions/9791127/using-make-for-cross-platform-compilation

