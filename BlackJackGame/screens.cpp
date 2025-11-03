/**
 * @brief Implementation of The Screens class. It handles all main screens of the program (main menu, playing blackjack, practice blackjack, and strategy screens)
 * It is the view for playing blackjack and takes all information from controller.
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins, John Chen
 * @date 4/22/2025
 */

#include "screens.h"
#include "botstrategy.h"
#include <QPalette>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>
#include <QRandomGenerator>
#include <QtMath>

Screens::Screens(Ui::MainWindow *ui, box2Dbase *m_scene, QWidget *parent)
    : QStackedWidget(parent), ui(ui), m_scene(m_scene)
{
    // Ensure the start Screen in displayed
    moveToStartScreen();

    // Set up function calls
    setUpQStyleSheets();
    setUpTable();
    setUpStartMenuButtons();
    setUpGamePlayButtons();
    setUpSettingsPopup();
    setUpBackGround();
    toggleVisibleSettingsPopup(false);
    setUpBasicStrategyCharts();
    setUpBettingMenu();
    setUpBankruptcyMenu();
    setUpRecomendedMove();

    // Set up timer
    timer = new TimerManager();

    // Set up tutorial popup
    tutorialPopup = new TutorialPopup(ui, QWidgetStyle, QPushButtonStyleSmallFont);

    // Connects
    setUpScreenConnects();
}

Screens::~Screens()
{
    delete timer;
    delete tableView;
    delete tutorialPopup;
}

void Screens::setUpScreenConnects()
{
    // Menu Buttons
    connect(ui->blackjackPlayButton,
            &QPushButton::clicked,
            this,
            &Screens::moveToPlayScreen);
    connect(ui->blackjackPracticeButton,
            &QPushButton::clicked,
            this,
            &Screens::moveToPlayScreen);
    connect(ui->blackjackTutorialButton,
            &QPushButton::clicked,
            this,
            &Screens::moveToPlayScreen);

    // Settings buttons
    connect(ui->acceptSettingsButton,
            &QPushButton::clicked,
            this,
            [this]()
            { toggleVisibleSettingsPopup(false); });
    connect(ui->acceptSettingsButton,
            &QPushButton::clicked,
            this,
            &Screens::acceptSettingsButtonPressed);
    connect(ui->cancelSettingsButton,
            &QPushButton::clicked,
            this,
            &Screens::moveToStartScreen);
    connect(ui->playerCountSettingsSlider,
            &QSlider::valueChanged,
            this,
            &Screens::updateSettingsSlider);
    connect(ui->deckCountSettingsSlider,
            &QSlider::valueChanged,
            this,
            &Screens::updateSettingsSlider);
    connect(ui->chipCountSettingsLineEdit,
            &QLineEdit::textChanged,
            this,
            &Screens::onEditChipCountLineEdit);

    // Tutorial Pages
    connect(ui->basicStrategyChartButton,
            &QPushButton::clicked,
            this,
            &Screens::moveToInfoScreen);
    connect(ui->infoMenuButton,
            &QPushButton::clicked,
            this,
            &Screens::moveToStartScreen);

    // Gameplay Buttons
    connect(ui->hitButton,
            &QPushButton::clicked,
            this,
            &Screens::onPressHitButton);
    connect(ui->standButton,
            &QPushButton::clicked,
            this,
            &Screens::onPressStandButton);
    connect(ui->doubleButton,
            &QPushButton::clicked,
            this,
            &Screens::onPressDoubleButton);
    connect(ui->splitButton,
            &QPushButton::clicked,
            this,
            &Screens::onPressSplitButton);

    // Betting Buttons
    connect(ui->placeBetButton,
            &QPushButton::clicked,
            this,
            &Screens::onPressPlacedBetButton);
    connect(ui->betSlider,
            &QSlider::valueChanged,
            this,
            &Screens::updateBetLabelText);
    connect(ui->minimumBettingButton,
            &QPushButton::clicked,
            this,
            &Screens::onPressBettingAmountButtons);
    connect(ui->halfButton,
            &QPushButton::clicked,
            this,
            &Screens::onPressBettingAmountButtons);
    connect(ui->allInButton,
            &QPushButton::clicked,
            this,
            &Screens::onPressBettingAmountButtons);

    // Continue, leave, or play again
    connect(ui->backToMainMenuFromPlay,
            &QPushButton::clicked,
            this,
            &Screens::onPressMainMenuButton);
    connect(ui->nextRound,
            &QPushButton::clicked,
            this,
            &Screens::onPressNextRound);
    connect(ui->backToMainMenuFromPlay_2,
            &QPushButton::clicked,
            this,
            &Screens::onPressMainMenuButton);
    connect(ui->playAgain,
            &QPushButton::clicked,
            this,
            &Screens::onPressPlayAgain);
    connect(this,
            &Screens::sendStopEverything,
            tutorialPopup,
            &TutorialPopup::resetAndHideTutorial);

    // Tutorial Buttons
    connect(ui->tutorialContinueButton,
            &QPushButton::clicked,
            tutorialPopup,
            &TutorialPopup::onContinuePressed);
    connect(tutorialPopup,
            &TutorialPopup::enableButton,
            this,
            &Screens::toggleEnabledQPushButton);
    connect(tutorialPopup,
            &TutorialPopup::sendNextRound,
            this,
            &Screens::onPressNextRound);
    connect(this,
            &Screens::dealAnimationComplete,
            tutorialPopup,
            &TutorialPopup::onDealAnimationComplete);
    connect(tutorialPopup,
            &TutorialPopup::backToMainMenu,
            this,
            &Screens::onPressMainMenuButton);
}

