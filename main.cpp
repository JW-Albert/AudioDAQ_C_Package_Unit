// main.cpp
#include "./include/AudioDAQ.h"
#include "./include/CSVWriter.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <cstring>
#include <atomic>
#include "./include/iniReader/INIReader.h"
extern "C" {
#include "./include/iniReader/ini.h"
}

using namespace std;

int main ( void ) {
    const string iniFilePath = "API/Master.ini";
    INIReader reader(iniFilePath);

    if (reader.ParseError() < 0) {
        cerr << "Cannot load INI file: " << iniFilePath << endl;
        return 1;
    }

    // A map used to store INI data.
    map<string, map<string, string>> ini_data;

    vector<string> sections = {"SaveUnit"};
    for (const string& section : sections) {
        if (reader.HasSection(section)) {
            vector<string> keys = {"second", "first"};
            for (const string& key : keys) {
                if (reader.HasValue(section, key)) {
                    ini_data[section][key] = reader.Get(section, key, "");
                }
            }
        }
    }

    // Target section and key.
    const string targetSection = "SaveUnit";
    const string targetKey = "second";
    int SaveUnit = 0;

     if (ini_data.find(targetSection) != ini_data.end() &&
        ini_data[targetSection].find(targetKey) != ini_data[targetSection].end()) {
        string value = ini_data[targetSection][targetKey];
        SaveUnit = stoi(value);
        cout << "[" << targetSection << "] " << targetKey << " = " << value << endl;
    } else {
        cerr << "Cannot find section: " << targetSection << " or key: " << targetKey << endl;
        return 1;
    }

    // Initialize NiDAQHandler and AudioDAQ instances for data acquisition
    AudioDAQ audioDaq_1;
    AudioDAQ audioDaq_2;

    // Path to configuration files for hardware initialization
    const char* audioDaq_1configPath = "API/AudioDAQ_1.ini";
    const char* audioDaq_2configPath = "API/AudioDAQ_2.ini";
    
    // Initialize DAQ task using configuration file
    audioDaq_1.initDevices(audioDaq_1configPath);
    audioDaq_2.initDevices(audioDaq_2configPath);

    // Validate initialization, ensure sample rate and channel count are valid
    if (audioDaq_1.getSampleRate() <= 0) {
        cerr << "AudioDAQ_1 Initialization failed, unable to initialize hardware." << endl;
        return 1; // Exit on initialization error
    }
    if (audioDaq_2.getSampleRate() <= 0) {
        cerr << "AudioDAQ_2 Initialization failed, unable to initialize hardware." << endl;
        return 1; // Exit on initialization error
    }

    // Initialize CSVWriter for logging data from AudioDAQ
    CSVWriter audioDaq_1csv(1, "output/AudioDAQ_1/");
    CSVWriter audioDaq_2csv(1, "output/AudioDAQ_2/");

    // Start audio data capture
    audioDaq_1.startCapture();
    audioDaq_2.startCapture();

    cout << "Start reading data, press Ctrl+C to terminate the program." << endl;

    // Variables to track loop iterations and data updates
    int audioDaq_1Timer = 0;
    int audioDaq_1tmpTimer = 0; // Tracks last data read times for AudioDAQ
    int audioDaq_1saveCounter = 0; // Counter for saving data to CSV

    int audioDaq_2Timer = 0;
    int audioDaq_2tmpTimer = 0; // Tracks last data read times for AudioDAQ
    int audioDaq_2saveCounter = 0; // Counter for saving data to CSV

    while (true) {
        // Check if new data is available from AudioDAQ
        int audioDaq_1tmpTimes = audioDaq_1.getTimes();
        if (audioDaq_1tmpTimes > audioDaq_1tmpTimer) {
            audioDaq_1Timer++;

            cout << "audioDaq_1 Program Count: " << audioDaq_1Timer << endl;
            cout << "audioDaq_1 Package Count: " << audioDaq_1tmpTimes << endl;

            // Retrieve the latest audio buffer and log to CSV
            auto buffer = audioDaq_1.getBuffer();
            audioDaq_1csv.addDataBlock(move(buffer));

            // Update read times
            audioDaq_1tmpTimer = audioDaq_1tmpTimes;
            audioDaq_1saveCounter++;

            if (audioDaq_1saveCounter == SaveUnit) {
                // Save data to CSV file
                audioDaq_1csv.saveToCSV();
                audioDaq_1saveCounter = 0;
            }
        }

        int audioDaq_2tmpTimes = audioDaq_2.getTimes();
        if (audioDaq_2tmpTimes > audioDaq_2tmpTimer) {
            audioDaq_2Timer++;

            cout << "audioDaq_2 Program Count: " << audioDaq_2Timer << endl;
            cout << "audioDaq_2 Package Count: " << audioDaq_2tmpTimes << endl;

            // Retrieve the latest audio buffer and log to CSV
            auto buffer = audioDaq_2.getBuffer();
            audioDaq_2csv.addDataBlock(move(buffer));

            // Update read times
            audioDaq_2tmpTimer = audioDaq_2tmpTimes;
            audioDaq_2saveCounter++;

            if (audioDaq_2saveCounter == SaveUnit) {
                // Save data to CSV file
                audioDaq_2csv.saveToCSV();
                audioDaq_2saveCounter = 0;
            }
        }
    }

    // Stop and clean up all tasks and resources
    audioDaq_1.stopCapture();
    audioDaq_2.stopCapture();

    return 0;
}
