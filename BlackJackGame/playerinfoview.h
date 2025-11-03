#ifndef PLAYERINFOVIEW_H
#define PLAYERINFOVIEW_H

#include <QObject>
#include <QLabel>
#include <QVector>
#include "player.h"
#include "playerStatus.h"
#include "ui_mainwindow.h"

using PlayerStatus::PLAYERSTATUS;

/**
 * @brief The PlayerInfoView class is responsible for creating and maintaining player info cards in the info bar in the UI
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/21/2025
 */
class PlayerInfoView : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief PlayerInfoView Constructor to create the player bar view
     * @param ui
     * @param parent
     */
    explicit PlayerInfoView(Ui::MainWindow *ui, QObject *parent = nullptr);

public slots:
    /**
     * @brief onSettingsAccepted Slot to create the player info cards once the settings and players have been setup
     * @param players The players in the game
     * @param deckCount The number of decks used in the game
     * @param deterministic 0 for random shuffle, 1 for single player determined shuffle, 2 > for multiple player determined shuffle
     */
    void onSettingsAccepted(const std::vector<Player> &players, int deckCount, int deterministic);

    /**
     * @brief onPlayerUpdated Slot to update the money and status of the player at the passed index
     * @param playerIndex The index of the player
     * @param hand The hand of the player
     * @param total The total of the cards in the hand
     * @param money The money the player has
     * @param status The status of the player
     */
    void onPlayerUpdated(int playerIndex, const Player &player, int money, int total); // TODO

    /**
     * @brief onUpdateAllPlayers Slot to update all of the players
     * @param players The players to update
     */
    void onUpdateAllPlayers(const std::vector<Player> &players);

    /**
     * @brief onSplitPlayers Slot that a player split
     * @param originalIndex The index of the original player who split
     * @param originalPlayer The original player
     * @param money The money of the first hand
     */
    void onSplitPlayers(int originalIndex, const Player &originalPlayer, int money);

    /**
     * @brief onCurrentPlayerTurn Slot to select a new player
     * @param newPlayerIndex The index of the player to select
     * @param money The money of the current player's turn
     * @param bet The bet of the current player's turn
     * @param handTotal The total value of the cards in the player's hand
     */
    void onCurrentPlayerTurn(int newPlayerIndex, int money, int bet, int handTotal);

    /**
     * @brief onStopEverything Hide the player info bar
     */
    void onStopEverything();

    /**
     * @brief onEndRound Slot to receive when the round has ended
     * @param players The players after the round has ended
     */
    void onEndRound(const std::vector<Player> &players);

private:
    QVector<int> modelToSeat;

    /**
     * @brief seatCount Number of players
     */
    int seatCount = 0;

    /**
     * @brief userIndex Index where the user is
     */
    int userIndex = -1;

    /**
     * @brief ui The ui of the game
     */
    Ui::MainWindow *ui;

    /**
     * @brief seatLabels A vector holding all of the player labels
     */
    QVector<QLabel *> seatLabels;

    /**
     * @brief buildLayout Builds the layout and adds the labels to it
     * @param seats The number of player labels to create
     */
    void buildLayout(int seats);

    /**
     * @brief rebuildMapping Rebuilds the labels to correctly correspond to a player after splitting
     */
    void rebuildMapping();

    /**
     * @brief insertSplitMapping Inserts a new player into the modelToSeat to get the proper label index after a split
     * @param modelIndex The index of the original player
     */
    void insertSplitMapping(int modelIndex);

    /**
     * @brief refreshSeat Refreshes the label for the player at the given seat
     * @param seat The seat to refresh
     * @param player The player info to use
     * @param money The money of the original player
     */
    void refreshSeat(int seat, const Player &player, int money);

    /**
     * @brief paintBorder Paints the border around a label to the given status
     * @param label The label to paint
     * @param status The status to paint the border
     */
    void paintBorder(QLabel *label, PLAYERSTATUS status);

    /**
     * @brief setSeatText Sets the text for the label at the given seat
     * @param seat The seat to update
     * @param money The money the label should display
     * @param bet The bet the label should display
     * @param handTotal The value of cards in the player's hand
     */
    void setSeatText(int seat, int money, int bet, PLAYERSTATUS status, int handTotal);

    /**
     * @brief getStatusColor Gets the QString representation of the player status
     * @param status An enum state of the players status
     * @return A QString of a hexidecimal color
     */
    QString getStatusColor(PLAYERSTATUS status);

    /**
     * @brief fillWholeCard Helper to get if the status should fill just the border or the whole card
     * @param status The status to display
     * @return True for WON, LOST, PUSHED, BANKRUPT, fales otherwise
     */
    bool fillWholeCard(PLAYERSTATUS status);
};

#endif // PLAYERINFOVIEW_H
