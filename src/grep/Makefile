TARGET = s21_grep
CC = gcc
FLAGS = -Werror -Wall -Wextra -std=c11 -ggdb

all: $(TARGET)

clean :
	rm -rf $(TARGET) *.o

$(TARGET).o: $(TARGET).c
	$(CC) $(FLAGS) -c $(TARGET).c -o $(TARGET).o

$(TARGET): $(TARGET).o
	$(CC) $(FLAGS) *.o -o $(TARGET)

rebuild: clean all
