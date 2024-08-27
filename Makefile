TARGETDIR = bin
TARGET = DominoGame

CC = g++

SRCDIR = src
OBJDIR = bin-int

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCES = $(call rwildcard, $(SRCDIR),*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

SFML = vendor\SFML-2.6.1
SFML_GRAPHICS = "$(SFML)\bin\sfml-graphics-2.dll"
SFML_SYSTEM = "$(SFML)\bin\sfml-system-2.dll"
SFML_WINDOW = "$(SFML)\bin\sfml-window-2.dll"
SFML_NETWORK = "$(SFML)\bin\sfml-network-2.dll"

CCFLAGS = -std=c++17 -Wall -I$(SFML)/include -I$(SRCDIR) -D_DEBUG
LDFLAGS = -L$(SFML)\lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lgdi32 -lopengl32 -lfreetype -lwinmm -lws2_32

all: $(TARGET)

debug: CCFLAGS += -g
debug: LDFLAGS += -g
debug: $(TARGET)

recompile: clean $(TARGET)

$(TARGET): $(OBJECTS)
	@if not exist $(TARGETDIR) mkdir $(TARGETDIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGETDIR)/$@
	@copy $(SFML_GRAPHICS) "$(TARGETDIR)\"
	@copy $(SFML_SYSTEM) "$(TARGETDIR)\"
	@copy $(SFML_WINDOW) "$(TARGETDIR)\"
	@copy $(SFML_NETWORK) "$(TARGETDIR)\"


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(CCFLAGS) -c -o $@ $<

clean:
	@powershell -Command "if (Test-Path -Path $(TARGETDIR)) { Remove-Item -Recurse -Force $(TARGETDIR) }"
	@powershell -Command "if (Test-Path -Path $(OBJDIR)) { Remove-Item -Recurse -Force $(OBJDIR) }"

.PHONY: all clean