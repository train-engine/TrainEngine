# Project name
EXEC = TrainEngine

# Build, bin and install directories (conserve root directories for clean)
BUILD_DIR_ROOT = build
BUILD_DIR := $(BUILD_DIR_ROOT)
BIN_DIR_ROOT = bin
BIN_DIR := $(BIN_DIR_ROOT)
INSTALL_DIR := ~/Desktop/$(EXEC)

# Sources
SRC_DIR = src
SRCS := $(sort $(shell find $(SRC_DIR) -name '*.cpp'))

# Includes
INCLUDE_DIR = include
INCLUDE_SUBDIRS := $(patsubst %/,%,$(sort $(dir $(wildcard $(INCLUDE_DIR)/*/.)))) # Note: only include immediate include subdirectories
INCLUDES := $(addprefix -I,$(INCLUDE_SUBDIRS))

# C preprocessor flags
CPPFLAGS := -MMD -MP $(INCLUDES)

# C++ compiler settings
CXX = g++
CXXFLAGS = -std=c++14
WARNINGS = -Wall -Wpedantic -Wextra -Wcast-align -Wduplicated-cond -Wextra -Wlogical-op -Wmissing-declarations\
		-Wmissing-include-dirs -Wno-aggressive-loop-optimizations -Wno-unused-parameter -Wnon-virtual-dtor -Wredundant-decls\
		-Wshadow -Wsuggest-override -Wundef -Wunreachable-code -Wuseless-cast -Wzero-as-null-pointer-constant

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
	CPPFLAGS += -DSFML_STATIC

	SFML_GRAPHICS_LIBS := $(SFML_GRAPHICS_LIBS)-s
	SFML_WINDOW_LIBS := $(SFML_WINDOW_LIBS)-s
	SFML_AUDIO_LIBS := $(SFML_AUDIO_LIBS)-s
	SFML_NETWORK_LIBS := $(SFML_NETWORK_LIBS)-s
	SFML_SYSTEM_LIBS := $(SFML_SYSTEM_LIBS)-s

	ifeq ($(release),1)
		# Link everything statically (including libgcc and libstdc++) and disable console output on release builds
		LDFLAGS = -static -mwindows
	else
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

# OS-specific assets-copying script selection
ifeq ($(OS),Windows_NT)
	ifeq ($(win32),1)
		ifeq ($(release),1)
			COPY_ASSETS_SCRIPT = u_copy_assets_windows32_r.sh
		else
			COPY_ASSETS_SCRIPT = u_copy_assets_windows32_d.sh
		endif
	else
		ifeq ($(release),1)
			COPY_ASSETS_SCRIPT = u_copy_assets_windows64_r.sh
		else
			COPY_ASSETS_SCRIPT = u_copy_assets_windows64_d.sh
		endif
	endif
else ifeq ($(UNAME),Linux)
	ifeq ($(release),1)
		COPY_ASSETS_SCRIPT = u_copy_assets_linux_r.sh
	else
		COPY_ASSETS_SCRIPT = u_copy_assets_linux_d.sh
	endif
endif

# Debug (default) and release modes
ifeq ($(release),1)
	BUILD_DIR := $(BUILD_DIR)/release
	BIN_DIR := $(BIN_DIR)/release
	CXXFLAGS += -O3
	CPPFLAGS += -DNDEBUG
else
	BUILD_DIR := $(BUILD_DIR)/debug
	BIN_DIR := $(BIN_DIR)/debug
	CXXFLAGS += -O0 -g
endif

# Linker flags for SFML
SFML_GRAPHICS_LIBS := $(addprefix -l,$(SFML_GRAPHICS_LIBS))
SFML_WINDOW_LIBS := $(addprefix -l,$(SFML_WINDOW_LIBS))
SFML_AUDIO_LIBS := $(addprefix -l,$(SFML_AUDIO_LIBS))
SFML_NETWORK_LIBS := $(addprefix -l,$(SFML_NETWORK_LIBS))
SFML_SYSTEM_LIBS := $(addprefix -l,$(SFML_SYSTEM_LIBS))

LDLIBS += $(SFML_GRAPHICS_LIBS) $(SFML_WINDOW_LIBS) $(SFML_AUDIO_LIBS) $(SFML_NETWORK_LIBS) $(SFML_SYSTEM_LIBS)

# Objects and dependencies
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Include program info and icon on Windows
ifeq ($(OS),Windows_NT)
	OBJS += $(BUILD_DIR)/resource.res
endif

# Rules

.PHONY: all
all: $(BIN_DIR)/$(EXEC)

# Build executable
$(BIN_DIR)/$(EXEC): $(OBJS)
	@echo "Building executable: $@"
	@mkdir -p $(@D)
	@$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Build C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNINGS) -c $< -o $@

# Add resource file to Windows executable
$(BUILD_DIR)/%.res: assets/%.rc
	@echo "Compiling Windows resource file"
	@windres $< -O coff -o $@

# Include automatically-generated dependencies
-include $(DEPS)

# Install packaged program
.PHONY: install
install: all copyassets
	@echo "Packaging program to $(INSTALL_DIR)"
	@mkdir -p $(INSTALL_DIR); cp -r $(BIN_DIR)/. $(INSTALL_DIR)

# Build and run
.PHONY: run
run: all
	@echo "Starting program $(BIN_DIR)/$(EXEC)"
	@cd ./$(BIN_DIR); ./$(EXEC)

# Copy assets to bin directory for selected platform
.PHONY: copyassets
copyassets:
	@echo "Copying assets with script: $(COPY_ASSETS_SCRIPT)"
	@./scripts/$(COPY_ASSETS_SCRIPT) 2> /dev/null

# Clean build and bin directories for all platforms
.PHONY: clean
clean:
	@echo "Cleaning $(BUILD_DIR_ROOT) and $(BIN_DIR_ROOT) directories"
	@$(RM) -r $(BUILD_DIR_ROOT)
	@$(RM) -r $(BIN_DIR_ROOT)

# Clean all assets from bin directories for all platforms
.PHONY: cleanassets
cleanassets:
	@echo "Cleaning assets for all platforms"
	@./scripts/u_clean_assets.sh

# Run clang-format on source code
.PHONY: format
format:
	@echo "Running clang-format"
	@clang-format -i src/*.cpp include/*.h

# Generate documentation with Doxygen
.PHONY: doc
doc:
	@echo "Generating documentation"
	@doxygen docs/Doxyfile

# Echo Makefile variables
.PHONY: printvars
printvars:
	@echo EXEC: $(EXEC)
	@echo BUILD_DIR: $(BUILD_DIR)
	@echo BIN_DIR: $(BIN_DIR)
	@echo INSTALL_DIR: $(INSTALL_DIR)
	@echo SRC_DIR: $(SRC_DIR)
	@echo SRCS: $(SRCS)
	@echo INCLUDE_DIR: $(INCLUDE_DIR)
	@echo INCLUDES: $(INCLUDES)
	@echo CXX: $(CXX)
	@echo CPPFLAGS: $(CPPFLAGS)
	@echo CXXFLAGS: $(CXXFLAGS)
	@echo WARNINGS: $(WARNINGS)
	@echo LDFLAGS: $(LDFLAGS)
	@echo LDLIBS: $(LDLIBS)
	@echo COPY_ASSETS_SCRIPT: $(COPY_ASSETS_SCRIPT)
