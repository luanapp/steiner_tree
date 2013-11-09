
TARGET=stein
SRC=types.c file_reader.c main.c
OBJ=$(SRC:.c=.o)
CFLAGS=-g -O2 -Wall -pedantic -pthread
stein_module=$(if $(findstring $(1), types.c), -DSTEIN_MODULE,)

all:  $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(call stein_module, $(notdir $<)) -c $< -o $@

clean:
	$(RM) *.o *.E *~ $(TARGET)
