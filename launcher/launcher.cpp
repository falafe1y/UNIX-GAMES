#include "launcher.h"

namespace fgames::launcher
{

void Launcher::run()
{
    // std::cout << "Launcher" << "\n";

    currentGame_ =std::make_unique<fgames::games::SnakeGame>();

    engine_.run(*currentGame_);

    if (currentGame_)
    {
        engine_.run(*currentGame_);
    }
}

}