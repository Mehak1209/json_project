#include <iostream>
#include <fstream>
#include <unordered_map>
#include "json.hpp"  // Include the JSON library

using json = nlohmann::json;
using namespace std;

// Structure to hold location data
struct Location {
    string id;
    double latitude;
    double longitude;
};

// Structure to hold metadata
struct Metadata {
    string id;
    string type;
    double rating;
    int reviews;
};

int main() {
    // Read JSON files
    ifstream locationsFile("locations.json");
    ifstream metadataFile("metadata.json");

    if (!locationsFile || !metadataFile) {
        cerr << "Error: Could not open one of the JSON files!" << endl;
        return 1;
    }

    json locationsJson, metadataJson;
    locationsFile >> locationsJson;
    metadataFile >> metadataJson;

    unordered_map<string, Location> locations;
    unordered_map<string, Metadata> metadata;

    // Parse locations JSON
    for (const auto& loc : locationsJson) {
        locations[loc["id"]] = {loc["id"], loc["latitude"], loc["longitude"]};
    }

    // Parse metadata JSON
    for (const auto& meta : metadataJson) {
        metadata[meta["id"]] = {meta["id"], meta["type"], meta["rating"], meta["reviews"]};
    }

    // Merging data
    unordered_map<string, int> typeCount;
    unordered_map<string, pair<double, int>> ratingSum;
    Metadata maxReviews = {"", "", 0.0, 0};
    vector<string> incompleteData;

    for (const auto& [id, meta] : metadata) {
        if (locations.find(id) != locations.end()) {
            typeCount[meta.type]++;
            ratingSum[meta.type].first += meta.rating;
            ratingSum[meta.type].second++;

            if (meta.reviews > maxReviews.reviews) {
                maxReviews = meta;
            }
        } else {
            incompleteData.push_back(id);
        }
    }

    // Display results
    cout << "Valid Locations Count by Type:\n";
    for (const auto& [type, count] : typeCount) {
        cout << type << ": " << count << "\n";
    }

    cout << "\nAverage Rating per Type:\n";
    for (const auto& [type, ratingData] : ratingSum) {
        cout << type << ": " << (ratingData.first / ratingData.second) << "\n";
    }

    cout << "\nLocation with Highest Reviews:\n";
    cout << "ID: " << maxReviews.id << ", Reviews: " << maxReviews.reviews << "\n";

    if (!incompleteData.empty()) {
        cout << "\nIncomplete Data Entries:\n";
        for (const auto& id : incompleteData) {
            cout << id << "\n";
        }
    }

    return 0;
}
