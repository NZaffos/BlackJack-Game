#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include "playerStatus.h"

/**
 * @brief The Player class represents a single player in a blackjack game. Stores the current state, hand, money, and user status of the player
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/20/2025
 */
class Player
{
public:
    /**
     * @brief hand The hand of cards and bet for a players round
     */
    Hand hand;

    /**
     * @brief money The total amount of money a player has
     */
    int money;

    /**
     * @brief isUser True if the player is a user, false otherwise
     */
    bool isUser;

    /**
     * @brief originalHand True if this is the original hand of the player (not a split)
     */
    bool originalHand = false;

    /**
     * @brief status Stores the current status of the player
     */
    PlayerStatus::PLAYERSTATUS status = PlayerStatus::PLAYERSTATUS::WAITING;

    /**
     * @brief playerHandCount The number of hands the player has control over (from splitting)
     */
    unsigned int playerHandCount;

    /**
     * @brief playerHandIndex The index of the hand that the player controls
     */
    unsigned int playerHandIndex;

    /**
     * @brief Player Constructor that creates a new player with the given bet and money
     * @param initialMoney The amount of money the player starts with
     * @param initialBet The inital bet for the first round
     * @param isUser True is the player is a user, false if they are a bot
     */
    Player(int initialMoney, int initialBet, bool isUser, unsigned int playerHandCount, unsigned int playerHandIndex) : hand(initialBet), money(initialMoney), isUser(isUser), playerHandCount(playerHandCount), playerHandIndex(playerHandIndex) {}
};

#endif // PLAYER_H
