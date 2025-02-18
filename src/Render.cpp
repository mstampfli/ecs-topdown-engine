#include <unordered_map>
#include "headers/Render.h"
#include "headers/Components.h"
#include "headers/Entity.h"
#include <iostream>
#include <cstdlib>
#include <cmath>


void renderPlayers() {
    system("cls");
    for (auto& [entity, position] : positionComponents) {
        int i, j;
        i = j = 0;
        int roundedXPos = static_cast<int>(std::floor(position.x));
        int roundedYPos = static_cast<int>(std::floor(position.y));

        while (i < roundedYPos) {
            std::cout << "\n";
            i++;
        }

        while (i == roundedYPos && j < roundedXPos){
            std::cout << " ";
            j++;
        }

        if (i == roundedYPos && j == roundedXPos) {
            std::cout << "x";
        }
    }
}