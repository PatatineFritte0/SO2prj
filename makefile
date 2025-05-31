# Nome dell'eseguibile
TARGET = main

# Compilatore
CC = gcc

# Flag di compilazione
CFLAGS = -Wall -Wextra -Iheader

# File sorgenti
SRCS = $(wildcard source/*.c) main.c

# Oggetti compilati
OBJS = $(SRCS:.c=.o)

# Regola principale
all: $(TARGET)

# Come costruire l'eseguibile
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ -lm

# Regola per pulire i file oggetto e l'eseguibile
clean:
	rm -f $(OBJS) $(TARGET)

# Per evitare che make cerchi un file chiamato 'clean'
.PHONY: all clean
