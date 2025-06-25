# Configuración del compilador
CC = gcc
JAVAC = javac

# Banderas de compilación
CFLAGS = -pthread 

# Nombres de los ejecutables
TARGETS = MergeSortTask.class QuickSortTask.class sudoku

.PHONY: all clean

all: $(TARGETS)

# Reglas para Java
MergeSortTask.class: MergeSortTask.java
	$(JAVAC) MergeSortTask.java

QuickSortTask.class: QuickSortTask.java
	$(JAVAC) QuickSortTask.java

# Regla para C
sudoku: sudoku.c
	$(CC) $(CFLAGS) -o sudoku sudoku.c

# Reglas para ejecutar los programas
run-mergesort: MergeSortTask.class
	java MergeSortTask $(ARGS)

run-quicksort: QuickSortTask.class
	java QuickSortTask $(ARGS)

run-sudoku: sudoku
	./sudoku $(ARGS)

# Limpieza
clean:
	rm -f $(TARGETS) *.class
