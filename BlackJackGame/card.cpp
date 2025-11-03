/**
 * @brief Implementation of The Card class. It handles a single card object. Holds a SUIT, RANK, and image path corresponding to its png
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/19/2025
 */

#include "card.h"
#include "suits.h"
#include "rank.h"

Card::Card(SUIT suit, RANK rank, QString imagePath) : suit(suit), rank(rank), imagePath(imagePath) {}

std::vector<QString> Card::images = {
    ":/cardImages/cards_pngsource/2_of_spades.png",
    ":/cardImages/cards_pngsource/2_of_hearts.png",
    ":/cardImages/cards_pngsource/2_of_clubs.png",
    ":/cardImages/cards_pngsource/2_of_diamonds.png",

    ":/cardImages/cards_pngsource/3_of_spades.png",
    ":/cardImages/cards_pngsource/3_of_hearts.png",
    ":/cardImages/cards_pngsource/3_of_clubs.png",
    ":/cardImages/cards_pngsource/3_of_diamonds.png",

    ":/cardImages/cards_pngsource/4_of_spades.png",
    ":/cardImages/cards_pngsource/4_of_hearts.png",
    ":/cardImages/cards_pngsource/4_of_clubs.png",
    ":/cardImages/cards_pngsource/4_of_diamonds.png",

    ":/cardImages/cards_pngsource/5_of_spades.png",
    ":/cardImages/cards_pngsource/5_of_hearts.png",
    ":/cardImages/cards_pngsource/5_of_clubs.png",
    ":/cardImages/cards_pngsource/5_of_diamonds.png",

    ":/cardImages/cards_pngsource/6_of_spades.png",
    ":/cardImages/cards_pngsource/6_of_hearts.png",
    ":/cardImages/cards_pngsource/6_of_clubs.png",
    ":/cardImages/cards_pngsource/6_of_diamonds.png",

    ":/cardImages/cards_pngsource/7_of_spades.png",
    ":/cardImages/cards_pngsource/7_of_hearts.png",
    ":/cardImages/cards_pngsource/7_of_clubs.png",
    ":/cardImages/cards_pngsource/7_of_diamonds.png",

    ":/cardImages/cards_pngsource/8_of_spades.png",
    ":/cardImages/cards_pngsource/8_of_hearts.png",
    ":/cardImages/cards_pngsource/8_of_clubs.png",
    ":/cardImages/cards_pngsource/8_of_diamonds.png",

    ":/cardImages/cards_pngsource/9_of_spades.png",
    ":/cardImages/cards_pngsource/9_of_hearts.png",
    ":/cardImages/cards_pngsource/9_of_clubs.png",
    ":/cardImages/cards_pngsource/9_of_diamonds.png",

    ":/cardImages/cards_pngsource/10_of_spades.png",
    ":/cardImages/cards_pngsource/10_of_hearts.png",
    ":/cardImages/cards_pngsource/10_of_clubs.png",
    ":/cardImages/cards_pngsource/10_of_diamonds.png",

    ":/cardImages/cards_pngsource/jack_of_spades.png",
    ":/cardImages/cards_pngsource/jack_of_hearts.png",
    ":/cardImages/cards_pngsource/jack_of_clubs.png",
    ":/cardImages/cards_pngsource/jack_of_diamonds.png",

    ":/cardImages/cards_pngsource/queen_of_spades.png",
    ":/cardImages/cards_pngsource/queen_of_hearts.png",
    ":/cardImages/cards_pngsource/queen_of_clubs.png",
    ":/cardImages/cards_pngsource/queen_of_diamonds.png",

    ":/cardImages/cards_pngsource/king_of_spades.png",
    ":/cardImages/cards_pngsource/king_of_hearts.png",
    ":/cardImages/cards_pngsource/king_of_clubs.png",
    ":/cardImages/cards_pngsource/king_of_diamonds.png",

    ":/cardImages/cards_pngsource/ace_of_spades.png",
    ":/cardImages/cards_pngsource/ace_of_hearts.png",
    ":/cardImages/cards_pngsource/ace_of_clubs.png",
    ":/cardImages/cards_pngsource/ace_of_diamonds.png"};

SUIT Card::getSuit() const
{
    return suit;
}

RANK Card::getRank() const
{
    return rank;
}

QString Card::getImagePath() const
{
    return imagePath;
}

void Card::setImagePath(QString imagePath)
{
    this->imagePath = imagePath;
}

QString Card::getImageFromVector(unsigned int index)
{
    return images[index];
}

std::string Card::toString()
{
    return Rank::toString(rank) + " of " + Suit::toString(suit);
}

std::ostream &operator<<(std::ostream &os, const Card &card)
{
    os << toString(card.rank) << " of " << toString(card.suit);
    return os;
}
