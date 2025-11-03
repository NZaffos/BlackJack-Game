#ifndef BOTSTRATEGY_H
#define BOTSTRATEGY_H

#include "hand.h"
#include "card.h"
#include "statistics.h"

/**
 * @brief The BotStrategy class computes the reccomended blackack move based on proper stretegy.
 * It is based on the player's hand and dealer face up card
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/20/2025
 */
class BotStrategy
{
public:
    /**
     * @brief BotStrategy Constructer for the bot strategy class
     */
    BotStrategy();

    /**
     * @brief getNextMove Determines the recommended move (hit, double, split, or stand) given a player's hand and the dealer's visible card.
     * @param playerHand The player's hand.
     * @param dealerCard The dealer's up card.
     * @return A MOVE corresponding to the recommended move (HIT, DOUBLE, SPLIT, STAND).
     */
    static MOVE getNextMove(const Hand &playerHand, const Card &dealerCard);

    /**
     * @brief isPair Checks if the hand is a pair (2 of the same card)
     * @param hand The hand to check
     * @return True if the hand is a pair, false if it is not
     */
    static bool isPair(const Hand &hand);

private:
    /**
     * @brief cardToIndex Converts the current card to an index for a table by subtracting 2
     * @param card The card to convert
     * @return Returns the int value for the index of the card
     */
    static int cardToIndex(const Card &card);

    /**
     * @brief getSoftHandMove Determines the recommended move for a soft hand (ace counted as 11)
     * @param playerHand The player's hand
     * @param dealerCard The dealer's up card
     * @return A MOVE corresponding to the recommended move (HIT, DOUBLE, SPLIT, STAND)
     */
    static MOVE getSoftHandMove(const Hand &playerHand, const Card &dealerCard);

    /**
     * @brief getHARDHandMove Determines the recommended move for a hard hand
     * @param playerHand The player's hand
     * @param dealerCard The dealer's up card
     * @return A MOVE corresponding to the recommended move (HIT, DOUBLE, SPLIT, STAND)
     */
    static MOVE getHardHandMove(const Hand &playerHand, const Card &dealerCard);

    /**
     * @brief getPairHandMove Determines the recommended move for a pair hand (Exactly 2 of the same RANK cards)
     * @param playerHand The player's hand
     * @param dealerCard The dealer's up card
     * @return A MOVE corresponding to the recommended move (HIT, DOUBLE, SPLIT, STAND)
     */
    static MOVE getPairHandMove(const Hand &playerHand, const Card &dealerCard);
};

#endif // BOTSTRATEGY_H
