

default: all

all: fork fork1 fork3 hello

clean:
	rm foo.txt

distclean: clean
	rm fork
	rm fork1
	rm fork3
	rm hello
