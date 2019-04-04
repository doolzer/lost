CC = gcc
CFLAGS = -lncurses

ODIR = ./

BINDIR = ./

_ALLOBJ = lost.o
ALLOBJ = $(patsubst %,$(ODIR)/%,$(_ALLOBJ))

_EXECUTABLES = lost
EXECUTABLES = $(patsubst %,$(BINDIR)/%,$(_EXECUTABLES))

all: $(EXECUTABLES)

$(BINDIR)/lost: $(ALLOBJ)
	$(CC) -o $@ $^ $(CFLAGS)


$(ODIR)/lost.o: lost.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o
