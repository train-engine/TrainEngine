################################################################################
##### Variables and settings
################################################################################

# Project name
EXEC = TrainEngine

# Build, bin and install directories (conserve root directories for clean)
BUILD_DIR_ROOT = build
BUILD_DIR := $(BUILD_DIR_ROOT)
BIN_DIR_ROOT = bin
BIN_DIR := $(BIN_DIR_ROOT)
INSTALL_DIR := ~/Desktop/$(EXEC)

# Sources (searches recursively inside the source directory)
SRC_DIR = src
SRCS := $(sort $(shell find $(SRC_DIR) -name '*.cpp'))

# Includes
INCLUDE_DIR = include
SFML_DIR = libs/SFML-2.4.2
INCLUDES := -I$(INCLUDE_DIR) -idirafter $(SFML_DIR)/include

# C preprocessor settings
CPPFLAGS := -MMD -MP $(INCLUDES)

# C++ compiler settings
CXX = g++
CXXFLAGS = -std=c++14
WARNINGS = -Wall -Wpedantic -Wextra -Wcast-align -Wduplicated-cond -Wextra -Wlogical-op -Wmissing-declarations\
		-Wmissing-include-dirs -Wno-aggressive-loop-optimizations -Wno-unused-parameter -Wnon-virtual-dtor -Wredundant-decls\
		-Wshadow -Wsuggest-override -Wundef -Wunreachable-code -Wuseless-cast -Wzero-as-null-pointer-constant

# Linker settings
LDFLAGS =
LDLIBS =

# XCode project path
XCODE_PROJ := proj/xcode/$(EXEC).xcodeproj

# SFML libraries
SFML_GRAPHICS_LIBS = sfml-graphics
SFML_WINDOW_LIBS = sfml-window
SFML_AUDIO_LIBS = sfml-audio
SFML_NETWORK_LIBS = sfml-network
SFML_SYSTEM_LIBS = sfml-system

# Target OS detection
ifeq ($(OS),Windows_NT) # OS is a preexisting environment variable on Windows
	OS = windows
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Darwin)
		ifeq ($(ios),1)
			OS = ios
		else
			OS = macos
		endif
	else ifeq ($(UNAME),Linux)
		OS = linux
	else
    	$(error OS not supported by this Makefile)
	endif
endif

# OS-specific build and bin directories
BUILD_DIR := $(BUILD_DIR)/make_$(OS)
BIN_DIR := $(BIN_DIR)/$(OS)
ifeq ($(OS),windows)
	# Windows 32-bit
	ifeq ($(win32),1)
		BUILD_DIR := $(BUILD_DIR)32
		BIN_DIR := $(BIN_DIR)32
	# Windows 64-bit
	else
		BUILD_DIR := $(BUILD_DIR)64
		BIN_DIR := $(BIN_DIR)64
	endif
endif

# OS-specific compilation and linking settings
ifeq ($(OS),windows)
	# Add .exe extension to executable
	EXEC := $(EXEC).exe

	# Link everything statically on Windows (including libgcc and libstdc++)
	LDFLAGS += -static

	# Disable console output on release builds
	ifeq ($(release),1)
		LDFLAGS += -mwindows
	endif

	# Link -s SFML libs in release, and -s-d in debug
	ifeq ($(release),1)
		SFML_LIBS_SUFFIX = -s
	else
		SFML_LIBS_SUFFIX = -s-d
	endif

	# Edit SFML lib names with suffix
	SFML_GRAPHICS_LIBS := $(SFML_GRAPHICS_LIBS)$(SFML_LIBS_SUFFIX)
	SFML_WINDOW_LIBS := $(SFML_WINDOW_LIBS)$(SFML_LIBS_SUFFIX)
	SFML_AUDIO_LIBS := $(SFML_AUDIO_LIBS)$(SFML_LIBS_SUFFIX)
	SFML_NETWORK_LIBS := $(SFML_NETWORK_LIBS)$(SFML_LIBS_SUFFIX)
	SFML_SYSTEM_LIBS := $(SFML_SYSTEM_LIBS)$(SFML_LIBS_SUFFIX)

	# Link SFML statically and add the dependencies for statically-linked SFML libraries
	CPPFLAGS += -DSFML_STATIC
	SFML_GRAPHICS_LIBS += freetype jpeg
	SFML_WINDOW_LIBS += opengl32 gdi32
	SFML_AUDIO_LIBS += openal32 flac vorbisenc vorbisfile vorbis ogg
	SFML_NETWORK_LIBS += ws2_32
	SFML_SYSTEM_LIBS += winmm

	# 32-bit flags
	ifeq ($(win32),1)
		CXXFLAGS += -m32
		LDFLAGS += -L$(SFML_DIR)/lib/windows-gcc-6.1.0-mingw-32-bit
	# 64-bit flags
	else
		CXXFLAGS += -m64
		LDFLAGS += -L$(SFML_DIR)/lib/windows-gcc-6.1.0-mingw-64-bit
	endif
else ifeq ($(OS),macos)
	EXEC := $(EXEC).app
	XCODE_SCHEME = macOS
	LDFLAGS += -L/Library/Frameworks
