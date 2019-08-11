#ifndef TREENODE_H
#define TREENODE_H

#include <QObject>
#include <vector>

class TreeNode {

private:
    bool _visited;
    qint8 _deph;
    qint8 _colIndex;
    qint8 _miniMax;
    qint16 _alpha;
    qint16 _beta;
    qint8 **_gameboard;
    qint16 _cost;
    TreeNode *_parent;
    std::vector<TreeNode*> *_children;

public:
    // Constructors and Destructors
    TreeNode();
    TreeNode(const TreeNode &obj);
    ~TreeNode();

    // Setters
    void setVisited(bool visited);
    void setDeph(qint8 deph);
    void setColIndex(qint8 colIndex);
    void setMiniMax(qint8 miniMax);
    void setAlpha(qint16 alpha);
    void setBeta(qint16 beta);
    void setGameboard(qint8 **gameboard);
    void setCost(qint16 cost);
    void setParent(TreeNode *parent);
    void setChildren(std::vector<TreeNode*> *children);

    // Getters
    bool visited();
    qint8 deph();
    qint8 colIndex();
    qint8 miniMax();
    qint16 alpha();
    qint16 beta();
    qint8 **gameboard();
    qint16 cost();
    TreeNode *parent();
    std::vector<TreeNode*> *children();

signals:

public slots:
};

#endif // TREENODE_H
