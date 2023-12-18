/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/*
 * File:   main.cpp
 * Author: alvin
 *
 * Created on November 10, 2023, 10:46 PM
 */

#include <cstdlib>

using namespace std;

#include <ostream>
#include<iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <stack>
#include <queue>
#include <list>
#include <algorithm>
#include <unordered_map>
#include <vector>
using namespace std;

/**
 * Project Title: Simple UNO Game Simulation
 *
 * Description:
 * This program simulates a simple UNO card game. UNO is a popular
 * shedding-type card game that is played with a specially printed deck.
 * The deck consists of cards of different colors and values, including
 * special cards that introduce unique actions.
 *
 * Rules:
 * - The game is played with a deck of 108 cards, divided into four colors:
 *   red, green, blue, and yellow. Each color has cards numbered from 0 to 9,
 *   along with special action cards such as "Draw Two," "Skip," and "Reverse."
 * - Additionally, there are two types of Wild cards: "Wild" and "Wild Draw Four."
 * - Players take turns playing a card that matches the top card of the discard
 *   pile in either number or color.
 * - Special action cards have unique effects, like forcing the next player to draw cards,
 *   skipping a player's turn, or reversing the order of play.
 * - Wild cards allow the player to choose the next color.
 * - The first player to play all their cards wins the game.
 *
 * Mechanism:
 * The program models the UNO game with a deck of cards and provides basic
 * functionality such as shuffling the deck, drawing cards, and simulating the
 * game flow.
 */

#define PRINT_ALL_PLAYERS 0
#define TEMP_DECK 1
#define TURN 2
#define TEST -1

#define DECK_SIZE 108

enum COLOR {
    wild, red, green, blue, yellow
};

class card {
public:
    int number; // 0-9 numbers, +2, skip, reverse, all color, +4 (all color)
    COLOR color; // 5 colors: red, green, blue, yellow, and no color

    /**
     * Equality operator.
     * @param other Other card to check equality with. (can be put as the next card)
     */
    bool operator==(card const& other) const {
        return number == other.number || color == other.color || color == wild || other.color == wild;

    }

    /**
     * Inequality operator.
     * @param other Other card to check inequality with.
     */
    bool operator!=(card const& other) const {
        return !(*this == other);
    }

    card() : number(0), color(wild) {

    }

    card(int num, COLOR col) : number(num), color(col) {

    }
};

/**
 * Stream operator that allows a card to be written to standard streams
 * (like cout).
 *
 * @param out Stream to write to.
 * @param temp_card to write to the stream.
 */
std::ostream& operator<<(std::ostream& out, card const& temp_card) {
    out << "Number:";
    switch (temp_card.number) {
        case 10:
            out << "DRAW-2";
            break;
        case 11:
            out << "SKIP";
            break;
        case 12:
            out << "REVERSE";
            break;
        case 13:
            out << "WILD";
            break;
        case 14:
            out << "DRAW-4-WILD";
            break;
        default:
            out << (int) temp_card.number;
            break;
    }

    out << "   Color:";
    switch (temp_card.color) {
        case wild:
            out << "wild";
            break;
        case red:
            out << "red";
            break;
        case green:
            out << "green";
            break;
        case blue:
            out << "blue";
            break;
        case yellow:
            out << "yellow";
            break;
        default:
            out << "N/A";
            break;
    }
    return out;
}

/**
 * Class: deck
 * Description:
 * The `deck` class represents a deck of UNO cards. It is derived from the `card` class
 * and includes functionalities to manage and manipulate the deck, such as shuffling,
 * drawing cards, and checking the status of the deck.
 *
 * Functionality:
 * - `isDeckEmpty`: Checks if the deck is empty.
 * - `reshuffle`: Reshuffles the entire deck.
 * - `addCardToBottom`: Adds a card to the bottom of the deck, simulating a queue-like behavior.
 * - `drawMultiple`: Draws a specific number of cards from the top of the deck.
 * - `removeCard`: Removes a specific card from the deck, if present.
 * - `create`: Populates the deck with UNO cards based on the game rules.
 * - `print_deck`: Prints the current state of the deck to the console.
 * - `get_size`: Gets the current size of the deck.
 * - `shuffle`: Shuffles the deck using the Fisher-Yates algorithm.
 * - `draw`: Draws the top card from the deck.
 * - `add_card`: Adds a card to the deck.
 * - `quick_shuffle`: Shuffles the deck quickly using a simplified algorithm.
 * - `copy`: Copies the content of another deck.
 * - `clear`: Clears the deck, releasing allocated memory.
 */

