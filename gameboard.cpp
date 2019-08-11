#include "gameboard.h"

Gameboard::Gameboard(QWidget *parent) : QFrame(parent) {
    _gameboard = new qint8*[7];
    for(int x = 0; x < 7; x++) {
        _gameboard[x] = new qint8[7];
    }
    setMouseTracking(true);
    _dificulty = HARD;
    _winner = NP;
    _isPvAI = true;
    _isPvP  = _draw = false;
    _aiPlayer = nullptr;
    _blueSphere.load(":/Resources/Images/BlueSphere.png");
    _greenSphere.load(":/Resources/Images/GreenSphere.png");
    newGame();
}

void Gameboard::newGame() {
    if(_aiPlayer != nullptr) {
        delete _aiPlayer;
        _aiPlayer = nullptr;
    }
    for(int x = 0; x < 7; x++) {
        for(int y = 0; y < 7; y++) {
            _gameboard[x][y] = NP;
        }
    }
    if(_isPvAI) {
        _aiPlayer = new AIPlayer(_dificulty);
    }
    _isPlayer1Turn = true;
    _winner = NP;
    _draw = false;
    repaint();
}

void Gameboard::setDificulty(qint8 dificulty) {
    _dificulty = dificulty;
    newGame();
}

void Gameboard::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(QRect(0, 0, width(), height()), Qt::black);


    qreal widthStep = width() / 7;
    qreal heightStep = height() / 7;

    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
    for(qint8 x = 1; x < 7; x++) {
        painter.drawLine(QPoint(0, static_cast<int>(heightStep * x)), QPoint(width(), static_cast<int>(heightStep * x)));
        painter.drawLine(QPoint(static_cast<int>(widthStep * x), 0), QPoint(static_cast<int>(widthStep * x), height()));
    }

    painter.setOpacity(0.5);
    painter.fillRect(QRect(0, 0, width(), static_cast<int>(heightStep)), Qt::gray);
    painter.setOpacity(1);

    for(qint8 x = 0; x < 7; x++) {
        for(qint8 y = 0; y < 7; y++) {
            if(_gameboard[x][y] == P1 || _gameboard[x][y] == P1TMP) {
                painter.drawImage(QRect(static_cast<int>(widthStep * y), static_cast<int>(heightStep * x),
                                        static_cast<int>(widthStep), static_cast<int>(heightStep)), _blueSphere);
            } else if(_gameboard[x][y] == P2 || _gameboard[x][y] == P2TMP) {
                painter.drawImage(QRect(static_cast<int>(widthStep * y), static_cast<int>(heightStep * x),
                                        static_cast<int>(widthStep), static_cast<int>(heightStep)), _greenSphere);
            }
        }
    }

    if(_winner) {
        painter.setPen(QPen(Qt::yellow));
        painter.setOpacity(0.5);
        painter.fillRect(QRect(0, 0, width(), height()), Qt::white);
        painter.setOpacity(1);
        painter.setFont(QFont("Arial", 60, QFont::Bold));
        painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, "GAME OVER\nPlayer "
                         + QString::number(_winner) + " won!");
    } else if(_draw) {
        painter.setPen(QPen(Qt::yellow));
        painter.setOpacity(0.5);
        painter.fillRect(QRect(0, 0, width(), height()), Qt::white);
        painter.setOpacity(1);
        painter.setFont(QFont("Arial", 60, QFont::Bold));
        painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, "GAME OVER\nDraw!");
    }
}

void Gameboard::mouseMoveEvent(QMouseEvent *event) {
    if(_isPvP || (_isPvAI && _isPlayer1Turn)) {
        clearTemporarySpheres();
        if(_isPlayer1Turn) {
            _gameboard[0][static_cast<int>(7.0 / width() * event->pos().x())] = P1TMP;
        } else {
            _gameboard[0][static_cast<int>(7.0 / width() * event->pos().x())] = P2TMP;
        }
        repaint();
    }
}

void Gameboard::mousePressEvent(QMouseEvent *event) {
    if(_winner || _draw) {
        _winner = NP;
        _draw = false;
        newGame();
    } else {
        qint8 dropIndex = static_cast<qint8>(7.0 / width() * event->pos().x());
        if((_isPvP || _isPlayer1Turn) && _gameboard[1][dropIndex] == NP) {
            if(_isPlayer1Turn) {
                dropBall(dropIndex, P1);
            } else {
                dropBall(dropIndex, P2);
            }
            _isPlayer1Turn = !_isPlayer1Turn;
            checkVictory();
            checkDraw();

            if(!_winner && !_draw && _isPvAI) {
                dropBall(_aiPlayer->play(_gameboard), P2);
                _isPlayer1Turn = !_isPlayer1Turn;
                checkVictory();
                 checkDraw();
            }
        }
    }
}

void Gameboard::dropBall(qint8 dropIndex, qint8 player) {
    for(int x = 1; x < 7; x++) {
        if(_gameboard[x][dropIndex] == NP) {
            clearTemporarySpheres();
            _gameboard[x][dropIndex] = player + 2;
           repaint();
           QThread::msleep(100);
        } else {
            _gameboard[x - 1][dropIndex] = player;
            break;
        }
        if(x == 6) {
            _gameboard[x][dropIndex] = player;
        }
    }
}