void Screens::setUpTable()
{
    tableView = new TableView(ui->table);
    if (!ui->table->layout())
    {
        ui->table->setLayout(new QVBoxLayout());
    }

    // Set style for the scene
    tableView->setStyleSheet(QGraphicsViewStyle);
    ui->table->setStyleSheet(QWidgetStyle);
    ui->table->layout()->addWidget(tableView);

    ui->table->setStyleSheet("background: transparent; border: none;");

    // Ensure the TableView's viewport is transparent
    tableView->setAttribute(Qt::WA_TranslucentBackground);
    tableView->viewport()->setAttribute(Qt::WA_TranslucentBackground);

    ui->coinAnimView->raise();
}

void Screens::setUpStartMenuButtons()
{
    // Button Area
    ui->startMenuButtonArea->setStyleSheet(QWidgetStyle);

    // TitleCard
    ui->startMenuTitleCard->setStyleSheet(QLabelStyle);
    ui->startMenuTitleCard->setAlignment(Qt::AlignCenter);
    ui->startMenuTitleCard->setText("Blackjack\nCard Simulator");

    // Buttons
    ui->blackjackPlayButton->setStyleSheet(QPushButtonStyle);
    ui->basicStrategyChartButton->setStyleSheet(QPushButtonStyle);
    ui->blackjackPracticeButton->setStyleSheet(QPushButtonStyle);
    ui->blackjackTutorialButton->setStyleSheet(QPushButtonStyle);
}

void Screens::setUpSettingsPopup()
{
    ui->settingsPopUp->setStyleSheet(QWidgetStyle);
    applyShadowToWidget(ui->settingsPopUp);

    // Make font smaller for text boxes
    QString tempQLabelStyle =
        "QLabel {"
        "    background-color: transparent;"
        "    border: none;"
        "    color: white;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    padding: 6px;"
        "}";

    // Labels
    ui->settingsMenuTitle->setStyleSheet(QLabelStyle);
    ui->playerCountTextSettingsLabel->setStyleSheet(tempQLabelStyle);
    ui->deckCountTextSettingsLabel->setStyleSheet(tempQLabelStyle);
    ui->chipCountTextSettingsLabel->setStyleSheet(tempQLabelStyle);

    // Sliders
    ui->playerCountSettingsSlider->setStyleSheet(QSliderStyle);
    ui->deckCountSettingsSlider->setStyleSheet(QSliderStyle);

    toggleEnabledQPushButton(ui->acceptSettingsButton, false);
    ui->cancelSettingsButton->setStyleSheet(QPushButtonStyle);

    // LineEdit
    ui->chipCountSettingsLineEdit->setStyleSheet(QLineEditStyle);
    ui->chipCountSettingsLineEdit->setPlaceholderText("Chip count: (minimum $1)");
}

void Screens::setUpGamePlayButtons()
{
    // Buttons Area
    ui->gamePlayButtons->setStyleSheet(QWidgetStyle);

    // Buttons
    ui->hitButton->setStyleSheet(QPushButtonStyle);
    ui->standButton->setStyleSheet(QPushButtonStyle);
    ui->doubleButton->setStyleSheet(QPushButtonStyle);
    ui->splitButton->setStyleSheet(QPushButtonStyle);

    // Next round and menu button
    ui->nextRound->setObjectName("nextRound");
    ui->backToMainMenuFromPlay->setObjectName("backToMenuFromPlay");
    toggleEnabledQPushButton(ui->nextRound, false);
    toggleEnabledQPushButton(ui->backToMainMenuFromPlay, true);
}

void Screens::setUpBettingMenu()
{
    ui->bettingArea->setStyleSheet(QWidgetStyle);
    ui->betSlider->setStyleSheet(QSliderStyle);
    ui->betSlider->setMinimum(1);

    ui->betLabel->setStyleSheet(QBetLabelStyle);
    ui->allInButton->setStyleSheet(QPushButtonStyle);
    ui->halfButton->setStyleSheet(QPushButtonStyle);
    ui->minimumBettingButton->setStyleSheet(QPushButtonStyle);

    toggleEnabledQPushButton(ui->placeBetButton, false);

    if (mode == GAMEPLAYMODE::BLACKJACK)
        updateBetLabelText(1);
    else
        updateBetLabelText(0);
}

void Screens::updateBetLabelText(unsigned int value)
{
    ui->betLabel->setText("Bet Amount: $" + QString::number(value));
    currentBet = value;
}

void Screens::updateSettingsSlider(unsigned int value)
{
    QSlider *slider = qobject_cast<QSlider *>(sender());

    QString name = slider->objectName();

    if (name == "playerCountSettingsSlider")
    {
        ui->playerCountTextSettingsLabel->setText("Player Count: " + QString::number(value));
        playerCount = value;
    }
    else if (name == "deckCountSettingsSlider")
    {
        ui->deckCountTextSettingsLabel->setText("Deck Count: " + QString::number(value));
        deckCount = value;
    }
}

