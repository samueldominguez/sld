CC=gcc
CFLAGS=-c -g
LDFLAGS=-g
SOURCES=sld.c label.c output.c sof.c reloc.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sld

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@$(CC) -o $@ $(OBJECTS) $(LDFLAGS)
	@echo "LINK	$@"

.c.o:
	@$(CC) $(CFLAGS) $< -o $@
	@echo "CC	$<"

clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
