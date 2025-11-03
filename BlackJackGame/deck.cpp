/**
 * @brief Implementation of The Deck class. It represents one or more standard 52-card decks.
 * Provides a shuffle (random or determined) and card dealing functionality
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/22/2025
 */

#include "deck.h"
#include "suits.h"
#include "rank.h"
#include <algorithm>
#include <random>
#include <QRandomGenerator>

Deck::Deck(int deckNumber, int deterministic) : deterministic(deterministic)
{
    createDeck();

    // Insert master deck into shuffled deck deckNumber times (creates a deck the size of deckNumber)
    for (int i = 0; i < deckNumber; i++)
        shuffledDeck.insert(shuffledDeck.end(), masterDeck.begin(), masterDeck.end());

    shuffle();
}

void Deck::createDeck()
{
    masterDeck.reserve(52);
    unsigned int i = 0;

    // Creates a deck of all 52 cards
    for (RANK rank : Rank::allRanks)
        for (SUIT suit : Suit::allSuits)
            masterDeck.emplace_back(suit, rank, Card::getImageFromVector(i++));
}

void Deck::shuffle()
{
    // Random shuffle
    if (deterministic == 0)
    {
        std::default_random_engine rand(std::random_device{}());
        std::shuffle(shuffledDeck.begin(), shuffledDeck.end(), rand);
        currentDeckIndex = 0;
    }
    // Tutorial ordered deck
    else if (deterministic == 1)
    {
        const std::initializer_list<const char *> tutorialOrder = {
            "7C",
            "KS",
            "5D",
            "2H",
            "TS", // First scenario done
            "8S",
            "TH",
            "4H",
            "6C",
            "6D", // Second scenario done
            "AS",
            "TD",
            "AC",
            "7D",
            "8H",
            "7S", // Third scenario done
            "TH",
            "6C",
            "TS",
            "6D",
            "TD", // Fourth scenario done
            "6H",
            "8C",
            "5S",
            "4C",
            "KH",
            "TS", // Fifth scenario done
            "AH",
            "7S",
            "7D",
            "9C",
            "3D",
            "5H", // Sixth scenario done
            "KH",
            "TH",
            "9D",
            "AC", // Seventh scenario done
            "AH",
            "9D",
            "6S",
            "9H",
            "2C", // Eigth scenario done
            "8S",
            "6H",
            "8H",
            "9D",
            "3C",
            "2D",
            "TS",
            "8C",
            "4C", // Ninth scenario done
            "9D",
            "QH",
            "9C",
            "7S", // Tenth scenario done
            "AH",
            "JS",
            "7S",
            "6D",
            "2D",
            "QC", // Eleventh scenario done
            "KH",
            "TC",
            "6C",
            "7D",
            "5D", // Twelfth scenario done
            "TS",
            "7D",
            "7H",
            "TC" // Thirteenth scenario done
        };

        // Clear and rebuild shuffledDeck in that exact order
        shuffledDeck.clear();
        for (auto code : tutorialOrder)
        {
            Rank::RANK r = charToRank(code[0]);
            Suit::SUIT s = charToSuit(code[1]);

            // Find the matching card in masterDeck
            auto it = std::find_if(masterDeck.begin(), masterDeck.end(), [&](const Card &c)
                                   { return c.getRank() == r && c.getSuit() == s; });
            if (it != masterDeck.end())
                shuffledDeck.push_back(*it);
        }

        // Fill the rest of shuffled deck with random cards
        for (int i = shuffledDeck.size(); i < 52 * 2; i++)
        {
            shuffledDeck.push_back(masterDeck[QRandomGenerator::global()->bounded(52)]);
        }
    }
    // Sets the entire deck to TWO of SPADES
    else
    {
        shuffledDeck.clear();
        // All twos baby
        for (int i = 0; i < 100; i++)
        {
            shuffledDeck.emplace_back(SUIT::SPADES, RANK::TWO, Card::getImageFromVector(0));
        }
    }
}

Card Deck::getNextCard()
{
    // Reshuffles if inside the last 20% of the deck
    if (currentDeckIndex > (shuffledDeck.size() * 0.8))
    {
        shuffle();
        currentDeckIndex = 0;
    }

    return shuffledDeck[currentDeckIndex++];
}

bool Deck::isEmpty() const
{
    return currentDeckIndex >= static_cast<int>(shuffledDeck.size());
}

Rank::RANK Deck::charToRank(char c)
{
    switch (c)
    {
    case '2':
        return Rank::RANK::TWO;
    case '3':
        return Rank::RANK::THREE;
    case '4':
        return Rank::RANK::FOUR;
    case '5':
        return Rank::RANK::FIVE;
    case '6':
        return Rank::RANK::SIX;
    case '7':
        return Rank::RANK::SEVEN;
    case '8':
        return Rank::RANK::EIGHT;
    case '9':
        return Rank::RANK::NINE;
    case 'T':
        return Rank::RANK::TEN;
    case 'J':
        return Rank::RANK::JACK;
    case 'Q':
        return Rank::RANK::QUEEN;
    case 'K':
        return Rank::RANK::KING;
    case 'A':
        return Rank::RANK::ACE;
    }
    return Rank::RANK::TWO;
}

Suit::SUIT Deck::charToSuit(char c)
{
    switch (c)
    {
    case 'S':
        return Suit::SUIT::SPADES;
    case 'H':
        return Suit::SUIT::HEARTS;
    case 'C':
        return Suit::SUIT::CLUBS;
    case 'D':
        return Suit::SUIT::DIAMONDS;
    }
    return Suit::SUIT::SPADES;
}