void Screens::setUpQStyleSheets()
{
    QWidgetStyle =
        "QWidget {"
        "    background-color: rgba(50, 25, 20, 200);"
        "    border-radius: 20px;"
        "    border: 1px solid rgba(255, 255, 255, 15);"
        "    padding: 16px;"
        "    color: white;"
        "    font-size: 16px;"
        "}";

    QPushButtonStyle =
        "QPushButton {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #222, stop:1 #333);"
        "    color: white;"
        "    border: 1px solid rgba(255, 255, 255, 0.2);"
        "    border-radius: 12px;"
        "    padding: 12px 24px;"
        "    font-size: 24px;"
        "    font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #444, stop:1 #555);"
        "    border: 1px solid rgba(255, 255, 255, 0.35);"
        "}"
        "QPushButton:pressed {"
        "    background-color: #111;"
        "    border: 1px solid #aaa;"
        "    padding-left: 14px;"
        "    padding-top: 14px;"
        "}";

    QPushButtonStyleSmallFont =
        "QPushButton {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #222, stop:1 #333);"
        "    color: white;"
        "    border: 1px solid rgba(255, 255, 255, 0.2);"
        "    border-radius: 12px;"
        "    padding: 12px 20px;"
        "    font-size: 18px;"
        "    font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #444, stop:1 #555);"
        "    border: 1px solid rgba(255, 255, 255, 0.35);"
        "}"
        "QPushButton:pressed {"
        "    background-color: #111;"
        "    border: 1px solid #aaa;"
        "    padding-left: 14px;"
        "    padding-top: 14px;"
        "}";

    QPushButtonDisabledStyle =
        "QPushButton:disabled {"
        "    background-color: #161616;"
        "    color: #555;"
        "    border: 1px solid rgba(255, 255, 255, 0.1);"
        "    border-radius: 12px;"
        "    padding: 12px 24px;"
        "    font-size: 24px;"
        "}";

    QPushButtonDisabledStyleSmallFont =
        "QPushButton:disabled {"
        "    background-color: #161616;"
        "    color: #555;"
        "    border: 1px solid rgba(255, 255, 255, 0.1);"
        "    border-radius: 12px;"
        "    padding: 12px 20px;"
        "    font-size: 18px;"
        "}";

    QSliderStyle =
        "QSlider {"
        "    background: transparent;"
        "    height: 18px;"
        "}"
        "QSlider::groove:horizontal {"
        "    background: rgba(100, 100, 100, 60);"
        "    border: 1px solid rgba(255, 255, 255, 15);"
        "    height: 8px;"
        "    border-radius: 4px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: radial-gradient(circle, #aaa, #888);"
        "    border: 1px solid #aaa;"
        "    width: 20px;"
        "    height: 20px;"
        "    margin: -6px 0;"
        "    border-radius: 10px;"
        "}"
        "QSlider::handle:horizontal:hover {"
        "    background: radial-gradient(circle, #fff, #aaa);"
        "    border: 1px solid #ccc;"
        "}"
        "QSlider::sub-page:horizontal {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #aaa, stop:1 #666);"
        "    border-radius: 4px;"
        "}"
        "QSlider::add-page:horizontal {"
        "    background: rgba(30, 30, 30, 150);"
        "    border-radius: 4px;"
        "}";

    QLabelStyle =
        "QLabel {"
        "    background-color: transparent;"
        "    border: none;"
        "    color: white;"
        "    font-size: 30px;"
        "    font-weight: bold;"
        "    padding: 6px;"
        "}";

    QBetLabelStyle =
        "QLabel {"
        "    color: #ffffaa;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    padding: 4px;"
        "    background-color: rgba(0, 0, 0, 0);"
        "    border: none;"
        "    border-radius: 0px;"
        "    qproperty-alignment: AlignCenter;"
        "}";

    QTextBrowserStyle =
        "QTextBrowser {"
        "    background-color: rgba(50, 25, 20, 220);"
        "    border-radius: 12px;"
        "    padding: 10px;"
        "    color: white;"
        "    font-size: 12px;"
        "}";

    QGraphicsViewStyle =
        "QGraphicsView {"
        "    background-color: transparent;"
        "    border: none;"
        "}";

    QStackedWidgetStyle =
        "QStackedWidget {"
        "    background-color: transparent;"
        "    border: none;"
        "}";

    QLineEditStyle =
        "QLineEdit {"
        "    background-color: transparent;"
        "    border: none;"
        "    color: white;"
        "    font-size: 14px;"
        "    font-weight: 600;"
        "    padding: 6px;"
        "}";
}

