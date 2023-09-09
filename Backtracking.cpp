/*Primer trabajo Backtracking */
/*Desarrollado por Javier Alejandro Agusto Roco*/
/*Asignatura:Analisis de algoritmos */
/*Profesor:Julio Andres Fuentealba Vivallo*/
/*Universidad Tecnologia Metropolitana */
/*Segundo Semestre del 2023*/

//Librerias a utilizar.

#include <iostream> // Para usar cout y cin que son entradas y salidas de datos.
#include <vector>   // Para usar vectores.
#include <climits>  // Para usar INT_MAX 
#include <ctime>    // Para usar time(nullptr) con el fin de inicializar la semilla del generador de números aleatorios.
#include <cstdlib>  // Para usar srand() y rand() con el fin de generar números aleatorios..
#include <chrono>   // Para medir el tiempo
#include <locale.h>   // Incluir la biblioteca 'locale' para mostrar acentos

using namespace std;
//Inicializar variables globales para que sean accesibles desde cualquier función del programa.

int N = 0; // Tamaño de la matriz cuadrada inicializado a 0
int** matriz = nullptr; // Puntero a la matriz para lograr que sea dinámica y empezar con un puntero nulo.
vector<vector<bool>> marked; // Para marcar las casillas del camino mínimo.
int foundPathsCount = 0;// Para almacenar el número de caminos encontrados .
int minCost = INT_MAX; // Para almacenar el costo mínimo encontrado.
vector<pair<int, int>> minPath; // Para almacenar el camino mínimo.
int operationCount = 0; // Contador de operaciones.
int minRandomValue = 100; // Valor mínimo para números aleatorios por defecto.
int maxRandomValue = 999; // Valor máximo para números aleatorios por defecto.


//Funciones para realizar las operaciones necesarias para el programa.
// Comprobar si la casilla es válida con el fin de evitar desbordamientos.
bool isValid(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N);
}

// Crear y rellenar aleatoriamente la matriz con números entre minRandomValue y maxRandomValue.
void createAndFillRandomMatrix(int newSize) {
    // Liberar la matriz anterior si existe
    if (matriz != nullptr) {
        for (int i = 0; i < N; i++) {
            delete[] matriz[i];
        }
        delete[] matriz;
    }
    // Establecer el nuevo tamaño de la matriz.
    N = newSize;
    // Crear una nueva matriz y rellenarla aleatoriamente con números entre minRandomValue y maxRandomValue
    matriz = new int*[N];
    srand(time(nullptr)); // Inicializar la semilla del generador de números aleatorios
    for (int i = 0; i < N; i++) {
        matriz[i] = new int[N];
        for (int j = 0; j < N; j++) {
            matriz[i][j] = rand() % (maxRandomValue - minRandomValue + 1) + minRandomValue;
        }
    }
    cout << "\nNueva matriz de tamaño " << N << "x" << N << " creada y rellenada aleatoriamente." << endl;
}


// Encontrar el camino mínimo con poda 
void findMinCost(int x, int y, int cost, vector<pair<int, int>>& path) {
    operationCount++; // Incrementar el contador de operaciones
    //cout << "Explorando (" << x << ", " << y << "), Costo: " << cost << endl; // Depuración
    if (x == N - 1 && y == N - 1) {
        if (cost < minCost) {
            minCost = cost;
            minPath = path;
        }
        foundPathsCount++; // Incrementar el contador de caminos encontrados
        return;
    }
    // Movimientos posibles: abajo y derecha como poda para evitar caminos repetidos.
    int dx[] = {1, 0};
    int dy[] = {0, 1};
    // Recorrer los movimientos posibles y llamar recursivamente a la función
    for (int i = 0; i < 2; i++) {
        int newX = x + dx[i];
        int newY = y + dy[i];
        // Comprobar si la casilla es válida y si el costo actual es menor que el costo mínimo encontrado hasta ahora.
        if (isValid(newX, newY)) {
            int newCost = cost + abs(matriz[newX][newY] - matriz[x][y]);

            // Poda: Si el costo actual es mayor o igual al costo mínimo encontrado hasta ahora, no seguimos explorando
            if (newCost >= minCost) {
                continue;
            }
            // Si el costo actual es menor que el costo mínimo encontrado hasta ahora, seguimos explorando.
            path.push_back({newX, newY});
            findMinCost(newX, newY, newCost, path);
            path.pop_back();
        }
    }
}

// Marcar el camino mínimo en la matriz
void markPathOnMatrix(const vector<pair<int, int>>& path) {
    marked.assign(N, vector<bool>(N, false));
    for (const auto& p : path) {
        marked[p.first][p.second] = true;
    }
}

// Imprimir la matriz con el camino mínimo marcado
void printMatrixWithMarkedPath() {
    cout << "\nMatriz con camino mínimo marcado:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (marked[i][j]) {
                cout << "[" << matriz[i][j] << "]\t";
            } else {
                cout << matriz[i][j] << "\t";
            }
        }
        cout << endl;
    }
}