class deck : public card {
private:
    card* ptr_deck;
    int size;

public:

    deck() {
        ptr_deck = new card[DECK_SIZE];
        size = 0;
    }

    bool isDeckEmpty() const {
        return size <= 0;
    }

    // Function to reshuffle the entire deck

    void reshuffle() {
        // Use std::random_shuffle to shuffle the deck
        std::random_shuffle(ptr_deck, ptr_deck + size);
    }

    // Function to add a card to the bottom of the deck (like putting it at the end of the queue)

    void addCardToBottom(card temp_card) {
        if (size < DECK_SIZE) {
            // Use a queue-like behavior to add at the end
            std::rotate(ptr_deck, ptr_deck + 1, ptr_deck + size + 1);
            ptr_deck[size - 1] = temp_card;
            size++;
        }
    }

    // Function to draw a specific number of cards from the deck

    std::list<card> drawMultiple(int numCards) {
        std::list<card> drawnCards;
        for (int i = 0; i < numCards && size > 0; ++i) {
            drawnCards.push_back(ptr_deck[size - 1]);
            size--;
        }
        return drawnCards;
    }

    // Function to remove a specific card from the deck (if present)

    bool removeCard(const card& targetCard) {
        auto iter = std::find(ptr_deck, ptr_deck + size, targetCard);
        if (iter != ptr_deck + size) {
            std::rotate(iter, iter + 1, ptr_deck + size);
            size--;
            return true;
        }
        return false;
    }

    void create() {
        int num = 0;

        // card rank 0
        for (int col = 1; col <= 4; col++) {
            ptr_deck[size].number = num;
            ptr_deck[size].color = static_cast<COLOR> (col);
            size++;
        }

        // card rank 1 till 9 , "draw-two", "skip", "reverse"
        for (num = 1; num <= 12; num++) {
            for (int x = 0; x < 2; x++) {
                for (int col = 1; col <= 4; col++) {
                    ptr_deck[size].number = num;
                    ptr_deck[size].color = static_cast<COLOR> (col);
                    size++;
                }
            }
        }

        // card "wild", "wild-draw-four"
        for (num = 13; num <= 14; num++) {
            for (int x = 0; x < 4; x++) {
                ptr_deck[size].number = num;
                ptr_deck[size].color = wild;
                size++;
            }
        }
    }

    void print_deck() {
        for (int i = 0; i < size; i++) {
            cout << i << ": " << ptr_deck[i] << endl;
        }
    }

    int get_size() {
        return size;
    }

    deck(const deck& other) {
        copy(other);
    }

    const deck& operator=(const deck& other) {
        if (this != &other) {
            clear();
            copy(other);
        }
        return *this;
    }

    ~deck() {
        clear();
    }

    void shuffle() {
        card* temp_deck = new card[size];
        for (int i = 0; i < size; i++) {
            temp_deck[i] = ptr_deck[i];
        }

        int temp_size = size;
        int temp_pos;
        int pos;
        for (int i = 0; i < size; i++) {
            srand(time(NULL));
            pos = rand() % temp_size;
            ptr_deck[i] = temp_deck[pos];

            temp_size--;
            for (temp_pos = pos; temp_pos < temp_size; temp_pos++) {
                temp_deck[temp_pos] = temp_deck[temp_pos + 1];
            }
        }

        delete[] temp_deck;
    }

    card draw() {
        if (size <= 0) {
            std::cout << "Deck is empty!" << std::endl;

            return card();
        }
        card temp_card = ptr_deck[size - 1];
        size--;
        return temp_card;
    }

    int add_card(card temp_card) {
        if (size < DECK_SIZE) {
            ptr_deck[size] = temp_card;
            size++;
            return 0;
        } else
            return -1;
    }

