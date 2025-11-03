#ifndef CARD_H
#define CARD_H

#include "suits.h"
#include "rank.h"
#include <QtCore/qobject.h>

using Rank::RANK;
using Suit::SUIT;

/**
 * @brief The Card class handles a single card object. Holds a SUIT, RANK, and image path corresponding to its png
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/19/2025
 */
class Card
{
    /**
     * @brief operator << Prints out the rank and suit of the card as "Rank of Suit"
     * @param os The stream to add the string to
     * @param card The card to print out
     * @return A string represenation of the card
     */
    friend std::ostream &operator<<(std::ostream &os, const Card &card);

public:
    /**
     * @brief Card Constructor which sets the rank and suit of the card
     * @param suit The suit of the card
     * @param rank The rank of the card
     * @param imagePath The path of the image for the card
     */
    Card(SUIT suit, RANK rank, QString imagePath);

    /**
     * @brief getSuit Gets the suit of the card
     * @return Returns the suit of the card as a SUIT
     */
    SUIT getSuit() const;

    /**
     * @brief getRank Gets the rank of the card
     * @return Returns the rank of the card as a RANK
     */
    RANK getRank() const;

    /**
     * @brief getImage Gets the QString path of the image
     * @return A QString of the path
     */
    QString getImagePath() const;

    /**
     * @brief setImagePath Sets the QString path of the image
     * @param imagePath The new imagePath
     */
    void setImagePath(QString imagePath);

    /**
     * @brief getImageFromVector Gets the corresponding image path from the private member variable images
     * The images are layed out in this rank order 2, 3, 4, 5, 6, 7, 8, 9, 10, jack, queen, king, ace.
     * The images are layoud out in this suit order spades, hearts, clubs, diamonds
     * @param index The index to get
     * @return
     */
    static QString getImageFromVector(unsigned int index);

    /**
     * @brief toString Gives the string output of this card as Rank of Suit
     * @return "Rank of Suit"
     */
    std::string toString();

private:
    /**
     * @brief suit The suit of the card
     */
    SUIT suit;

    /**
     * @brief rank The rank of the card
     */
    RANK rank;

    /**
     * @brief imagePath The image path of the card
     */
    QString imagePath;

    /**
     * @brief images A vector of alll the card images
     */
    static std::vector<QString> images;
};

#endif // CARD_H
