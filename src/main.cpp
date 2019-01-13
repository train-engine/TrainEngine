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
        pAndroidBuffer = new AndroidBuffer;
        std::cout.rdbuf(pAndroidBuffer);
        std::cerr.rdbuf(pAndroidBuffer);
    #endif

    GameEngine trainEngine;

    trainEngine.RequestPush(new SplashScreenState(trainEngine));
    trainEngine.GameLoop();

    #if defined(SFML_SYSTEM_ANDROID)
        std::cout.rdbuf(nullptr);
        std::cerr.rdbuf(nullptr);
        delete pAndroidBuffer;
    #endif
}
