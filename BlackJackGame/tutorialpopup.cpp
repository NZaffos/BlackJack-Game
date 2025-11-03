/**
 * @brief Implementation of The TutorialPopup class. It handles displaying informational popup messages for the tutorial.
 * Additionally, handles activating the correct gameplay button after a tip message to control game flow
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins, John Chen
 * @date 4/22/2025
 */

#include "tutorialpopup.h"

TutorialPopup::TutorialPopup(Ui::MainWindow *ui, QString qWidgetStyle, QString qPushButtonStyle, QObject *parent) : QObject(parent), ui(ui)
{
    // Setup the tutorial
    setUpTutorialButtons(qWidgetStyle, qPushButtonStyle);
    popupMessages = new std::vector<QString>();
    moveOrder = new std::vector<MOVE>();
    createMessages();

    toggleVisableTutorialPopup(false);
}

TutorialPopup::~TutorialPopup()
{
    delete popupMessages;
    delete moveOrder;
}

void TutorialPopup::createMessages()
{
    // Create all of the tutorial messages
    const std::initializer_list<QString> tutorialMessages = {
        "Tip: The dealer has a 2, which is a bust card (2-6), but it is too small to justify standing when you also have a bad total. Strategy is to hit because your odds to bust are lower than the dealer's advantage", // Add messages in order here
        "Lesson: Even though you bust, it was still the right play. Long-term odds favor hitting on 12 vs 2",                                                                                                              // scenario one done
        "Tip: The dealer has a 6, which is a bust card, but is a strong bust card (3-6). The dealer is likely to bust, so you should stand if you are over 11",
        "Lesson: When the dealer shows 3-6 and you have greater than 11, you don't risk your own hand and know they have to hit and hope they bust", // scenario 2 done
        "Tip: With 2 aces, both are likely to give you 2 strong hands if you split. Basic strategy is to always split aces",
        "Lesson: One hand became 19, the other 18. Both became good hands instead of one bad 12 hand", // scenario 3 done
        "Tip: You already have a strong 20 hand, splitting risks turning a strong hand into 2 weaker ones. Dealer has a strong bust card (6) anyways. Strategy says to stand",
        "Lesson: Don't mess with a great hand, especially when a dealer is likely to bust. Never split 10's", // scenario 4 done
        "Tip: 11 is the best hand to double on. Most cards will get you close to or at 21",
        "Lesson: Always double on 11", // scenario 5 done
        "Tip: Soft 18 isn't great vs a dealer 9 or 10. Doubling only gets you one card and you might be stuck at a worse position. Strategy is to hit if the dealer's card is greater than yours and over 7 as long as you are under 17",
        "Tip: You now have 21, always stand",
        "Lesson: Don't overcommit against strong dealer upcard",                               // scenario 6 done
        "Lesson: Sometimes there's no escaping a loss. Unfortunetly, that's part of the game", // scenario 7 done
        "Tip: Soft 17 is a weak hand, treat it more like 7 than 17. Since the dealer has a 9, strategy says to hit",
        "Tip: 19 is a strong hand, strategy says to now stand",
        "Lesson: Keep hitting soft 17 until you reach over the dealer's card with a 10 (19) or until hard 17", // scenario 8 done
        "Tip: Any 8's should be split, hard 16 is the worst possible hand since it is still considered a weak hand but closest to busting. Two chances at 18+ beats one guarenteed 16",
        "Tip: While normally doubling on 11, you cannot double after splitting. Strategy says to hit since no change at busting",
        "Tip: you now have 21, always stand",
        "Tip: Strategy says to hit on a 10.",
        "Tip: You never hit once at or above hard 17, strategy is to stand",
        "Lesson: First hand reaches 21 and wins, while the other reaches 18 pushes the dealer's 18. ALways split 8's, turns a horrible 17 into 2 possibly good hands", // scenario 9
        "Tip: A hard 18 is strong vs a dealer 7. The dealer is not allowed to hit once he reaches 17. Basic strategy is to stand on a pair of 9's against a 7 (only split when the dealer has 2-6, and 8-9)",
        "Lesson: Know when not to split. Nine pairs are an exception to the 'always split pairs'", // scenario 10 done
        "Tip: Soft 18 vs a weak dealer card (2-6) is a prime doubling spot. You can't bust and you'll often end up around 20",
        "Lesson: Don't treat soft 18 or soft 17 like a regular 18/17 vs a dealer 2-6, it's a double opportunity", // scenario 11 done
        "Tip: The dealer has a 7, which is a strong face up card. You as the player should always hit until you reach 17 if the dealer has a strong card",
        "Tip: You reached 21! Always stand once at or above hard 17",
        "Lesson: Never stand on a 16 vs a dealer 7 or higher. Take the risk to reduce the dealer's advantage", // scenario 12 done
        "Tip: You should always stand on any hard 17 or higher. Hitting a 17 has too high of a chance to bust, and one hit won't guarentee beeing the dealer's 10",
        "Lesson: Hard 17 is a 'stand no exception' hand, even vs a dealer 10. You still managed to push which is a wash of a round",                                                  // scenario 13 done
        "Tutorial Complete: You have now finished the tutorial, pressing continue will bring you back to the main menu. There is a practice mode to help continue learning blackjack" // main menu
    };

    // Create all the tutorial moves
    const std::initializer_list<MOVE> tutorialButtonMoves = {
        MOVE::HIT,    // Add moves in order here || scenario 1 done
        MOVE::STAND,  // scenario 2 done
        MOVE::SPLIT,  // scenario 3 done
        MOVE::STAND,  // scenario 4 done
        MOVE::DOUBLE, // scenario 5 done
        MOVE::HIT,
        MOVE::STAND, // scenario 6 done
        // Dealer blackjack so no move here || scenario 7 done
        MOVE::HIT,
        MOVE::STAND, // scenario 8 done
        MOVE::SPLIT,
        MOVE::HIT,
        MOVE::STAND,
        MOVE::HIT,
        MOVE::STAND,  // scenario 9 done
        MOVE::STAND,  // scenario 10 done
        MOVE::DOUBLE, // scenario 11 done
        MOVE::HIT,
        MOVE::STAND, // scenario 12 done
        MOVE::STAND  // scenario 13 done
    };

    for (const QString &message : tutorialMessages)
    {
        popupMessages->push_back(message);
    }

    for (const MOVE &move : tutorialButtonMoves)
    {
        moveOrder->push_back(move);
    }
    messageIndex = 0;
    moveIndex = 0;
}

