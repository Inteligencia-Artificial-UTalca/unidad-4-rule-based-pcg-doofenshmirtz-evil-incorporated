#include <iostream>
#include <vector>
#include <random>   // For random number generation
#include <chrono>   // For seeding the random number generator

// Define Map as a vector of vectors of integers.
// You can change 'int' to whatever type best represents your cells (e.g., char, bool).
using Map = std::vector<std::vector<int>>;




/**
 * @brief Prints the map (matrix) to the console.
 * @param map The map to print.
 */
void printMap(const Map& map) {
    std::cout << "\033[32m--- \033[92mCurrent Map \033[32m---\033[0m" << std::endl;
    for (const auto& row : map) {
        for (int cell : row) {
            // Adapt this to represent your cells meaningfully (e.g., ' ' for empty, '#' for occupied).
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\033[32m-------------------\033[0m" << std::endl;
}

int gen_Random(int a, int b){
    std::random_device rd;  
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(a,b);
    return distrib(gen);
}

void set_direction(int& dirX, int& dirY){
    int direction = gen_Random(1, 4);
    switch(direction){
        case 1: //up
            dirX = 0;
            dirY = -1;
        break;
        case 2: //down
            dirX = 0;
            dirY = 1;
        break;
        case 3: //left
            dirX = -1;
            dirY = 0;
        break;
        case 4: //right
            dirX = 1;
            dirY = 0;
        break;
    }
}

bool is_legal_coor(int limX, int limY, int x, int y) {
    return (x >= 0 && x < limX && y >= 0 && y < limY);
    std::cout << "\nis illegal move\n";
    /*if (x > 0 && x < limX) {
        if (y > 0 && y < limY){
            //std::cout << "\nis legal move\n";
            return true;
        }
    }
    
    return false;*/
}

void update_map(Map& map, int x, int y, int value) {
    if (map[y][x] == 0){
        map[y][x] = value;
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

    // TODO: IMPLEMENTATION GOES HERE for the cellular automata logic.
    // Iterate over each cell and apply the transition rules.
    // Remember that updates should be based on the 'currentMap' state
    // and applied to the 'newMap' to avoid race conditions within the same iteration.

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
    for (int i = 0; i < J; i++){
        set_direction(direct_x, direct_y);
        for (int j = 0; j < I; j++){
            agentX += direct_x;
            agentY += direct_y;
            if (is_legal_coor(W, H, agentX, agentY)==true){
                //newMap[agentY][agentX] = 1;
                update_map(newMap, agentX, agentY, 1);
            }
            else {
                agentX -= direct_x;
                agentY -= direct_y;
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
    //std::cout << "agent_coor: X=" << agentX << "    Y=" << agentY << std::endl << newMap[agentY][agentX] << std::endl;


    return newMap;
}


int main() {
    std::cout << "\033[33m--- \033[93mCELLULAR AUTOMATA AND DRUNK AGENT SIMULATION \033[33m---\033[0m" << std::endl;
    
    // --- Initial Map Configuration ---
    int mapRows = 10;
    int mapCols = 20;
    Map myMap(mapRows, std::vector<int>(mapCols, 0)); // Map initialized with zeros


    // TODO: IMPLEMENTATION GOES HERE: Initialize the map with some pattern or initial state.
    // For example, you might set some cells to 1 for the cellular automata
    // or place the drunk agent at a specific position.

    // Drunk Agent's initial position
    int drunkAgentX = mapRows / 2;
    int drunkAgentY = mapCols / 2;

    do {
        drunkAgentX = gen_Random(0, mapRows);
        drunkAgentY = gen_Random(0, mapCols);
    }while(!is_legal_coor(mapCols, mapRows, drunkAgentX, drunkAgentY));

    update_map(myMap, drunkAgentX, drunkAgentY, 1);

    // If your agent modifies the map at start, you could do it here:
    // myMap[drunkAgentX][drunkAgentY] = 2; // Assuming '2' represents the agent

    std::cout << "\nInitial map state:" << std::endl;
    printMap(myMap);

    // --- Simulation Parameters ---
    int numIterations = 5; // Number of simulation steps

    // Cellular Automata Parameters
    int ca_W = mapCols;
    int ca_H = mapRows;
    int ca_R = 1;      // Radius of neighbor window
    double ca_U = 0.5; // Threshold

    // Drunk Agent Parameters
    int da_W = mapCols;
    int da_H = mapRows;
    int da_J = 5;      // Number of "walks"
    int da_I = 5;     // Steps per walk
    int da_roomSizeX = 5;
    int da_roomSizeY = 3;
    double da_probGenerateRoom = 0.1;
    double da_probIncreaseRoom = 0.05;
    double da_probChangeDirection = 0.2;
    double da_probIncreaseChange = 0.03;


    // --- Main Simulation Loop ---
    for (int iteration = 0; iteration < numIterations; ++iteration) {
        std::cout << "\n\033[34m--- \033[94mIteration " << iteration + 1 << " \033[34m---\033[0m" << std::endl;

        // TODO: IMPLEMENTATION GOES HERE: Call the Cellular Automata and/or Drunk Agent functions.
        // The order of calls will depend on how you want them to interact.

        // Example: First the cellular automata, then the agent
        //myMap = cellularAutomata(myMap, ca_W, ca_H, ca_R, ca_U);
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