else ifeq ($(OS),ios)
	EXEC := $(EXEC).app
	XCODE_SCHEME = iOS
	LDFLAGS += -L$(SFML_DIR)/lib/ios
endif

# OS-specific assets-copying script selection
ifeq ($(OS),windows)
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
else ifeq ($(OS),linux)
	ifeq ($(release),1)
		COPY_ASSETS_SCRIPT = u_copy_assets_linux_r.sh
	else
		COPY_ASSETS_SCRIPT = u_copy_assets_linux_d.sh
	endif
endif

# Debug (default) and release modes settings
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

# Objects and dependencies
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Include program info and icon on Windows
ifeq ($(OS),Windows_NT)
	OBJS += $(BUILD_DIR)/resource.res
endif

# Add SFML to linked libraries
SFML_GRAPHICS_LIBS := $(addprefix -l,$(SFML_GRAPHICS_LIBS))
SFML_WINDOW_LIBS := $(addprefix -l,$(SFML_WINDOW_LIBS))
SFML_AUDIO_LIBS := $(addprefix -l,$(SFML_AUDIO_LIBS))
SFML_NETWORK_LIBS := $(addprefix -l,$(SFML_NETWORK_LIBS))
SFML_SYSTEM_LIBS := $(addprefix -l,$(SFML_SYSTEM_LIBS))

LDLIBS += $(SFML_GRAPHICS_LIBS) $(SFML_WINDOW_LIBS) $(SFML_AUDIO_LIBS) $(SFML_NETWORK_LIBS) $(SFML_SYSTEM_LIBS)

################################################################################
##### Targets
################################################################################

.PHONY: all
ifneq (,$(filter $(OS),macos ios)) # Build using xcodebuild if target is macOS or iOS
all: xcodebuild
else # Build normally if target is neither macOS nor iOS
all: $(BIN_DIR)/$(EXEC)
endif

# Build executable
$(BIN_DIR)/$(EXEC): $(OBJS)
	@echo "Building executable: $@"
	@mkdir -p $(@D)
	@$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Compile C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNINGS) -c $< -o $@

# Add resource file to Windows executable
$(BUILD_DIR)/%.res: assets/%.rc
	@echo "Compiling Windows resource file"
	@windres $< -O coff -o $@

# Build Xcode project using xcodebuild
.PHONY: xcodebuild
xcodebuild:
	@echo "Building project: $(XCODE_PROJ)"
	@xcodebuild -quiet -project $(XCODE_PROJ) -scheme $(XCODE_SCHEME) build

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
	@echo "Starting program: $(BIN_DIR)/$(EXEC)"
    ifeq ($(OS),macos) # Open app bundle if on macOS
		@open $(BIN_DIR)/$(EXEC)
    else
		@cd ./$(BIN_DIR); ./$(EXEC)
    endif

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
	@clang-format -i $$(find $(SRC_DIR) $(INCLUDE_DIR) -name '*.cpp' -o -name '*.h' -o -name '*.inl')

# Generate documentation with Doxygen
.PHONY: doc
doc:
	@echo "Generating documentation"
	@doxygen docs/Doxyfile

# Print help information
.PHONY: help
help:
	@printf "\
	Usage: make target... [options]...\n\
	\n\
	Targets:\n\
	  all             Build executable (debug mode by default) (default target)\n\
	  install         Install packaged program to desktop (debug mode by default)\n\
	  run             Build and run executable (debug mode by default)\n\
	  copyassets      Copy assets to executable directory for selected platform and configuration\n\
	  clean           Clean build and bin directories (all platforms)\n\
	  cleanassets     Clean assets from executable directories (all platforms)\n\
	  format          Run clang-format on source code\n\
	  doc             Generate documentation with Doxygen\n\
	  help            Print this information\n\
	  printvars       Print Makefile variables for debugging\n\
	\n\
	Options:\n\
	  release=1       Run target using release configuration rather than debug\n\
	  win32=1         Build for 32-bit Windows (valid when built on Windows only)\n\
	  ios=1           Build for iOS (valid when built on macOS only)\n\
	\n\
	Note: the above options affect all, install, run, copyassets, and printvars targets\n"

# Print Makefile variables
.PHONY: printvars
printvars:
	@printf "\
	OS: $(OS)\n\
	EXEC: $(EXEC)\n\
	BUILD_DIR: $(BUILD_DIR)\n\
	BIN_DIR: $(BIN_DIR)\n\
	INSTALL_DIR: $(INSTALL_DIR)\n\
	SRC_DIR: $(SRC_DIR)\n\
	SRCS: $(SRCS)\n\
	INCLUDE_DIR: $(INCLUDE_DIR)\n\
	INCLUDES: $(INCLUDES)\n\
	CXX: $(CXX)\n\
	CPPFLAGS: $(CPPFLAGS)\n\
	CXXFLAGS: $(CXXFLAGS)\n\
	WARNINGS: $(WARNINGS)\n\
	LDFLAGS: $(LDFLAGS)\n\
	LDLIBS: $(LDLIBS)\n\
	COPY_ASSETS_SCRIPT: $(COPY_ASSETS_SCRIPT)\n"
