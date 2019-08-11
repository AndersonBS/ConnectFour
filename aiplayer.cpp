#include "aiplayer.h"

AIPlayer::AIPlayer(qint8 dificulty) {
    _dificulty = dificulty;
    _deph = 5;
}

qint8 AIPlayer::play(qint8 **gameboard) {
    if(_dificulty == HARD) {
        _deph = 5;
    }
    if(_dificulty == MEDIUM) {
        _deph = 1;
    }
    if(_dificulty == EASY) {
        return randomPlay(gameboard);
    }

    if(possibleCols(gameboard) == 1) {
        return randomPlay(gameboard);
    }

    TreeNode *tree = buildTree(gameboard);
    miniMax(tree);
    if(isValidPlay(gameboard, tree->colIndex())) {
        return tree->colIndex();
    }
    return randomPlay(gameboard);
}

qint16 AIPlayer::heuristic(qint8 **gameboard) {
    if(_dificulty == HARD) {
        return inARowScore(gameboard, P2) - inARowScore(gameboard, P1) * 10;
    }
    return inARowScore(gameboard, P2) - inARowScore(gameboard, P1) * 2;
}

TreeNode *AIPlayer::buildTree(qint8 **gameboard) {
    TreeNode *treeRoot = new TreeNode();
    treeRoot->setMiniMax(MIN);
    treeRoot->setDeph(0);
    treeRoot->setGameboard(gameboard);
    treeRoot->setCost(heuristic(gameboard));
    treeRoot->setChildren(buildChildren(treeRoot, gameboard, P2));
    treeRoot->setMiniMax(MAX);
    treeRoot->setAlpha(treeRoot->cost());
    return treeRoot;
}

std::vector<TreeNode*> *AIPlayer::buildChildren(TreeNode *parent, qint8 **gameboard, qint8 player) {
    std::vector<TreeNode*> *children = new std::vector<TreeNode*>();
    for(qint8 col = 0; col < 7; col++) {
        for(int row = 6; row > 0; row--) {
            if(gameboard[row][col] == NP) {
                qint8 **newGameboard = cloneGamebord(gameboard);
                newGameboard[row][col] = player;

                TreeNode *tmp = new TreeNode;
                tmp->setDeph(parent->deph() + 1);
                tmp->setColIndex(col);
                tmp->setGameboard(newGameboard);
                tmp->setCost(heuristic(newGameboard));
                tmp->setParent(parent);
                tmp->setMiniMax(!tmp->parent()->miniMax());
                if(tmp->miniMax() == MIN) {
                    tmp->setBeta(tmp->cost());
                } else {
                    tmp->setAlpha(tmp->cost());
                }
                if(tmp->deph() < _deph) {
                    if(player == P1) {
                        tmp->setChildren(buildChildren(tmp, newGameboard, P2));
                    } else {
                        tmp->setChildren(buildChildren(tmp, newGameboard, P1));
                    }
                }
                children->push_back(tmp);
                break;
            }
        }
    }
    return children;
}

qint8 **AIPlayer::cloneGamebord(qint8 **gameboard) {
    qint8 **copy = new qint8*[7];
    for(int x = 0; x < 7; x++) {
        copy[x] = new qint8[7];
    }

    for(int x = 0; x < 7; x++) {
        for(int y = 0; y < 7; y++) {
            copy[x][y] = gameboard[x][y];
        }
    }

    return copy;
}

qint8 AIPlayer::inARow(qint8 **gameboard, qint8 x, qint8 y, qint8 dx, qint8 dy, qint8 player) {
    if(gameboard[x][y] == player) {
        if(gameboard[x + dx][y + dy] == player) {
            if(gameboard[x + 2 * dx][y + 2 * dy] == player) {
                if(gameboard[x + 3 * dx][y + 3 * dy] == player) {
                    return 4;
                } else if(gameboard[x + 3 * dx][y + 3 * dy] == NP) {
                    return 3;
                }
            } else if(gameboard[x + 2 * dx][y + 2 * dy] == NP) {
                if(gameboard[x + 3 * dx][y + 3 * dy] == player) {
                    return 3;
                } else if(gameboard[x + 3 * dx][y + 3 * dy] == NP) {
                    return 2;
                }
            }
        } else if(gameboard[x + dx][y + dy] == NP) {
            if(gameboard[x + 2 * dx][y + 2 * dy] == player) {
                if(gameboard[x + 3 * dx][y + 3 * dy] == player) {
                    return 3;
                } else if(gameboard[x + 3 * dx][y + 3 * dy] == NP) {
                    return 2;
                }
            } else if(gameboard[x + 2 * dx][y + 2 * dy] == NP) {
                if(gameboard[x + 3 * dx][y + 3 * dy] == player) {
                    return 2;
                }
            }
        }
    } else if(gameboard[x][y] == NP) {
        if(gameboard[x + dx][y + dy] == player) {
            if(gameboard[x + 2 * dx][y + 2 * dy] == player) {
                if(gameboard[x + 3 * dx][y + 3 * dy] == player) {
                    return 3;
                } else if(gameboard[x + 3 * dx][y + 3 * dy] == NP) {
                    return 2;
                }
            }
        } else if(gameboard[x + dx][y + dy] == NP) {
            if(gameboard[x + 2 * dx][y + 2 * dy] == player) {
                if(gameboard[x + 3 * dx][y + 3 * dy] == player) {
                    return 2;
                }
            }
        }
    }
    return 0;
}

