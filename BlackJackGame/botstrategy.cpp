/**
 * @brief Implementation of The BotStrategy class. It computes the reccomended blackack move based on proper stretegy.
 * It is based on the player's hand and dealer face up card
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/20/2025
 */

#include "botstrategy.h"
#include "statistics.h"
#include "rank.h"

BotStrategy::BotStrategy() {}

MOVE BotStrategy::getNextMove(const Hand &playerHand, const Card &dealerCard)
{
    // Check for a pair, only valid for 2 cards
    if (isPair(playerHand))
        return getPairHandMove(playerHand, dealerCard);

    // Check for soft hand
    if (playerHand.isSoft())
        return getSoftHandMove(playerHand, dealerCard);

    // Treat as a hard hand
    return getHardHandMove(playerHand, dealerCard);
}

MOVE BotStrategy::getHardHandMove(const Hand &playerHand, const Card &dealerCard)
{
    int row = playerHand.getTotal() - 5;

    return Statistics::HardTable[row][cardToIndex(dealerCard)];
}

MOVE BotStrategy::getSoftHandMove(const Hand &playerHand, const Card &dealerCard)
{
    int effective = playerHand.getTotal() - 11;
    int row = effective - 2;

    return Statistics::SoftTable[row][cardToIndex(dealerCard)];
}

MOVE BotStrategy::getPairHandMove(const Hand &playerHand, const Card &dealerCard)
{
    int row = cardToIndex(playerHand.getCards()[0]);
    return Statistics::PairTable[row][cardToIndex(dealerCard)];
}

bool BotStrategy::isPair(const Hand &hand)
{
    const std::vector<Card> &cards = hand.getCards();
    if (cards.size() != 2)
        return false;
    return (cards[0].getRank() == cards[1].getRank());
}

int BotStrategy::cardToIndex(const Card &card)
{
    return Rank::blackjackValue(card.getRank()) - 2;
}