// Realizar el recorrido del camino mínimo con poda
void calculateMinCostWithPruning() {
    if (N == 0) {
        cout << "La matriz no está definida. Cree una matriz aleatoriamente antes de calcular el camino mínimo." << endl;
        return;
    }

    minCost = INT_MAX;
    vector<pair<int, int>> path = {{0, 0}};

    auto start_time = chrono::high_resolution_clock::now(); // Registrar el tiempo de inicio

    findMinCost(0, 0, 0, path);
    markPathOnMatrix(minPath);

    auto end_time = chrono::high_resolution_clock::now(); // Registrar el tiempo de finalización
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

    cout << "\nEl camino de menor costo con poda es: " << minCost << endl;
    cout << "\nOperaciones realizadas por el algoritmo: " << operationCount << endl;
    cout << "\nTiempo de ejecución: " << fixed << duration.count() / 1e6 << " segundos" << endl;
    printMatrixWithMarkedPath();
}

// Realizar el recorrido del camino mínimo sin poda
void calculateMinCostWithoutPruning() {
    if (N == 0) {
        cout << "La matriz no está definida. Cree una matriz aleatoriamente antes de calcular el camino mínimo." << endl;
        return;
    }

    minCost = INT_MAX;
    vector<pair<int, int>> path = {{0, 0}};

    auto start_time = chrono::high_resolution_clock::now(); // Registrar el tiempo de inicio

    findMinCost(0, 0, 0, path);
    markPathOnMatrix(minPath);

    auto end_time = chrono::high_resolution_clock::now(); // Registrar el tiempo de finalización
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

    cout << "\nEl camino de menor costo sin poda es: " << minCost << endl;
    cout << "\nOperaciones realizadas por el algoritmo: " << operationCount << endl;
    cout << "\nTiempo de ejecución: " << fixed << duration.count() / 1e6 << " segundos" << endl;
    printMatrixWithMarkedPath();
}

// Calcular y mostrar el camino mínimo
void calculateAndPrintPath() {
    if (N == 0) {
        cout << "La matriz no está definida. Cree una matriz aleatoriamente antes de calcular el camino mínimo." << endl;
        return;
    }

    minCost = INT_MAX;
    vector<pair<int, int>> path = {{0, 0}};
    findMinCost(0, 0, 0, path);
    markPathOnMatrix(minPath);
    cout << "\nEl camino de menor costo con avance de casillas y coste entre cada una es :" << endl;
    for (int i = 0; i < minPath.size(); i++) {
        cout << "(" << minPath[i].first << "," << minPath[i].second << ")";
        if (i < minPath.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
    cout << "\nCosto total : " << minCost << endl;
    cout << "\nOperaciones realizadas por el algoritmo : " << operationCount << endl;
}

// Imprimir la matriz
void printMatrix() {
    cout << "Matriz:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << matriz[i][j] << "\t";
        }
        cout << endl;
    }
}

int main() {
    // Configurar la localización para mostrar caracteres acentuados
    setlocale(LC_CTYPE,"Spanish");

    int choice;
    bool exitMenu = false;

    while (!exitMenu) {
        cout << "-------------------\n" << endl;
        cout << "Menú interactivo:\n" << endl;
        cout << "-------------------\n" << endl;
        cout << "1. Mostrar matriz\n" << endl;
        cout << "2. Calcular el camino mínimo con poda\n" << endl;
        cout << "3. Calcular el camino mínimo sin poda\n" << endl;
        cout << "4. Mostrar avance de casillas \n" << endl;
        cout << "5. Cambiar el tamaño de la matriz y rellenar aleatoriamente\n" << endl;
        cout << "6. Cambiar el rango de números aleatorios (Por defecto = [100-999]) \n" << endl;
        cout << "7. Mostrar número de caminos encontrados\n" << endl; 
        cout << "8. Salir\n" << endl;
        cout << "Seleccione una opción :";
        cin >> choice;

        if (choice == 1) {
            if (N == 0) {
                // Si la matriz no está definida, la creamos aleatoriamente
                int newSize;
                cout << "\nLa matriz no está definida. Ingrese el tamaño de la matriz :";
                cin >> newSize;
                createAndFillRandomMatrix(newSize);
            }
            printMatrix();
        } else if (choice == 2) {
            operationCount = 0; // Restablecer el contador de operaciones
            
            calculateMinCostWithPruning();
            cout << "\nNúmero de otros caminos encontrados : " << foundPathsCount << endl; // Mostrar el número de caminos encontrados
        } else if (choice == 3) {
            operationCount = 0; // Restablecer el contador de operaciones
            
            calculateMinCostWithoutPruning();
            cout << "\nNúmero de otros caminos encontrados : " << foundPathsCount << endl; // Mostrar el número de caminos encontrados
        } else if (choice == 4) {
            calculateAndPrintPath();
        } else if (choice == 5) {
            int newSize;
            cout << "\nIngrese el nuevo tamaño de la matriz : ";
            cin >> newSize;
            createAndFillRandomMatrix(newSize);
        } else if (choice == 6) {
            cout << "\nIngrese el valor mínimo para números aleatorios : ";
            cin >> minRandomValue;
            cout << "\nIngrese el valor máximo para números aleatorios : ";
            cin >> maxRandomValue;
        } else if (choice == 7) {
            cout << "\nNúmero de otros caminos encontrados : " << foundPathsCount << endl;
        } else if (choice == 8) {
            exitMenu = true;
        } else {
            cout << "\nOpción no válida. Por favor, seleccione una opción válida." << endl;
        }
    }
    // Liberar memoria al finalizar el programa 
    if (matriz != nullptr) {
        for (int i = 0; i < N; i++) {
            delete[] matriz[i];
        }
        delete[] matriz;
    }

    return 0;
}
