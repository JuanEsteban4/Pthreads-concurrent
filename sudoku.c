#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estructura para pasar parámetros
typedef struct {
    int row;
    int col;
} parameters;

// Variables globales para configuración
int SUBGRID_SIZE = 3;
int SIZE;
int **sudoku;
char *input_filename = NULL;

// Variables para almacenar resultados
bool *valid_rows;
bool *valid_col;
bool *valid_subgrids;

bool load_sudoku_from_file();
bool check_set(int *set);
void *check_row(void *arg);
void *check_col(void *arg);
void *check_subgrid(void *arg);
void print_results();
void initialize_validation_arrays();
void cleanup();

bool load_sudoku_from_file() {
    FILE *file = fopen(input_filename, "r");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", input_filename);
        return false;
    }

    // Primera pasada: determinar el tamaño
    int lines = 0;
    int max_numbers = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        // Saltar líneas vacías
        if (line[0] == '\n') continue;
        
        int count = 0;
        char *token = strtok(line, " \t\n");
        while (token != NULL) {
            if (isdigit(token[0])) count++;
            token = strtok(NULL, " \t\n");
        }
        
        if (count > 0) {
            lines++;
            if (count > max_numbers) max_numbers = count;
        }
    }

    // Verificar si es un cuadrado perfecto
    if (lines != max_numbers) {
        printf("Error: El sudoku debe ser cuadrado (mismo número de filas y columnas)\n");
        fclose(file);
        return false;
    }

    // Calcular SUBGRID_SIZE (asumimos que es un sudoku estándar)
    for (SUBGRID_SIZE = 2; SUBGRID_SIZE * SUBGRID_SIZE < max_numbers; SUBGRID_SIZE++);
    if (SUBGRID_SIZE * SUBGRID_SIZE != max_numbers) {
        printf("Error: Tamaño de sudoku no estándar (%dx%d)\n", max_numbers, max_numbers);
        fclose(file);
        return false;
    }

    SIZE = max_numbers;
    printf("Cargando sudoku %dx%d (subgrid %dx%d) desde %s\n", 
           SIZE, SIZE, SUBGRID_SIZE, SUBGRID_SIZE, input_filename);

    // Asignar memoria para el sudoku
    sudoku = (int **)malloc(SIZE * sizeof(int *));
    for (int i = 0; i < SIZE; i++) {
        sudoku[i] = (int *)malloc(SIZE * sizeof(int));
    }

    // Segunda pasada: leer los valores
    rewind(file);
    int row = 0;
    while (row < SIZE && fgets(line, sizeof(line), file)) {
        // Saltar líneas vacías o de comentarios
        if (line[0] == '#' || line[0] == '\n') continue;
        
        int col = 0;
        char *token = strtok(line, " \t\n");
        while (token != NULL && col < SIZE) {
            if (isdigit(token[0])) {
                sudoku[row][col] = atoi(token);
                col++;
            }
            token = strtok(NULL, " \t\n");
        }
        row++;
    }

    fclose(file);
    return true;
}

bool check_set(int *set) {
    bool *found = calloc(SIZE + 1, sizeof(bool));
    for (int i = 0; i < SIZE; i++) {
        int num = set[i];
        if (num < 1 || num > SIZE || found[num]) {
            free(found);
            return false;
        }
        found[num] = true;
    }
    free(found);
    return true;
}

void *check_row(void *arg) {
    int i = *((int *)arg);
    free(arg);
    int *row = malloc(SIZE * sizeof(int));
    
    for (int j = 0; j < SIZE; j++) {
        row[j] = sudoku[i][j];
    }
    
    if (!check_set(row)) {
        valid_rows[i] = false;
    }
    
    free(row);
    pthread_exit(NULL);
}

void *check_col(void *arg) {
    int j = *((int *)arg);
    free(arg);
    int *column = malloc(SIZE * sizeof(int));
    
    for (int i = 0; i < SIZE; i++) {
        column[i] = sudoku[i][j];
    }
    
    if (!check_set(column)) {
        valid_col[j] = false;
    }
    
    free(column);
    pthread_exit(NULL);
}

