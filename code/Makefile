
TARGET=stein
SRC=types.c file_reader.c mst.c population.c main.c
OBJ=$(SRC:.c=.o)
CFLAGS=-O3 -Wall -pedantic -pthread
PRINT_LEVEL=0
stein_module=$(if $(findstring $(1), types.c), -DSTEIN_MODULE,)

.PHONY: debug clean

all:  $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(call stein_module, $(notdir $<)) -DPRINT_LEVEL=$(PRINT_LEVEL) -c $< -o $@

clean:
	$(RM) *.o *.E *~ $(TARGET)

# The debug target is built without optimization and
# with the gcc debug flag -g.
# Also, the print level is the debug level (print all).
debug: CFLAGS:=-g $(subst -O3,-O0,$(CFLAGS))
debug: PRINT_LEVEL=3
debug: $(TARGET)