void Screens::setUpBackGround()
{
    QPixmap bgImage(":/backGround/brown_wooden_background.png");

    bgImage = bgImage.scaled(ui->centralwidget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPalette bgPalette;
    bgPalette.setBrush(QPalette::Window, QBrush(bgImage));
    ui->centralwidget->setPalette(bgPalette);
    ui->centralwidget->setAutoFillBackground(true);

    QPixmap widgetImage(":/backGround/start_menu_screen.png");

    widgetImage = widgetImage.scaled(ui->startMenuMainWidget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette widgetPalette;
    widgetPalette.setBrush(QPalette::Window, QBrush(widgetImage));
    ui->startMenuMainWidget->setPalette(widgetPalette);
    ui->startMenuMainWidget->setAutoFillBackground(true);

    ui->screens->setStyleSheet(QStackedWidgetStyle);
}

void Screens::setUpBankruptcyMenu()
{
    ui->bankruptcyMenu->setStyleSheet(QWidgetStyle);
    QString bankruptcyLabel =
        "QLabel {"
        "    background-color: transparent;"
        "    border: none;"
        "    color: red;"
        "    font-size: 30px;"
        "    font-weight: bold;"
        "    padding: 6px;"
        "    qproperty-alignment: AlignCenter;"
        "}";
    ui->bankruptcyLabel->setStyleSheet(bankruptcyLabel);
    ui->backToMainMenuFromPlay_2->setStyleSheet(QPushButtonStyle);
    ui->playAgain->setStyleSheet(QPushButtonStyle);

    auto *effect = new QGraphicsColorizeEffect(ui->bankruptcyLabel);
    effect->setColor(Qt::darkRed);
    ui->bankruptcyLabel->setGraphicsEffect(effect);

    auto *animation = new QPropertyAnimation(effect, "color");
    animation->setDuration(2000);
    animation->setLoopCount(-1);                 // infinite
    animation->setStartValue(QColor(100, 0, 0)); // dark red
    animation->setEndValue(QColor(255, 0, 0));   // bright red
    animation->setEasingCurve(QEasingCurve::InOutSine);
    animation->start();
}

void Screens::setUpRecomendedMove()
{
    QString QLabelStyleSmallFont =
        "QLabel {"
        "    background-color: rgba(50, 25, 20, 225);"
        "    border: none;"
        "    color: white;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    padding: 6px;"
        "    qproperty-alignment: 'AlignCenter';"
        "}";
    ui->practiceBestMoveWidget->setStyleSheet(QLabelStyleSmallFont);
}

void Screens::moveToStartScreen()
{
    ui->screens->setCurrentIndex(0);
    toggleVisibleSettingsPopup(false);
    toggleVisibleBettingView(false);
    toggleVisibleGamePlayButtons(false);
    mode = UNSELECTED;
}

void Screens::moveToPlayScreen()
{
    // Hide all gameplay features initially
    toggleEnabledGamePlayButtons(false);
    toggleVisibleGamePlayButtons(false);
    toggleVisableBankruptcyMenu(false);
    tableView->clearTable();
    toggleEnabledQPushButton(ui->nextRound, false);
    toggleEnabledQPushButton(ui->placeBetButton, false);

    ui->screens->setCurrentIndex(1);
    ui->bettingArea->hide();
    QPushButton *button = qobject_cast<QPushButton *>(sender());

    QString name = button->objectName();

    if (name == "blackjackPlayButton" || name == "playAgain")
    {
        mode = GAMEPLAYMODE::BLACKJACK;
        toggleVisibleSettingsPopup(true);
        toggleVisableRecommendedMove(false);
        updateBetLabelText(1);
    }
    else if (name == "blackjackTutorialButton")
    {
        mode = GAMEPLAYMODE::BLACKJACKTUTORIAL;
        playerCount = 1;
        acceptSettingsButtonPressed();
        toggleVisableRecommendedMove(false);
        updateBetLabelText(0);
    }
    else if (name == "blackjackPracticeButton")
    {
        mode = GAMEPLAYMODE::BLACKJACKPRACTICE;
        playerCount = 1;
        acceptSettingsButtonPressed();
        toggleVisableRecommendedMove(true);
        updateBetLabelText(0);
    }
    else
    {
        mode = GAMEPLAYMODE::UNSELECTED;
    }
}

void Screens::toggleVisibleSettingsPopup(bool show)
{
    if (show)
    {
        ui->settingsPopUp->show();
    }
    else
    {
        ui->settingsPopUp->hide();
    }
}

void Screens::toggleVisibleBettingView(bool show)
{
    if (show)
    {
        ui->bettingArea->show();
    }
    else
    {
        ui->bettingArea->hide();
    }
}

void Screens::toggleVisibleGamePlayButtons(bool show)
{
    if (show)
    {
        ui->gamePlayButtons->show();
    }
    else
    {
        ui->gamePlayButtons->hide();
    }
}

void Screens::toggleVisableBankruptcyMenu(bool show)
{
    if (show)
    {
        ui->bankruptcyMenu->show();
    }
    else
    {
        ui->bankruptcyMenu->hide();
    }
}

void Screens::toggleVisableRecommendedMove(bool show)
{
    if (show)
    {
        ui->practiceBestMoveWidget->show();
    }
    else
    {
        ui->practiceBestMoveWidget->hide();
    }
}

void Screens::applyShadowToWidget(QWidget *widget)
{
    // Create a shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();

    // Set how the shadow looks
    shadow->setBlurRadius(15);
    shadow->setOffset(5, 5);
    shadow->setColor(QColor(0, 0, 0, 160));

    widget->setGraphicsEffect(shadow);
}

void Screens::moveToInfoScreen()
{
    ui->infoMenuButton->setStyleSheet(QPushButtonStyleSmallFont);
    ui->screens->setCurrentIndex(2);
}

void Screens::setUpBasicStrategyCharts()
{
    // Set up the basic strategy charts
    QPixmap originalPixmap(":/StrategyCharts/BasicStrategyCharts.png");
    QGraphicsPixmapItem *imageItem = new QGraphicsPixmapItem(originalPixmap);
    imageItem->setScale(0.48);
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addItem(imageItem);
    scene->setSceneRect(imageItem->sceneBoundingRect());
    ui->basicStrategyChart->setScene(scene);
    ui->basicStrategyChart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->basicStrategyChart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->basicStrategyChart->setStyleSheet(QGraphicsViewStyle);

    // Set up for player hand example picture
    QPixmap playerHandPixmap(":/StrategyCharts/playerhandexample.png");
    QGraphicsPixmapItem *playerHandItem = new QGraphicsPixmapItem(playerHandPixmap);
    QGraphicsScene* playerHandScene = new QGraphicsScene(this);
    playerHandScene->addItem(playerHandItem);
    playerHandScene->setSceneRect(playerHandItem->sceneBoundingRect());
    ui->playerHandEgPic->setScene(playerHandScene);
    ui->playerHandEgPic->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerHandEgPic->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerHandEgPic->setStyleSheet(QGraphicsViewStyle);

    QPixmap playerStrategyPixmap(":/StrategyCharts/BasicStrategyCharts_playerHand.png"); // Update path
    QGraphicsPixmapItem *playerStrategyItem = new QGraphicsPixmapItem(playerStrategyPixmap);
    QGraphicsScene* playerStrategyScene = new QGraphicsScene(this);
    playerStrategyScene->addItem(playerStrategyItem);
    playerStrategyScene->setSceneRect(playerStrategyItem->sceneBoundingRect());
    ui->playerStrategyPic->setScene(playerStrategyScene);
    ui->playerStrategyPic->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerStrategyPic->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerStrategyPic->setStyleSheet(QGraphicsViewStyle);

    QPixmap dealerhandPixmap(":/StrategyCharts/dealerhandexample.png"); // Update path
    QGraphicsPixmapItem *dealerhandItem = new QGraphicsPixmapItem(dealerhandPixmap);
    QGraphicsScene* dealerhandScene = new QGraphicsScene(this);
    dealerhandScene->addItem(dealerhandItem);
    dealerhandScene->setSceneRect(dealerhandItem->sceneBoundingRect());
    ui->dealershandeg->setScene(dealerhandScene);
    ui->dealershandeg->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->dealershandeg->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->dealershandeg->setStyleSheet(QGraphicsViewStyle);

    QPixmap dealerupperPixmap(":/StrategyCharts/example1.png"); // Update path
    QGraphicsPixmapItem *dealerupperItem = new QGraphicsPixmapItem(dealerupperPixmap);
    QGraphicsScene* dealerupperScene = new QGraphicsScene(this);
    dealerupperScene->addItem(dealerupperItem);
    dealerupperScene->setSceneRect(dealerupperItem->sceneBoundingRect());
    ui->dealersupperpic->setScene(dealerupperScene);
    ui->dealersupperpic->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->dealersupperpic->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->dealersupperpic->setStyleSheet(QGraphicsViewStyle);

    QPixmap example1Pixmap(":/StrategyCharts/example1.png"); // Update path
    QGraphicsPixmapItem *example1Item = new QGraphicsPixmapItem(example1Pixmap);
    QGraphicsScene* example1Scene = new QGraphicsScene(this);
    example1Scene->addItem(example1Item);
    example1Scene->setSceneRect(example1Item->sceneBoundingRect());
    ui->example1->setScene(example1Scene);
    ui->example1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->example1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->example1->setStyleSheet(QGraphicsViewStyle);

    QPixmap example2Pixmap(":/StrategyCharts/example2.png"); // Update path
    QGraphicsPixmapItem *example2Item = new QGraphicsPixmapItem(example2Pixmap);
    QGraphicsScene* example2Scene = new QGraphicsScene(this);
    example2Scene->addItem(example2Item);
    example2Scene->setSceneRect(example2Item->sceneBoundingRect());
    ui->example2->setScene(example2Scene);
    ui->example2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->example2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->example2->setStyleSheet(QGraphicsViewStyle);

    QPixmap example3Pixmap(":/StrategyCharts/example3.png"); // Update path
    QGraphicsPixmapItem *example3Item = new QGraphicsPixmapItem(example3Pixmap);
    QGraphicsScene* example3Scene = new QGraphicsScene(this);
    example3Scene->addItem(example3Item);
    example3Scene->setSceneRect(example3Item->sceneBoundingRect());
    ui->example3->setScene(example3Scene);
    ui->example3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->example3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->example3->setStyleSheet(QGraphicsViewStyle);

    //Set up for the textBrowser
    ui->infoTextBrowser->setStyleSheet(QTextBrowserStyle);
    ui->infoTextBrowser_2->setStyleSheet(QTextBrowserStyle);
    ui->infoTextBrowser_3->setStyleSheet(QTextBrowserStyle);
    ui->infoTextBrowser_4->setStyleSheet(QTextBrowserStyle);

    ui->charttabs->setStyleSheet(
        "QTabWidget::pane {"
        "   border: none;"
        "   background: rgba(50, 25, 20, 220);"
        "}"
        "QTabBar::tab {"
        "   background: rgba(70, 35, 30, 180);"
        "   color: white;"
        "   padding: 8px;"
        "   border: none;"
        "}"
        "QTabBar::tab:selected {"
        "   background: rgba(90, 45, 40, 220);"
        "}"
        );
    ui->charttabs->setDocumentMode(true);
}

void Screens::onPressHitButton()
{
    emit sendHitButtonPressed();

    // In tutorial mode, if the next message is a Tip, show it immediately
    if (mode == GAMEPLAYMODE::BLACKJACKTUTORIAL && tutorialPopup->nextIsTip())
    {
        toggleEnabledGamePlayButtons(false);
        tutorialPopup->toggleVisableTutorialPopup(true);
    }
}

void Screens::onPressStandButton()
{
    emit sendStandButtonPressed();
}

void Screens::onPressDoubleButton()
{
    emit sendDoubleButtonPressed();
}

void Screens::onPressSplitButton()
{
    emit sendSplitButtonPressed();
}

void Screens::onPressPlacedBetButton()
{
    toggleVisibleBettingView(false);
    toggleVisibleGamePlayButtons(true);
    emit sendOnBet(currentBet);
}

void Screens::onPressBettingAmountButtons()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());

    QString name = button->objectName();

    if (name == "allInButton")
    {
        currentBet = players[userIndex].money;
    }
    else if (name == "halfButton")
    {
        currentBet = players[userIndex].money / 2;
    }
    else
    {
        // Called when the minimum button is pressed
        currentBet = 1;
    }
    ui->betSlider->setValue(currentBet);
    updateBetLabelText(currentBet);
}

