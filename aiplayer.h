#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <QObject>
#include <cstdlib>
#include "treenode.h"

#define EASY    1
#define MEDIUM  2
#define HARD    3

#define NP      0
#define P1      1
#define P2      2
#define P1TMP   3
#define P2TMP   4

#define MIN     0
#define MAX     1

class AIPlayer : public QObject {
    Q_OBJECT

public:
    AIPlayer(qint8 dificulty);
    qint8 play(qint8 **gameboard);

private:
    qint8 _dificulty;
    qint8 _deph;

    qint16 heuristic(qint8 **gameboard);
    TreeNode *buildTree(qint8 **gameboard);
    std::vector<TreeNode*> *buildChildren(TreeNode *parent, qint8 **gameboard, qint8 player);
    qint8 **cloneGamebord(qint8 **gameboard);
    qint8 inARow(qint8 **gameboard, qint8 x, qint8 y, qint8 dx, qint8 dy, qint8 player);
    qint16 inARowScore(qint8 **gameboard, qint8 player);
    void miniMax(TreeNode *treeRoot);
    qint16 alphaAncester(TreeNode *treeNode);
    qint16 betaAncester(TreeNode *treeNode);
    bool isValidPlay(qint8 **gameboard, qint8 col);
    qint8 randomPlay(qint8 **gameboard);
    qint8 possibleCols(qint8 **gameboard);
};

#endif // AIPLAYER_H
