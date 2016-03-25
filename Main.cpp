#include <iostream>
#include <cstring>
#include <vector>
#include <unordered_set>

typedef struct match {
    int p1;
    int p2;

    bool operator==(const match& rhs) const {
        return p1 == rhs.p1 && p2 == rhs.p2;
    }
} match;

void PlaceMatch(std::vector<std::unordered_set<int>> &days, std::vector<std::vector<match>>& matches, match& m) {
    int day_num = 0;
    for (auto & day : days) {
        // Each day has a corresponding set of players and list of matches
        // The set contains which players have played that day (so that another day can be chosen if either player has already played)
        // It's probably overkill, but whatever
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
    // Base case; make a match with these two players and insert it in the next available day
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