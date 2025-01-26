#include "Game.h"

#include <iostream>
#include <filesystem>

int main( int, char** )
{
    Game& game = Game::GetInstance();

    game.Init();
    game.Run();
    game.Deinit();

    return 0;
}