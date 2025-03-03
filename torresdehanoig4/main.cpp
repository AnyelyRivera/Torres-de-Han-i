#include <iostream>
#include <vector>      // para vectores y arreglos dinámicos
#include <cstddef>     // Para usar size_t
#include "lib/raylib.h" // Para usar la librería Raylib (gráficos)

using namespace std;

// Constantes del juego
const int ANCHO_PANTALLA = 800; // Ancho de la ventana del juego
const int ALTO_PANTALLA = 600;  // Alto de la ventana del juego
const int MAX_DISCOS = 5;       // Número máximo de discos
const int ANCHO_TORRE = 10;     // Ancho de las torres
const int ALTO_TORRE = 300;     // Alto de las torres
const int ANCHO_BASE = 300;     // Ancho de la base
const int ALTO_BASE = 20;      // Alto de la base
const int ALTO_DISCO = 30;      // Alto de los discos
const Color COLORES[] = {RED, ORANGE, YELLOW, GREEN, BLUE}; // Colores para los discos

// Estado del juego
vector<vector<int>> torres(3); // Vector de vectores que representa las 3 torres
int discoSeleccionado = -1;    // Índice del disco seleccionado (-1 si ninguno)
int torreSeleccionada = -1;    // Índice de la torre seleccionada (-1 si ninguna)
int movimientos = 0;          // Contador de movimientos realizados
bool juegoTerminado = false;   // Indica si el juego ha terminado

// Resuelve Torres de Hanoi recursivamente
// 'discos' es el número de discos a mover
// 'origen', 'destino' y 'auxiliar' son los índices de las torres
void resolverHanoi(int discos, int origen, int destino, int auxiliar) {
    if (discos == 1) { // Caso base: si solo hay un disco, moverlo directamente
        torres[destino].push_back(torres[origen].back()); // Mueve el disco de origen a destino
        torres[origen].pop_back();                       // Remueve el disco de origen
        movimientos++;                                   // Incrementa el contador de movimientos
        return;
    }
    // Mueve n-1 discos de origen a auxiliar, usando destino como auxiliar
    resolverHanoi(discos - 1, origen, auxiliar, destino);
    // Mueve el disco restante de origen a destino
    resolverHanoi(1, origen, destino, auxiliar);
    // Mueve los n-1 discos de auxiliar a destino, usando origen como auxiliar
    resolverHanoi(discos - 1, auxiliar, destino, origen);
}

// Dibuja elementos gráficos en la pantalla
void dibujarTorres() {
    // Base
    DrawRectangle(ANCHO_PANTALLA / 2 - ANCHO_BASE / 2, ALTO_PANTALLA - ALTO_BASE, ANCHO_BASE, ALTO_BASE, BROWN);

    // Torres
    for (int i = 0; i < 3; i++) {
        int x = ANCHO_PANTALLA / 4 * (i + 1);
        DrawRectangle(x - ANCHO_TORRE / 2, ALTO_PANTALLA - ALTO_TORRE - ALTO_BASE, ANCHO_TORRE, ALTO_TORRE, BROWN);
    }

    // Discos
    for (int i = 0; i < 3; i++) {
        for (size_t j = 0; j < torres[i].size(); j++) {
            int tamañoDisco = torres[i][j];
            int x = ANCHO_PANTALLA / 4 * (i + 1) - tamañoDisco * 10;
            int y = ALTO_PANTALLA - ALTO_BASE - (j + 1) * ALTO_DISCO;
            DrawRectangle(x, y, tamañoDisco * 20, ALTO_DISCO, COLORES[tamañoDisco - 1]);
        }
    }
}

// Verifica si el jugador ha ganado el juego
bool verificarVictoria() {
    return torres[2].size() == MAX_DISCOS; // Victoria si todos los discos están en la tercera torre
}

// Reinicia el juego a su estado inicial
void reiniciarJuego() {
    torres = vector<vector<int>>(3); // Reinicializa las torres
    for (int i = MAX_DISCOS; i > 0; i--) {
        torres[0].push_back(i); // Coloca todos los discos en la primera torre
    }
    movimientos = 0;         // Reinicia el contador de movimientos
    juegoTerminado = false;  // Reinicia el estado del juego
}

int main() {
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "Torres de Hanoi"); // Inicializa la ventana
    SetTargetFPS(60); // Establece los FPS deseados

    reiniciarJuego(); // Inicializa el juego

    // Bucle principal del juego
    while (!WindowShouldClose()) { // Mientras la ventana no se cierre
        if (!juegoTerminado) { // Si el juego no ha terminado
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { // Si se hace clic con el botón izquierdo
                Vector2 posicionRaton = GetMousePosition(); // Obtiene la posición del ratón
                for (int i = 0; i < 3; i++) { // Itera sobre las 3 torres
                    int x = ANCHO_PANTALLA / 4 * (i + 1); // Calcula la posición x de la torre
                    if (posicionRaton.x > x - 50 && posicionRaton.x < x + 50) { // Si el clic está dentro de la torre
                        if (discoSeleccionado == -1 && !torres[i].empty()) { // Si no hay disco seleccionado y la torre tiene discos
                            discoSeleccionado = torres[i].back(); // Selecciona el disco superior
                            torreSeleccionada = i;                 // Guarda el índice de la torre seleccionada
                        } else if (discoSeleccionado != -1) { // Si hay un disco seleccionado
                            if (torres[i].empty() || torres[i].back() > discoSeleccionado) { // Si la torre está vacía o el disco seleccionado es más pequeño
                                torres[i].push_back(discoSeleccionado); // Mueve el disco a la torre
                                torres[torreSeleccionada].pop_back();   // Remueve el disco de la torre anterior
                                discoSeleccionado = -1;              // Deselecciona el disco
                                torreSeleccionada = -1;              // Deselecciona la torre
                                movimientos++;                       // Incrementa el contador de movimientos
                            }
                        }
                    }
                }
            }

            if (verificarVictoria()) juegoTerminado = true; // Verifica si el juego ha terminado
        } else { // Si el juego ha terminado
            DrawText("Grupo 4", ANCHO_PANTALLA / 2 - 100, ALTO_PANTALLA / 2 + 50, 30, BLACK);
            DrawText("¡Victoria!", ANCHO_PANTALLA / 2 - 100, ALTO_PANTALLA / 2 - 50, 40, GREEN);
            DrawText("¿Continuar jugando?", ANCHO_PANTALLA / 2 - 150, ALTO_PANTALLA / 2, 30, BLACK);
            DrawText("1: Sí, 2: Salir", ANCHO_PANTALLA / 2 - 100, ALTO_PANTALLA / 2 + 50, 30, BLACK);

            if (IsKeyPressed(KEY_ONE)) reiniciarJuego(); // Reinicia el juego si se presiona '1'
            else if (IsKeyPressed(KEY_TWO)) break;        // Sale del juego si se presiona '2'
        }

        BeginDrawing();
        ClearBackground(RAYWHITE); // Limpia la pantalla
        dibujarTorres(); // Dibuja las torres y los discos
        DrawText(TextFormat("Movimientos: %d", movimientos), 10, 10, 20, BLACK); // Muestra el contador de movimientos
        EndDrawing();
    }

    CloseWindow(); // Cierra la ventana
    return 0;      // Indica que el programa terminó correctamente
}