void Screens::onEditChipCountLineEdit()
{
    QString input = ui->chipCountSettingsLineEdit->text();

    bool ok;
    int value = input.toInt(&ok);

    if (!ok || value < 1)
    {
        value = 1;
    }
    else if (value > 1000000)
    {
        value = 1000000;
    }

    if (input.isEmpty())
    {
        toggleEnabledQPushButton(ui->acceptSettingsButton, false);
        return;
    }

    // Set the clamped value back into the line edit
    ui->chipCountSettingsLineEdit->setText(QString::number(value));
    initialMoney = value;
    toggleEnabledQPushButton(ui->acceptSettingsButton, true);
}

void Screens::onGameOver()
{
    toggleVisableBankruptcyMenu(true);
    toggleEnabledQPushButton(ui->nextRound, false);
}

void Screens::acceptSettingsButtonPressed()
{
    // Hide gameplay buttons during setup
    toggleVisibleGamePlayButtons(false);

    int determinedDeck;

    // Set up based on game mode
    if (mode == GAMEPLAYMODE::BLACKJACK)
    {
        // Validate chip count and player count
        onEditChipCountLineEdit();
        playerCount = ui->playerCountSettingsSlider->value();

        // Show betting UI
        toggleVisibleBettingView(true);
        determinedDeck = 0;
    }
    else if (mode == GAMEPLAYMODE::BLACKJACKTUTORIAL)
    {
        initialMoney = 1000000;
        determinedDeck = 1;
        deckCount = 2;
    }
    else if (mode == GAMEPLAYMODE::BLACKJACKPRACTICE)
    {
        initialMoney = 1000000;
        determinedDeck = 0;
        deckCount = 1;
    }

    ui->betSlider->setMaximum(initialMoney);
    toggleEnabledQPushButton(ui->placeBetButton, false);

    // Choose random start location for user
    userIndex = QRandomGenerator::global()->bounded(playerCount);

    // Initialize players
    for (unsigned int i = 0; i < playerCount; i++)
    {
        players.emplace_back(initialMoney, currentBet, i == userIndex, 1, 0);
        players[i].originalHand = true;
    }

    // Prepare card containers
    tableView->createPlayerCardContainers(playerCount);

    // Notify model
    emit sendSettingsAccepted(players, deckCount, determinedDeck);

    // Show dealer's card pile
    tableView->createDealerPile();

    // Start game or delay start depending on mode
    if (mode == GAMEPLAYMODE::BLACKJACK)
    {
        emit sendGameSetupCompleteStartBetting();
    }
    else if (mode == GAMEPLAYMODE::BLACKJACKTUTORIAL || mode == GAMEPLAYMODE::BLACKJACKPRACTICE)
    {
        timer->scheduleSingleShot(2100, [=]()
                                  {
            emit sendGameSetupCompleteStartBetting();
            updateBetLabelText(0);
            onPressPlacedBetButton(); });
    }

    // Ensure buttons remain disabled initially
    toggleEnabledGamePlayButtons(false);
}

