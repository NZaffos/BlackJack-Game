#ifndef RANK_H
#define RANK_H

#include <string>
#include <ostream>

namespace Rank
{

    /**
     * @brief RANK An enum of card ranks
     *
     * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
     * @date 4/13/2025
     */
    enum class RANK
    {
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        ACE
    };

    /**
     * @brief toString Converts a RANK to a string
     * @param rank The RANK to convert
     * @return A string of the rank provided
     */
    inline std::string toString(RANK rank)
    {
        switch (rank)
        {
        case RANK::TWO:
            return "Two";
        case RANK::THREE:
            return "Three";
        case RANK::FOUR:
            return "Four";
        case RANK::FIVE:
            return "Five";
        case RANK::SIX:
            return "Six";
        case RANK::SEVEN:
            return "Seven";
        case RANK::EIGHT:
            return "Eight";
        case RANK::NINE:
            return "Nine";
        case RANK::TEN:
            return "Ten";
        case RANK::JACK:
            return "Jack";
        case RANK::QUEEN:
            return "Queen";
        case RANK::KING:
            return "King";
        case RANK::ACE:
            return "Ace";
        }

        return "Unknown rank";
    }

    /**
     * @brief blackjackValue Returns the standard Blackjack value of a rank.
     * @param rank The RANK
     * @return The numeric value (2 - 10, 10 for J/Q/K, 11 for Ace)
     */
    inline int blackjackValue(RANK rank)
    {
        switch (rank)
        {
        case RANK::TWO:
            return 2;
        case RANK::THREE:
            return 3;
        case RANK::FOUR:
            return 4;
        case RANK::FIVE:
            return 5;
        case RANK::SIX:
            return 6;
        case RANK::SEVEN:
            return 7;
        case RANK::EIGHT:
            return 8;
        case RANK::NINE:
            return 9;
        case RANK::TEN:
        case RANK::JACK:
        case RANK::QUEEN:
        case RANK::KING:
            return 10;
        case RANK::ACE:
            return 11;
        }

        return 0;
    }

    /**
     * @brief operator << Prints out the string of the RANK when used with the ostream operator
     * @param os The stream to add the string to
     * @param rank The rank to convert
     * @return The ostream with the rank as a string
     */
    inline std::ostream &operator<<(std::ostream &os, RANK rank)
    {
        return os << toString(rank);
    }

    /**
     * @brief allRanks An array of all card ranks
     */
    inline constexpr RANK allRanks[] = {
        RANK::TWO,
        RANK::THREE,
        RANK::FOUR,
        RANK::FIVE,
        RANK::SIX,
        RANK::SEVEN,
        RANK::EIGHT,
        RANK::NINE,
        RANK::TEN,
        RANK::JACK,
        RANK::QUEEN,
        RANK::KING,
        RANK::ACE};

}
#endif // RANK_H
