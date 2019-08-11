#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QFrame>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QThread>

#include "aiplayer.h"

class Gameboard : public QFrame {
    Q_OBJECT

public:
    Gameboard(QWidget *parent = nullptr);
    void setDificulty(qint8 dificulty);

private:
    bool _isPvP, _isPvAI, _isPlayer1Turn, _draw;
    qint8 _dificulty, _winner;
    qint8 **_gameboard;
    QImage _blueSphere;
    QImage _greenSphere;
    AIPlayer *_aiPlayer;

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void dropBall(qint8 dropIndex, qint8 player);
    void clearTemporarySpheres();
    void checkVictory();
    void checkDraw();

public slots:
    void newGame();
    void setPvP();
    void setPvAI();
    void setEasyDificulty();
    void setMediumDificulty();
    void setHardDificulty();
};

#endif // GAMEBOARD_H
