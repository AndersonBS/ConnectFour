#include "treenode.h"

// Constructors

TreeNode::TreeNode(const TreeNode &obj) {
    TreeNode tmp = obj;

    _gameboard = new qint8*[7];
    for(int x = 0; x < 7; x++) {
        _gameboard[x] = new qint8[7];
    }

    for(int x = 0; x < 7; x++) {
        for(int y = 0; y < 7; y++) {
            _gameboard[x][y] = tmp.gameboard()[x][y];
        }
    }

    _visited = tmp.visited();
    _deph = tmp.deph();
    _colIndex = tmp.colIndex();
    _miniMax = tmp.miniMax();
    _alpha = tmp.alpha();
    _beta = tmp.beta();
    _cost = tmp.cost();
    _parent = tmp.parent();
    _children = tmp.children();
}

TreeNode::TreeNode() {
    _gameboard = new qint8*[7];
    for(int x = 0; x < 7; x++) {
        _gameboard[x] = new qint8[7];
    }
    _visited = false;
    _colIndex = 3;
    _miniMax = 1;
    _alpha = -32000;
    _beta = 32000;
    _cost = -32000;
    _parent = nullptr;
    _children = nullptr;
}

TreeNode::~TreeNode() {
    if(_gameboard != nullptr) {
        delete _gameboard;
    }
    if(_parent != nullptr) {
        delete _parent;
    }
    if(_children != nullptr) {
        delete _children;
    }
}


// Setters

void TreeNode::setVisited(bool visited) {
    _visited = visited;
}

void TreeNode::setDeph(qint8 deph) {
    _deph = deph;
}

void TreeNode::setColIndex(qint8 colIndex) {
    _colIndex = colIndex;
}

void TreeNode::setMiniMax(qint8 miniMax) {
    _miniMax = miniMax;
}

void TreeNode::setAlpha(qint16 alpha) {
    _alpha = alpha;
}

void TreeNode::setBeta(qint16 beta) {
    _beta = beta;
}

void TreeNode::setGameboard(qint8 **gameboard) {
    for(int x = 0; x < 7; x++) {
        for(int y = 0; y < 7; y++) {
            _gameboard[x][y] = gameboard[x][y];
        }
    }
}

void TreeNode::setCost(qint16 cost) {
    _cost = cost;
}

void TreeNode::setParent(TreeNode *parent) {
    _parent = parent;
}

void TreeNode::setChildren(std::vector<TreeNode*> *children) {
    _children = children;
}



// Getters

bool TreeNode::visited() {
    return _visited;
}

qint8 TreeNode::deph() {
    return _deph;
}

qint8 TreeNode::colIndex() {
    return _colIndex;
}

qint8 TreeNode::miniMax() {
    return _miniMax;
}

qint16 TreeNode::alpha() {
    return _alpha;
}

qint16 TreeNode::beta() {
    return _beta;
}

qint8 **TreeNode::gameboard() {
    return _gameboard;
}

qint16 TreeNode::cost() {
    return _cost;
}

TreeNode *TreeNode::parent() {
    return _parent;
}

std::vector<TreeNode*> *TreeNode::children() {
    return _children;
}