    void quick_shuffle() {
        int pos;
        int temp_size = size - 1;
        card temp_card;
        while (temp_size > 0) {
            srand(time(NULL));
            pos = rand() % temp_size;
            temp_card = ptr_deck[temp_size];
            ptr_deck[temp_size] = ptr_deck[pos];
            ptr_deck[pos] = temp_card;
            temp_size--;
        }
    }

    void copy(const deck& other) {
        size = other.size;
        ptr_deck = new card[size];
        for (int i = 0; i < size; i++) {
            ptr_deck[i] = other.ptr_deck[i];
        }
    }

    void clear() {
        delete[]ptr_deck;
        ptr_deck = NULL;
        size = 0;
    }


};

class Graph {
public:
    std::unordered_map<player*, std::vector<player*>, PlayerHash> adjacencyList;

    // Function to add an edge between two players

    void addEdge(player* player1, player* player2) {
        adjacencyList[player1].push_back(player2);
        adjacencyList[player2].push_back(player1);
    }

    // Function to print the connections in the graph

    void printGraph() {
        for (const auto& entry : adjacencyList) {
            cout << "PLAYER " << (entry.first - play_array) + 1 << " is connected to: ";
            for (const auto& connectedPlayer : entry.second) {
                cout << "PLAYER " << (connectedPlayer - play_array) + 1 << " ";
            }
            cout << endl;
        }
    }
};

struct PlayerHash {

    std::size_t operator()(const player* p) const {
        return reinterpret_cast<std::size_t> (p);
    }
};

/**
 * Class: player
 * Description:
 * The `player` class represents a player in the UNO game. It manages the player's hand,
 * which is a collection of UNO cards. The class provides functionalities to add, remove,
 * and manipulate cards in the player's hand. Each player object maintains a linked list
 * of cards, allowing for dynamic size adjustments.
 *
 * Purpose and Reasoning:
 * In the UNO game, players need a way to hold and manage their cards. The `player` class
 * serves as a container for the player's hand, allowing easy addition, removal, and
 * manipulation of cards. The linked list structure is chosen for the hand to handle
 * dynamic size changes efficiently.
 *
 * Key Functionality:
 * - `hand_add`: Adds a card to the player's hand.
 * - `hand_remove`: Removes a card from the player's hand at a specified position.
 * - `print`: Displays the cards in the player's hand to the console.
 * - `get_size`: Gets the current size of the player's hand.
 * - `peek`: Retrieves a card from the player's hand without removing it.
 *
 * Private Nested Class: card_elem
 * This class represents an element in the linked list, holding a card and a pointer to
 * the next element. It is a private nested class to encapsulate the linked list
 * implementation details from the external users of the `player` class.
 *
 * Private Members:
 * - `head`: Pointer to the first element in the linked list.
 * - `size`: Current size of the player's hand.
 *
 * Member Functions:
 * - `copy`: Copies the content of another player's hand.
 * - `clear`: Clears the player's hand, releasing allocated memory.
 *
 *  This class is designed to be part of a larger UNO game implementation,
 * and it works in conjunction with the `card` class and potentially a `deck` class.
 */

class player {
public:

    player() {
        head = NULL;
        size = 0;
    }

    player(const player& other) {
        copy(other);
    }

    const player& operator=(const player& other) {
        if (this != &other) {
            clear();
            copy(other);
        }

        return *this;
    }

    ~player() {
        clear();
    }

    // Function to add a card to the player's hand

    void hand_add(card temp_card) {
        card_elem* temp_ptr;
        temp_ptr = new card_elem();
        temp_ptr->data = temp_card;
        temp_ptr->next = head;
        head = temp_ptr;
        size++;
    }

    // Function to remove a card from the player's hand at a specified position

    card hand_remove(int pos) {
        if (pos < 0 || pos >= size) {
            return card();
        }

        card_elem* prev_ptr = head;
        card_elem* target = prev_ptr->next;
        card temp_card;
        int temp_pos = pos;

        if (pos == 0) {
            temp_card = head->data;
            head = head->next;
            delete prev_ptr;
            size--;
            return temp_card;
        }

        while (temp_pos > 1) {
            prev_ptr = prev_ptr->next;
            target = prev_ptr->next;
            temp_pos--;
        }

        prev_ptr->next = target->next;
        temp_card = target->data;
        delete target;
        size--;
        return temp_card;
    }


