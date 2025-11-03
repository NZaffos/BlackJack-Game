#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>

#include "controller.h"
#include "screens.h"
#include "playerinfoview.h"
#include "box2dbase.h"
#include "gamestate.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief The MainWindow class creates the window screen of the application and sets up QT connections
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/21/2025
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief MainWindow Constructor for the MainWindow
     * @param controller A pointer to the controller to handle game logic
     * @param parent The parent of this object
     */
    MainWindow(Controller *controller, QWidget *parent = nullptr);

    /**
     * @brief ~MainWindow Deconstructor fot the MainWindow
     */
    ~MainWindow();

private:
    /**
     * @brief ui The main windows visuals
     */
    Ui::MainWindow *ui;

    /**
     * @brief controller Handles conneciting to the game logic
     */
    Controller *controller;

    /**
     * @brief screens Handles all ui processes
     */
    Screens *screens;

    /**
     * @brief infoBar Handles the players info
     */
    PlayerInfoView *infoBar;

    /**
     * @brief m_scene Box2D for winning animations
     */
    box2Dbase *m_scene;

    /**
     * @brief state The state of the game
     */
    GameState *state;

    /**
     * @brief setupCoinAnimViews sets up the box2D visual area fro the coin animations to play
     */
    void setupCoinAnimViews();

    /**
     * @brief setUpMainWindowConnects Creats all the QT connect statments
     */
    void setUpMainWindowConnects();
};
#endif // MAINWINDOW_H
