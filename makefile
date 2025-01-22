# Nom de l'exécutable
TARGET = main

# Fichiers source

SRCS = src/main.c src/board.c src/game.c src/Piece.c

# Options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Règle principale pour compiler le programme
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(TARGET)

# Exécution du programme
run: $(TARGET)
	./$(TARGET)

# Éviter les conflits avec des fichiers du même nom
.PHONY: all clean run
