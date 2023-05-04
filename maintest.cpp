#include "main.h"

int main(int argc, char* argv[]) {

    
  // Create the game object
    Game* game = new Game();
        // Initialize and run the game
        if (game->Init()) {
            if (game->menugame() == 1)
       
                    game->Run();
        }

        // Clean up
        delete game;
        return 0;
}
