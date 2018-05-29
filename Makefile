CFLAGS = -g -Wall -Werror -pedantic -Wextra -Iinclude
 
all: clean utf
 
utf: utfconverter
	@mkdir -p bin
	gcc -g -o bin/utf build/utfconverter.o
 
utfconverter:
	@mkdir -p build
	gcc $(CFLAGS) -c src/utfconverter.c -o build/utfconverter.o

debug: clean utf

clean:
	rm -rf bin
	rm -rf build
