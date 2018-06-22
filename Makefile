#Saad Rasheed

#Compiler
CC = gcc

#Compiler Flags
CFLAGS = -Wall -g -I.

#Linker Flags
LDFLAGS = -ldl

#The build target executable
TARGET = Example

#Object files to build:
OBJ = ExampleCode.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

clean:
	rm ExampleCode.o Example
