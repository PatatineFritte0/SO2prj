# Compilatore da usare
CC = gcc -std=c90

# Flag per la compilazione:
# -Iheader: indica dove trovare i file .h
# -Wall: attiva tutti gli avvisi (warning)
CFLAGS = -Iheader -Wall

# Prende tutti i file .c nella cartella source/
SRC = $(wildcard source/*.c)

# Converte i .c in .o (file oggetto)
OBJ = $(SRC:.c=.o)

# Nome dell'eseguibile finale
TARGET = main_exec

# Obiettivo principale: compila, esegue e pulisce
all: $(TARGET) run clean

# Compila l'eseguibile finale
# Usa main.c e tutti i file oggetto generati da source/*.c
$(TARGET): main.c $(OBJ)
	@$(CC) $(CFLAGS) -o $(TARGET) main.c $(OBJ) -lm

# Regola generica per compilare ogni .c in .o
# $< (file sorgente (.c))
# $@ (file oggetto (.o))
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# Esegue il programma appena compilato
run:
	@echo "Esecuzione del programma:"
	@./$(TARGET)

# Rimuove tutti i file oggetto e l'eseguibile
clean:
	@rm -f $(OBJ) $(TARGET)