    // Function to print the cards in the player's hand to the console

    void print() const {
        int temp_size = size;
        int i = 0;
        card_elem* temp_ptr = head;
        while (temp_size > 0) {
            std::cout << i + 1 << ":  " << temp_ptr->data << std::endl;
            temp_ptr = temp_ptr->next;
            i++;
            temp_size--;
        }
    }
    // Function to get the current size of the player's hand

    int get_size() const {
        return size;
    }

    // Function to peek at a card in the player's hand without removing it

    card peek(int pos) const {
        int temp_pos = pos;
        card_elem* temp_elem = head;
        while (temp_pos > 0) {
            temp_elem = temp_elem->next;
            temp_pos--;
        }

        return temp_elem->data;
    }

private:
    // Nested class representing an element in the linked list

    class card_elem {
    public:

        card_elem() {
            next = NULL;
        }
        card data;
        card_elem* next;
    };

    card_elem* head; // Pointer to the first element in the linked list
    int size;

    // Function to copy the content of another player's hand

    void copy(const player& other) {
        size = other.size;

        if (size > 0) {
            head = new card_elem();
            head->data = other.head->data;
        } else {
            head = NULL;
            return;
        }

        card_elem* other_ptr = other.head->next;
        card_elem* temp_ptr;
        card_elem* prev_ptr = head;
        for (int i = 1; i < size; i++) {
            temp_ptr = new card_elem();
            prev_ptr->next = temp_ptr;
            temp_ptr->data = other_ptr->data;
            prev_ptr = temp_ptr;
            temp_ptr = NULL;
            other_ptr = other_ptr->next;
        }
    }
    // Function to clear the player's hand, releasing allocated memory

    void clear() {
        card_elem* temp_ptr = head;
        card_elem* next_ptr;
        while (size > 0) {
            next_ptr = temp_ptr->next;
            delete temp_ptr;
            temp_ptr = next_ptr;
            size--;
        }
        head = NULL;
    }
};

void confirm_turn(int x) {

    cout << "Confirm Player" << x << " by typing " << "'" << x << "'" << " and pressing enter" << ": ";
    int temp = 0;
    while (temp != x) {
        cin >> temp;
    }
}

COLOR FromString(const string& str) {
    if (str == "red")
        return red;
    else if (str == "green")
        return green;
    else if (str == "blue")
        return blue;
    else if (str == "yellow")
        return yellow;
    else
        return wild;
}

// Function to check if a player has won the game recursively

bool checkWinner(const player* play_array, int amount_players, int currentPlayerIndex) {
    // Base case: player's hand is empty
    if (play_array[currentPlayerIndex].get_size() == 0) {
        cout << "PLAYER " << currentPlayerIndex + 1 << " has won the game." << endl;
        return true;
    }

    // Recursive case: check the next player
    int nextPlayerIndex = (currentPlayerIndex + 1) % amount_players;
    return checkWinner(play_array, amount_players, nextPlayerIndex);


}

class TreeNode {
public:
    player* playerNode;
    TreeNode* left;
    TreeNode* right;

    TreeNode(player* playerNode) : playerNode(playerNode), left(nullptr), right(nullptr) {
    }
};

// Function to build a binary tree of players

TreeNode* buildPlayerTree(player* play_array, int amount_players, int start, int end) {
    if (start > end)
        return nullptr;

    int mid = (start + end) / 2;
    TreeNode* root = new TreeNode(&play_array[mid]);
    root->left = buildPlayerTree(play_array, amount_players, start, mid - 1);
    root->right = buildPlayerTree(play_array, amount_players, mid + 1, end);

    return root;
}

