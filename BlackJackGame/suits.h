#ifndef SUITS_H
#define SUITS_H

#include <string>

namespace Suit
{

    /**
     * @brief The SUIT enum An enum for the suits of playing cards
     *
     * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
     * @date 4/13/2025
     */
    enum class SUIT
    {
        SPADES,
        HEARTS,
        CLUBS,
        DIAMONDS
    };

    /**
     * @brief to_string Converts a SUIT to a string
     * @param suit The SUIT to convert
     * @return A string of the SUIT provided
     */
    inline std::string toString(SUIT suit)
    {
        switch (suit)
        {
        case SUIT::HEARTS:
            return "Hearts";
        case SUIT::DIAMONDS:
            return "Diamonds";
        case SUIT::CLUBS:
            return "Clubs";
        case SUIT::SPADES:
            return "Spades";
        }

        return "Unknown suit";
    }

    /**
     * @brief operator << Prints out the string of the SUIT when used with the ostream operator
     * @param os The stream to add the string to
     * @param suit The suit to convert
     * @return The ostream with the suit as a string
     */
    inline std::ostream &operator<<(std::ostream &os, SUIT suit)
    {
        return os << toString(suit);
    }

    /**
     * @brief allSuits An array of all the suit values
     */
    inline constexpr SUIT allSuits[] = {
        SUIT::SPADES,
        SUIT::HEARTS,
        SUIT::CLUBS,
        SUIT::DIAMONDS};

}
#endif // SUITS_H