void Screens::dealCard(int playerIndex, int handIndex, QString imagePath)
{
    QPointF startPos(500, 49);
    QPointF endPos = tableView->getCardEndPosition(indexToSeat(playerIndex), handIndex, players[playerIndex].hand.getCards().size());
    qreal rotation = tableView->getCardEndRotation(indexToSeat(playerIndex), handIndex);

    tableView->addCardAnimated(indexToSeat(playerIndex), handIndex, imagePath, startPos, endPos, rotation);
}

int Screens::indexToSeat(unsigned int playerIndex)
{
    if (playerIndex >= players.size())
        return -1;
    int seatIndex = -1;
    for (int i = 0; i <= (int)playerIndex; i++)
    {
        if (players[i].originalHand)
            seatIndex++;
    }
    return seatIndex;
}

void Screens::playerUpdated(int playerIndex, const Player &player, int)
{
    // If only one card, deal it immediately
    if (player.hand.getCards().size() == 1)
    {
        dealCard(playerIndex, player.playerHandIndex, player.hand.getCards()[0].getImagePath());
    }
    // If multiple cards, animate new ones only
    else if (player.hand.getCards().size() >= 2)
    {
        int prevHandSize = players[playerIndex].hand.getCards().size();
        bool firstLoop = true;

        for (int i = prevHandSize; i < static_cast<int>(player.hand.getCards().size()); i++)
        {
            // If first card updated, deal immediately
            if (firstLoop)
            {
                dealCard(playerIndex, player.playerHandIndex, player.hand.getCards()[i].getImagePath());
                firstLoop = false;
                continue;
            }
            // Delay dealing additional cards for animation effect
            timer->scheduleSingleShot(600, [=]()
                                      { dealCard(playerIndex, player.playerHandIndex, player.hand.getCards()[i].getImagePath()); });
        }
    }

    // If in practice mode, show recommended move after 2 cards
    if (mode == GAMEPLAYMODE::BLACKJACKPRACTICE && players[userIndex].hand.getCards().size() >= 2)
    {
        updateRecommendedMove(player.hand);
    }

    // Update player state
    players[playerIndex].hand = player.hand;
    players[playerIndex].money = player.money;
    players[playerIndex].status = player.status;
}

