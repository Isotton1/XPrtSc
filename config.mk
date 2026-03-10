# XPrtSc version
VERSION = 0.2.0

# paths
PREFIX = /usr/local

# libs
LIBS = -lX11

# flags
CFLAGS = -std=c99 -pedantic -Wall -Wextra
LDFLAGS = $(LIBS)

# compiler
CC = cc
