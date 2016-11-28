CC=g++
CFLAGS=-std=c++11 -Wall -g -Iinclude/
SD=src/
ID=include/
BD=build/
ATTACKS=example.o histogram.o
CIPHERS=caesar.o vigenere.o
VER=0.1
TARG=cryptoproject

.PHONY: all clean bd

all: $(TARG)

main.o: bd $(SD)main.cpp $(ID)main.h
	$(CC) $(CFLAGS) -DVERSION=$(VER) -c $(SD)main.cpp -o $(BD)main.o

module.o: bd $(SD)module.cpp $(ID)module.h
	$(CC) $(CFLAGS) -c $(SD)module.cpp -o $(BD)module.o

example.o: bd $(SD)attacks/example.cpp $(ID)attacks/example.h
	$(CC) $(CFLAGS) -c $(SD)attacks/example.cpp -o $(BD)example.o

histogram.o: bd $(SD)attacks/histogram.cpp $(ID)attacks/histogram.h
	$(CC) $(CFLAGS) -c $(SD)attacks/histogram.cpp -o $(BD)histogram.o

caesar.o: bd $(SD)ciphers/caesar.cpp $(ID)ciphers/caesar.h
	$(CC) $(CFLAGS) -c $(SD)ciphers/caesar.cpp -o $(BD)caesar.o

vigenere.o: bd $(SD)ciphers/vigenere.cpp $(ID)ciphers/vigenere.h
	$(CC) $(CFLAGS) -c $(SD)ciphers/vigenere.cpp -o $(BD)vigenere.o

$(TARG): main.o module.o $(ATTACKS) $(CIPHERS) $(TOOLS)
	$(CC) $(CFLAGS) $(BD)main.o $(BD)module.o $(addprefix $(BD), $(ATTACKS)) $(addprefix $(BD), $(CIPHERS)) -o $(TARG)

bd:
	@if [ ! -d $(BD) ]; then mkdir $(BD); fi

clean:
	if [ -x $(TARG) ]; then rm $(TARG); fi
	rm -rf $(BD)*.o
