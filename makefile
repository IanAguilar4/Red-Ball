# Directorios de origen y destino
SRC_DIR := src
BIN_DIR := bin

SFML := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SFML_BOX2D := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d

# Obtener todos los archivos .cpp en el directorio de origen
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# Generar los nombres de los archivos .exe en el directorio de destino
EXE_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.exe,$(CPP_FILES))

# Regla especial para archivos que usan Box2D (07_Fisica.cpp y 08_RedBallGame.cpp)
$(BIN_DIR)/07_Fisica.exe: $(SRC_DIR)/07_Fisica.cpp
	g++ $< -o $@ $(SFML_BOX2D) -Iinclude

$(BIN_DIR)/08_RedBallGame.exe: $(SRC_DIR)/08_RedBallGame.cpp
	g++ $< -o $@ $(SFML_BOX2D) -Iinclude

# Regla para compilar RedBall.cpp sin Box2D
$(BIN_DIR)/RedBall.exe: $(SRC_DIR)/RedBall.cpp
	g++ $< -o $@ $(SFML) -Iinclude

# Regla para compilar los demás archivos .cpp sin Box2D
$(BIN_DIR)/%.exe: $(SRC_DIR)/%.cpp
	g++ $< -o $@ $(SFML) -Iinclude

# Regla por defecto para compilar todos los archivos .cpp
all: $(EXE_FILES)

# Regla para ejecutar cada archivo .exe
run%: $(BIN_DIR)/%.exe
	./$<

# Regla para limpiar los archivos generados
clean:
	rm -f $(EXE_FILES)

.PHONY: all clean
.PHONY: run-%
