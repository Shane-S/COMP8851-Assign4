#include <iostream>
#include <cstring>
#include <vector>
#include <unordered_set>
#include "MurmurHash2.h"

typedef struct match {
    int p1;
    int p2;

    bool operator==(const match& rhs) const {
        return p1 == rhs.p1 && p2 == rhs.p2;
    }
} match;

namespace std {
    template<>
    struct hash<match> {
        size_t operator()(const match &m) const
        {
            size_t seed = 2166136261;
            return MurmurHash64((void*)&m, sizeof(m), seed);
        }
    };
}

void PlaceMatch(std::vector<std::unordered_set<int>> &days, std::vector<std::vector<match>>& matches, match& m) {
    int day_num = 0;
    for (auto & day : days) {
        if (day.find(m.p1) == day.end() && day.find(m.p2) == day.end()) {
            day.insert(m.p1);
            day.insert(m.p2);
            matches[day_num].push_back(m);
            break;
        }
        ++day_num;
    }
}

void RoundRobinWorker(std::vector<std::unordered_set<int>> &days, std::vector<std::vector<match>> &matches, int n, int first_player) {
    // Base case; find a day we can insert this in
    if (n == 2) {
        match m = { first_player, first_player + 1 };
        PlaceMatch(days, matches, m);
    } else {
        // Otherwise, subdivide and then have all players in one half play all in the other
        int half = n >> 1;
        RoundRobinWorker(days, matches, half, first_player);
        RoundRobinWorker(days, matches, half, first_player + half);

        for (int i = 0; i < half; i++) {
            for (int j = half; j < n; j++) {
                match m = { i + first_player, j + first_player };
                PlaceMatch(days, matches, m);
            }
        }
    }
}

// Precondition: k > 0
std::vector<std::vector<match>> RoundRobin(int k) {
    int n = 1 << k;
    int* players = new int[n];
    for (int i = 0; i < n; i++) {
        players[i] = i;
    }

    std::vector<std::unordered_set<int>> days(n - 1);
    std::vector<std::vector<match>> matches(n - 1);
    RoundRobinWorker(days, matches, n, 0);
    return matches;
}

int main() {
    int k = 3;
    std::vector<std::vector<match>> schedule = RoundRobin(k);
    
    int day_num = 1;
    std::cout << "k = " << k << std::endl << std::endl;

    for (auto & day : schedule) {
        std::cout << "Day " << day_num++ << std::endl;

        for (auto & m : day) {
            std::cout << "Player " << m.p1 << " vs. Player " << m.p2 << std::endl;
        }
        std::cout << std::endl;
    }
    std::cin.get();
    return 0;
}