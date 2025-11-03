#ifndef TUTORIALPOPUP_H
#define TUTORIALPOPUP_H

#include "ui_mainwindow.h"
#include "statistics.h"

// Forward Declaration
using Move::MOVE;

/**
 * @brief The TutorialPopup class handles displaying informational popup messages for the tutorial.
 * Additionally, handles activating the correct gameplay button after a tip message to control game flow
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/22/2025
 */
class TutorialPopup : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief TutorialPopup Creates the popup message class for the tutorial
     * @param ui The ui holding all elements on the screen
     */
    explicit TutorialPopup(Ui::MainWindow *ui, QString qWidgetStyle, QString qPushButtonStyle, QObject *parent = nullptr);

    /**
     * @brief Deconstructor
     */
    ~TutorialPopup();

    /**
     * @brief showNextPopup Shows the next popup message on screen
     * @param show A bool for the state of visability for the tutorial popup
     */
    void toggleVisableTutorialPopup(bool show);

    /**
     * @brief nextIsTip Gets if the next message will be a tip message
     * @return True if the next message is a tip, false otherwise
     */
    bool nextIsTip() const;

signals:
    /**
     * @brief sendNextRound Signal to move to the next round of blackjack
     */
    void sendNextRound();

    /**
     * @brief enableButton Signal to set the passed button to enabled
     * @param button The button to change
     * @param enabled The value of true for on, or false for off
     */
    void enableButton(QPushButton *button, bool enabled);

    /**
     * @brief toggleFirstTutorialRound Signal to toggle first tutorial round
     * @param enabled The state to set first tutorial round
     */
    void toggleTutorialFirstRound(bool enabled);

    /**
     * @brief backToMainMenu Signal to move back to the main menu
     */
    void backToMainMenu();

public slots:
    /**
     * @brief onContinuePressed Slot for when the continue button is pressed on the popup page
     */
    void onContinuePressed();

    /**
     * @brief resetAndHideTutorial Hides the popup and resets to the start
     */
    void resetAndHideTutorial();

    /**
     * @brief onDealAnimationComplete Starts the tutorial popup once received for the first round
     */
    void onDealAnimationComplete();

private:
    /**
     * @brief ui The mainwindow ui holding all on screen elements
     */
    Ui::MainWindow *ui;

    /**
     * @brief popupMessages A vector containing all of the popup messages
     */
    std::vector<QString> *popupMessages;

    /**
     * @brief toggleButtonOrder A vector containing the MOVE at each message popup so the correct button is turned on
     */
    std::vector<MOVE> *moveOrder;

    /**
     * @brief storedButton The next button to activate
     */
    QPushButton *storedButton = nullptr;

    /**
     * @brief messageIndex The current index in the messages list
     */
    int messageIndex;

    /**
     * @brief moveIndex The current index in the moves list
     */
    int moveIndex;

    /**
     * @brief createMessages Helper method to create all of the popup messages and add them to the list
     */
    void createMessages();

    /**
     * @brief setUpTutorialButtons Styalizes the tutorial buttons
     */
    void setUpTutorialButtons(QString qWidgetStyle, QString qPushButtonStyle);

    /**
     * @brief firstTutorialRound True when it is the first tutorial round, false afterwards
     */
    bool firstTutorialRound = true;
};

#endif // TUTORIALPOPUP_H
