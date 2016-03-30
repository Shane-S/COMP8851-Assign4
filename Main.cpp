#include <iostream>
#include <vector>

struct match {
    int p1;
    int p2;
};

void RoundRobinWorker(std::vector<std::vector<match>> &matches, int n, int first_player) {
	if (n == 2) {
		// Base case: everybody plays their neighbour on day 0
		match m = { first_player, first_player + 1 };
		matches[0].push_back(m);
	} else {
		unsigned half = n / 2;

		RoundRobinWorker(matches, half, first_player);
		RoundRobinWorker(matches, half, first_player + half);

		for (unsigned i = 0; i < half; ++i) {
			for (unsigned j = half; j < n; ++j) {
				match m = { i + first_player, j + first_player };

				unsigned day = ((i + j) % half) + half - 1;
				matches[day].push_back(m);
			}
		}
	}
}

// Precondition: k > 0
std::vector<std::vector<match>> RoundRobin(int k) {
    int n = 1 << k;

    std::vector<std::vector<match>> matches(n - 1);
    RoundRobinWorker(matches, n, 0);
    return matches;
}

int main() {
    int k = 4;
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