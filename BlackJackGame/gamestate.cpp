/**
 * @brief Implementation of The GameState class. It is the core model of the blackjack game. Handles all game logic
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/21/2025
 */

#include "gamestate.h"
#include "playerStatus.h"

using PlayerStatus::PLAYERSTATUS;

GameState::GameState(std::vector<Player> players, int deckCount, int deterministic) : players(players), deck(deckCount, deterministic), dealerHand(0) {}

void GameState::dealInitialCards()
{
    deck.shuffle();

    // Deal 2 cards to each player and set their status to waiting
    for (int i = 0; i < 2; i++)
    {
        for (Player &player : players)
        {
            if (player.status == PLAYERSTATUS::BANKRUPT)
                continue;

            player.hand.addCard(deck.getNextCard());
            player.status = PLAYERSTATUS::WAITING;
        }

        dealerHand.addCard(deck.getNextCard());
    }
}

void GameState::clearHands()
{
    dealerHand = Hand(0);

    // Reset player hands and remove split hands
    for (int i = players.size() - 1; i >= 0; i--)
    {
        if (!players[i].originalHand)
            players.erase(players.begin() + i);
        else
            players[i].hand = Hand(players[i].hand.getBet());

        if (players[i].status != PLAYERSTATUS::BANKRUPT)
            players[i].status = PLAYERSTATUS::WAITING;
    }
}

void GameState::hit(int playerIndex)
{
    Player &currPlayer = players[playerIndex];
    currPlayer.hand.addCard(deck.getNextCard());
    if (isBust(currPlayer.hand))
    {
        currPlayer.status = PLAYERSTATUS::BUST;
    }
}

void GameState::doubleDown(int playerIndex)
{
    Player &currPlayer = players[playerIndex];
    Player &originalPlayer = players[playerIndex - currPlayer.playerHandIndex];

    // Move the doubled money from the player's money to their current bet
    int currentBet = currPlayer.hand.getBet();
    originalPlayer.money -= currentBet;
    currPlayer.hand.setBet(currentBet * 2);

    // Do one hit and then stand/check bust
    currPlayer.hand.addCard(deck.getNextCard());
    currPlayer.status = PLAYERSTATUS::STAND;
    if (isBust(currPlayer.hand))
    {
        currPlayer.status = PLAYERSTATUS::BUST;
    }
}

void GameState::stand(int playerIndex)
{
    Player &currPlayer = players[playerIndex];
    currPlayer.status = PLAYERSTATUS::STAND;
}

void GameState::split(int playerIndex)
{
    Player &currPlayer = players[playerIndex];
    Player &originalPlayer = players[playerIndex - currPlayer.playerHandIndex];
    // Get the index of the current hand
    int handIndex = 1;
    for (int i = playerIndex; i >= 0; i--)
    {
        if (players[i].originalHand)
        {
            players[i].playerHandCount++;
            break;
        }
        handIndex++;
    }

    Player secondHandPlayer = Player(0, currPlayer.hand.getBet(), currPlayer.isUser, 0, handIndex);

    // Remove money for new bet from current player
    originalPlayer.money -= currPlayer.hand.getBet();

    // Splits the hand of the original player and hits once for original and new hand
    const Card &removedCard = currPlayer.hand.removeLastCard();
    secondHandPlayer.hand.addCard(removedCard);
    currPlayer.hand.addCard(deck.getNextCard());
    secondHandPlayer.hand.addCard(deck.getNextCard());

    // Set status of players
    if (removedCard.getRank() == RANK::ACE)
    {
        currPlayer.status = PLAYERSTATUS::STAND;
        secondHandPlayer.status = PLAYERSTATUS::STAND;
    }
    else
    {
        currPlayer.status = PLAYERSTATUS::ACTIVE;
        secondHandPlayer.status = PLAYERSTATUS::WAITING;
    }

    players.insert(players.begin() + playerIndex + 1, secondHandPlayer);
}

void GameState::dealerPlay()
{
    while (dealerHand.getTotal() < 17)
        dealerHand.addCard(deck.getNextCard());
}

bool GameState::isBust(const Hand &hand) const
{
    return hand.getTotal() > 21;
}

void GameState::endRound()
{
    int dealerTotal = dealerHand.getTotal();
    bool dealerBust = isBust(dealerHand);

    for (int i = 0; i < (int)players.size(); i++)
    {
        Player &player = players[i];
        Player &originalPlayer = players[i - player.playerHandIndex];
        // If player busts, they either simply lose or go bankrupt
        if (player.status == PLAYERSTATUS::BUST)
        {
            player.status = PLAYERSTATUS::LOST;
            if (player.money <= 0)
                player.status = PLAYERSTATUS::BANKRUPT;
            continue;
        }

        int playerTotal = player.hand.getTotal();
        if (dealerBust || playerTotal > dealerTotal)
        {
            // If player gets blackjack, player gets 2.5 times their bet
            if (playerTotal == 21 && player.hand.getCards().size() == 2)
            {
                originalPlayer.money += player.hand.getBet() * 2.5;

                player.status = PLAYERSTATUS::BLACKJACK;
            }
            // Else, player won and doubles their bet
            else
            {
                originalPlayer.money += player.hand.getBet() * 2;

                player.status = PLAYERSTATUS::WON;
            }
        }
        // Player gets their money back if they have the same total
        else if (playerTotal == dealerTotal)
        {
            originalPlayer.money += player.hand.getBet();
            player.status = PLAYERSTATUS::PUSHED;
        }
        // Player loses
        else
        {
            player.status = PLAYERSTATUS::LOST;
            if (player.money <= 0)
                player.status = PLAYERSTATUS::BANKRUPT;
        }
    }
}

void GameState::setPlayerActive(int index)
{
    players[index].status = PLAYERSTATUS::ACTIVE;
}

void GameState::setPlayerBet(int index, int amount)
{
    players[index].money -= amount;
    players[index].hand.setBet(amount);

    // Set the status to bet submitted if not bankrupt
    if (players[index].status != PLAYERSTATUS::BANKRUPT)
        players[index].status = PLAYERSTATUS::BETSUBMITTED;
}

const Player &GameState::getPlayer(int index) const
{
    return players.at(index);
}

const Player &GameState::getOriginalPlayer(int index) const
{
    return players[index - players[index].playerHandIndex];
}

const Hand &GameState::getDealerHand() const
{
    return dealerHand;
}

int GameState::getPlayerCount() const
{
    return static_cast<int>(players.size());
}

const std::vector<Player> GameState::getAllPlayers() const
{
    return players;
}
