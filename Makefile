.PHONY: all clean install

all: Polar-clang.c
	clang -o Polar-clang Polar-clang.c

install:
	cp -a Polar-clang /usr/local/bin/Polar-clang

clean:
	rm -rf Polar-clang
