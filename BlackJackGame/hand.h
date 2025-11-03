#ifndef HAND_H
#define HAND_H

#include <vector>
#include "card.h"

/**
 * @brief The Hand class reprsents a single blackjack hand. It holds the cards, bet, and supporting functions
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/22/2025
 */
class Hand
{
public:
    /**
     * @brief Hand Constructor that creates a new empty hand with a bet of 0
     */
    Hand();

    /**
     * @brief Hand Constructor that creates a new empty hand with the initial bet
     * @param bet The initial bet for the hand
     */
    Hand(int bet);

    /**
     * @brief Hand Constructor that copies another hand
     * @param other The hand to copy
     */
    Hand(const Hand &other);

    /**
     * @brief operator = Assignment operator to set the current hand to the other hand
     * @param other The hand to copy
     * @return A reference to the new hand
     */
    Hand &operator=(const Hand other);

    /**
     * @brief addCard Adds a card to the hand
     * @param card The card to add
     */
    void addCard(const Card &card);

    /**
     * @brief getTotal Gets the total blackjack value of the hand
     * @return Returns an int of the value of the hand
     */
    int getTotal() const;

    /**
     * @brief isSoft Gets if the hand is a soft or a hard hand
     * @return True if the hand is soft and false if it is a hard hand
     */
    bool isSoft() const;

    /**
     * @brief getBet Gets the current ammount bet
     * @return Returns an int of the bet
     */
    int getBet() const;

    /**
     * @brief setBet Sets the bet to the given amount
     * @param amount The amount to set the bet at
     */
    void setBet(int amount);

    /**
     * @brief setCardImagePath Sets the card at the current index to the new card
     * @param index The index to change in cards
     * @param imagePath The new image path
     */
    void setCardImagePath(int index, QString imagePath);

    /**
     * @brief getCards Gets the cards in the hand
     * @return A vector of the cards in the hand
     */
    const std::vector<Card> &getCards() const;

    /**
     * @brief removeLastCard Removes the last card in the hand
     * @return Returns the removed card from the hands
     */
    const Card &removeLastCard();

private:
    /**
     * @brief cards The cards in the hand
     */
    std::vector<Card> cards;

    /**
     * @brief bet The amount bet on this hand
     */
    int bet;

    /**
     * @brief calculateTotalAndSoft Calculates the hand's total and determines if it is soft.
     * @return A pair where the first element is the total value of the hand and the second element is a bool indicating if the hand is soft (true if soft, false if hard).
     */
    std::pair<int, bool> calculateTotalAndSoft() const;
};

#endif // HAND_H