void *check_subgrid(void *arg) {
    parameters *params = (parameters *)arg;
    int row_start = params->row;
    int col_start = params->col;
    int subgrid_id = (row_start / SUBGRID_SIZE) * SUBGRID_SIZE + (col_start / SUBGRID_SIZE);

    int *subgrid = malloc(SIZE * sizeof(int));
    int index = 0;

    for (int i = row_start; i < row_start + SUBGRID_SIZE; i++) {
        for (int j = col_start; j < col_start + SUBGRID_SIZE; j++) {
            subgrid[index++] = sudoku[i][j];
        }
    }

    if (!check_set(subgrid)) {
        valid_subgrids[subgrid_id] = false;
    }

    free(subgrid);
    free(params);
    pthread_exit(NULL);
}

void print_results() {
    bool all_valid = true;

    printf("\nResultados de validación:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!valid_rows[i]) {
            printf("Fila %d es inválida\n", i + 1);
            all_valid = false;
        }
    }

    for (int j = 0; j < SIZE; j++) {
        if (!valid_col[j]) {
            printf("Columna %d es inválida\n", j + 1);
            all_valid = false;
        }
    }

    for (int k = 0; k < SIZE; k++) {
        if (!valid_subgrids[k]) {
            printf("Subcuadro %d es inválido\n", k + 1);
            all_valid = false;
        }
    }

    if (all_valid) {
        printf("\n✔ El Sudoku %dx%d es completamente válido!\n", SIZE, SIZE);
    } else {
        printf("\n✖ El Sudoku %dx%d contiene errores.\n", SIZE, SIZE);
    }
}

void initialize_validation_arrays() {
    valid_rows = malloc(SIZE * sizeof(bool));
    valid_col = malloc(SIZE * sizeof(bool));
    valid_subgrids = malloc(SIZE * sizeof(bool));
    
    for (int i = 0; i < SIZE; i++) {
        valid_rows[i] = true;
        valid_col[i] = true;
        valid_subgrids[i] = true;
    }
}

void cleanup() {
    // Liberar memoria del sudoku
    for (int i = 0; i < SIZE; i++) {
        free(sudoku[i]);
    }
    free(sudoku);
    
    // Liberar arrays de validación
    free(valid_rows);
    free(valid_col);
    free(valid_subgrids);
}

int main(int argc, char *argv[]) {
    // Procesar argumentos de línea de comandos
    if (argc < 2) {
        printf("Uso: %s <archivo_sudoku>\n", argv[0]);
        printf("Ejemplo: %s sudoku.txt\n", argv[0]);
        return 1;
    }

    input_filename = argv[1];
   
    // Cargar sudoku desde archivo
    if (!load_sudoku_from_file()) {
        return 1;
    }

    initialize_validation_arrays();

    pthread_t *row_thread = malloc(SIZE * sizeof(pthread_t));
    pthread_t *col_thread = malloc(SIZE * sizeof(pthread_t));
    pthread_t *subgrid_threads = malloc(SIZE * sizeof(pthread_t));

    // Crear hilos para columnas
    for (int j = 0; j < SIZE; j++) {
        int *col_num = malloc(sizeof(int));
        *col_num = j;
        pthread_create(&col_thread[j], NULL, check_col, col_num);
    }

    // Crear hilos para filas
    for (int i = 0; i < SIZE; i++) {
        int *row_num = malloc(sizeof(int));
        *row_num = i;
        pthread_create(&row_thread[i], NULL, check_row, row_num);
    }

    // Crear hilos para subcuadrados
    for (int i = 0; i < SIZE; i += SUBGRID_SIZE) {
        for (int j = 0; j < SIZE; j += SUBGRID_SIZE) {
            parameters *data = malloc(sizeof(parameters));
            data->row = i;
            data->col = j;
            int subgrid_id = (i / SUBGRID_SIZE) * SUBGRID_SIZE + (j / SUBGRID_SIZE);
            pthread_create(&subgrid_threads[subgrid_id], NULL, check_subgrid, data);
        }
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < SIZE; i++) {
        pthread_join(row_thread[i], NULL);
        pthread_join(col_thread[i], NULL);
        pthread_join(subgrid_threads[i], NULL);
    }

    print_results();

    // Liberar memoria
    free(row_thread);
    free(col_thread);
    free(subgrid_threads);
    cleanup();

    return 0;
}
