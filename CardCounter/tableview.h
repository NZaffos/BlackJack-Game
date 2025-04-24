#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "timermanager.h"
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QAudioOutput>

// Forward Declaration
class AnimatableCardItem;

/**
 * @brief The TableView class is responsible for animating and displaying the player and dealer cards on the blackjack table
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/22/2025
 */
class TableView : public QGraphicsView {
    Q_OBJECT
public:
    /**
     * @brief TableView Constructor for the TableView
     * @param parent The parent object
     */
    explicit TableView(QWidget *parent = nullptr);

    /**
     * @brief ~TableView Deconstructor for TableView
     */
    ~TableView();

    /**
     * @brief createPlayerCardContainers Creates the vectors to hold the AnimatableCardItems
     * @param playerCount Number of containers to make
     */
    void createPlayerCardContainers(unsigned int playerCount);

    /**
     * @brief addPlayerHandContainerAt Add a new container at a certian index
     * @param playerIndex The index to add the container
     * @param handIndex Which hand the container needs to be added to
     */
    void addPlayerHandContainerAt(unsigned int playerIndex, unsigned int handIndex);

    /**
     * @brief splitPlayerHand Splits the AnimatableCardItems inside a players hand into their other hands
     * @param playerIndex The player to modify
     * @param handIndex The hand to split
     */
    void splitPlayerHand(unsigned int playerIndex, unsigned int handIndex);

    /**
     * @brief getCardEndPosition Gets the ending position of the AnimatableCardItem
     * @param playerIndex The player's card's end position
     * @param handIndex The hand that holds that card
     * @param cardIndex The index of the card in the hand
     * @return QPointF A point (x, y) on the table where the card ends
     */
    QPointF getCardEndPosition(int playerIndex, int handIndex, int cardIndex);

    /**
     * @brief getCardEndRotation Gets the ending rotation of the AnimateableCardItem
     * @param playerIndex The player's card's end rotation
     * @param handIndex The hand that holds that card
     * @return qreal The rotation of the card
     */
    qreal getCardEndRotation(int playerIndex, int handIndex);

    /**
     * @brief addCardAnimated Adds a card to the TableView and animates it
     * @param playerIndex The containers index to add to
     * @param handIndex The container's hand index to add to
     * @param imagePath The path of the image to create the card visual
     * @param startPos The start pos of the card
     * @param endPos The end pos of the card
     * @param rotationAngle The end rotation of the card
     */
    void addCardAnimated(int playerIndex, int handIndex, const QString& imagePath, QPointF startPos, QPointF endPos, qreal rotationAngle);

    /**
     * @brief addPlayerCardAt Adds a card to the TableView without animation
     * @param playerIndex The containers index to add to
     * @param handIndex The container's hand index to add to
     * @param imagePath The path of the image to create the card visual
     * @param pos The position to add the card to the table
     * @param rotationAngle The rotation of the card
     */
    void addPlayerCardAt(int playerIndex, int handIndex, const QString& imagePath, QPointF pos, qreal rotationAngle);

    /**
     * @brief addDealerCardAt Adds a card for the dealer
     * @param imagePath The path of the image to create the card visual
     * @param pos The position to add the card to the table
     * @param rotationAngle The rotation of the card
     */
    void addDealerCardAt(const QString& imagePath, QPointF pos, qreal rotationAngle);

    /**
     * @brief createDealerPile Creates a pile of cards for the dealer
     */
    void createDealerPile();

    /**
     * @brief revealDealerCard Replaces the dealers upsidown card with the real image
     * @param imagePath The front of the card to reveal
     */
    void revealDealerCard(const QString &imagePath);

    /**
     * @brief createCardItem Creates the inital AnimatableCardItem for the animation
     * @param imagePath The path of the image to create the card visual
     * @param startPos The start pos of the card
     * @param rotationAngle The rotation of the card
     * @param setShadow A bool for if the card should have shadow or not
     * @return AnimatableCardItem* A new AnimatableCardItem for either placing or animating
     */
    AnimatableCardItem* createCardItem(const QString& imagePath, QPointF startPos, qreal rotationAngle, bool setShadow);