void Screens::allPlayersUpdated(const std::vector<Player> &players)
{
    toggleEnabledGamePlayButtons(false);
    unsigned int waitTime = 0;
    std::vector<Player> tempPlayers;

    // Creates temporary players to allow for dealing animation
    for (const Player &player : players)
    {
        tempPlayers.emplace_back(player.money, player.hand.getBet(), player.isUser, player.playerHandCount, player.playerHandIndex);
    }

    for (int j = 0; j < 2; j++)
    {
        // Deal one card to each player
        for (int i = 0; i < static_cast<int>(tempPlayers.size()); i++)
        {
            tempPlayers[i].hand.addCard(players[i].hand.getCards()[j]);
            timer->scheduleSingleShot(waitTime, [=]()
                                      { playerUpdated(i, tempPlayers[i], tempPlayers[i].hand.getTotal()); });
            waitTime += 600;
        }
        waitTime += 600;
    }

    // Round can start once deal animation is complete
    timer->scheduleSingleShot(waitTime, [=]()
                              {
        if (mode != GAMEPLAYMODE::BLACKJACKTUTORIAL)
        {
            toggleEnabledGamePlayButtons(true);
        }
        emit dealAnimationComplete(); });

    if (mode == GAMEPLAYMODE::BLACKJACKPRACTICE && players[userIndex].hand.getCards().size() >= 2)
    {
        updateRecommendedMove(players[userIndex].hand);
    }
}

void Screens::updateRecommendedMove(const Hand &playerHand)
{
    if (dealerHand.getCards().size() < 2)
    {
        return;
    }
    // Get best move from botstrategy
    MOVE move = BotStrategy::getNextMove(playerHand, dealerHand.getCards()[1]);
    QString moveString = QString::fromStdString(Move::toString(move));

    ui->practiceBestMoveLabel->setText(QString("Best move is:\n%1").arg(moveString));
}

void Screens::dealerUpdated(const Hand &hand, int)
{
    // Turn off buttons and update the dealer hand in the vector
    toggleEnabledGamePlayButtons(false);
    int prevHandSize = dealerHand.getCards().size();
    dealerHand = hand;

    // If middle of round, change first card to show back of card
    if (!showDealerCard && dealerHand.getCards().size() > 0)
    {
        dealerHand.setCardImagePath(0, ":/cardImages/cards_pngsource/back_of_card.png");
    }

    unsigned int waitTime = 600 * players.size();

    // If dealer card flipped, get the correct card image
    if (showDealerCard)
    {
        waitTime = 600;

        QString imagePath = dealerHand.getCards().at(0).getImagePath();
        tableView->revealDealerCard(imagePath);
    }

    // Deal card to dealer
    for (int i = prevHandSize; i < static_cast<int>(dealerHand.getCards().size()); i++)
    {
        timer->scheduleSingleShot(waitTime, [=]()
                                  { dealCard(-1, 0, dealerHand.getCards()[i].getImagePath()); });
        if (!showDealerCard)
        {
            waitTime *= 2;
        }
        waitTime += 800;
    }

    // Show end of round results once dealer is done playing
    if (showDealerCard)
    {
        timer->scheduleSingleShot(waitTime, [=]()
                                  { emit sendDealerDonePlaying(); });
    }

    // Show the first reccomended move after dealer gets cards
    if (dealerHand.getCards().size() == 2 && mode == GAMEPLAYMODE::BLACKJACKPRACTICE)
    {
        timer->scheduleSingleShot(waitTime / 1.5, [=]()
                                  { updateRecommendedMove(players[userIndex].hand); });
    }
}

void Screens::updateShowDealerCardBool(bool flipped)
{
    showDealerCard = flipped;
}

void Screens::onSplitPlayers(int originalIndex, const Player &originalPlayer, const Player &newPlayer)
{
    int playerIndex = originalIndex - players[originalIndex].playerHandIndex;
    tableView->splitPlayerHand(indexToSeat(originalIndex), originalIndex - playerIndex);
    players[originalIndex].hand.removeLastCard();

    // Create the new player and add half of their hand into the player vector
    Player tempPlayer = Player(newPlayer.money, newPlayer.hand.getBet(), newPlayer.isUser, newPlayer.playerHandCount, newPlayer.playerHandIndex);
    Hand tempHand = Hand(newPlayer.hand.getBet());
    tempHand.addCard(newPlayer.hand.getCards()[0]);
    tempPlayer.hand = tempHand;
    players.insert(players.begin() + originalIndex + 1, tempPlayer);

    // Update original player hand
    players[originalIndex].playerHandCount++;
    playerUpdated(originalIndex, originalPlayer, originalPlayer.hand.getTotal());

    // Updates new player hand
    timer->scheduleSingleShot(600 * 2, [=]()
                              { playerUpdated(originalIndex + 1, newPlayer, newPlayer.hand.getTotal()); });

    if (mode == GAMEPLAYMODE::BLACKJACKTUTORIAL)
    {
        toggleEnabledGamePlayButtons(false);
        // show the next tip
        tutorialPopup->toggleVisableTutorialPopup(true);
    }
}

