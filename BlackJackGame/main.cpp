/**
 * @brief Main class is responsible for starting the application and passing controller to mainwindow for connections
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/15/2025
 */

#include "mainwindow.h"
#include "controller.h"
#include <QApplication>

/**
 * @brief main The point of execution
 * @param argc Number of args
 * @param argv Char array of args
 * @return int An int for the success or failues of the program
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller c;
    MainWindow w(&c);
    w.show();
    return a.exec();
}