    /**
     * @brief createAnimationCardItem Uses an AnimatableCardItem to start the animation
     * @param cardItem The AnimatableCardItem to apply an animation to
     * @param startPos The start pos of the card
     * @param endPos The end pos of the card
     * @param startRotation The starting rotation of the card
     * @param endRotation The ending rotation of the card
     * @return QParallelAnimationGroup* Returns a group of animations
     */
    QParallelAnimationGroup* createAnimationCardItem(AnimatableCardItem* cardItem, QPointF startPos, QPointF endPos, qreal startRotation, qreal endRotation);

    /**
     * @brief clearTable Clears all objects from the table
     */
    void clearTable();

    /**
     * @brief stopEverything Stops all timers and animations
     */
    void stopEverything();

private:
    /**
     * @brief playerCards Vector of players, hands of the players, and cards in said hands
     */
    std::vector<std::vector<std::vector<AnimatableCardItem*>>> playerCards;

    /**
     * @brief dealerCards Vector of the dealers cards
     */
    std::vector<AnimatableCardItem*> dealerCards;

    /**
     * @brief sizeX The x size of the card to scale down to
     */
    unsigned int sizeX = 63;

    /**
     * @brief sizeY The y size of the card to scale down to
     */
    unsigned int sizeY = 91;

    /**
     * @brief setUpTableViewConnects Creates all the QT connect statements for the TableView
     */
    void setUpTableViewConnects();

    /**
     * @brief scene QGraphicsScene responsible for displaying the table
     */
    QGraphicsScene* scene;

    /**
     * @brief tableBackground The background of the tableView
     */
    QGraphicsPixmapItem* tableBackground;

    /**
     * @brief setPlayerTextPositionsOnTable Sets the text of the players on the table
     */
    void setPlayerTextPositionsOnTable();

    /**
     * @brief applyShadowToWidget Applies a shadow effect to a card
     * @param card The card to apply the shadow to
     */
    void applyShadowToCard(AnimatableCardItem *card);

    /**
     * @brief updateCardPosition Moves a card from one point to another
     * @param playerIndex The player to index
     * @param oldHandIndex The player's old hand index
     * @param oldCardIndex The old card in the old hand;s index
     * @param newHandIndex The player's new hand index
     * @param newCardIndex The new card in the new hand's index
     */
    void updateCardPosition(unsigned int playerIndex, unsigned int oldHandIndex, unsigned int oldCardIndex, unsigned int newHandIndex, unsigned int newCardIndex);

    /**
     * @brief timer An instance of the timer class
     */
    TimerManager *timer;

    /**
     * @brief shuffleNoise Stores a reference to the scard shuffling noise to play it
     */
    QMediaPlayer *shuffleNoise;

    /**
     * @brief dealerdeckNoise The audio that plays when the dealer piles the deck at the beginning
     */
    QMediaPlayer *dealerdeckNoise;

    QMediaPlayer *buttonclick;

    /**
     * @brief audioOutput Connects the output device to play the audio/MediaPlayer file attached
     */
    QAudioOutput *audioOutput = new QAudioOutput(this);
};

/**
 * @brief The AnimatableCardItem class
 *
 * This class is a helper for animating playing cards
 * Uses properties like position (`pos`) and rotation (`rotation`) via
 * Q_PROPERTY, to assist in animations
 */
class AnimatableCardItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

    // Enable animation of the item's position using QPropertyAnimation
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

    // Enable animation of the item's rotation using QPropertyAnimation
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)

public:
    /**
     * @brief Constructor for AnimatableCardItem
     * @param pixmap The image to display for the item
     * @param parent The parent graphics item (optional)
     */
    AnimatableCardItem(const QPixmap& pixmap, QGraphicsItem* parent = nullptr)
        : QObject(), QGraphicsPixmapItem(pixmap, parent) {}
};

#endif // TABLEVIEW_H
