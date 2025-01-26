#include "Game.h"

#include <iostream>
#include <filesystem>

int main( int, char** )
{
    std::cout << "Current path: " << std::filesystem::current_path() << std::endl;

    std::cout << "Test starts" << std::endl;

    Game& game = Game::GetInstance();
    game.Init();
    game.Run();
    game.Deinit();

    std::cout << "Test ends" << std::endl;

    return 0;
}