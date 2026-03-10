# XPrtSc
Minimal screenshot/print screen program for X11

The output format is ppm. Conversion to your favourite format can be done using
your favourite image converter (e.g. ffmpeg).

Inspired/Modified from sxot (https://codeberg.org/NRK/sxot).

## Installation
### Linux
#### With Make
```sh
$ make
$ make clean install
```

#### Without Make
```sh
$ cc -o nob nob.c
$ ./nob
$ ./nob clean install
```

### Other OSes
Good luck!

## Usage
### Printing to STDOUT
```sh
$ xprtsc
```

### Saving to a file
```sh
$ xprtsc > file.ppm
```

### Converting with FFmpeg
```sh
$ xprtsc | ffmpeg -i - file.png
```
