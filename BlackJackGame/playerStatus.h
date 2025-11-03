#ifndef PLAYERSTATUS_H
#define PLAYERSTATUS_H

#include <string>

namespace PlayerStatus
{

    /**
     * @brief The PLAYERSTATUS enum An enum for all the possible blackjack states a player could be in
     *
     * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
     * @date 4/20/2025
     */
    enum class PLAYERSTATUS
    {
        ACTIVE,
        BUST,
        STAND,
        WAITING,
        BETSUBMITTED,
        BANKRUPT,
        PUSHED,
        WON,
        BLACKJACK,
        LOST
    };

    /**
     * @brief toString Converts a PLAYERSTATUS to a string
     * @param PLAYERSTATUS The PLAYERSTATUS to convert
     * @return A string of the PLAYERSTATUS provided
     */
    inline std::string toString(PLAYERSTATUS playerStatus)
    {
        switch (playerStatus)
        {
        case PLAYERSTATUS::ACTIVE:
            return "Active";
        case PLAYERSTATUS::BUST:
            return "Bust";
        case PLAYERSTATUS::WAITING:
            return "Waiting";
        case PLAYERSTATUS::STAND:
            return "Stood";
        case PLAYERSTATUS::BANKRUPT:
            return "Bankrupt";
        case PLAYERSTATUS::BETSUBMITTED:
            return "Bet Submitted";
        case PLAYERSTATUS::PUSHED:
            return "Pushed";
        case PLAYERSTATUS::WON:
            return "Won";
        case PLAYERSTATUS::LOST:
            return "Lost";
        case PLAYERSTATUS::BLACKJACK:
            return "Blackjack";
        }

        return "Unknown player status";
    }

    /**
     * @brief operator << Prints out the string of the PLAYERSTATUS when used with the ostream operator
     * @param os The stream to add the string to
     * @param PLAYERSTATUS The PLAYERSTATUS to convert
     * @return The ostream with the PLAYERSTATUS as a string
     */
    inline std::ostream &operator<<(std::ostream &os, PLAYERSTATUS playerStatus)
    {
        return os << toString(playerStatus);
    }

    /**
     * @brief allPLAYERSTATUSs An array of all the PLAYERSTATUS values
     */
    inline constexpr PLAYERSTATUS allPlayerStatus[] = {
        PLAYERSTATUS::STAND,
        PLAYERSTATUS::ACTIVE,
        PLAYERSTATUS::WAITING,
        PLAYERSTATUS::BUST,
        PLAYERSTATUS::BETSUBMITTED,
        PLAYERSTATUS::BANKRUPT,
        PLAYERSTATUS::PUSHED,
        PLAYERSTATUS::WON,
        PLAYERSTATUS::BLACKJACK,
        PLAYERSTATUS::LOST};

}
#endif // PLAYERSTATUS_H
