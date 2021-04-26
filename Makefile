
.PHONY:	Makefile

err-first: err-first.cpp
	gcc -g -o $@ $<

all:	err-first

Debian: err-first

Ununtu:	Debian

Makefile:
	while IFS== read N V; do case "$N" in ID)make $N;; esac ; done </etc/os-release
	