qint16 AIPlayer::inARowScore(qint8 **gameboard, qint8 player) {
    qint8 twoInARow = 0, threeInARow = 0, fourInARow = 0;

    // Check Rows
    for(qint8 x = 1; x < 7; x++) {
        for(qint8 y = 0; y < 4; y++) {
            qint8 result = inARow(gameboard, x, y, 0, 1, player);
            if(result == 4) {
                fourInARow++;
            } else if(result == 3) {
                threeInARow++;
            } else if(result == 2) {
                twoInARow++;
            }
        }
    }

    // Check Columns
    for(qint8 y = 0; y < 7; y++) {
        for(qint8 x = 1; x < 4; x++) {
            qint8 result = inARow(gameboard, x, y, 1, 0, player);
            if(result == 4) {
                fourInARow++;
            } else if(result == 3) {
                threeInARow++;
            } else if(result == 2) {
                twoInARow++;
            }
        }
    }

    // Check Left-to-Right Diagonals
    for(qint8 index = 4; index < 7; index++) {
        for(qint8 x = 1, y = 7 - index; y < 4; x++, y++) {
            qint8 result = inARow(gameboard, x, y, 1, 1, player);
            if(result == 4) {
                fourInARow++;
            } else if(result == 3) {
                threeInARow++;
            } else if(result == 2) {
                twoInARow++;
            }
        }

        for(qint8 x = 7 - index, y = 0; x < 4; x++, y++) {
            qint8 result = inARow(gameboard, x, y, 1, 1, player);
            if(result == 4) {
                fourInARow++;
            } else if(result == 3) {
                threeInARow++;
            } else if(result == 2) {
                twoInARow++;
            }
        }
    }

    // Check Right-to-Left Diagonals
    for(qint8 index = 3; index < 6; index++) {
        for(qint8 x = 1, y = index; y >= 3; x++, y--) {
            qint8 result = inARow(gameboard, x, y, 1, -1, player);
            if(result == 4) {
                fourInARow++;
            } else if(result == 3) {
                threeInARow++;
            } else if(result == 2) {
                twoInARow++;
            }
        }

        for(qint8 x = 6 - index, y = 6; y >= 9 - index; x++, y--) {
            qint8 result = inARow(gameboard, x, y, 1, -1, player);
            if(result == 4) {
                fourInARow++;
            } else if(result == 3) {
                threeInARow++;
            } else if(result == 2) {
                twoInARow++;
            }
        }
    }
    return fourInARow * 1000 + threeInARow * 25 + twoInARow;
}

void AIPlayer::miniMax(TreeNode *treeRoot) {
    TreeNode *currentNode = treeRoot;
    while(currentNode->children() != nullptr) {
        currentNode = currentNode->children()->front();
    }
    while(currentNode->parent() != nullptr) {
        currentNode = currentNode->parent();
        for(unsigned int index = 0; index < currentNode->children()->size(); index++) {
            bool needBreak = false;
            TreeNode *currentChild = currentNode->children()->at(index);
            if(currentChild->visited()) {
                continue;
            }
            if(currentChild->children() != nullptr) {
                for(unsigned int chIndex = 0; chIndex < currentChild->children()->size(); chIndex++) {
                    TreeNode *tmpChild = currentChild->children()->at(chIndex);
                    if(!tmpChild->visited()) {
                        currentNode = tmpChild;
                        needBreak = true;
                        break;
                    }
                }
            }
            if(needBreak) {
                break;
            }
            currentChild->setVisited(true);
            if(currentNode->miniMax() == MIN) {
                if(currentNode->cost() > currentChild->cost()) {
                    currentNode->setCost(currentChild->cost());
                    currentNode->setColIndex(currentChild->colIndex());
                    currentNode->setBeta(currentChild->cost());
                }
            } else {
                if(currentNode->cost() < currentChild->cost()) {
                    currentNode->setCost(currentChild->cost());
                    currentNode->setColIndex(currentChild->colIndex());
                    currentNode->setAlpha(currentChild->cost());
                }
            }
            if(currentNode->miniMax() == MIN) {
                if(alphaAncester(currentNode) >= currentNode->beta()) {
                    break;
                }
            } else {
                if(betaAncester(currentNode) <= currentNode->alpha()) {
                    break;
                }
            }
        }
        currentNode->setVisited(true);
    }
    for(unsigned int index = 0; index < treeRoot->children()->size(); index++) {
        currentNode = treeRoot->children()->at(index);
        if(treeRoot->cost() < currentNode->cost()) {
            treeRoot->setCost(currentNode->cost());
            treeRoot->setColIndex(currentNode->colIndex());
            treeRoot->setAlpha(currentNode->cost());
        }
    }
}

qint16 AIPlayer::alphaAncester(TreeNode *treeNode) {
    qint16 alpha = -32000;
    while(treeNode->parent() != nullptr) {
        if(treeNode->alpha() > alpha) {
            alpha = treeNode->alpha();
        }
        treeNode = treeNode->parent();
    }
    return alpha;
}

qint16 AIPlayer::betaAncester(TreeNode *treeNode) {
    qint16 beta = 32000;
    while(treeNode->parent() != nullptr) {
        if(treeNode->beta() < beta) {
            beta = treeNode->beta();
        }
        treeNode = treeNode->parent();
    }
    return beta;
}

bool AIPlayer::isValidPlay(qint8 **gameboard, qint8 col) {
    if(gameboard[1][col] == NP) {
        return true;
    }
    return false;
}

qint8 AIPlayer::randomPlay(qint8 **gameboard) {
    qint8 play, currentTry = 0;
    do {
        if(currentTry >= 10) {
            play = currentTry % 7;
        }
        play = std::rand() % 7;
        currentTry++;
    } while(!isValidPlay(gameboard, play));
    return play;
}

qint8 AIPlayer::possibleCols(qint8 **gameboard) {
    qint8 possibleCols = 0;
    for(int col = 0; col < 7; col++) {
        if(gameboard[1][col] == NP) {
            possibleCols++;
        }
    }
    return possibleCols;
}
