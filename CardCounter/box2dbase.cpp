/**
 * @brief Implementation of The box2Dbase class. It includes all logic for creating coin objects that simulate physics such as collisions and weights
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/22/2025
 */

#include "box2dbase.h"

box2Dbase::box2Dbase(QObject *parent) : QGraphicsScene{parent},
    coinPixmap(new QPixmap(":/cash/otherimg/coin.png")),  // initialize the world
    m_world(new b2World(b2Vec2(0.0f, 9.8f))),
    m_timeStep(1.0f/60.0f),
    m_velocityIterations(6),
    m_positionIterations(2)
{
    m_coinTimer = new QTimer(this);
    connect(m_coinTimer, &QTimer::timeout, this, &box2Dbase::spawnNextCoin);

    setBackgroundBrush(Qt::transparent);

    QMediaPlayer *jackpotNoise = new QMediaPlayer(this);
    QAudioOutput *audioOutput = new QAudioOutput(this);

    jackpotNoise->setAudioOutput(audioOutput);
    jackpotNoise->setSource(QUrl("qrc::/sounds/otherimg/jackpot.mp3"));

    audioOutput->setVolume(30);  // 0-100

    // Set graphics scene size to match physics world
    setSceneRect(0, 0, WORLD_WIDTH * pixels_PerMeter, WORLD_HEIGHT * pixels_PerMeter);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &box2Dbase::advance);
    timer->start(1000 / 60); // ~60 FPS
}

box2Dbase::~box2Dbase(){
    // Clean up all QGraphicsItems
    clear();

    // Delete Box2D world
    delete m_world;
}

float box2Dbase::randomFloat(float min, float max){
    return static_cast<float>(QRandomGenerator::global()->generateDouble() * (min - max) + min);
}

void box2Dbase::advance(){
    if (!m_world) return; // if the world doesnt exist

    // step physics simulation forward
    m_world->Step(m_timeStep, m_velocityIterations, m_positionIterations);

    // vector of all bodies that have left world bounds
    QVector<b2Body*> bodiesOutofBounds;

    // iterate through all current bodies spawned
    for (b2Body* body = m_world->GetBodyList(); body; body = body->GetNext()) {
        if (body->GetType() != b2_dynamicBody) continue;
        b2Vec2 position = body->GetPosition();

        // out of world bounds check
        if (position.x < 0 || position.x > WORLD_WIDTH ||
            position.y < 0 || position.y > WORLD_HEIGHT) {
            bodiesOutofBounds.append(body);
        }
    }

    // remove all bodies out of bounds
    for (b2Body* body : bodiesOutofBounds) {
        if (body->GetUserData()) {
            QGraphicsItem* item = static_cast<QGraphicsItem*>(body->GetUserData());
            removeItem(item);  // Remove from scene
            delete item;       // Free memory
        }
        m_world->DestroyBody(body);
    }

    // update graphics view
    for (b2Body* body = m_world->GetBodyList(); body; body = body->GetNext()){
        if (body->GetUserData()){
            QGraphicsItem* item = static_cast<QGraphicsItem*>(body->GetUserData());
            b2Vec2 position = body->GetPosition();
            item->setPos(position.x * pixels_PerMeter,
                         position.y * pixels_PerMeter);
            item->setRotation(body->GetAngle() * 180.0f / b2_pi);
        }
    }

    QGraphicsScene::advance();
}

void box2Dbase::onWinSpawnCoins(QPointF position, int coinsToSpawn) {
    // clear any existing coins first
    m_coinQueue.clear();

    const float payoutWidth = 60.0f; //wider spread when spawned
    for (int i = 0; i < coinsToSpawn; i++) {
        float offset = randomFloat(-payoutWidth/2, payoutWidth/2);
        m_coinQueue.enqueue(position + QPointF(offset, 0));
    }

    // start spawning with celebratory timing
    initialBurst();

    if (!m_coinTimer->isActive()) {
        m_coinTimer->start(100); // slightly slower for visibility

        jackpotNoise->play();
    }

}

