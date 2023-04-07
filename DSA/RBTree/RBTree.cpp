#include <iostream>

using namespace std;

struct Node
{
    int data;
    Node *parent;
    Node *left;
    Node *right;
    int color;
};

// 定义节点指针类型，方便引用
typedef Node *NodePtr;

class RBTree
{
private:
    NodePtr root;
    NodePtr TNULL;

    NodePtr searchTreeHelper(NodePtr node, int key)
    {
        if (node == TNULL || key == node->data)
        {
            return node;
        }

        if (key < node->data)
        {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    NodePtr minimum(NodePtr node)
    {
        while (node->left != TNULL)
        {
            node = node->left;
        }
        return node;
    }

    void leftRotate(NodePtr x)
    {
        NodePtr y = x->right;
        x->right = y->left;
        if (y->left != TNULL)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(NodePtr x)
    {
        NodePtr y = x->left;
        x->left = y->right;
        if (y->right != TNULL)
        {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }
    void insertFix(NodePtr k)
    {
        NodePtr u;
        // 情况1:根结点&情况2:黑父节点
        while (k->parent != nullptr && k->parent->color == 1)
        {
            // 父节点为右孩子
            if (k->parent == k->parent->parent->right)
            {
                // 获取叔叔节点
                u = k->parent->parent->left;

                // 情况3:叔叔节点为红
                if (u->color == 1)
                {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    // 矛盾转移至祖父节点
                    k = k->parent->parent;
                }

                // 叔叔节点为黑
                else
                {
                    // 情况4:父子偏向不一致
                    if (k == k->parent->left)
                    {
                        k = k->parent;
                        rightRotate(k);
                    }
                    // 情况5:父子偏向一致
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
                }
            }
            // 父节点为左孩子，类似
            else
            {
                u = k->parent->parent->right;

                if (u->color == 1)
                {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->right)
                    {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
        }
        // 根结点染黑
        root->color = 0;
    }

    void deleteFix(NodePtr x)
    {
        NodePtr s;
        // 情况1:根结点&情况2:红节点
        while (x != root && x->color == 0)
        {
            // x为左节点
            if (x == x->parent->left)
            {
                s = x->parent->right;
                // 情况3:兄弟节点为红色
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent);
                    // 获得新的兄弟节点
                    s = x->parent->right;
                }

                // 情况4:兄弟节点为黑色，侄子节点为黑色
                if (s->left->color == 0 && s->right->color == 0)
                {
                    s->color = 1;
                    // 父亲节点变为双黑，再次判断
                    x = x->parent;
                }
                else
                {
                    // 情况5: 侄子远黑近红
                    if (s->right->color == 0)
                    {
                        s->left->color = 0;
                        s->color = 1;
                        rightRotate(s);
                        s = x->parent->right;
                    }

                    // 情况6: 侄子远红近黑
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    leftRotate(x->parent);
                    // 终止循环
                    x = root;
                }
            }
            // x为右节点，类似
            else
            {
                s = x->parent->left;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }

                if (s->right->color == 0 && s->right->color == 0)
                {
                    s->color = 1;
                    x = x->parent;
                }
                else
                {
                    if (s->left->color == 0)
                    {
                        s->right->color = 0;
                        s->color = 1;
                        leftRotate(s);
                        s = x->parent->left;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = 0;
    }

public:
    RBTree()
    {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }
    NodePtr searchTree(int k)
    {
        return searchTreeHelper(this->root, k);
    }

    void insertNode(int key)
    {
        // 1. new一个新节点
        NodePtr node = new Node;
        node->parent = nullptr;
        node->data = key;
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1;

        NodePtr y = nullptr;
        NodePtr x = this->root;

        // 2. 找到插入位置
        while (x != TNULL)
        {
            y = x;
            if (node->data < x->data)
            {
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }

        // 3. 建立父子连接
        node->parent = y;
        if (y == nullptr)
        {
            root = node;
        }
        else if (node->data < y->data)
        {
            y->left = node;
        }
        else
        {
            y->right = node;
        }

        insertFix(node);
    }

    void deleteNode(int key)
    {
        // 1. 找到key对应的节点
        NodePtr z = TNULL;
        z = searchTreeHelper(this->root, key);
        if (z == TNULL)
        {
            cout << "Key not found in the tree" << endl;
            return;
        }

        // 2. 找到对应的代替节点
        NodePtr y = TNULL;
        if (z->left == TNULL && z->right == TNULL)
        {
            y = z;
        }
        else if (z->left == TNULL)
        {
            y = z->right;
        }
        else if (z->right == TNULL)
        {
            y = z->left;
        }
        else
        {
            y = minimum(z->right);
        }
        z->data = y->data;
        z = y;

        // 3. 删除修正
        deleteFix(y);
        // 修正完后删除这个节点
        if (y->data < y->parent->data){
            y->parent->left = TNULL;
        }
        else{
            y->parent->right = TNULL;
        }
        delete y;
        y = NULL;
        z = NULL;
    }

    void printHelper(NodePtr root, string indent, bool last)
    {
        if (root != TNULL)
        {
            cout << indent;
            if (last)
            {
                cout << "R----";
                indent += "   ";
            }
            else
            {
                cout << "L----";
                indent += "|  ";
            }

            string sColor = root->color ? "RED" : "BLACK";
            cout << root->data << "(" << sColor << ")" << endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

    void printTree()
    {
        if (root)
        {
            printHelper(this->root, "", true);
        }
    }
};

int main()
{
    RBTree bst;
    bst.insertNode(55);
    bst.insertNode(40);
    bst.insertNode(65);
    bst.insertNode(60);
    bst.insertNode(75);
    bst.insertNode(57);

    bst.printTree();
    cout << endl
         << "After deleting" << endl;
    bst.deleteNode(55);
    bst.printTree();
    return 0;
}