#include <iostream>

using namespace std;

struct Node
{
    int data;
    Node *left;
    Node *right;
};

// 定义节点指针类型，方便引用
typedef Node *NodePtr;

class BSTree
{
private:
    NodePtr root;

    NodePtr newNode(int data)
    {
        NodePtr node = new Node;
        node->data = data;
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }

    NodePtr searchTreeHelper(NodePtr node, int key)
    {
        if (node == nullptr || key == node->data)
        {
            return node;
        }

        if (key < node->data)
        {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    NodePtr findMin(NodePtr node)
    {
        while (node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }

public:
    BSTree(int data)
    {
        root = newNode(data);
    }
    NodePtr searchTree(int k)
    {
        return searchTreeHelper(this->root, k);
    }

    NodePtr insertNode(NodePtr node, int data)
    {
        if (node == nullptr)
            return newNode(data);

        if (data < node->data)
        {
            node->left = insertNode(node->left, data);
        }
        else
        {
            node->right = insertNode(node->right, data);
        }
        return node;
    }

    void deleteNode(NodePtr &root, int value)
    {
        if (!root)
        {
            return;
        }
        if (value < root->data)
        {
            deleteNode(root->left, value);
        }
        else if (value > root->data)
        {
            deleteNode(root->right, value);
        }
        else
        {
            // 情况1&情况2的一部分：为叶子节点或只有右节点
            if (!root->left)
            {
                NodePtr temp = root->right;
                delete root;
                root = temp;
            }
            // 只有左节点
            else if (!root->right)
            {
                NodePtr temp = root->left;
                delete root;
                root = temp;
            }
            // 左右节点都没有，使用后继节点代替，并在右子树中删除后继节点
            else
            {
                NodePtr temp = findMin(root->right);
                root->data = temp->data;
                deleteNode(root->right, temp->data);
            }
        }
    }
};