#include <iostream>
#include <vector>
#include <random>   // For random number generation
#include <chrono>   // For seeding the random number generator
using namespace std;
// Define Map as a vector of vectors of integers.
// You can change 'int' to whatever type best represents your cells (e.g., char, bool).
using Map = std::vector<std::vector<int>>;


mt19937 rng(random_device{}()); // Initialize random number generator

/**
 * @brief Prints the map (matrix) to the console.
 * @param map The map to print.
 */
void printMap(const Map& map) {
    std::cout << "\033[32m--- \033[92mCurrent Map \033[32m---\033[0m" << std::endl;
    for (const auto& row : map) {
        for (int cell : row) {
            // Adapt this to represent your cells meaningfully (e.g., ' ' for empty, '#' for occupied).
            if (cell == 0){
                std::cout << "\033[90m.";
            }
            else if(cell==1){
                std::cout << "\033[93m@";
            }
            else if(cell==3){
                cout<< "\033[94m#";
            }
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\033[32m-------------------\033[0m" << std::endl;
}

int gen_Random(int a, int b){
    std::uniform_int_distribution<> distrib(a,b);
    int num = distrib(rng);
    return num;
}

void set_direction(int& dirX, int& dirY){
    std::cout << " direccion es: ";
    int direction = gen_Random(1, 4);
    switch(direction){
        case 1: //up
            std::cout << "Arriba\n";
            dirX = 0;
            dirY = -1;
        break;
        case 2:
            //down
            std::cout << "Abajo\n";
            dirX = 0;
            dirY = 1;
        break;
        case 3: 
            //left
            std::cout << "Izquierda\n";
            dirX = -1;
            dirY = 0;
        break;
        case 4: 
            //right
            std::cout << "Derecha\n";
            dirX = 1;
            dirY = 0;
        break;
    }
}

bool is_legal_coor(int limX, int limY, int x, int y) {
    return (x >= 0 && x < limX && y >= 0 && y < limY);
}

void update_map(Map& map, int x, int y, int value) {
    if (map[y][x] == 0){
        map[y][x] = value;
    }
}
bool is_legal_room(int lX, int lY, int x, int y, int roomSizeX, int roomSizeY, Map& map){
    for (int i = 0; i < roomSizeY; i++){
        for (int j = 0; j < roomSizeX; j++){
            if (!is_legal_coor(lX, lY, x + j, y + i)){
                return false;
            }
        }
    }
    vector<pair<int, int>> occupiedCells = {
        {-1,-1},{roomSizeX,-1},{-1,roomSizeY},{roomSizeX,roomSizeY},
        {0,-1},{roomSizeX-1,-1},{0,roomSizeY},{roomSizeX-1,roomSizeY}

    };

    for(auto& cell : occupiedCells){
        int cx= x + cell.first;
        int cy= y + cell.second;
        if(is_legal_coor(lX, lY, cx, cy) && map[cy][cx] == 3){
            return false;

        }
    }

    return true;

}

void create_room(int x, int y, int roomSizeX, int roomSizeY, int& W, int& H, Map& map) {
    for (int i = 0; i < roomSizeY; i++){
        for (int j = 0; j < roomSizeX; j++){
            update_map(map, x + j, y + i, 3);
        }
    }
}



/**
 * @brief Function to implement the Cellular Automata logic.
 * It should take a map and return the updated map after one iteration.
 * @param currentMap The map in its current state.
 * @param W Width of the map.
 * @param H Height of the map.
 * @param R Radius of the neighbor window (e.g., 1 for 3x3, 2 for 5x5).
 * @param U Threshold to decide if the current cell becomes 1 or 0.
 * @return The map after applying the cellular automata rules.
 */
Map cellularAutomata(const Map& currentMap, int W, int H, int R, double U) {
    Map newMap = currentMap; // Initially, the new map is a copy of the current one

    for(int y=0;y<H;y++){
        for(int x=0;x<W;x++){
            int activeNeighbors = 0;
            int totalNeighbors = 0;

            for (int dy = -R; dy <= R; dy++){
                for(int dx=-R;dx<=R;dx++){
                    if (dx == 0 && dy == 0) continue; // Skip the cell itself
                    int neighborX = x + dx;
                    int neighborY = y + dy;

                    if (is_legal_coor(W, H, neighborX, neighborY)) {
                        totalNeighbors++;
                        if (currentMap[neighborY][neighborX] == 1) {
                            activeNeighbors++;
                        }
                    }
                }
            }
            double actividad;
            if(totalNeighbors > 0) {
                actividad = static_cast<double>(activeNeighbors) / totalNeighbors;
            } else {
                actividad = 0.0; // No neighbors, no activity
            }
            
            if(actividad>=U){
                newMap[y][x] = 1; // Cell becomes active
            } else {
                newMap[y][x] = 0; // Cell becomes inactive
            }
        }
    }

    return newMap;
}

/**
 * @brief Function to implement the Drunk Agent logic.
 * It should take a map and parameters controlling the agent's behavior,
 * then return the updated map after the agent performs its actions.
 *
 * @param currentMap The map in its current state.
 * @param W Width of the map.
 * @param H Height of the map.
 * @param J The number of times the agent "walks" (initiates a path).
 * @param I The number of steps the agent takes per "walk".
 * @param roomSizeX Max width of rooms the agent can generate.
 * @param roomSizeY Max height of rooms the agent can generate.
 * @param probGenerateRoom Probability (0.0 to 1.0) of generating a room at each step.
 * @param probIncreaseRoom If no room is generated, this value increases probGenerateRoom.
 * @param probChangeDirection Probability (0.0 to 1.0) of changing direction at each step.
 * @param probIncreaseChange If direction is not changed, this value increases probChangeDirection.
 * @param agentX Current X position of the agent (updated by reference).
 * @param agentY Current Y position of the agent (updated by reference).
 * @return The map after the agent's movements and actions.
 */
Map drunkAgent(const Map& currentMap, int W, int H, int J, int I, int roomSizeX, int roomSizeY,
               double probGenerateRoom, double probIncreaseRoom,
               double probChangeDirection, double probIncreaseChange,
               int& agentX, int& agentY) {
    Map newMap = currentMap; // The new map is a copy of the current one
    //newMap[agentY][agentX] = 1;
    update_map(newMap, agentX, agentY, 1);
    int direct_x = 0;
    int direct_y = 0;
    double generateRoomProb = probGenerateRoom * 10;
    double ChangeDirection = probChangeDirection * 10;

    for (int i = 0; i < J; i++){
        //cambio de direccion
        if (double(gen_Random(0, 10)) <=ChangeDirection) {
            cout << "Se cambia direccion \nNueva";
            ChangeDirection = probChangeDirection * 10;
            set_direction(direct_x, direct_y);
        }       
        else {
            cout << "No se cambia direccion \nNueva posibilidad de cambio de direccion: " << ChangeDirection * 10 <<"%\n";
            ChangeDirection += probIncreaseChange*10;
        }
        //generacion de habitacion
        if (double(gen_Random(0, 10)) <= generateRoomProb){
            
            if (is_legal_room(W, H,agentX-(roomSizeX/2),agentY-(roomSizeY/2), roomSizeX, roomSizeY, newMap)){
                create_room(agentX-(roomSizeX/2),agentY-(roomSizeY/2), roomSizeX, roomSizeY, W, H, newMap);
                generateRoomProb = probGenerateRoom * 10;
                std::cout << "Se genero habitacion \nNueva posibilidad de generacion: " << generateRoomProb * 10 <<"%\n";
            }
            else {
                std::cout << "No se pudo generar habitacion \nNueva posibilidad de generacion:" << generateRoomProb * 10 <<"%\n";
                generateRoomProb += probIncreaseRoom*10;
            }
        }
        else {
            std::cout << "No se genera habitacion \nNueva posibilidad de generacion: " << generateRoomProb * 10 <<"%\n";
            generateRoomProb += probIncreaseRoom * 10;
        }
        for (int j = 0; j < I; j++){
            int newX = agentX + direct_x;
            int newY = agentY + direct_y;

            if (is_legal_coor(W, H, newX, newY)==true){
                agentX = newX;
                agentY = newY;
                update_map(newMap, agentX, agentY, 1);
            }
            else {
                set_direction(direct_x, direct_y);
                j--;
            }
        }
    }
    
    // TODO: IMPLEMENTATION GOES HERE for the Drunk Agent logic.
    // The agent should move randomly.
    // You'll need a random number generator.
    // Consider:
    // - How the agent moves (possible steps).
    // - What it does if it encounters a border or an obstacle (if applicable).
    // - How it modifies the map (e.g., leaving a trail, creating rooms, etc.).
    // - Use the provided parameters (J, I, roomSizeX, roomSizeY, probabilities)
    //   to control its behavior.
    return newMap;
}


int main() {
    std::cout << "\033[33m--- \033[93mSimulacion Del Automata Celular y el Agente Borracho\033[33m---\033[0m" << std::endl;
    
    // --- Initial Map Configuration ---
    int mapRows = 20;
    int mapCols = 40;
    Map myMap(mapRows, std::vector<int>(mapCols, 0)); // Map initialized with zeros


    // TODO: IMPLEMENTATION GOES HERE: Initialize the map with some pattern or initial state.
    // For example, you might set some cells to 1 for the cellular automata
    // or place the drunk agent at a specific position.

    // Drunk Agent's initial position
    int drunkAgentX = mapRows / 2;
    int drunkAgentY = mapCols / 2;

    do {
        drunkAgentX = gen_Random(2, mapRows-3);
        drunkAgentY = gen_Random(2, mapCols-3);
    }while(!is_legal_coor(mapCols, mapRows, drunkAgentX, drunkAgentY));

    update_map(myMap, drunkAgentX, drunkAgentY, 1);
    Map myMap2=myMap; // Copy the initial map to myMap2 for later use

    // If your agent modifies the map at start, you could do it here:
    // myMap[drunkAgentX][drunkAgentY] = 2; // Assuming '2' represents the agent

    std::cout << "\nInitial map state:" << std::endl;
    printMap(myMap);

    // --- Simulacion de Parametros ---
    int numIterations = 5; //Numero de Pasos de la simulacion

    // Parametros del Automata Celular
    int ca_W = mapCols;
    int ca_H = mapRows;
    int ca_R = 1;      // Radio de las celdad vecinas
    double ca_U = 0.5; // Umbral para decidir si la celda se convierte en 1 o 0

    // Drunk Agent Parameters
    int da_W = mapCols;
    int da_H = mapRows;
    int da_J = 5;      // Numero de veces que el agente "camina" (inicia un camino)
    int da_I = 5;     // pasos que el agente da por "camino"
    int da_roomSizeX = 5;
    int da_roomSizeY = 3;
    double da_probGenerateRoom = 0.5;
    double da_probIncreaseRoom = 0.05;
    double da_probChangeDirection = 0.2;
    double da_probIncreaseChange = 0.03;


    // --- Main Simulation Loop ---
    for (int iteration = 0; iteration < numIterations; ++iteration) {
        std::cout << "\n\033[34m--- \033[94mIteration " << iteration + 1 << " \033[34m---\033[0m" << std::endl;

        // TODO: IMPLEMENTATION GOES HERE: Call the Cellular Automata and/or Drunk Agent functions.
        // The order of calls will depend on how you want them to interact.

        // Example: First the cellular automata, then the agent
        cout<<"\n\033[32m--- Calma Conductor Automata Pasando ---\033[0m\n";
        myMap = cellularAutomata(myMap2, ca_W, ca_H, ca_R, ca_U);
        printMap(myMap2);
        cout<<"\n\033[31m--- Cuidado Conductor Borracho Pasando ---\033[0m\n";
        myMap = drunkAgent(myMap, da_W, da_H, da_J, da_I, da_roomSizeX, da_roomSizeY,
                           da_probGenerateRoom, da_probIncreaseRoom,
                           da_probChangeDirection, da_probIncreaseChange,
                           drunkAgentX, drunkAgentY);

        printMap(myMap);
        //std::cout << "hola";

        // You can add a delay to visualize the simulation step by step
        // #include <thread> // For std::this_thread::sleep_for
        // #include <chrono> // For std::chrono::milliseconds
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "\n\033[35m--- \033[95mSimulation Finished \033[35m---\033[0m" << std::endl;
    return 0;
}