int main() {
    std::unordered_map<player*, player, PlayerHash> playerHashTable;
    Graph playerGraph;
    while (!checkWinner(play_array, amount_players, turn % amount_players)) {
        int amount_players;
        int flag = 0;
        while (flag == 0) {
            cout << "Please enter amount of players: ";
            cin >> amount_players;
            if (amount_players >= 2 && amount_players <= 5) {
                cout << amount_players << " players entering game .... " << endl;
                flag = 1;
                system("pause");
                break;
            } else {
                cout << "invalid amount of players" << endl;
            }
        }

        TreeNode* playerTree = buildPlayerTree(play_array, amount_players, 0, amount_players - 1);

        /* create the components of the game */
        /* creating deck */
        deck main_deck;
        main_deck.create();
        main_deck.quick_shuffle();
        /* creating player array */
        player* play_array;
        play_array = new player[amount_players];
        /* distributing 7 starting cards to each player */
        for (int i = 0; i < amount_players; i++) {
            for (int k = 0; k < 7; k++) {
                card temp_card;
                temp_card = main_deck.draw();
                play_array[i].hand_add(temp_card);
            }
        }
        /* all cards that are played will go to temp_deck */
        deck temp_deck;
        /* create the first starting card, by drawing from deck */
        card played_card;
        card temp_card;
        int card_flag = 0;
        while (card_flag == 0) {
            temp_card = main_deck.draw();
            /* if first card is not wild, then okay*/
            if (temp_card.color != wild) {
                card_flag = 1;
                played_card = temp_card;
            }/* if first card is wild, redraw */
            else {
                temp_deck.add_card(temp_card);
            }
        }


#if TEST == PRINT_ALL_PLAYERS
        /*print out testing */
        for (int i = 0; i < amount_players; i++) {
            cout << "player: " << i + 1 << endl;
            play_array[i].print();
        }
#endif
        /* randomize who starts first */

        srand(time(NULL));
        int turn = rand() % amount_players;
        cout << "PLAYER " << turn << " is randomly selected to play first" << endl;
        confirm_turn(turn);

        bool force_draw_bool = false;
        int turn_flag = 1;
        int win = 0;
        /* keep playing until a player wins */
        while (win == 0) {
            // clear screen
            system("cls");


#if TEST == TEMP_DECK
            temp_deck.print_deck();
#endif

            // Fixed error , at first created a new player each time , therefore did not update
            // Have to use player pointer instead
            player* curr_player = &play_array[turn % amount_players];


            // checked for forced draw cards
            cout << "PLAYER " << (turn % amount_players) + 1 << endl;

            if (force_draw_bool) {
                // checked for Draw-2
                if (played_card.number == 10) {
                    cout << "Forced Draw-2" << endl;
                    card draw_2;
                    for (int i = 0; i < 2; i++) {
                        draw_2 = main_deck.draw();
                        curr_player->hand_add(draw_2);
                    }
                }

                // check for Draw-4
                if (played_card.number == 14) {
                    cout << "Forced Draw-4" << endl;
                    card draw_4;
                    for (int i = 0; i < 4; i++) {
                        draw_4 = main_deck.draw();
                        curr_player->hand_add(draw_4);
                    }

                }
                force_draw_bool = false;
            }


            // print out the cards remaining for each player
            cout << "Cards remaining for each player " << endl;
            cout << "====================================" << endl;
            for (int i = 0; i < amount_players; i++) {
                cout << "PLAYER " << i + 1 << ": " << play_array[i].get_size() << "   ";
            }
            cout << endl;
            // print out the temporary card
            cout << "Played Card: " << played_card << endl;
            // print out cards in player's hand
            cout << "PLAYER " << (turn % amount_players) + 1 << endl;
            cout << "====================================" << endl;

            curr_player->print();
            int check_flag = 0;
            int index;
            int size = curr_player->get_size();
            // ask for which card to play into middle
            while (check_flag == 0) {
                cout << "which card do you want to play? " << endl;
                cout << "If you want to draw a card please enter '-1' " << endl;

                cin >> index;
                //check if index is to draw a card
                if (index == -1) {
                    card draw_temp;
                    draw_temp = main_deck.draw();
                    cout << "DRAWN CARD: " << draw_temp << endl;
                    if (draw_temp == played_card && draw_temp.color != wild) {

                        int play_draw_flag = 0;
                        while (play_draw_flag == 0) {

                            string temp_play;
                            cout << "Do you want to play the drawn card [y/n] : ";
                            cin >> temp_play;
                            if (temp_play == "y") {
                                played_card = draw_temp;
                                temp_deck.add_card(draw_temp);
                                if (played_card.number >= 10 && played_card.number <= 14) {
                                    force_draw_bool = true;
                                }
                                play_draw_flag = 1;
                            }
                            if (temp_play == "n") {
                                curr_player->hand_add(draw_temp);
                                play_draw_flag = 1;
                            }


                        }

                    } else {
                        curr_player->hand_add(draw_temp);
                    }
                    check_flag = 1;

                }
                //check if index is valid
                if (index >= 0 && index < size) {
                    // check if card is compatilbe with played card
                    card temp = curr_player->peek(index);
                    if (temp == played_card) {
                        //remove from player's hand
                        curr_player->hand_remove(index);
                        // add to the discarded pile
                        temp_deck.add_card(temp);
                        // change the played card
                        played_card = temp;
                        // check if card is a wild card
                        if (played_card.color == wild) {
                            int check_color = 0;
                            COLOR temp_color;
                            string str_color;
                            while (check_color == 0) {
                                // ask for new color
                                cout << "Please choose a color (red , green, blue, yellow) :";
                                cin >> str_color;
                                // change string to enum type COLOR
                                temp_color = FromString(str_color);
                                // check if valid color
                                if (temp_color != wild) {
                                    played_card.color = temp_color;
                                    check_color = 1;
                                } else {
                                    cout << "invalid color" << endl;
                                }

                            }
                        }
                        if (played_card.number >= 10 && played_card.number <= 14) {
                            force_draw_bool = true;
                        }
                        check_flag = 1;
                    } else {
                        cout << "card cannot be played " << endl;
                    }
                } else {
                    cout << "invalid index " << endl;
                }
            }




            // check if there is a winner, and break while loop
            if (curr_player->get_size() == 0) {
                win = 1;
                cout << "PLAYER " << (turn % amount_players) + 1 << " has won the game." << endl;
                break;
            }

            // check for action cards that influence the turn here
            // skip case
            if (played_card.number == 11 && force_draw_bool == true) {
                if (turn_flag == 1)
                    turn = turn + 2;
                else
                    turn = turn - 2;
            }// reverse case
            else if (played_card.number == 12 && force_draw_bool == true) {
                // if only two players, behaves like a skip card
                if (amount_players == 2) {
                    turn = turn + 2;
                } else {
                    // changes the rotation of game (from CW to CCW or vice versa)
                    if (turn_flag == 1) {
                        turn_flag = -1;
                        turn--;
                    } else {
                        turn_flag = 1;
                        turn++;
                    }

                }
            }// for other cards
            else {
                // depending on going Clockwise or CounterClockwise
                if (turn_flag == 1)
                    turn++;
                else
                    turn--;
            }


            system("cls");
            // print out the cards remaining for each player
            cout << "Cards remaining for each player: " << endl;
            cout << "====================================" << endl;
            for (int i = 0; i < amount_players; i++) {
                cout << "PLAYER " << i + 1 << ": " << play_array[i].get_size() << "   ";
            }
            cout << endl;

            cout << "====================================" << endl;

            // print out the temporary card
            cout << "Played Card: " << played_card << endl;
            confirm_turn(turn % amount_players);


            // when main deck is running out of cards
            if (main_deck.get_size() < 10) {
                // flush remaining cards from main to temp deck
                for (int i = 0; i < main_deck.get_size(); i++) {
                    temp_deck.add_card(main_deck.draw());
                }
                //recreate main_deck and shuffle it
                main_deck = temp_deck;
                main_deck.quick_shuffle();
                // clear temp deck
                temp_deck = deck();

            }
              for (int i = 0; i < amount_players; ++i) {
                playerHashTable[&play_array[i]] = play_array[i];
                playerGraph.addEdge(&play_array[i], &play_array[(i + 1) % amount_players]);
            }

            // Print the connections in the graph
            cout << "Graph Connections:" << endl;
            playerGraph.printGraph();

        }

    }



    return 0;
}
