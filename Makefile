EXEC = TrainEngine

CXX = g++
CXXFLAGS = -std=c++14 -Wall

BUILD_DIR = build
BIN_DIR = bin

# SFML variables
SFML_DIR = libs/SFML-2.4.2
SFML_GRAPHICS_LIBS = sfml-graphics
SFML_WINDOW_LIBS = sfml-window
SFML_AUDIO_LIBS = sfml-audio
SFML_NETWORK_LIBS = sfml-network
SFML_SYSTEM_LIBS = sfml-system

# OS-specific settings
ifeq ($(OS),Windows_NT)
	EXEC := $(EXEC).exe
	BUILD_DIR := $(BUILD_DIR)/make_windows
	BIN_DIR := $(BIN_DIR)/windows

	# Link SFML statically on Windows
	CXXFLAGS += -DSFML_STATIC
	LDFLAGS = -Wl,-Bstatic

	SFML_GRAPHICS_LIBS := $(SFML_GRAPHICS_LIBS)-s
	SFML_WINDOW_LIBS := $(SFML_WINDOW_LIBS)-s
	SFML_AUDIO_LIBS := $(SFML_AUDIO_LIBS)-s
	SFML_NETWORK_LIBS := $(SFML_NETWORK_LIBS)-s
	SFML_SYSTEM_LIBS := $(SFML_SYSTEM_LIBS)-s

	ifneq ($(release),1)
		# Disable console output on release builds
		CXXFLAGS += -mconsole

		# Link the debug versions of SFML when compiling for debug on Windows
		SFML_GRAPHICS_LIBS := $(SFML_GRAPHICS_LIBS)-d
		SFML_WINDOW_LIBS := $(SFML_WINDOW_LIBS)-d
		SFML_AUDIO_LIBS := $(SFML_AUDIO_LIBS)-d
		SFML_NETWORK_LIBS := $(SFML_NETWORK_LIBS)-d
		SFML_SYSTEM_LIBS := $(SFML_SYSTEM_LIBS)-d
	endif

	# Add dependencies for SFML libraries when statically linking
	SFML_GRAPHICS_LIBS += freetype jpeg
	SFML_WINDOW_LIBS += opengl32 gdi32
	SFML_AUDIO_LIBS += openal32 flac vorbisenc vorbisfile vorbis ogg
	SFML_NETWORK_LIBS += ws2_32
	SFML_SYSTEM_LIBS += winmm

	# 32-bit
	ifeq ($(win32),1)
		BUILD_DIR := $(BUILD_DIR)32
		BIN_DIR := $(BIN_DIR)32
		CXXFLAGS += -m32
		LDFLAGS += -L$(SFML_DIR)/lib/windows-gcc-6.1.0-mingw-32-bit
	else
		BUILD_DIR := $(BUILD_DIR)64
		BIN_DIR := $(BIN_DIR)64
		CXXFLAGS += -m64
		LDFLAGS += -L$(SFML_DIR)/lib/windows-gcc-6.1.0-mingw-64-bit
	endif
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Linux)
		BUILD_DIR := $(BUILD_DIR)/make_linux
		BIN_DIR := $(BIN_DIR)/linux
	else
    	$(error OS not supported by this Makefile)
	endif
endif

# OS-specific assets-copying script
ifeq ($(OS),Windows_NT)
	ifeq ($(win32),1)
		ifeq ($(release),1)
			COPY_ASSETS_SCRIPT = l_copy_assets_windows32_r.sh
		else
			COPY_ASSETS_SCRIPT = l_copy_assets_windows32_d.sh
		endif
	else
		ifeq ($(release),1)
			COPY_ASSETS_SCRIPT = l_copy_assets_windows64_r.sh
		else
			COPY_ASSETS_SCRIPT = l_copy_assets_windows64_d.sh
		endif
	endif
else ifeq ($(UNAME),Linux)
	ifeq ($(release),1)
		COPY_ASSETS_SCRIPT = l_copy_assets_linux_r.sh
	else
		COPY_ASSETS_SCRIPT = l_copy_assets_linux_d.sh
	endif
endif

# Debug (default) and release modes
ifeq ($(release),1)
	BUILD_DIR := $(BUILD_DIR)/release
	BIN_DIR := $(BIN_DIR)/release
	CXXFLAGS += -O3 -DNDEBUG
else
	BUILD_DIR := $(BUILD_DIR)/debug
	BIN_DIR := $(BIN_DIR)/debug
	CXXFLAGS += -O0 -g
endif

# Linker flags
SFML_GRAPHICS_LIBS := $(addprefix -l,$(SFML_GRAPHICS_LIBS))
SFML_WINDOW_LIBS := $(addprefix -l,$(SFML_WINDOW_LIBS))
SFML_AUDIO_LIBS := $(addprefix -l,$(SFML_AUDIO_LIBS))
SFML_NETWORK_LIBS := $(addprefix -l,$(SFML_NETWORK_LIBS))
SFML_SYSTEM_LIBS := $(addprefix -l,$(SFML_SYSTEM_LIBS))

LDFLAGS += $(SFML_GRAPHICS_LIBS) $(SFML_WINDOW_LIBS) $(SFML_AUDIO_LIBS) $(SFML_NETWORK_LIBS) $(SFML_SYSTEM_LIBS)

# Sources, objects and includes
SRC_DIR = src
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

INC_DIRS = include $(SFML_DIR)/include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Recipes
.PHONY: all
all: $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MP -MMD $(INC_FLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: run
run: all
	@cd ./$(BIN_DIR); ./$(EXEC)

.PHONY: copyassets
copyassets:
	@./scripts/$(COPY_ASSETS_SCRIPT)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(BIN_DIR)

.PHONY: cleanall
cleanall:
	$(RM) -r build
	$(RM) -r bin

.PHONY: cleanassets
cleanassets:
	@./scripts/l_clean_assets.sh

.PHONY: printvars
printvars:
	@echo EXEC: $(EXEC)
	@echo BUILD_DIR: $(BUILD_DIR)
	@echo BIN_DIR: $(BIN_DIR)
	@echo CXX: $(CXX)
	@echo CXXFLAGS: $(CXXFLAGS)
	@echo LDFLAGS: $(LDFLAGS)
	@echo COPY_ASSETS_SCRIPT: $(COPY_ASSETS_SCRIPT)
