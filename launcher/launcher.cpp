#include "launcher.h"

namespace FGames::launcher
{

void Launcher::run()
{
    std::cout << "Launcher" << "\n";

    currentGame_ =std::make_unique<FGames::games::DemoGame>();


    engine_.run(*currentGame_);

    if (currentGame_)
    {
        engine_.run(*currentGame_);
    }
}

}