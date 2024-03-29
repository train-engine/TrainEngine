#include <SFML/Config.hpp>
#include "Core/GameEngine.h"
#include "States/SplashScreenState.h"
#if defined(SFML_SYSTEM_IOS)
#include <SFML/Main.hpp>
#elif defined(SFML_SYSTEM_ANDROID)
#include <iostream>
#include "Misc/AndroidCout.h"
#endif

int main()
{
#if defined(SFML_SYSTEM_ANDROID)
    androidBuffer = new AndroidBuffer;
    std::cout.rdbuf(pAndroidBuffer);
    std::cerr.rdbuf(pAndroidBuffer);
#endif

    GameEngine trainEngine;

    trainEngine.requestPush(new SplashScreenState(trainEngine));
    trainEngine.startGameLoop();

#if defined(SFML_SYSTEM_ANDROID)
    std::cout.rdbuf(nullptr);
    std::cerr.rdbuf(nullptr);
    delete androidBuffer;
#endif
}
