#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "gamestate.h"
#include "hand.h"
#include "playerStatus.h"
#include "botstrategy.h"
#include "timermanager.h"

using PlayerStatus::PLAYERSTATUS;

/**
 * @brief The Controller class is responsible for managing the gamestate(model) and connecting it with the view.
 * It handles user input as well as bot input to make the game flow
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/21/2025
 */
class Controller : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Controller Constructor that creates a new controller to the the blackjack game
     * @param parent The parent of this object
     */
    explicit Controller(QObject *parent = nullptr);

    /**
     * @brief Controller Destructor for Controller class
     */
    ~Controller();

public slots:
    /**
     * @brief onHit The current player chooses to hit
     */
    void onHit();

    /**
     * @brief onStand The current player chooses to stand
     */
    void onStand();

    /**
     * @brief onDoubleDown The current player chooses to dobule down
     */
    void onDoubleDown();

    /**
     * @brief onSplit The current player chooses to split
     */
    void onSplit();

    /**
     * @brief startRound The next round of betting starts
     */
    void startBetting();

    /**
     * @brief onStopEverything Stops all play and single shot calls
     */
    void onStopEverything();

    /**
     * @brief dealCards Deals out the initial cards and moves to the first player's turn
     */
    void dealCards();

    /**
     * @brief onBet The current player sends in there bet
     * @param bet The bet of the player
     */
    void onBet(int bet);

    /**
     * @brief onDealingAnimationComplete Slot that the dealing animation is complete and the round can start
     */
    void onDealingAnimationComplete();

    /**
     * @brief onDealerDonePlaying Slot that the dealer animation is complete and the round can end
     */
    void onDealerDonePlaying();

    /**
     * @brief createNewGame Create a new gamestate and initalizes blackjack with the set players with the given amount of money
     * @param players The number of players in the game
     * @param decks The number of decks to play with
     * @param deterministic 0 = random shuffle, 1 = shuffle for single player, 2 = shuffle for 3 players
     */
    void createNewGame(std::vector<Player> players, int decks, int deterministic = 0);
signals:
    /**
     * @brief playerUpdated Signal that a player in the game has updated with their updated info
     * @param playerIndex The player that has updated
     * @param hand The new hand for the player
     * @param total The total value of their cards
     * @param money The money they have left
     * @param status The status of the player
     */
    void playerUpdated(int playerIndex, const Player &player, int money, int total); // TODO

    /**
     * @brief dealerUpdated Signal that the dealer has updated with their updated info
     * @param hand The new hand of the dealer
     * @param total The total value of their cards
     */
    void dealerUpdated(const Hand &hand, int total);

    /**
     * @brief currentPlayerTurn Signal that it is the next player's turn
     * @param nextPlayerIndex The index of the next player that is up now
     * @param money The money of the current player's turn
     * @param bet The bet of the current player's turn
     * @param handTotal The total of the cards in the player's hand
     */
    void currentPlayerTurn(int nextPlayerIndex, int money, int bet, int handTotal);

    /**
     * @brief flipDealerCard Signal to flip over the dealer's first card
     * @param showCard True if to show dealer cards, false to hide it
     */
    void showDealerCard(bool showCard);

    /**
     * @brief endRound Signal that the round has ended
     * @param players A vector of the players after the round has ended
     */
    void endRound(const std::vector<Player> &players);

    /**
     * @brief gameMessage Signal to send a game message (ex. player 0 has busted)
     * @param message The message to display
     */
    void gameMessage(QString message);

    /**
     * @brief gameOver Signal for the game to be over
     */
    void gameOver();

    /**
     * @brief endBetting Signal for the betting phase to be over
     */
    void endBetting();

    /**
     * @brief updateAllPlayers Signal to update all of the players in the game
     * @param players The vector of players to update in the view
     */
    void updateAllPlayers(const std::vector<Player> &players);

    /**
     * @brief splitPlayers Signal to split the player at the given index with the updated players
     * @param originalIndex The index of the player to split
     * @param originalPlayer The original player
     * @param newPlayer The new player
     */
    void splitPlayers(int originalIndex, const Player &originalPlayer, const Player &newPlayer);

    /**
     * @brief splitPlayerUpdateInfo Signal to playerinfoview to update player information
     * @param originalIndex The index of the player to update
     * @param originalPlayer The original player
     * @param money Money of the first hand
     */
    void splitPlayerUpdateInfo(int originalIndex, const Player &originalPlayer, int money);

private:
    /**
     * @brief model The Gamestate model that handles the players playing blackjack
     */
    GameState *model = nullptr;

    /**
     * @brief botPlayer The BotStrategy that determines the bot's move
     */
    BotStrategy *botStrategy;

    /**
     * @brief timer The timer class to schedule all single shot calls
     */
    TimerManager *timer;

    /**
     * @brief currentPlayerIndex The index of the current player in the round
     */
    int currentPlayerIndex;

    /**
     * @brief advanceToNextPlayer Move to the next player's turn
     */
    void advanceToNextPlayer();

    /**
     * @brief advanceToNextBet Move to the next player's turn in betting
     */
    void advanceToNextBet();

    /**
     * @brief checkTurnEnd
     */
    void checkTurnEnd(const Player &player);

    /**
     * @brief onePlayerStillAlive Checks if there is at least one player still with a stood hand
     * @return True if there is a player who stood, false otherwise
     */
    bool onePlayerStillAlive();

    /**
     * @brief botMove Performs a move for a bot player
     */
    void botMove();

    /**
     * @brief botBet Performs a bet for a bot player
     */
    void botBet();

    /**
     * @brief getPlayerMoney Returns the amount of money a player has
     * @param playerIndex the index of the player whose money to return
     */
    int getPlayerMoney(unsigned int playerIndex);
};

#endif // CONTROLLER_H
