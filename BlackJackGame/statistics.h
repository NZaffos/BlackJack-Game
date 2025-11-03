#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>

namespace Move
{

    /**
     * @brief The MOVE enum An enum for the moves of backjack
     *
     * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins, John Chen
     * @date 4/20/2025
     */
    enum class MOVE
    {
        HIT,
        DOUBLE,
        SPLIT,
        STAND
    };

    /**
     * @brief toString Converts a MOVE to a string
     * @param move The MOVE to convert
     * @return A string of the MOVE provided
     */
    inline std::string toString(MOVE move)
    {
        switch (move)
        {
        case MOVE::HIT:
            return "Hit";
        case MOVE::DOUBLE:
            return "Double";
        case MOVE::SPLIT:
            return "Split";
        case MOVE::STAND:
            return "Stand";
        }

        return "Unknown move";
    }
}

using Move::MOVE;

/**
 * @brief The Statistics class A class that holds the tables of correct moves for blackjack
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/20/2025
 */
class Statistics
{

public:
    /**
     * @brief Statistics Constructor for the Statisitics class
     */
    explicit Statistics();

    /**
     * @brief HardTable The table for hard hand's correct moves
     */
    static const MOVE HardTable[17][10];

    /**
     * @brief SoftTable The table for soft hand's correct moves
     */
    static const MOVE SoftTable[9][10];

    /**
     * @brief PairTable The table for pair hand's correct moves
     */
    static const MOVE PairTable[10][10];
};

#endif // STATISTICS_H
