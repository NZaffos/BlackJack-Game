/**
 * @brief Implementation of The Hand class. It reprsents a single blackjack hand. It holds the cards, bet, and supporting functions
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/22/2025
 */

#include "hand.h"
#include "rank.h"
#include <utility>

Hand::Hand() : Hand(0) {};

Hand::Hand(int bet) : bet(bet) {}

Hand::Hand(const Hand &hand)
{
    this->bet = hand.getBet();

    // Copy all of the cards in the given hand
    for (const Card &card : hand.getCards())
    {
        cards.emplace_back(card.getSuit(), card.getRank(), card.getImagePath());
    }
}

Hand &Hand::operator=(const Hand hand)
{
    this->bet = hand.getBet();

    cards.clear();

    // Copy all of the cards in the given hand
    for (const Card &card : hand.getCards())
    {
        cards.emplace_back(card.getSuit(), card.getRank(), card.getImagePath());
    }
    return *this;
}

void Hand::addCard(const Card &card)
{
    cards.push_back(card);
}

int Hand::getBet() const
{
    return bet;
}

void Hand::setBet(int amount)
{
    bet = amount;
}

void Hand::setCardImagePath(int index, QString imagePath)
{
    cards[index].setImagePath(imagePath);
}

const std::vector<Card> &Hand::getCards() const
{
    return cards;
}

int Hand::getTotal() const
{
    return calculateTotalAndSoft().first;
}

const Card &Hand::removeLastCard()
{
    const Card &card = cards[cards.size() - 1];
    cards.pop_back();
    return card;
}

bool Hand::isSoft() const
{
    return calculateTotalAndSoft().second;
}

std::pair<int, bool> Hand::calculateTotalAndSoft() const
{
    int total = 0;
    int softAces = 0;

    // Count total value and number of Aces counted as 11.
    for (const Card &card : cards)
    {
        int value = Rank::blackjackValue(card.getRank());
        if (value == 11)
            softAces++;
        total += value;
    }

    // Adjust Aces from 11 to 1 if the total exceeds 21.
    while (total > 21 && softAces > 0)
    {
        total -= 10;
        softAces--;
    }

    // The hand is considered soft if at least one Ace is still counted as 11.
    bool soft = (softAces > 0);
    return std::make_pair(total, soft);
}
