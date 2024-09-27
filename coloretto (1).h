// archivo para la definicion del juego
#ifndef COLORETTO_H //comprobacion de que el archivo este definido 
#define COLORETTO_H //definde el archivo de cabecera 

#include <iostream> // libreria para utilizar el (cout y cin)
#include <vector> // para utilizar vectores
#include <string> // esta para utilizar las cadenas de texto
#include <algorithm> // esta sirve para manipular datos (busqueda y la ordenacion de un vector)
#include <cstdlib> // libreria para generar algo aleatorio 
#include <ctime> // esta es para que lo que se genera aleatorio sean diferentes en cada ejecucion 

using namespace std;

//con esto se gestino si la carta es un color, comodin o una carta bonus 
class Carta {
public:
    std::string color;
    bool esComodin;
    bool esBonus;

    Carta(const std::string& c, bool comodin = false, bool bonus = false) 
        : color(c), esComodin(comodin), esBonus(bonus) {
    }
};

class Jugador {
private:
    std::string nombre;
    std::vector<Carta> cartasRobadas;
    int conteoColores[6] = {0}; // Conteo de cada color de carta

public:
    Jugador(const std::string& n) : nombre(n) {}

    //aca es para acomular las carta y ayuda para el conteo del puntaje del final 
    void agregarCarta(const Carta& carta) {
        cartasRobadas.push_back(carta);
        if (carta.color == "rojo") conteoColores[0]++;
        else if (carta.color == "verde") conteoColores[1]++;
        else if (carta.color == "azul") conteoColores[2]++;
        else if (carta.color == "amarillo") conteoColores[3]++;
        else if (carta.color == "naranja") conteoColores[4]++;
        else if (carta.color == "negro") conteoColores[5]++;
    }

    // esto es para ver las cartas acomuladas de cada jugador 
    void mostrarCartas() const {
        std::cout << nombre << " tiene las siguientes cartas: ";
        for (const auto& carta : cartasRobadas) {
            std::cout << carta.color << (carta.esComodin ? " (Comodín)" : "") << (carta.esBonus ? " (+2)" : "") << " ";
        }
        std::cout << std::endl;
    }

    // aca estanlas condiciones que se deben cumplir para los puntajes que le da la carta
    int puntaje() const {
        int puntajePositivo = 0;
        int puntajeNegativo = 0;

        for (int i = 0; i < 6; i++) {
            int cantidad = conteoColores[i];
            if (cantidad > 6) cantidad = 6;

            if (cantidad > 0) {
                puntajePositivo += cantidad * 2;
            } else {
                puntajeNegativo += 1;
            }
        }

        for (const auto& carta : cartasRobadas) {
            if (carta.esBonus) {
                puntajePositivo += 2;
            }
        }

        return puntajePositivo - puntajeNegativo; 
    }

    std::string getNombre() const {
        return nombre;
    }
};

class Juego {
private:
    std::vector<Carta> mazo;// el mazo de cartas
    std::vector<Jugador> jugadores; // los jugadore
    std::vector<std::vector<Carta>> columnas; // Columnas
    std::vector<bool> columnasRobadas; // Estado de las columnas
    bool finJuego;
    int jugadoresRobando;

public:
    Juego(int n) : finJuego(false), columnas(3), columnasRobadas(3, false), jugadoresRobando(0) {
        inicializarMazo();
        crearJugadores(n);
    }

    void inicializarMazo() {
        std::vector<std::string> colores = {"rojo", "verde", "azul", "amarillo", "naranja", "negro"};
        for (const auto& color : colores) {
            for (int i = 0; i < 9; ++i) {
                mazo.emplace_back(color);
            }
        }
        for (int i = 0; i < 10; ++i) {
            mazo.emplace_back("Bonus", false, true);
        }
        mazo.emplace_back("Comodín", true);
        mazo.emplace_back("Comodín Dorado", true);
        mazo.emplace_back("Última Ronda"); 
        std::random_shuffle(mazo.begin(), mazo.end()); // aca se baraja el maazo
    }

    void crearJugadores(int n) {
        for (int i = 0; i < n; ++i) {
            std::string nombre;
            std::cout << "Ingresa el nombre del jugador " << (i + 1) << ": ";
            std::cin >> nombre;
            jugadores.emplace_back(nombre);
        }
    }

