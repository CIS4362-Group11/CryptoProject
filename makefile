CC=g++
CFLAGS=-Wall -g -Iinclude/
SD=src/
ID=include/
BD=build/
ATTACKS=test.o
TOOLS=
TARG=cryptoproject

.PHONY: all clean bd

all: $(TARG)

test.o: bd $(SD)test.cpp $(ID)test.h
	$(CC) $(CFLAGS) -c $(SD)test.cpp -o $(BD)test.o

$(TARG): $(ATTACKS) $(TOOLS)
	$(CC) $(CFLAGS) $(addprefix $(BD), $(ATTACKS)) $(addprefix $(BD), $(TOOLS)) -o $(TARG)

bd:
	@if [ ! -d $(BD) ]; then mkdir $(BD); fi

clean:
	if [ -x $(TARG) ]; then rm $(TARG); fi
	rm -rf $(BD)*.o
