#include <iostream>

using namespace std;

struct Node
{
    int data;
    Node *left;
    Node *right;
    int height;
};

typedef Node *NodePtr;

class AVLTree
{
private:
    NodePtr root;

    NodePtr createNewNode(int data)
    {
        NodePtr node = new Node;
        node->data = data;
        node->left = nullptr;
        node->right = nullptr;
        node->height = 1;
        return node;
    }

    int getHeight(NodePtr node)
    {
        if (!node)
        {
            return 0;
        }
        return node->height;
    }

    int getBalanceFactor(NodePtr node)
    {
        if (!node)
        {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }

    void updateHeight(NodePtr node)
    {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    NodePtr leftRotate(NodePtr node)
    {
        // rotate
        NodePtr right = node->right;
        NodePtr rleft = right->left;

        right->left = node;
        node->right = rleft;

        // update height
        // only child-changed node need to update
        updateHeight(node);
        updateHeight(right);

        return right;
    }

    NodePtr rightRotate(NodePtr node)
    {
        // rotate
        NodePtr left = node->left;
        NodePtr lright = left->right;

        left->right = node;
        node->left = lright;

        // update height
        // only child-changed node need to update
        updateHeight(node);
        updateHeight(left);

        return left;
    }

    NodePtr insertHelper(NodePtr node, int data)
    {
        // 1. 二叉搜索树式插入
        // 叶子节点插入位置
        if (!node)
        {
            return createNewNode(data);
        }
        if (data < node->data)
        {
            node->left = insertHelper(node->left, data);
        }
        else if (data > node->data)
        {
            node->right = insertHelper(node->right, data);
        }
        else
        {
            return node;
        }

        // 2.后序方式，更新当前节点平衡因子并进行平衡修正
        // 实际上在插入节点的祖父节点处才会进入以下的计算
        updateHeight(node);
        int balanceFactor = getBalanceFactor(node);

        // 情況1a
        if (balanceFactor > 1 && data > node->left->data)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // 情況1b
        if (balanceFactor > 1 && data < node->left->data)
        {
            return rightRotate(node);
        }
        // 情況2a
        if (balanceFactor < -1 && data < node->right->data)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        // 情況2b
        if (balanceFactor < -1 && data > node->right->data)
        {
            return leftRotate(node);
        }

        return node;
    }

    NodePtr findMin(NodePtr node)
    {
        while (node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }

    NodePtr deleteHelper(NodePtr node, int data)
    {
        // 1. 二叉搜索树方式删除节点
        if (!node)
        {
            return node;
        }
        if (data < node->data)
        {
            node->left = deleteHelper(node->left, data);
        }
        else if (data > node->data)
        {
            node->right = deleteHelper(node->right, data);
        }
        else
        {
            if (!root->left)
            {
                NodePtr temp = root->right;
                delete root;
                root = temp;
            }
            else if (!root->right)
            {
                NodePtr temp = root->left;
                delete root;
                root = temp;
            }
            else
            {
                NodePtr temp = findMin(root->right);
                root->data = temp->data;
                deleteHelper(root->right, temp->data);
            }
        }

        // 2.后序方式，更新当前节点平衡因子并进行平衡修正
        updateHeight(node);
        int balanceFactor = getBalanceFactor(node);

        // 情況1a
        if (balanceFactor > 1 && getBalanceFactor(node->left) < 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // 情況1b
        if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0)
        {
            return rightRotate(node);
        }

        // 情況2a
        if (balanceFactor < -1 && getBalanceFactor(node->right) > 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        // 情況2b
        if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0)
        {
            return leftRotate(node);
        }

        return node;
    }

public:
    NodePtr insertNode(int data)
    {
        insertHelper(this->root, data);
    }

    NodePtr deleteNode(int data)
    {
        deleteHelper(this->root, data);
    }
};