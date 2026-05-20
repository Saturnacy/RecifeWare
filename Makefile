# Configurações de Nome e Caminhos
APP_NAME = RecifeWare
SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
BUILD_DIR = build

# Detecção de Sistema Operacional
ifeq ($(OS),Windows_NT)
    PLATFORM = WINDOWS
    CC = gcc
    LDFLAGS = -L$(LIB_DIR)/win -lraylib -lopengl32 -lgdi32 -lwinmm
    EXT = .exe
    MKDIR = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    RM = rmdir /S /Q $(BUILD_DIR)
else
    PLATFORM = LINUX
    CC = gcc
    LDFLAGS = -L$(LIB_DIR)/linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    EXT =
    MKDIR = mkdir -p $(BUILD_DIR)
    RM = rm -rf $(BUILD_DIR)
endif

# Adicionando as subpastas ao escopo de inclusão (Include Paths)
CFLAGS = -I$(INC_DIR) -I$(SRC_DIR)/core -I$(SRC_DIR)/games -Wall -std=c99 -Wno-missing-braces

# Busca arquivos .c no src
SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)

# Define o caminho final do executável
TARGET = $(BUILD_DIR)/$(APP_NAME)$(EXT)

# Regra principal
all: $(TARGET)

# Como construir o executável
$(TARGET): $(SOURCES)
    $(MKDIR)
    $(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Limpa a pasta build
clean:
    $(RM)

# Roda o jogo
run: all
    ./$(TARGET)