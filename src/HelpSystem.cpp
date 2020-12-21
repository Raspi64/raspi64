#include "HelpSystem.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
// ______________________
// | Funktionen für die Suche |
//----------------------------------------------------------------------------------------------------------------------
// Suchen nach Schlagwörtern in den searchwords der einzelnen Einträge.
// die Funktion ruft sich so lange wieder selbst auf, bis Sie in den Ordnern eine unter Ordner mehr findet.
// bei der Suche wird zwischen dem Typ Ordner und Datei Unterschieden.
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

// searchEntries wird aufgerufen und es muss der BaseEntry und ein Suchbegriff mit übergeben werden.
std::vector<Entry *> searchEntries(Entry *entry, std::string searchword) {

    std::vector<Entry *> searchResults;

    // toLowerCase, da alle Schlagwörter in den Datein klein Geschrieben sind
    std::transform(searchword.begin(), searchword.end(), searchword.begin(), [](unsigned char c) { return std::tolower(c); });

    //aufrufen der Rekursiven Suche durch die Baumstruktur aus Entrys.
    //der Entry wird als Startpunkt mit übergeben.
    //die Searchresults enthalten am Ende das Ergebniss der Suche.
    subSearch(entry, &searchResults, searchword);

    return searchResults;
}

// _______________________________________________________
// | Funktionen für das Inintialisieren des Hilfesystems |
// ----------------------------------------------------------------------------------------------------------------------
// Einlesen der Dateien, geteilt in die erste Zeile in der die Schlagwörter stehen und den Rest der Datei in dem der Text steht.
void readFile(const std::string &filePath, Entry *entry) {
    // line für das kurze zwischen Speichern der eingelesenen Zeile.
    std::string line;
    // Filesystem Reader init, mit übergabe des Pfades, zu dem Root Folder.
    std::ifstream file;
    file.open(filePath);

    // firstline enthält später die Schlagwörter nach denen gesucht werden kann.
    // content enthält den Rest der Datei.
    std::string content;
    std::string firstLine;

    // hier wird die Datei eingelesen und der Inhalt zwischen gespeichert.
    if (file.is_open()) {
        // die erste Zeile mit den Schlagwörtern
        std::getline(file, firstLine);
        // in der while Schleife wird der Restliche Text der Datei zwischengespeichert.
        while (std::getline(file, line)) {
            content.append(line + "\n");
        }
        file.close();
    }

    // Der Text wird hier in das Entry übertragen.
    entry->content = content;


    // Hier werden die Schlagwörter nach denen später gesucht werden soll, gespalten und in einen Vector gespeichert.
    // Der delimiter gibt an, mit welchem Zeichen die Schlagwörter getrennt werden.
    std::string delimiter = ";";
    size_t pos = 0;

    // \r am ende entfernen, damit dies ohne \r gespeichert werden kann
    firstLine.erase(std::remove(firstLine.begin(), firstLine.end(), '\r'), firstLine.end());

    // Die While Schleife geht Delimiter für Delimiter durch den String
    while ((pos = firstLine.find(delimiter)) != std::string::npos) {

        // Das aktuelle Schlagwort, wird dem Vector an möglichen Suchbegriffen hinzugefügt.
        entry->searchWords.push_back(firstLine.substr(0, pos));

        // Das vorher eingefügte Wort, wird aus dem String firstline entfernt.
        firstLine.erase(0, pos + delimiter.length());
    }
    // Das letze Schlagwort, welches keinen Delimiter am Ende hat, wird dem Vector angefügt.
    entry->searchWords.push_back(firstLine);
}

// Rekursive Traversion durch die Ordner Struktur

void traverseFolders(const std::string &path, Entry *parent) {
    // ließt die Daten aus dem Dateisystem ein und geht diese dann durch.
    for (auto &child: std::filesystem::directory_iterator(path)) {

        // Der Pfad der Datei wird so abgeschnitten, das nur noch der Name der Datei übrig bleibt.
        std::string child_path = child.path();
        std::string last_element(child_path.substr(child_path.rfind('/') + 1));

        // Hier wird das Child Entry erstellt, was befüllt wird und dann dem Parent Entry hinzugefügt wird.
        auto *entry = new Entry();

        // Wenn eine Datei, ein Ordner ist, so wird dieser als dieses Markiert und mit einem vector für Datein und unter Ordner versehen.
        // Als name wird last_element angegeben, was den Vorher aus dem Pfad extrahierten Dateinamen enthält.
        // Wenn eine Datei kein Ordner ist, so wird Sie als solches gespeichert, als solche Markiert und mit dem Namen versehen.
        if (child.is_directory()) {
            entry->is_file = false;
            entry->name = last_element;
            entry->sub_entries = std::vector<Entry>();

            traverseFolders(child_path, entry);
        } else {
            readFile(child_path, entry);
            entry->is_file = true;
            entry->name = last_element;

        }
        // Fügt die erstellten Entrys, dem Base oder auch Root Entry an, so entsteht die Baum Struktur.
        parent->sub_entries.push_back(*entry);
    }
}

// in der init wird das einlesen der Ordner Struktur mit den Darin befindlichen Test Datein aufgerufen.
Entry initHelpSystem(const std::string &path) {
    //Root Entry für die Später darran anknüpfende Baum Struktur.
    Entry baseDirectory;
    // is_file muss gesetzt werden damit das DateiSystem traversieren Funktioniert, sonst kann der Root Folder nicht als Folder erkannt werden.
    baseDirectory.is_file = false;
    //einlesen des Dateisystems
    traverseFolders(path, &baseDirectory);

    return baseDirectory;
}

