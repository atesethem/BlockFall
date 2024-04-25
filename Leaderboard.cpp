
#include "Leaderboard.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#define _CRT_SECURE_NO_WARNINGS

void Leaderboard::insert_new_entry(LeaderboardEntry* new_entry) {
    if (!head_leaderboard_entry) {
        head_leaderboard_entry = new_entry;
    }
    else {
        LeaderboardEntry* current = head_leaderboard_entry;
        LeaderboardEntry* prev = nullptr;

        // Find the correct position to insert the new entry
        while (current && current->score >= new_entry->score) {
            prev = current;
            current = current->next_leaderboard_entry;
        }

        // Insert at the beginning
        if (!prev) {
            new_entry->next_leaderboard_entry = head_leaderboard_entry;
            head_leaderboard_entry = new_entry;
        }
        else {
            prev->next_leaderboard_entry = new_entry;
            new_entry->next_leaderboard_entry = current;
        }
    }

    // Trim excess entries, keep only top 10
    LeaderboardEntry* current = head_leaderboard_entry;
    LeaderboardEntry* prev = nullptr;
    int count = 0;
    while (current && count < 10) {
        ++count;
        prev = current;
        current = current->next_leaderboard_entry;
    }

    if (current) {
        prev->next_leaderboard_entry = nullptr;
        while (current) {
            LeaderboardEntry* temp = current;
            current = current->next_leaderboard_entry;
            delete temp;
        }
    }
}

void Leaderboard::write_to_file(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file" << endl;
        return;
    }

    LeaderboardEntry* current = head_leaderboard_entry;
    while (current) {
        file << current->score << " " << current->last_played << " " << current->player_name << endl;
        current = current->next_leaderboard_entry;
    }
    file.close();
}

void Leaderboard::read_from_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file" << endl;
        return;
    }

    LeaderboardEntry* current = head_leaderboard_entry;
    LeaderboardEntry* prev = nullptr;
    while (file && !file.eof()) {
        unsigned long score;
        time_t last_played;
        string player_name;
        file >> score >> last_played;
        file.ignore(); // Ignore space
        getline(file, player_name);

        LeaderboardEntry* new_entry = new LeaderboardEntry(score, last_played, player_name);
        if (!head_leaderboard_entry) {
            head_leaderboard_entry = new_entry;
        }
        else {
            prev->next_leaderboard_entry = new_entry;
        }
        prev = new_entry;
    }
    file.close();
}

void Leaderboard::print_leaderboard() {
    cout << "Leaderboard" << endl;
    cout << "-----------" << endl;

    LeaderboardEntry* current = head_leaderboard_entry;
    int count = 1;
    while (current) {
        struct tm currentTime;
#ifdef _WIN32 
        localtime_s(&currentTime, &current->last_played);
#else 
        localtime_r(&current->last_played, &currentTime);
#endif

        char timeStr[30];
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S/%d.%m.%Y", &currentTime);

        cout << count << ". " << current->player_name << " " << current->score << " "
            << timeStr << endl;
        current = current->next_leaderboard_entry;
        ++count;
    }
}

Leaderboard::~Leaderboard() {
    LeaderboardEntry* current = head_leaderboard_entry;
    while (current) {
        LeaderboardEntry* temp = current;
        current = current->next_leaderboard_entry;
        delete temp;
    }
}