void box2Dbase::spawnNextCoin() {
    if (m_coinQueue.isEmpty()) { // if queue is empty stop the timer
        m_coinTimer->stop();
        return;
    }

    const float COIN_SIZE_PIXELS = 50.0f;

    QPointF pos = m_coinQueue.dequeue();

    // physics setup
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x() / pixels_PerMeter, pos.y() / pixels_PerMeter);
    bodyDef.angularDamping = 0.1f;  // resist spinning
    bodyDef.linearDamping = 0.1f;   // air resistance
    b2Body* body = m_world->CreateBody(&bodyDef);

    QPixmap scaledCoin = coinPixmap->scaled(COIN_SIZE_PIXELS, COIN_SIZE_PIXELS, Qt::KeepAspectRatio);

    // shape setup
    b2CircleShape circle;
    circle.m_radius = (COIN_SIZE_PIXELS / 2.0f) / pixels_PerMeter;

    // fixture setup
    b2FixtureDef fixture;
    fixture.shape = &circle;
    fixture.density = 0.4f;
    fixture.friction = 0.1f;     // high friction
    fixture.restitution = 0.6f;  // minimal bounce
    body->CreateFixture(&fixture);

    QGraphicsPixmapItem* coin = new QGraphicsPixmapItem(scaledCoin);
    coin->setOffset(-scaledCoin.width()/2, -scaledCoin.height()/2);
    coin->setPos(pos);
    addItem(coin);
    body->SetUserData(coin);

    float angle = QRandomGenerator::global()->bounded(-20, 20) * (M_PI/180);
    float force = 4.0f + QRandomGenerator::global()->bounded(2.0f);

    body->ApplyLinearImpulse(b2Vec2(force * sin(angle), -force * cos(angle)), body->GetWorldCenter(), true);
}

void box2Dbase::initialBurst(){
    // queue coins with horizontal spread (like a real slot)
    const float slotWidth = 30.0f;
    QPointF position;
    const float COIN_SIZE_PIXELS = 50.0f;
    QPixmap scaledCoin = coinPixmap->scaled(COIN_SIZE_PIXELS, COIN_SIZE_PIXELS, Qt::KeepAspectRatio);

    for (int i = 0; i < 40; i++) {
        float offset = randomFloat(-slotWidth/2, slotWidth/2);
        position = QPointF(540, 730) + QPointF(offset, 0);
        m_coinQueue.enqueue(position);
    }

    for (int i = 0; i < 40; i++) {
        // create physics body (circle shape)
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(position.x() / pixels_PerMeter, position.y() / pixels_PerMeter);
        b2Body* body = m_world->CreateBody(&bodyDef);

        // create CIRCULAR physics shape (matches image dimensions)
        b2CircleShape circleShape;
        circleShape.m_radius = (COIN_SIZE_PIXELS / 2.0f) / pixels_PerMeter;

        // create fixture
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;  // Use the circle shape, not the pixmap
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.2f;
        fixtureDef.restitution = 0.6f;
        body->CreateFixture(&fixtureDef);

        // create graphics (PNG)
        QGraphicsPixmapItem *coinItem = new QGraphicsPixmapItem(scaledCoin);
        coinItem->setOffset(-scaledCoin.width()/2, -scaledCoin.height()/2);
        coinItem->setPos(position);
        addItem(coinItem);

        // link physics and graphics
        body->SetUserData(coinItem);

        float randx = static_cast<float>(QRandomGenerator::global()->generateDouble() * 16.0 - 8.0);
        float randy = static_cast<float>(QRandomGenerator::global()->generateDouble() * 5.0 - 16.0);
        body->ApplyLinearImpulse(b2Vec2(randx, randy), body->GetWorldCenter(), true);
    }

    // start with rapid initial burst
    if (!m_coinTimer->isActive()) {
        m_coinTimer->start(50); // First coins fast
        QTimer::singleShot(300, [this]() {
            m_coinTimer->setInterval(1000 / m_coinsPerSecond);
        });
    }
}

void box2Dbase::stopSpawning(){
    m_coinQueue.clear();
    if (m_coinTimer->isActive()) {
        m_coinTimer->stop();
    }
}

bool box2Dbase::isCoinSpawning() {
    return m_coinTimer->isActive();
}

void box2Dbase::clearCoins(){
    stopSpawning();

    b2Body* body = m_world->GetBodyList();
    while (body) {
        b2Body* next = body->GetNext();
        if (body->GetUserData()) {
            QGraphicsItem* item = static_cast<QGraphicsItem*>(body->GetUserData());
            removeItem(item);
            delete item;
        }
        m_world->DestroyBody(body);
        body = next;
    }
}
