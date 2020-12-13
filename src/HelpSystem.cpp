#include "HelpSystem.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>

//----------------------------------------------------------------------------------------------------------------------
// Suchen nach Schlagwörtern in den searchwords der einzelnen Einträhge
void subSearch(Entry *posEntry, std::vector<Entry *> *results, const std::string &searchword) {

    if (!posEntry->is_file) {
        for (Entry &entry : posEntry->sub_entries) {
            subSearch(&entry, results, searchword);
        }
    } else {

        for (auto &keyword : posEntry->searchWords) {
            if (keyword == searchword) {
                results->push_back(posEntry);
            }

        }
    }
}

// wird von der UI aufgerunfen, gibt den Knoten mit von dem aus gesucht werden soll und dem suchbegriff
std::vector<Entry *> searchEntries(Entry *entry, std::string searchword) {
    std::vector<Entry *> searchResults;

    // to lower case, da alle Schlagwörter in den Dateien klein geschrieben sind.
    std::transform(searchword.begin(), searchword.end(), searchword.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    subSearch(entry, &searchResults, searchword);

    return searchResults;
}

//----------------------------------------------------------------------------------------------------------------------
// Einlesen der Dateien, geteilt in die erste Zeile in der die Schlagwörter stehen und den Rest der Datei in dem der Text steht
void readFile(const std::string &filePath, Entry *entry) {
    std::string line;
    std::ifstream file;
    file.open(filePath);

    std::string contend;
    std::string firstLine;

    //Topic 9: Kontrollfluß: grundlegende Kontrollstrukturen
    if (file.is_open()) {
        //in der ersten Zeile stehen die Schlagwörter nach denen gesucht wird.
        std::getline(file, firstLine);
        while (std::getline(file, line)) {
            contend.append(line + "\n");
        }
        file.close();
    }
    entry->content = contend;


    // Split first line, save Words in Vector
    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;

    // \r am ende entfernen, damit dies ohne \r gespeichert wird
    firstLine.erase(std::remove(firstLine.begin(), firstLine.end(), '\r'), firstLine.end());

    while ((pos = firstLine.find(delimiter)) != std::string::npos) {
        token = firstLine.substr(0, pos);

        entry->searchWords.push_back(token);

        //std::cout << token << std::endl;
        firstLine.erase(0, pos + delimiter.length());
    }
    //letztes SearchWord wird noch hinzugefügt
    entry->searchWords.push_back(firstLine);
}

//----------------------------------------------------------------------------------------------------------------------
// Rekursive Traversion durch die Ordner Struktur

void traverseFolders(const std::string &path, Entry *parent) {
    for (auto &p: std::filesystem::directory_iterator(path)) {
        //std::cout << p.path() << std::endl;
        std::string s = p.path();

        std::string last_element(s.substr(s.rfind('/') + 1));

        auto *entry = new Entry();

        if (p.is_directory()) {
            entry->is_file = false;
            entry->name = last_element;
            entry->sub_entries = std::vector<Entry>();

            traverseFolders(p.path(), entry);
        } else {
            readFile(p.path(), entry);
            entry->is_file = true;
            entry->name = last_element;

        }
        parent->sub_entries.push_back(*entry);
    }
}

//----------------------------------------------------------------------------------------------------------------------

Entry initHelpSystem(const std::string &path) {
    //Pfad zum root Ordner des HilfeSystems
    Entry baseDirectory;
    // is_file muss gesetzt werden damit das DateiSystem indexing funktioniert
    baseDirectory.is_file = false;
    //einlesen des Dateisystems
    traverseFolders(path, &baseDirectory);
    return baseDirectory;
}

