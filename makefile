CC=g++
CFLAGS=-Wall -g -Iinclude/
SD=src/
ID=include/
BD=build/
ATTACKS=example.o
TARG=cryptoproject

.PHONY: all clean bd

all: $(TARG)

main.o: bd $(SD)main.cpp $(ID)main.h
	$(CC) $(CFLAGS) -c $(SD)main.cpp -o $(BD)main.o

example.o: bd $(SD)attacks/example.cpp $(ID)attacks/example.h
	$(CC) $(CFLAGS) -c $(SD)attacks/example.cpp -o $(BD)example.o

$(TARG): main.o $(ATTACKS) $(TOOLS)
	$(CC) $(CFLAGS) $(BD)main.o $(addprefix $(BD), $(ATTACKS)) $(addprefix $(BD), $(TOOLS)) -o $(TARG)

bd:
	@if [ ! -d $(BD) ]; then mkdir $(BD); fi

clean:
	if [ -x $(TARG) ]; then rm $(TARG); fi
	rm -rf $(BD)*.o
