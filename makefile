# Directorios de origen y destino
SRC_DIR := src
BIN_DIR := bin

SFML := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SFML_BOX2D := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d

# Archivos fuente del juego principal
GAME_SOURCES := $(SRC_DIR)/RedBall.cpp $(SRC_DIR)/Platform.cpp $(SRC_DIR)/GameWorld.cpp $(SRC_DIR)/PlayerScore.cpp $(SRC_DIR)/Utils.cpp

# Obtener todos los archivos .cpp en el directorio de origen
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# Generar los nombres de los archivos .exe en el directorio de destino
EXE_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.exe,$(CPP_FILES))

# Regla especial para archivos que usan Box2D (07_Fisica.cpp y 08_RedBallGame.cpp)
$(BIN_DIR)/07_Fisica.exe: $(SRC_DIR)/07_Fisica.cpp
	g++ $< -o $@ $(SFML_BOX2D) -Iinclude

$(BIN_DIR)/08_RedBallGame.exe: $(SRC_DIR)/08_RedBallGame.cpp
	g++ $< -o $@ $(SFML_BOX2D) -Iinclude

# Regla para compilar RedBall.cpp con sus dependencias
$(BIN_DIR)/RedBall.exe: $(GAME_SOURCES)
	g++ $(GAME_SOURCES) -o $@ $(SFML) -Iinclude

# Excluir los archivos auxiliares de compilación individual
$(BIN_DIR)/Platform.exe:
	@echo "Platform.cpp es un archivo auxiliar, no se compila individualmente"

$(BIN_DIR)/GameWorld.exe:
	@echo "GameWorld.cpp es un archivo auxiliar, no se compila individualmente"

$(BIN_DIR)/PlayerScore.exe:
	@echo "PlayerScore.cpp es un archivo auxiliar, no se compila individualmente"

$(BIN_DIR)/Utils.exe:
	@echo "Utils.cpp es un archivo auxiliar, no se compila individualmente"

$(BIN_DIR)/RedBall_backup.exe:
	@echo "RedBall_backup.cpp es un archivo de respaldo, no se compila"

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
