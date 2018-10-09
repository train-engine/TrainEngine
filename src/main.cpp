#include <SFML/Config.hpp>
#include "GameEngine.h"
#include "SplashScreenState.h"
#if defined(SFML_SYSTEM_IOS)
    #include <SFML/Main.hpp>
#elif defined(SFML_SYSTEM_ANDROID)
    #include <iostream>
    #include "AndroidCout.h"
#endif

int main()
{
    #if defined(SFML_SYSTEM_ANDROID)
        std::cout.rdbuf(new AndroidBuffer);
    #endif

    GameEngine trainEngine;

    trainEngine.RequestPush(new SplashScreenState(trainEngine));
    trainEngine.GameLoop();

    #if defined(SFML_SYSTEM_ANDROID)
        delete std::cout.rdbuf(nullptr);
    #endif
}
