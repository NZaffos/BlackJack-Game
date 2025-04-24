#ifndef BOX2DBASE_H
#define BOX2DBASE_H

#include <QGraphicsScene>
#include <Box2D/Box2D.h>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QQueue>
#include <QMediaPlayer>
#include <QAudioOutput>

/**
 * @brief The box2Dbase class that includes all logic for creating coin objects that simulate physics such as collisions and weights
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/22/2025
 */
class box2Dbase : public QGraphicsScene{
    Q_OBJECT
public:
    /**
     * @brief box2Dbase Constructor
     * @param parent
     */
    explicit box2Dbase(QObject *parent = nullptr);

    /**
     * Destructor
     */
    ~box2Dbase();

    /**
     * @brief advance Responsible for updating the physics simulation forward
     * and the graphics view display to visually show Box2D elements interacting/moving in the world
     */
    void advance();

    /**
     * @brief randomFloat Helper method to generate a random float between a range
     * @param min Minimum value
     * @param max Maximum value
     * @return Random float
     */
    float randomFloat(float min, float max);

    /**
     * @brief removeBodies Remove all bodies once they are no longer needed
     */
    //void removeBodies();

    /**
     * @brief coinPixmap The pixmap image for the coin
     */
    QPixmap *coinPixmap;

    /**
     * @brief spawnNextCoin Spawns the next coin in the queue sequentially to emulate a slot machine
     * Responsible for creating a Box2D object and adding to queue - including the body, shape and fixture such that the
     * element can be simulated in the world scene
     */
    void spawnNextCoin();

    /**
     * @brief onWinSpawnCoins Function called by other classes to begin spawning coins only when the player has won
     * @param position To spawn the coins at
     * @param coinsToSpawn The number of coins to spawn
     */
    void onWinSpawnCoins(QPointF position, int coinsToSpawn);

    /**
     * @brief initialBurst Spawns a burst of coins
     */
    void initialBurst();

    /**
     * @brief isCoinSpawning Helper method to check if the spawn timer for the coins is running - thus the coins are spawning
     * @return true or false depending on if the timer is active or not
     */
    bool isCoinSpawning();

    /**
     * @brief stopSpawning Helper method to clear the coin queue and stop any new coins from spawning
     */
    void stopSpawning();

    /**
     * @brief clearCoins Clears all current coins in the scene
     */
    void clearCoins();

private:
    /**
     * @brief m_world The physics world where our Box2D bodies are instantiated
     */
    b2World *m_world;

    /**
     * @brief m_timeStep Determines how much the physics world moves per second
     * The value is equivalent to 60 fps
     */
    float32 m_timeStep = 1.0f/60.0f;

    /**
     * @brief m_velocityIterations
     */
    int32 m_velocityIterations = 6;

    /**
     * @brief m_positionIterations Stabilizes the position shifting
     */
    int32 m_positionIterations = 2;

    /**
     * @brief pixels_PerMeter The conversion factor between pixels and meters
     * Used to convert/translate the coordinates and position values between Qt's and Box2D's coordinate systems
     */
    const float pixels_PerMeter = 50.0f;

    /**
     * @brief WORLD_WIDTH Width bounds of the Box 2D Physics World (in meters)
     */
    const float WORLD_WIDTH = 20.0f;

    /**
     * @brief WORLD_HEIGHT Height bounds of the Box 2D Physics World (in meters)
     */
    const float WORLD_HEIGHT = 15.0f;

    /**
     * @brief m_coinQueue Stores the pending coin positions
     */
    QQueue<QPointF> m_coinQueue;

    /**
     * @brief m_coinTimer Controls the spawn timing of coins
     */
    QTimer* m_coinTimer;

    /**
     * @brief m_coinsPerSecond Adjusts spawn rate
     */
    int m_coinsPerSecond = 10;

    QMediaPlayer *jackpotNoise;
    QAudioOutput *audioOutput;


signals:

};

#endif // BOX2DBASE_H
