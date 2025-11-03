/**
 * @brief Implementation of The Controller class. It is responsible for managing the gamestate(model) and connecting it with the view.
 * It handles user input as well as bot input to make the game flow
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/21/2025
 */

#include "controller.h"
#include "statistics.h"
#include <QTimer>

Controller::Controller(QObject *parent) : QObject{parent}
{
    botStrategy = new BotStrategy();
    timer = new TimerManager();
}

Controller::~Controller()
{
    delete model;
    delete botStrategy;
    delete timer;
}

void Controller::checkTurnEnd(const Player &player)
{
    emit playerUpdated(currentPlayerIndex, player, model->getOriginalPlayer(currentPlayerIndex).money, player.hand.getTotal());
    // Move to the next player if the current player's turn is over
    if (player.status == PLAYERSTATUS::BUST || player.status == PLAYERSTATUS::STAND)
    {
        advanceToNextPlayer();
        return;
    }

    if (!player.isUser)
        botMove();
}

void Controller::onHit()
{
    model->hit(currentPlayerIndex);
    checkTurnEnd(model->getPlayer(currentPlayerIndex));
}

void Controller::onStand()
{
    model->stand(currentPlayerIndex);
    checkTurnEnd(model->getPlayer(currentPlayerIndex));
}

void Controller::onDoubleDown()
{
    const Player &player = model->getPlayer(currentPlayerIndex);
    // Check if the player has enough money  and only 2 cards to double down
    if (model->getOriginalPlayer(currentPlayerIndex).money < player.hand.getBet() || player.hand.getCards().size() != 2)
    {
        return;
    }
    model->doubleDown(currentPlayerIndex);
    checkTurnEnd(player);
}

void Controller::onSplit()
{
    const Player &player = model->getPlayer(currentPlayerIndex);
    // Check if the player has enough money to split
    if (model->getOriginalPlayer(currentPlayerIndex).money < player.hand.getBet() || !botStrategy->isPair(player.hand))
    {
        return;
    }
    model->split(currentPlayerIndex);

    // Update card positions and hand count
    emit splitPlayers(currentPlayerIndex, model->getPlayer(currentPlayerIndex), model->getPlayer(currentPlayerIndex + 1));
    emit splitPlayerUpdateInfo(currentPlayerIndex, model->getPlayer(currentPlayerIndex), model->getOriginalPlayer(currentPlayerIndex).money);
    checkTurnEnd(model->getPlayer(currentPlayerIndex));
}

void Controller::onBet(int bet)
{
    model->setPlayerBet(currentPlayerIndex, bet);
    advanceToNextBet();
}

void Controller::advanceToNextPlayer()
{
    currentPlayerIndex++;
    // Get to the next player who is able to play
    while (currentPlayerIndex < model->getPlayerCount() && (model->getPlayer(currentPlayerIndex).status == PLAYERSTATUS::BANKRUPT || model->getPlayer(currentPlayerIndex).status == PLAYERSTATUS::STAND))
    {
        currentPlayerIndex++;
    }

    // All players have went, dealer turn
    if (currentPlayerIndex >= model->getPlayerCount() || model->getDealerHand().getTotal() == 21)
    {
        if (onePlayerStillAlive())
            model->dealerPlay();
        emit showDealerCard(true);
        emit dealerUpdated(model->getDealerHand(), model->getDealerHand().getTotal());
        return;
    }

    // Switch to next player
    const Player &player = model->getPlayer(currentPlayerIndex);
    emit currentPlayerTurn(currentPlayerIndex, model->getOriginalPlayer(currentPlayerIndex).money, player.hand.getBet(), player.hand.getTotal());
    model->setPlayerActive(currentPlayerIndex);

    if (!player.isUser)
        botMove();
}

bool Controller::onePlayerStillAlive()
{
    for (const Player &player : model->getAllPlayers())
    {
        if (player.status == PLAYERSTATUS::STAND)
            return true;
    }
    return false;
}

