LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := trainengine

LOCAL_SHARED_LIBRARIES := sfml-system
LOCAL_SHARED_LIBRARIES += sfml-window
LOCAL_SHARED_LIBRARIES += sfml-graphics
LOCAL_SHARED_LIBRARIES += sfml-audio
LOCAL_SHARED_LIBRARIES += sfml-network
LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../include

LOCAL_SRC_FILES := ../../../src/AnimatedSprite.cpp
LOCAL_SRC_FILES += ../../../src/Camera.cpp
LOCAL_SRC_FILES += ../../../src/CharacterEntity.cpp
LOCAL_SRC_FILES += ../../../src/CreatorState.cpp
LOCAL_SRC_FILES += ../../../src/Entity.cpp
LOCAL_SRC_FILES += ../../../src/EntityTracker.cpp
LOCAL_SRC_FILES += ../../../src/FileManager.cpp
LOCAL_SRC_FILES += ../../../src/GameEngine.cpp
LOCAL_SRC_FILES += ../../../src/GrassBlock.cpp
LOCAL_SRC_FILES += ../../../src/Gui.cpp
LOCAL_SRC_FILES += ../../../src/InputManager.cpp
LOCAL_SRC_FILES += ../../../src/Level.cpp
LOCAL_SRC_FILES += ../../../src/LevelCreator.cpp
LOCAL_SRC_FILES += ../../../src/LevelSandbox.cpp
LOCAL_SRC_FILES += ../../../src/LoadPlayState.cpp
LOCAL_SRC_FILES += ../../../src/LoopDebugInfo.cpp
LOCAL_SRC_FILES += ../../../src/main.cpp
LOCAL_SRC_FILES += ../../../src/MainMenuState.cpp
LOCAL_SRC_FILES += ../../../src/Map.cpp
LOCAL_SRC_FILES += ../../../src/MenuOptionsState.cpp
LOCAL_SRC_FILES += ../../../src/ParallaxSprite.cpp
LOCAL_SRC_FILES += ../../../src/PauseState.cpp
LOCAL_SRC_FILES += ../../../src/Player.cpp
LOCAL_SRC_FILES += ../../../src/PlayState.cpp
LOCAL_SRC_FILES += ../../../src/ResourceManager.cpp
LOCAL_SRC_FILES += ../../../src/SplashScreenState.cpp
LOCAL_SRC_FILES += ../../../src/State.cpp
LOCAL_SRC_FILES += ../../../src/Textbox.cpp
LOCAL_SRC_FILES += ../../../src/Tile.cpp

include $(BUILD_SHARED_LIBRARY)

$(call import-module,sfml)
