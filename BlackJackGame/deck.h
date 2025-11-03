#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <vector>

/**
 * @brief The Deck class represents one or more standard 52-card decks.
 * Provides a shuffle (random or determined) and card dealing functionality
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/22/2025
 */
class Deck
{
public:
    /**
     * @brief Deck The constructor for the deck class
     * @param deckNumber The number of decks to use in the shuffled deck
     * @param deterministic 0 = random shuffle, 1 = shuffle for single player, 2 = shuffle for 3 players
     */
    Deck(int deckNumber = 1, int deterministic = 0);

    /**
     * @brief shuffle Shuffes the shuffleDeck so it is randomized
     */
    void shuffle();

    /**
     * @brief deterministicShuffle Shuffles the deck in a deterministic state
     */
    void deterministicShuffle();

    /**
     * @brief getNextCard Gets the next card in the shuffled deck and moves the index to the next card
     * @return The next card in the shuffled deck
     */
    Card getNextCard();

    /**
     * @brief isEmpty Checks if the there are any more cards left in the shuffled deck
     * @return Returns true if there are no more cards and false if the shuffled deck is not empty
     */
    bool isEmpty() const;

private:
    /**
     * @brief masterDeck The master deck holding all 52 cards in order
     */
    std::vector<Card> masterDeck;

    /**
     * @brief shuffledDeck The deck that becomes shuffled to use for getting the next card
     */
    std::vector<Card> shuffledDeck;

    /**
     * @brief shuffledDeckIndex The current index in the shuffled deck
     */
    int currentDeckIndex = 0;

    /**
     * @brief deterministic 0 = random shuffle, 1 = shuffle for single player, 2 = shuffle for 3 players
     */
    int deterministic;

    /**
     * @brief createDeck Creates a full 52 card deck in number order. Suit goes
     */
    void createDeck();

    /**
     * @brief charToRank Converts a char from 2 - 9, A, K, Q, J, T to their RANK
     * @param c The char to convert
     * @return Returns the RANK of the char
     */
    Rank::RANK charToRank(char c);

    /**
     * @brief charToSuit Converts a char from S, H, C, D to their SUIT
     * @param c The char to convert
     * @return Returns the RANK of the char
     */
    Suit::SUIT charToSuit(char c);
};

#endif // DECK_H
