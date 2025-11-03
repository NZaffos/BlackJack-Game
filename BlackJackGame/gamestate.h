#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include "player.h"
#include "deck.h"

/**
 * @brief The GameState class is the core model of the blackjack game. Handles all game logic
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/21/2025
 */
class GameState
{
public:
    /**
     * @brief Constructs the GameState with a given number of players, a certain number of decks, and an initial bet for each player
     * @param players A vector of all of the players in the game
     * @param deckCount The number of decks to use
     * @param deterministic 0 = random shuffle, 1 = shuffle for single player, 2 = shuffle for 3 players
     */
    GameState(std::vector<Player> players, int deckCount, int deterministic);

    /**
     * @brief dealInitialCards Deals two cards to each player and two to the dealer
     */
    void dealInitialCards();

    /**
     * @brief hit Deals a single card to the specified player, checks for bust
     * @param playerIndex The index of the current player
     */
    void hit(int playerIndex);

    /**
     * @brief doubleDown Double bet for the specified player, deal one card, check bust
     * @param playerIndex The index of the current player
     */
    void doubleDown(int playerIndex);

    /**
     * @brief stand The player stands
     * @param playerIndex The index of the current player
     */
    void stand(int playerIndex);

    /**
     * @brief split The player splits
     * @param playerIndex The index of the current player
     */
    void split(int playerIndex);

    /**
     * @brief dealerPlay Dealer draws cards until total >= 17 or bust
     */
    void dealerPlay();

    /**
     * @brief endRound Compare each player's total to the dealer's total, pay winners, take bets from losers, then remove bankrupt players
     */
    void endRound();

    /**
     * @brief isBust Checks if a hand is over 21
     * @param hand The hand to check
     */
    bool isBust(const Hand &hand) const;

    /**
     * @brief getPlayerCount Gets how many players are active in the game
     */
    int getPlayerCount() const;

    /**
     * @brief getPlayer Gets a player's current hand
     * @param index The current player's index
     */
    const Player &getPlayer(int index) const;

    /**
     * @brief getOriginalPlayer Returns the original player's index (not a split index)
     * @param index The index of one of the player's hands
     * @return The index of the original hand
     */
    const Player &getOriginalPlayer(int index) const;

    /**
     * @brief getDealerHand Gets the dealer's current hand
     */
    const Hand &getDealerHand() const;

    /**
     * @brief setPlayerActive Sets the player at given index to ACTIVE
     * @param index The player to set to ACTIVE
     */
    void setPlayerActive(int index);

    /**
     * @brief setPlayerActive Sets the bet for the player at given index and removes the money from the player
     * @param amount The amount to set the bet to
     */
    void setPlayerBet(int index, int amount);

    /**
     * @brief clearHands Resets the players and dealer hands to empty
     */
    void clearHands();

    /**
     * @brief getAllPlayers Returns a constant of all players in the model
     * @return A vector containing all of the players in the game
     */
    const std::vector<Player> getAllPlayers() const;

private:
    /**
     * @brief players The players in the game
     */
    std::vector<Player> players;

    /**
     * @brief deck The deck for the game
     */
    Deck deck;

    /**
     * @brief dealerHand The dealer's hand
     */
    Hand dealerHand;
};

#endif // GAMESTATE_H
