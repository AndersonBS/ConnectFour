#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    this->setWindowTitle("Connect Four");
    _gameboard = new Gameboard(this);
    setCentralWidget(_gameboard);

    QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));
    QAction *newGameAction = gameMenu->addAction(tr("&New"));
    connect(newGameAction, SIGNAL(triggered(bool)), _gameboard, SLOT(newGame()));
    QAction *quitGameAction = gameMenu->addAction(tr("&Quit"));
    connect(quitGameAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    QMenu *settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addSection("Players");
    QActionGroup *playerGroup = new QActionGroup(this);
    QAction *pvpAction = settingsMenu->addAction("Player Vs Player");
    pvpAction->setCheckable(true);
    playerGroup->addAction(pvpAction);
    connect(pvpAction, SIGNAL(triggered(bool)), _gameboard, SLOT(setPvP()));
    QAction *pvaiAction = settingsMenu->addAction("Player Vs AI");
    pvaiAction->setCheckable(true);
    pvaiAction->setChecked(true);
    playerGroup->addAction(pvaiAction);
    connect(pvaiAction, SIGNAL(triggered(bool)), _gameboard, SLOT(setPvAI()));

    settingsMenu->addSection("Difficulty");
    QActionGroup *dificultyGroup = new QActionGroup(this);
    QAction *easyAction = settingsMenu->addAction(tr("&Easy"));
    easyAction->setCheckable(true);
    dificultyGroup->addAction(easyAction);
    connect(easyAction, SIGNAL(triggered(bool)), _gameboard, SLOT(setEasyDificulty()));
    QAction *mediumAction = settingsMenu->addAction(tr("&Medium"));
    mediumAction->setCheckable(true);
    dificultyGroup->addAction(mediumAction);
    connect(mediumAction, SIGNAL(triggered(bool)), _gameboard, SLOT(setMediumDificulty()));
    QAction *hardAction = settingsMenu->addAction(tr("&Hard"));
    hardAction->setCheckable(true);
    hardAction->setChecked(true);
    dificultyGroup->addAction(hardAction);
    connect(hardAction, SIGNAL(triggered(bool)), _gameboard, SLOT(setHardDificulty()));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *gameRulesAction = helpMenu->addAction(tr("&Game Rules"));
    connect(gameRulesAction, SIGNAL(triggered(bool)), this, SLOT(gameRules()));

    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(aboutGame()));

    setStatusBar(new QStatusBar(this));
}

MainWindow::~MainWindow() {

}

void MainWindow::gameRules() {
    QDialog dialog(this);
    QVBoxLayout form(&dialog);
    form.addWidget(new QLabel("GAME RULES:"));
    form.addWidget(new QLabel(""));
    form.addWidget(new QLabel("Connect Four (also known as Captain's Mistress, Four Up, Plot Four, Find Four, Fourplay, \n"
                              "Four in a Row, Four in a Line and Gravitrips (in Soviet Union) ) is a two-player connection \n"
                              "game in which the players first choose a color and then take turns dropping colored discs \n"
                              "from the top into a seven-column, six-row vertically suspended grid. The pieces fall straight \n"
                              "down, occupying the next available space within the column. The objective of the game is to be \n"
                              "the first to form a horizontal, vertical, or diagonal line of four of one's own discs. Connect \n"
                              "Four is a strongly solved game. The first player can always win by playing the right moves."));
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    form.addWidget(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    dialog.exec();
}

void MainWindow::aboutGame() {
    QDialog dialog(this);
    QVBoxLayout form(&dialog);
    form.addWidget(new QLabel("UNOESC - Universidade do Estado de Santa Catarina:"));
    form.addWidget(new QLabel(""));
    form.addWidget(new QLabel("Artificial Intelligence I"));
    form.addWidget(new QLabel("Anderson B. Sensolo (anderson.sensolo@gmail.com)"));
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    form.addWidget(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    dialog.exec();
}
