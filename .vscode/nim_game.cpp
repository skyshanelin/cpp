// File: misere_nim_game.cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

void displayPiles(const vector<int>& piles) {
    cout << "\nCurrent Piles:\n";
    for (size_t i = 0; i < piles.size(); i++) {
        cout << "Pile " << i + 1 << ": " << piles[i] << " stones\n";
    }
}

bool isGameOver(const vector<int>& piles) {
    for (int stones : piles) {
        if (stones > 0) return false;
    }
    return true;
}

void userMove(vector<int>& piles) {
    int pileIndex, stones;
    while (true) {
        cout << "\nYour turn:\n";
        cout << "Choose a pile (1-" << piles.size() << "): ";
        cin >> pileIndex;
        if (pileIndex < 1 || pileIndex > (int)piles.size() || piles[pileIndex - 1] == 0) {
            cout << "Invalid pile. Try again.\n";
            continue;
        }
        cout << "How many stones to remove from pile " << pileIndex << "? ";
        cin >> stones;
        if (stones < 1 || stones > piles[pileIndex - 1]) {
            cout << "Invalid number of stones. Try again.\n";
            continue;
        }
        piles[pileIndex - 1] -= stones;
        break;
    }
}

void computerMove(vector<int>& piles) {
    cout << "\nComputer's turn:\n";
    int idx;
    do {
        idx = rand() % piles.size();
    } while (piles[idx] == 0);
    int stones = 1 + (rand() % piles[idx]);
    piles[idx] -= stones;
    cout << "Computer removes " << stones << " stones from pile " << idx + 1 << ".\n";
}

int main() {
    srand(time(0));

    // Random number of piles between 1 and 7
    int numPiles = 1 + (rand() % 7);
    vector<int> piles(numPiles);

    // Each pile gets between 1 and 10 stones randomly
    for (int i = 0; i < numPiles; i++) {
        piles[i] = 1 + (rand() % 10);
    }

    cout << "Welcome to Misère Nim Game!\n";
    cout << "Rules: Each turn remove one or more stones from a single pile.\n";
    cout << "The player who takes the last stone LOSES.\n";
    cout << "This game starts with " << numPiles << " pile(s).\n";

    while (true) {
        displayPiles(piles);

        // User move
        userMove(piles);
        if (isGameOver(piles)) {
            cout << "\nYou took the last stone. You LOSE!\n";
            cout << "Computer WINS!\n";
            break;
        }

        displayPiles(piles);

        // Computer move
        computerMove(piles);
        if (isGameOver(piles)) {
            cout << "\nComputer took the last stone. Computer LOSES!\n";
            cout << "You WIN!\n";
            break;
        }
    }

    return 0;
}
