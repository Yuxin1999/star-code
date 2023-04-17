#include <iostream>
#include <stack>
#include <vector>
#include <queue>
using namespace std;

struct node
{
    int data;
    node *left;
    node *right;
};

typedef node *NodePtr;

class BinaryTree
{
private:
    NodePtr root;

    void preOrderRecursion(NodePtr node)
    {
        if (node == nullptr)
        {
            return;
        }
        cout << node->data << " ";
        preOrderRecursion(node->left);
        preOrderRecursion(node->right);
    }

    void preOrderIteration(NodePtr node)
    {
        if (node == nullptr)
        {
            return;
        }

        stack<NodePtr> nodeStack;
        nodeStack.push(node);

        while (!nodeStack.empty())
        {
            NodePtr currNode = nodeStack.top();
            nodeStack.pop();

            cout << currNode->data << " ";
            // 先进后出，因此要先放右节点，再放子节点
            if (currNode->right != nullptr)
            {
                nodeStack.push(currNode->right);
            }
            if (currNode->left != nullptr)
            {
                nodeStack.push(currNode->left);
            }
        }
    }

    void inorderRecursion(NodePtr node)
    {
        if (node == nullptr)
        {
            return;
        }

        inorderRecursion(node->left);
        cout << node->data << " ";
        inorderRecursion(node->right);
    }

    void inorderIteration(NodePtr node)
    {
        if (node == nullptr)
        {
            return;
        }

        stack<NodePtr> nodeStack;
        NodePtr currNode = node;

        while (currNode != nullptr || !nodeStack.empty())
        {
            // Traverse left subtree
            while (currNode != nullptr)
            {
                nodeStack.push(currNode);
                currNode = currNode->left;
            }

            // Process current node
            currNode = nodeStack.top();
            nodeStack.pop();
            cout << currNode->data << " ";

            // Traverse right subtree
            currNode = currNode->right;
        }
    }

    void postorderRecursion(NodePtr node)
    {
        if (node == nullptr)
        {
            return;
        }

        postorderRecursion(node->left);
        postorderRecursion(node->right);
        cout << node->data << " ";
    }

    void postorderIteration(NodePtr node)
    {
        if (node == nullptr)
        {
            return;
        }

        stack<NodePtr> nodeStack;
        NodePtr currNode = node;
        NodePtr lastVisitedNode = nullptr;

        while (currNode != nullptr || !nodeStack.empty())
        {
            // 一路向左
            while (currNode != nullptr)
            {
                nodeStack.push(currNode);
                currNode = currNode->left;
            }

            NodePtr peekNode = nodeStack.top();
            // 判断栈顶节点的右节点是否被访问过
            if (peekNode->right != nullptr && peekNode->right != lastVisitedNode)
            {
                // 遍历右子树
                currNode = currNode->right;
            }
            else
            {
                // 访问当前节点
                cout << peekNode->data << " ";
                lastVisitedNode = nodeStack.top();
                nodeStack.pop();
            }
        }
    }

    void levelOrderIteration(NodePtr node)
    {
        if (node == nullptr)
        {
            return;
        }

        queue<NodePtr> nodeQueue;
        nodeQueue.push(node);

        while (!nodeQueue.empty())
        {
            NodePtr currNode = nodeQueue.front();
            nodeQueue.pop();
            cout << currNode->data << " ";

            if (currNode->left != nullptr)
            {
                nodeQueue.push(currNode->left);
            }
            if (currNode->right != nullptr)
            {
                nodeQueue.push(currNode->right);
            }
        }
    }

    void levelOrderRecursion(NodePtr cur, vector<vector<int>> &result, int depth)
    {
        if (cur == nullptr)
        {
            return;
        }
        if (result.size() == depth)
            result.push_back(vector<int>());
        result[depth].push_back(cur->data);
        levelOrderRecursion(cur->left, result, depth + 1);
        levelOrderRecursion(cur->right, result, depth + 1);
    }

    int searchHelper(NodePtr node, int value)
    {
        if (node == nullptr)
        {
            return false;
        }
        else if (node->data == value)
        {
            return true;
        }
        else if (value < node->data)
        {
            return searchHelper(node->left, value);
        }
        else
        {
            return searchHelper(node->right, value);
        }
    }

public:
    BinaryTree()
    {
        root->data = NULL;
        root->left = nullptr;
        root->right = nullptr;
    }

    void travesal()
    {
        preOrderRecursion(this->root);
        preOrderIteration(this->root);
        inorderRecursion(this->root);
        inorderIteration(this->root);
        postorderRecursion(this->root);
        postorderIteration(this->root);
        levelOrderIteration(this->root);
        vector<vector<int>> result;
        levelOrderRecursion(this->root, result, 0);
    }

    int search(int value)
    {
        return searchHelper(root, value);
    }

    void insert(NodePtr root, int value)
    {
        NodePtr newNode = new node;
        newNode->data = value;
        newNode->left = nullptr;
        newNode->right = nullptr;
        if (root->left != nullptr)
        {
            root->right = newNode;
        }
        else
        {
            root->left = newNode;
        }
    }

    NodePtr buildTreeFromPreIn(vector<int> &preorder, vector<int> &inorder)
    {
        if (preorder.empty() || inorder.empty())
        {
            return nullptr;
        }

        // 找到根节点在中序遍历中的索引
        int rootVal = preorder[0];
        int rootIdx = 0;
        while (inorder[rootIdx] != rootVal)
        {
            rootIdx++;
        }
        NodePtr root = new node;
        root->data = rootVal;

        // 切割成左子树和右子树
        vector<int> leftInorder(inorder.begin(), inorder.begin() + rootIdx);
        vector<int> rightInorder(inorder.begin() + rootIdx + 1, inorder.end());
        vector<int> leftPreorder(preorder.begin() + 1, preorder.begin() + 1 + rootIdx);
        vector<int> rightPreorder(preorder.begin() + 1 + rootIdx, preorder.end());

        root->left = buildTreeFromPreIn(leftPreorder, leftInorder);
        root->right = buildTreeFromPreIn(rightPreorder, rightInorder);

        return root;
    }

    NodePtr buildTreeFromInPost(vector<int> &inorder, vector<int> &postorder)
    {
        if (inorder.empty() || postorder.empty())
        {
            return nullptr;
        }

        // 找到根节点在中序遍历中的索引
        int rootVal = postorder.back();
        int rootIdx = 0;
        while (inorder[rootIdx] != rootVal)
        {
            rootIdx++;
        }

        NodePtr root = new node;
        root->data = rootVal;

        // 切割成左子树和右子树
        vector<int> leftInorder(inorder.begin(), inorder.begin() + rootIdx);
        vector<int> rightInorder(inorder.begin() + rootIdx + 1, inorder.end());
        vector<int> leftPostorder(postorder.begin(), postorder.begin() + rootIdx);
        vector<int> rightPostorder(postorder.begin() + rootIdx, postorder.end() - 1);

        root->left = buildTreeFromInPost(leftInorder, leftPostorder);
        root->right = buildTreeFromInPost(rightInorder, rightPostorder);

        return root;
    }
};