void Gameboard::clearTemporarySpheres() {
    for(int x = 0; x < 7; x++) {
        for(int y = 0; y < 7; y++) {
            if(_gameboard[x][y] == P1TMP || _gameboard[x][y] == P2TMP) {
                _gameboard[x][y] = NP;
            }
        }
    }
}

void Gameboard::checkVictory() {
    qint8 p1InARow, p2InARow;

    // Check Rows
    for(int x = 1; x < 7; x++) {
        p1InARow = p2InARow = 0;
        for(int y = 0; y < 7; y++) {
            if(_gameboard[x][y] == P1) {
                p1InARow++;
                if(p2InARow) {
                    p2InARow = 0;
                }
            } else if(_gameboard[x][y] == P2) {
                p2InARow++;
                if(p1InARow) {
                    p1InARow = 0;
                }
            } else {
                p1InARow = p2InARow = 0;
            }
            if(p1InARow == 4) {
               _winner = P1;
               repaint();
               return;
            }
            if(p2InARow == 4) {
               _winner = P2;
               repaint();
               return;
            }
        }
    }

    // Check Columns
    for(int y = 0; y < 7; y++) {
        p1InARow = p2InARow = 0;
        for(int x = 1; x < 7; x++) {
            if(_gameboard[x][y] == P1) {
                p1InARow++;
                if(p2InARow) {
                    p2InARow = 0;
                }
            } else if(_gameboard[x][y] == P2) {
                p2InARow++;
                if(p1InARow) {
                    p1InARow = 0;
                }
            } else {
                p1InARow = p2InARow = 0;
            }
            if(p1InARow == 4) {
               _winner = P1;
               repaint();
               return;
            }
            if(p2InARow == 4) {
               _winner = P2;
               repaint();
               return;
            }
        }
    }

    // Check Left-to-Right Diagonals
    for(int index = 4; index < 7; index++) {
        p1InARow = p2InARow = 0;
        for(int x = 1, y = 7 - index; y < 7; x++, y++) {
            if(_gameboard[x][y] == P1) {
                p1InARow++;
                if(p2InARow) {
                    p2InARow = 0;
                }
            } else if(_gameboard[x][y] == P2) {
                p2InARow++;
                if(p1InARow) {
                    p1InARow = 0;
                }
            } else {
                p1InARow = p2InARow = 0;
            }
            if(p1InARow == 4) {
               _winner = P1;
               repaint();
               return;
            }
            if(p2InARow == 4) {
               _winner = P2;
               repaint();
               return;
            }
        }
        p1InARow = p2InARow = 0;
        for(int x = 7 - index, y = 0; x < 7; x++, y++) {
            if(_gameboard[x][y] == P1) {
                p1InARow++;
                if(p2InARow) {
                    p2InARow = 0;
                }
            } else if(_gameboard[x][y] == P2) {
                p2InARow++;
                if(p1InARow) {
                    p1InARow = 0;
                }
            } else {
                p1InARow = p2InARow = 0;
            }
            if(p1InARow == 4) {
               _winner = P1;
               repaint();
               return;
            }
            if(p2InARow == 4) {
               _winner = P2;
               repaint();
               return;
            }
        }
    }

    // Check Right-to-Left Diagonals
    for(int index = 3; index < 6; index++) {
        p1InARow = p2InARow = 0;
        for(int x = 1, y = index; y >= 0; x++, y--) {
            if(_gameboard[x][y] == P1) {
                p1InARow++;
                if(p2InARow) {
                    p2InARow = 0;
                }
            } else if(_gameboard[x][y] == P2) {
                p2InARow++;
                if(p1InARow) {
                    p1InARow = 0;
                }
            } else {
                p1InARow = p2InARow = 0;
            }
            if(p1InARow == 4) {
               _winner = P1;
               repaint();
               return;
            }
            if(p2InARow == 4) {
               _winner = P2;
               repaint();
               return;
            }
        }

        p1InARow = p2InARow = 0;
        for(int x = 6 - index, y = 6; y >= 6 - index; x++, y--) {
            if(_gameboard[x][y] == P1) {
                p1InARow++;
                if(p2InARow) {
                    p2InARow = 0;
                }
            } else if(_gameboard[x][y] == P2) {
                p2InARow++;
                if(p1InARow) {
                    p1InARow = 0;
                }
            } else {
                p1InARow = p2InARow = 0;
            }
            if(p1InARow == 4) {
               _winner = P1;
               repaint();
               return;
            }
            if(p2InARow == 4) {
               _winner = P2;
               repaint();
               return;
            }
        }
    }

}

void Gameboard::checkDraw() {
    qint8 possibleCols = 0;
    for(int col = 0; col < 7; col++) {
        if(_gameboard[1][col] == NP) {
            possibleCols++;
        }
    }
    if(!possibleCols) {
        _draw = true;
        repaint();
    }
}

void Gameboard::setPvP() {
    _isPvP = true;
    _isPvAI = false;
    newGame();
}

void Gameboard::setPvAI() {
    _isPvP = false;
    _isPvAI = true;
    newGame();
}

void Gameboard::setEasyDificulty() {
    _dificulty = EASY;
    newGame();
}

void Gameboard::setMediumDificulty() {
    _dificulty = MEDIUM;
    newGame();
}

void Gameboard::setHardDificulty() {
    _dificulty = HARD;
    newGame();
}