    bool agregarACartaColumna(int columnaIndex, const Carta& carta) {
        if (columnaIndex >= 0 && columnaIndex < columnas.size()) {
            if (columnasRobadas[columnaIndex]) {
                std::cout << "No se puede agregar cartas a la columna " << (columnaIndex + 1) << " porque ya ha sido robada." << std::endl;
                return false;
            }
            if (columnas[columnaIndex].size() < 3) {
                columnas[columnaIndex].push_back(carta);
                return true;
            } else {
                std::cout << "La columna " << (columnaIndex + 1) << " ya tiene 3 cartas." << std::endl;
            }
        } else {
            std::cout << "Columna inválida: " << (columnaIndex + 1) << "." << std::endl;
        }
        return false;
    }

    std::vector<Carta> robarCartas(int columnaIndex, Jugador& jugador) {
        if (columnaIndex >= 0 && columnaIndex < columnas.size() && !columnas[columnaIndex].empty()) {
            std::vector<Carta> cartasRobadas = columnas[columnaIndex];
            columnas[columnaIndex].clear(); // Limpia la columna después de robar
            columnasRobadas[columnaIndex] = true; // Marca la columna como robada
            return cartasRobadas;
        }
        std::cout << "No hay cartas en la columna " << (columnaIndex + 1) << " para robar." << std::endl;
        return {};
    }

    void mostrarColumnas() const {
        std::cout << "Estado de las columnas:\n";
        for (size_t i = 0; i < columnas.size(); ++i) {
            std::cout << "Columna " << (i + 1) << ": ";
            for (const auto& carta : columnas[i]) {
                std::cout << carta.color << " ";
            }
            std::cout << (columnasRobadas[i] ? "(Robada)" : "") << std::endl;
        }
        std::cout << std::endl;
    }

    void jugar() {
        while (!finJuego) {
            for (size_t i = 0; i < jugadores.size(); ++i) {
                if (mazo.empty()) {
                    std::cout << "El mazo está vacío. Fin del juego." << std::endl;
                    finJuego = true;
                    break;
                }

                std::cout << "\nTurno de " << jugadores[i].getNombre() << std::endl;

                // Jalar carta
                Carta carta = mazo.back();
                mazo.pop_back();
                std::cout << jugadores[i].getNombre() << " ha tomado una carta de color: " << carta.color << std::endl;

                // Elegir columna para agregar la carta
                int columnaIndex;
                std::cout << "Elige la columna (1-" << columnas.size() << ") para agregar la carta: ";
                std::cin >> columnaIndex;
                columnaIndex--; // Ajustar a índice 0

                agregarACartaColumna(columnaIndex, carta);

                // Mostrar estado de las columnas
                mostrarColumnas();

                // Preguntar si quiere robar cartas de cualquier columna
                int columnaRobar;
                std::cout << "¿De qué columna (1-" << columnas.size() << ") quieres robar cartas? (0 para no robar): ";
                std::cin >> columnaRobar;
                columnaRobar--; 

                if (columnaRobar >= 0 && columnaRobar < columnas.size()) {
                    auto cartasRobadas = robarCartas(columnaRobar, jugadores[i]);
                    if (!cartasRobadas.empty()) {
                        for (const auto& c : cartasRobadas) {
                            jugadores[i].agregarCarta(c);
                        }
                        std::cout << jugadores[i].getNombre() << " ha robado las siguientes cartas de la columna " << (columnaRobar + 1) << ": ";
                        for (const auto& c : cartasRobadas) {
                            std::cout << c.color << " ";
                        }
                        std::cout << std::endl;
                    }
                }

                if (carta.color == "Última Ronda") {
                    std::cout << "¡Es la última ronda!" << std::endl;
                    finJuego = true;
                    break;
                }
            }

            // Verificar si todas las columnas han sido robadas
            jugadoresRobando++;
            if (jugadoresRobando >= jugadores.size()) {
                std::cout << "Todos los jugadores han robado en este turno." << std::endl;
                // Resetear el estado de las columnas para el siguiente turno
                columnasRobadas.assign(3, false);
                jugadoresRobando = 0;
            }
        }

        mostrarResultados();
    }

    void mostrarResultados() const {
        int maxPuntaje = -999;
        std::string ganador;

        for (const auto& jugador : jugadores) {
            jugador.mostrarCartas();
            int puntaje = jugador.puntaje();
            std::cout << "Puntaje de " << jugador.getNombre() << ": " << puntaje << std::endl;

            if (puntaje > maxPuntaje) {
                maxPuntaje = puntaje;
                ganador = jugador.getNombre();
            }
        }

        std::cout << "El ganador es: " << ganador << " con " << maxPuntaje << " puntos." << std::endl;
    }
};

#endif // finalizacion de archivo de cabecera
