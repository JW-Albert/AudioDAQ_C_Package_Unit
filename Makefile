CC = g++
CFLAGS = -Wall -std=c++17 -Iinclude
LDFLAGS = -lasound
TARGET = main
SOURCES = main.cpp include/AudioDAQ.cpp include/CSVWriter.cpp include/iniReader/INIReader.cpp include/iniReader/ini.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
