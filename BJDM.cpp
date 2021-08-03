// Code by Cevat Sonmez Yucel
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <set>

// can possibly cause a namespace collision; should be removed 
// if adjustments to code are meant to be made
using namespace std; 


// Function to calculate probability of busting for the given turn
float calculateProbability(vector<int> deck, int playerTotal, int numberOfAces) {
    int cardsLeft = 0;
    for (int i = 1; i <= 10; i++) {
        cardsLeft += deck[i];
    }
    int minToBust = 22 - playerTotal;
    int willBustAmount = 0;
    for (int i = minToBust; i <= 10; i++) {
        willBustAmount += deck[i];
    }
    return willBustAmount / float(cardsLeft);
}

// Takes in the card's value as a string and converts it to its value in the game
// Returns the value as an integer
int convertInput(string input) {
    int toReturn;
    // Checking for edge case: Ace
    if (input == "A") {
        toReturn = 1;
    }
    // Checking for edge cases: Joker, Queen, and King
    else if (input == "J" or input == "Q" or input == "K") {
        toReturn = 10;
    }
    else {
        toReturn = stoi(input);
    }   
    return toReturn;
}

// Randomly pick the dealer's unknown card from the remaining deck
int unknownCard(vector<int> deck) {
    vector<int> pickFrom;
    random_device rd;
    mt19937 g(rd()); //create pseudo-random number-generator

    // create a vector representing the deck
    for (int i = 1; i <= 10; i++) {
        int amountToInsert = deck[i];
        for (int j = 0; j < i; j++) {
            pickFrom.push_back(i);
        }
    }

    shuffle(pickFrom.begin(), pickFrom.end(), g); //pseudo-randomly shuffles the vector representing the deck
    return pickFrom[0];
}

int main() {
    cout << "------------------------------------\n";
    cout << "Welcome to Blackjack Decision Maker\n";
    cout << "------------------------------------\n";

    set<string> validInput{"A", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    // vector used as a dictionary with the value at any index representing how many cards 
    // of the index's value are in the deck (if curDeck[1] = 4, there are 4 cards in deck of value 1)
    vector<int> curDeck{0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 16};

    int aceInPlayerDeck = 0; // keeps track of number of aces in player's deck
    int aceInDealerDeck = 0; // keeps track of number of aces in dealer's deck
    cout << "Enter your beginning hand\n";
    cout << "------------------------------------\n";
    string input1; // player's known card1
    string input2; // player's known card2
    cout << "Card 1: ";
    cin >> input1;
    cout << "------------------------------------\n";

    // Checking for valid input
    while (validInput.find(input1) == validInput.end()) {
        cout << "Invalid input, please enter Card 1 again: ";
        cin >> input1;
        cout << "------------------------------------\n";
    }
    cout << "Card 2: ";
    cin >> input2;
    cout << "------------------------------------\n";

    // Checking for valid input
    while (validInput.find(input2) == validInput.end()) {
        cout << "Invalid input, please enter Card 2 again: ";
        cin >> input2;
        cout << "------------------------------------\n";
    }
    int input1Int = convertInput(input1);
    int input2Int = convertInput(input2);

    curDeck[input1Int]--; // adjusting how many of dealed card is in the deck
    curDeck[input2Int]--; // adjusting how many of dealed card is in the deck

    if (input1Int == 1) {
        aceInPlayerDeck++;
    }
    if (input2Int == 1) {
        aceInPlayerDeck++;
    }

    string input3; // dealer's known card
    cout << "Enter the known card of the dealer: ";
    cin >> input3;
    cout << "------------------------------------\n";

    // Checking for valid input
    while (validInput.find(input3) == validInput.end()) {
        cout << "Invalid input, please enter known card of dealer again: ";
        cin >> input3;
        cout << "------------------------------------\n";
    }

    int input3Int = convertInput(input3);
    curDeck[input3Int]--; // adjusting how many of dealed card is in the deck

    if (input3Int == 1) {
        aceInDealerDeck++;
    }

    int curPlayerTotal = input1Int + input2Int; // calculating minimum value of player's hand

    int guessDealerCard = unknownCard(curDeck);
    curDeck[guessDealerCard]--; // adjusting how many of guessed card is in deck

    while (true) {
        float bustProb = calculateProbability(curDeck, curPlayerTotal, aceInPlayerDeck);
        cout << "Probability of busting: " << bustProb * 100 << "%\n";
        cout << "------------------------------------\n";
        string decisionInput;
        cout << "Hit or Stand (h or s)? ";
        cin >> decisionInput;
        cout << "------------------------------------\n";

        // Checking for valid input
        while (decisionInput != "h" and decisionInput != "s") {
            cout << "Please enter either 'h' or 's'";
            cin >> decisionInput;
            cout << "------------------------------------\n";
        }

        if (decisionInput == "s") {
            cout << "Decisions completed\n";
            cout << "------------------------------------\n";
            break;
        }
        if (decisionInput == "h") {
            string cardDrawn;
            cout << "Please enter the card that was drawn: ";
            cin >> cardDrawn;
            cout << "------------------------------------\n";

            // Checking for valid input
            while (validInput.find(cardDrawn) == validInput.end()) {
                cout << "Not a valid card, please enter card that was drawn again: ";
                cin >> cardDrawn;
                cout << "------------------------------------\n";
            }

            int cardDrawnInt = convertInput(cardDrawn);
            curDeck[cardDrawnInt]--;

            // If the card guessed for the unknown card of the dealer is known to be wrong, another card is guessed.
            if (curDeck[cardDrawnInt] < 0) {
                curDeck[cardDrawnInt]++;
                curDeck[unknownCard(curDeck)]--;
            }
            if (cardDrawnInt == 1) {
                aceInPlayerDeck++;
            }
            curPlayerTotal += cardDrawnInt; // adjusting minumum value of player's hand

            if (curPlayerTotal == 21) {
                cout << "BLACKJACK!\n";
                cout << "------------------------------------\n";
                break;
            }
            
            if (curPlayerTotal > 21) {
                cout << "BUST\n";
                cout << "------------------------------------\n";
                break;
            }
        }
    }
    return 0;
}