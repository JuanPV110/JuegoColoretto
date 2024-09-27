//archivo para la implementacion del juego
#include <iostream>
#include "coloretto.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(0))); // esto es la inicializacion para el "random"
    int numJugadores;
    std::cout << "¿Cuántos jugadores hay? (2-4): ";
    std::cin >> numJugadores;

    // este  condicional es para verificar el numero de jugadores que es entre 2-4
    if (numJugadores < 2 || numJugadores > 4) {
        std::cout << "Número de jugadores no válido." << std::endl;
        return 1;
    }

    Juego juego(numJugadores);
    juego.jugar();
    return 0;
}

