
GET_ID=while IFS== read N V ; do case \$$N in ID) echo \$$V ;; esac ; done </etc/os-release

.PHONY: Makefile

Makefile: $(shell bash -c "${GET_ID}")

all: err-first

err-first: err-first.cpp
	gcc -o $@ $<

err-first-0/usr/bin/%: %
	mkdir -p $(dir $@)
	ln -f $< $@
	chmod 755 $@

%.deb: %/usr/bin/err-first
	dpkg-deb --root-owner-group --build $*

Debian: err-first-0.deb

ubuntu:	Debian