void TutorialPopup::toggleVisableTutorialPopup(bool show)
{
    if (show)
    {
        ui->tutorialLabel->setText(popupMessages->at(messageIndex));
        ui->tutorialWidget->show();
    }
    else
    {
        ui->tutorialWidget->hide();
    }
}

bool TutorialPopup::nextIsTip() const
{
    return popupMessages->at(messageIndex).startsWith("Tip");
}

void TutorialPopup::onContinuePressed()
{
    toggleVisableTutorialPopup(false);

    const QString &currentMessage = popupMessages->at(messageIndex);

    // If the next message will be a tip for a move
    if (currentMessage.startsWith("Tip"))
    {
        // it’s a Tip, disable everything, then enable the one move button
        emit enableButton(ui->hitButton, false);
        emit enableButton(ui->standButton, false);
        emit enableButton(ui->doubleButton, false);
        emit enableButton(ui->splitButton, false);

        // look up which MOVE to enable
        MOVE move = moveOrder->at(moveIndex++);
        QPushButton *button = nullptr;
        switch (move)
        {
        case MOVE::HIT:
            button = ui->hitButton;
            break;
        case MOVE::STAND:
            button = ui->standButton;
            break;
        case MOVE::DOUBLE:
            button = ui->doubleButton;
            break;
        case MOVE::SPLIT:
            button = ui->splitButton;
            break;
        }
        if (button)
        {
            if (!firstTutorialRound)
                emit enableButton(button, true);
            else
                storedButton = button;
        }
    }
    // If the next message will be a lesson at the end of the round
    else if (currentMessage.startsWith("Lesson"))
    {
        // it’s a Lesson so disable all play buttons
        emit enableButton(ui->hitButton, false);
        emit enableButton(ui->standButton, false);
        emit enableButton(ui->doubleButton, false);
        emit enableButton(ui->splitButton, false);

        // This is the last lesson, so show the final message
        if (popupMessages->at(messageIndex + 1).startsWith("Tutorial"))
        {
            messageIndex++;
            toggleVisableTutorialPopup(true);
            return;
        }
        // End of round, start next round
        else
            emit sendNextRound();
    }
    // On last message, go back to main screen
    else
    {
        emit backToMainMenu();
        return;
    }

    if (messageIndex < (int)popupMessages->size() - 1)
    {
        messageIndex++;
    }
}

void TutorialPopup::resetAndHideTutorial()
{
    // Reset all variables to default and hide popup
    toggleVisableTutorialPopup(false);
    storedButton = nullptr;
    firstTutorialRound = true;
    messageIndex = 0;
    moveIndex = 0;
}

void TutorialPopup::setUpTutorialButtons(QString qWidgetStyle, QString qPushButtonStyle)
{
    QString qLabelStyle =
        "QLabel {"
        "    background-color: transparent;"
        "    border: none;"
        "    color: white;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    padding: 6px;"
        "}";

    ui->tutorialWidget->setStyleSheet(qWidgetStyle);
    ui->tutorialContinueButton->setStyleSheet(qPushButtonStyle);
    ui->tutorialLabel->setStyleSheet(qLabelStyle);
}

void TutorialPopup::onDealAnimationComplete()
{
    // Delay the activation of the gameplay buttons for the first round
    if (firstTutorialRound && storedButton)
    {
        emit enableButton(storedButton, true);
    }
    firstTutorialRound = false;
}
