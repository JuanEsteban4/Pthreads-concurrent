
#  Concurrency Projects – Sudoku Validator & Fork-Join Sorting

Este repositorio contiene dos proyectos multihilo desarrollados en el contexto de concurrencia y paralelismo, inspirados en *Operating System Concepts*:

1. **Validador multihilo de Sudoku** (C, con 27 hilos (caso base) usando pthreads).
2. **Ordenamiento paralelo en Java** (Fork/Join con QuickSort y MergeSort híbrido con InsertionSort).

---

##  Proyecto 1: Validador Multihilo de Sudoku

###  Descripción
Valida si una solución de Sudoku n×n es correcta. Se utilizan 3n hilos (uno por cada fila, columna y subcuadro). Cada hilo informa si su sección es válida y el hilo principal agrega los resultados.


##  Proyecto 3: Ordenamiento Fork-Join en Java

###  Descripción
Se implementan dos algoritmos de ordenamiento en paralelo usando `ForkJoinPool`:

- `QuickSortTask`
- `MergeSortTask`

Ambos aplican InsertionSort cuando el subarreglo tiene ≤100 elementos.

###  Genéricos y Comparables
Los algoritmos aceptan cualquier tipo `T` que implemente `Comparable<? super T>`, lo cual permite ordenar clases como `Integer`, `String` o `Persona`.


##  Compilación y ejecución con Makefile

Este repositorio incluye un `Makefile` que automatiza la compilación y ejecución de los programas en C y Java.

###  Requisitos

- `gcc` con soporte para `pthread`
- `javac` y `java` (Java 8+)
- GNU `make`

---

###  Comandos principales

####  Compilar todo

```bash
make
```

Compila:

- `sudoku.c` → `sudoku` (C con pthreads)
- `MergeSortTask.java`
- `QuickSortTask.java`
- `BenchmarkSorting.java`

---

####  Ejecutar cada aplicación

```bash
make run-sudoku           # Ejecuta el validador de Sudoku
make run-quicksort ARGS=10000      # Ejecuta QuickSort con 10000 elementos
make run-mergesort ARGS=10000      # Ejecuta MergeSort con 10000 elementos
make run-benchmarksort    # Ejecuta los benchmarks de ordenamiento
```

Es Posible cambiar el número de elementos ajustando el valor de `ARGS`.

---

####  Limpiar archivos generados

```bash
make clean
```

Elimina archivos `.class` y el ejecutable `sudoku`.

---

###  Archivos gestionados por el Makefile

| Archivo fuente           | Salida generada              | Descripción                              |
|--------------------------|------------------------------|------------------------------------------|
| `sudoku.c`               | `sudoku`                     | Validador multihilo de Sudoku (C)        |
| `QuickSortTask.java`     | `QuickSortTask.class`        | Ordenamiento paralelo con Fork/Join      |
| `MergeSortTask.java`     | `MergeSortTask.class`        | Ordenamiento con mezcla (Fork/Join)      |
| `BenchmarkSorting.java`  | `BenchmarkSorting.class`     | Benchmark de rendimiento                 |



##  Referencias

- **Operating System Concepts** – Silberschatz, Galvin, Gagne  
  Capítulo: *Threads & Concurrency*  
  Páginas: 262–265  
  Proyectos:
  - Sudoku Solution Validator
  - Fork-Join Sorting Application

---