void Controller::onDealerDonePlaying()
{
    model->endRound();
    emit endRound(model->getAllPlayers());

    // If at least one user is not bankrupt, the game continues
    for (int i = 0; i < model->getPlayerCount(); i++)
    {
        if (model->getPlayer(i).isUser && model->getPlayer(i).status != PLAYERSTATUS::BANKRUPT)
            return;
    }
    emit gameOver();
}

void Controller::startBetting()
{
    currentPlayerIndex = -1;
    emit showDealerCard(false);
    // Clear all hands and update view
    model->clearHands();
    for (int i = 0; i < model->getPlayerCount(); i++)
    {
        const Player &player = model->getPlayer(i);
        emit playerUpdated(i, player, player.money, player.hand.getTotal());
    }
    emit dealerUpdated(model->getDealerHand(), model->getDealerHand().getTotal());

    advanceToNextBet();
}

void Controller::advanceToNextBet()
{
    // Tell the view the player bet and change their status
    if (currentPlayerIndex >= 0)
    {
        const Player &betPlayer = model->getPlayer(currentPlayerIndex);
        emit playerUpdated(currentPlayerIndex, betPlayer, betPlayer.money, betPlayer.hand.getTotal());
    }

    currentPlayerIndex++;

    // Skip all bankrupt players
    while (currentPlayerIndex < model->getPlayerCount() && model->getPlayer(currentPlayerIndex).status == PLAYERSTATUS::BANKRUPT)
    {
        currentPlayerIndex++;
    }

    // Check if everyone has bet
    if (currentPlayerIndex == model->getPlayerCount())
    {
        emit endBetting();
        dealCards();
        return;
    }

    const Player &player = model->getPlayer(currentPlayerIndex);
    emit currentPlayerTurn(currentPlayerIndex, model->getOriginalPlayer(currentPlayerIndex).money, player.hand.getBet(), player.hand.getTotal());

    if (!player.isUser)
        botBet();
}

void Controller::dealCards()
{
    currentPlayerIndex = -1;
    model->dealInitialCards();

    emit updateAllPlayers(model->getAllPlayers());
    emit showDealerCard(false);
    emit dealerUpdated(model->getDealerHand(), model->getDealerHand().getTotal());
}

void Controller::onDealingAnimationComplete()
{
    advanceToNextPlayer();
}

void Controller::botMove()
{
    // Get the correct MOVE
    const Player &player = model->getPlayer(currentPlayerIndex);
    MOVE move = botStrategy->getNextMove(player.hand, model->getDealerHand().getCards()[1]);

    unsigned int waitTime = 1000;

    // Call the correct MOVE
    if (move == MOVE::HIT)
        QTimer::singleShot(waitTime, this, [=]()
                           { onHit(); });
    else if (move == MOVE::DOUBLE)
    {
        // If not enough money or too many cards to double down, hit instead
        if (player.money < player.hand.getBet() || player.hand.getCards().size() != 2)
        {
            timer->scheduleSingleShot(waitTime, [=]()
                                      { onHit(); });
        }
        else
            timer->scheduleSingleShot(waitTime, [=]()
                                      { onDoubleDown(); });
    }
    else if (move == MOVE::SPLIT)
    {
        timer->scheduleSingleShot(waitTime, [=]()
                                  { onSplit(); });
    }
    else
        timer->scheduleSingleShot(waitTime, [=]()
                                  { onStand(); });
}

void Controller::botBet()
{
    const Player &player = model->getPlayer(currentPlayerIndex);
    int bet = std::max(player.money / 10, 1);
    timer->scheduleSingleShot(500, [=]()
                              { onBet(bet); });
}

void Controller::createNewGame(std::vector<Player> players, int decks, int deterministic)
{
    if (model != nullptr)
        delete model;
    timer->cancelAllTimers();
    model = new GameState(players, decks, deterministic);
}

void Controller::onStopEverything()
{
    timer->cancelAllTimers();
}
