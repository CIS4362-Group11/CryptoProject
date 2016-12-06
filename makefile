CC=g++
CFLAGS=-std=c++11 -Wall -g -Iinclude/
SD=src/
ID=include/
BD=build/
ATTACKS=example.o histogram.o caesar_attack.o ngram.o kasiski.o hillatt.o
CIPHERS=caesar.o vigenere.o hill.o
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

caesar_attack.o: bd $(SD)attacks/caesar_attack.cpp $(ID)attacks/caesar_attack.h
	$(CC) $(CFLAGS) -c $(SD)attacks/caesar_attack.cpp -o $(BD)caesar_attack.o

ngram.o: bd $(SD)attacks/ngram.cpp $(ID)attacks/ngram.h
	$(CC) $(CFLAGS) -c $(SD)attacks/ngram.cpp -o $(BD)ngram.o

kasiski.o: bd $(SD)attacks/kasiski.cpp $(ID)attacks/kasiski.h
	$(CC) $(CFLAGS) -c $(SD)attacks/kasiski.cpp -o $(BD)kasiski.o
	
hill.o: bd $(SD)ciphers/hill.cpp $(ID)ciphers/hill.h
	$(CC) $(CFLAGS) -c $(SD)ciphers/hill.cpp -o $(BD)hill.o
	
hillatt.o: bd $(SD)attacks/hillatt.cpp $(ID)attacks/hillatt.h
	$(CC) $(CFLAGS) -c $(SD)attacks/hillatt.cpp -o $(BD)hillatt.o

$(TARG): main.o module.o $(ATTACKS) $(CIPHERS) $(TOOLS)
	$(CC) $(CFLAGS) $(BD)main.o $(BD)module.o $(addprefix $(BD), $(ATTACKS)) $(addprefix $(BD), $(CIPHERS)) -o $(TARG)

bd:
	@if [ ! -d $(BD) ]; then mkdir $(BD); fi

clean:
	if [ -x $(TARG) ]; then rm $(TARG); fi
	rm -rf $(BD)*.o