void Screens::currentPlayerTurn(int nextPlayerIndex, int, int, int)
{
    if (mode == GAMEPLAYMODE::BLACKJACKTUTORIAL)
    {
        // If the next popup is a tip, show it to the user
        if (tutorialPopup->nextIsTip())
        {
            toggleEnabledGamePlayButtons(false);
            tutorialPopup->toggleVisableTutorialPopup(true);
        }
        return;
    }

    // Turn buttons on if the player is user
    if (players[nextPlayerIndex].isUser)
    {
        toggleEnabledGamePlayButtons(true);
        toggleEnabledQPushButton(ui->placeBetButton, true);
    }
    // Turn off the buttons if it isn't a user
    else
    {
        toggleEnabledGamePlayButtons(false);
        toggleEnabledQPushButton(ui->placeBetButton, false);
    }
}

void Screens::endBetting()
{
    toggleVisibleBettingView(false);
    toggleVisibleGamePlayButtons(true);
}

void Screens::toggleEnabledGamePlayButtons(bool enabled)
{
    toggleEnabledQPushButton(ui->hitButton, enabled);
    toggleEnabledQPushButton(ui->standButton, enabled);
    toggleEnabledQPushButton(ui->doubleButton, enabled);
    toggleEnabledQPushButton(ui->splitButton, enabled);
}

void Screens::toggleEnabledQPushButton(QPushButton *button, bool enabled)
{
    if (enabled)
    {
        if (button->objectName() == "nextRound" || button->objectName() == "backToMenuFromPlay")
        {
            button->setStyleSheet(QPushButtonStyleSmallFont);
        }
        else
        {
            button->setStyleSheet(QPushButtonStyle);
        }
        button->setEnabled(enabled);
        return;
    }
    if (button->objectName() == "nextRound" || button->objectName() == "backToMenuFromPlay")
    {
        button->setStyleSheet(QPushButtonDisabledStyleSmallFont);
    }
    else
    {
        button->setStyleSheet(QPushButtonDisabledStyle);
    }
    button->setEnabled(enabled);
}

void Screens::endRound(const std::vector<Player> &players)
{
    toggleEnabledQPushButton(ui->nextRound, true);
    toggleEnabledGamePlayButtons(false);

    // Check if in tutorial mode
    if (mode == GAMEPLAYMODE::BLACKJACKTUTORIAL)
    {
        tutorialPopup->toggleVisableTutorialPopup(true);
        return;
    }

    std::vector<Player> localPlayers = players;

    for (Player &player : localPlayers)
    {
        ui->coinAnimView->viewport()->update();

        // Check player status and spawn coins if user won or has blackjack
        if (player.status == PLAYERSTATUS::WON && player.isUser)
        {
            m_scene->onWinSpawnCoins(QPointF(540.0, 720.0), 0);
        }
        else if (player.status == PLAYERSTATUS::BLACKJACK && player.isUser)
        {
            m_scene->onWinSpawnCoins(QPointF(540.0, 720.0), 25);
        }
    }
}

void Screens::onPressNextRound()
{
    // In tutorial mode, don’t advance the table, just forward to the popup
    if (mode == GAMEPLAYMODE::BLACKJACKTUTORIAL && ui->tutorialWidget->isVisible())
    {
        tutorialPopup->onContinuePressed();
        return;
    }
    tutorialPopup->toggleVisableTutorialPopup(false);

    // if the scene is active/valid
    if (m_scene)
    {
        m_scene->stopSpawning();
        m_scene->clearCoins();
        m_scene->update();
    }

    timer->cancelAllTimers();
    tableView->clearTable();

    // Remove split hands
    for (int i = players.size() - 1; i >= 0; i--)
    {
        if (!players[i].originalHand)
            players.erase(players.begin() + i);
        else
            players[i].playerHandCount = 1;
    }

    // Start the next round in the model
    emit sendNewRound();

    // Set buttons and betting to correct state
    toggleEnabledQPushButton(ui->nextRound, false);
    toggleEnabledGamePlayButtons(false);
    tableView->createDealerPile();
    ui->betSlider->setMaximum(players[userIndex].money);

    // If blackjack, show betting
    if (mode == GAMEPLAYMODE::BLACKJACK)
    {
        toggleVisibleBettingView(true);
        toggleVisibleGamePlayButtons(false);
    }
    // If tutorial or practice, wait for dealerpile to finish and then start next round
    else if (mode == GAMEPLAYMODE::BLACKJACKTUTORIAL || mode == GAMEPLAYMODE::BLACKJACKPRACTICE)
    {
        timer->scheduleSingleShot(2100, [=]()
                                  { onPressPlacedBetButton(); });
        ui->practiceBestMoveLabel->setText("Best move is: \n");
    }
}

void Screens::onPressMainMenuButton()
{
    resetEverything();
    moveToStartScreen();
}

void Screens::onPressPlayAgain()
{
    resetEverything();
    moveToPlayScreen();
}

void Screens::resetEverything()
{
    emit sendStopEverything();

    // If win money still spawning, stop it and clear coins
    if (m_scene)
    {
        m_scene->stopSpawning();
        m_scene->clearCoins();
        m_scene->clear();
    }
    tableView->stopEverything();
    timer->cancelAllTimers();
    players.clear();
    ui->practiceBestMoveLabel->setText("Best move is: \n");
}
