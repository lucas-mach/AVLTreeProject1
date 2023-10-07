#include <iostream>
#include <string>
#include <vector>
#pragma once
using namespace std;

class AVLTree {
public:
    struct TreeNode //Each Node of the AVL Tree
    {
        string name;
        int id;
        int height;
        TreeNode *left = nullptr;
        TreeNode *right = nullptr;
        int balanceFactor = 0;
        TreeNode(string name, int id, int height) :name(name),id(id),height(height){}   //Default Constructor
    };

private: //Helper Functions
    TreeNode *root = nullptr;

    //Add Nodes to vector to be deleted
    void DeleteNodes(TreeNode *currNode, vector<TreeNode *> &v) {
        if (currNode == nullptr) { return; }    //If node does not exist
        v.push_back(currNode);
        DeleteNodes(currNode->left, v);
        DeleteNodes(currNode->right, v);
    }

    //Insert recursively
    TreeNode *Insert(TreeNode *currNode, const string &name, const int &id, int height)
    {
        //Either creates new node, or keeps recursively going down BST
        if (currNode == nullptr) //If node does not exist, create new Node
        {
            cout << "successful" << endl;
            return new TreeNode(name, id, 0);
        } else if (id < currNode->id) {
            currNode->left = Insert(currNode->left, name, id, height - 1);
        }   //If new id is less than current node id, call function with root->left as new parameter
        else if (id > currNode->id) { currNode->right = Insert(currNode->right, name, id, height - 1); }   // If new id is greater
        else if (id == currNode->id) {cout << "unsuccessful" << endl; return currNode;}

        //Get height by finding the max height of left and right and adding 1
        currNode->height = max(GetHeight(currNode->left), GetHeight(currNode->right)) + 1;

        //Get balanceFactor by subtracting left and right subtree heights
        currNode->balanceFactor = GetHeight(currNode->left) - GetHeight(currNode->right);

        //Check balanceFactor and balance accordingly
        if (abs(currNode->balanceFactor) > 1) {
            //Left Rotation, if Parent and Child balance factors negative
            if (currNode->balanceFactor < -1 and currNode->right->balanceFactor < 0) {
                currNode = LeftRotation(currNode);
            }
                //Right Rotation, if Parent and Child balance factors positive
            else if (currNode->balanceFactor > 1 and currNode->left->balanceFactor > 0) {
                currNode = RightRotation(currNode);
            }
                //LeftRight Rotation if, parent, child = pos, neg
            else if (currNode->balanceFactor > 1 and currNode->left->balanceFactor < 0) {
                currNode = LeftRightRotation(currNode);
            }
                //RightLeft, neg, pos
            else if (currNode->balanceFactor < -1 and
                     currNode->right->balanceFactor > 0) { currNode = RightLeftRotation(currNode); }
        }
        return currNode;
    }

    //Rotations --------------
    //left-left case
    TreeNode *LeftRotation(TreeNode *root) {
        TreeNode *grandchild = root->right->left;
        TreeNode *newRoot = root->right;
        newRoot->left = root;
        root->right = grandchild;

        //Update heights and balanceFactors
        root->height = max(GetHeight(root->right), GetHeight(root->left)) + 1;
        root->balanceFactor = GetHeight(root->left) - GetHeight(root->right);
        newRoot->height = max(GetHeight(newRoot->right), GetHeight(newRoot->left)) + 1;
        newRoot->balanceFactor = GetHeight(newRoot->left) - GetHeight(newRoot->right);

        return newRoot;
    }

    //right-right case
    TreeNode *RightRotation(TreeNode *root) {
        TreeNode *grandchild = root->left->right;
        TreeNode *newRoot = root->left;
        newRoot->right = root;
        root->left = grandchild;

        //Get New heights
        root->height = max(GetHeight(root->right), GetHeight(root->left)) + 1;
        root->balanceFactor = GetHeight(root->left) - GetHeight(root->right);
        newRoot->height = max(GetHeight(newRoot->right), GetHeight(newRoot->left)) + 1;
        newRoot->balanceFactor = GetHeight(newRoot->left) - GetHeight(newRoot->right);

        return newRoot;
    }

    //left-right case
    TreeNode *LeftRightRotation(TreeNode *root) {
        root->left = LeftRotation(root->left);
        return RightRotation(root);
    }

    //right-left case
    TreeNode *RightLeftRotation(TreeNode *root) {
        root->right = RightRotation(root->right);
        return LeftRotation(root);
    }

    //Get height of root
    int GetHeight(TreeNode *root) {
        if (root == nullptr) { return -1; }   //If no Node return -1
        return root->height;
    }

    //Finds ID and returns Node
    TreeNode *FindID(TreeNode *root, int id) {
        if (root == nullptr) { return nullptr; }
        if (root->id == id) { return root; }
        else if (id < root->id) { return FindID(root->left, id); }
        else if (id > root->id) { return FindID(root->right, id); }
    }

    //Find Parent of corresponding id
    TreeNode *FindIDParent(TreeNode *root, int id) {
        if (root == nullptr) { return nullptr; }
        if (root->left->id == id or root->right->id == id) { return root; }
        if (id < root->id) { return FindIDParent(root->left, id); }
        else if (id > root->id) { return FindIDParent(root->right, id); }
    }

    //Get In Order Successor at specified subtree
    TreeNode* GetInOrderSuccessor(TreeNode* root)
    {
        if (root->left != nullptr) {return GetInOrderSuccessor(root->left);}    //If left subtree exists move there
        else
        {
            if (root->right == nullptr) {return root;}  //If left does not exist and right, return this node
            else {return GetInOrderSuccessor(root->right);} //If right exists go down right tree
        }
    }

    //SearchIDHelper
    TreeNode* SearchIDHelper(TreeNode* root, int id)
    {
        if (root == nullptr) {return nullptr;}
        if (root->id == id) {return root;}
        if (id < root->id) {return SearchIDHelper(root->left, id);}
        else if (id > root->id) {return SearchIDHelper(root->right, id);}
    }

    //Helper for Search Name
    void SearchNamePreOrderHelper(TreeNode* root, string name, vector<TreeNode*> &found)
    {
        if (root == nullptr) {return;}
        if (root->name == name) {found.push_back(root);}
        SearchNamePreOrderHelper(root->left, name, found);
        SearchNamePreOrderHelper(root->right, name, found);
    }

    //Helper for Printing in InOrderTraversal
    void PrintNamesHelperInOrder(TreeNode* root, string &s)
    {
        if (root == nullptr) {return;}
        PrintNamesHelperInOrder(root->left, s);
        s += root->name + ", ";
        PrintNamesHelperInOrder(root->right, s);
    }

    //Helper for Printing PostOrder
    void PrintNamesHelperPostOrder(TreeNode* root, string &s)
    {
        if (root == nullptr) {return;}
        PrintNamesHelperPostOrder(root->left, s);
        PrintNamesHelperPostOrder(root->right, s);
        s += root->name + ", ";
    }

    //Helper for Removing In order Nth node, by pushing ids "InOrder" to a vector
    void RemoveInOrderHelper(TreeNode* root, int N, vector<int> &ids)
    {
        if (root == nullptr) {return;}
        RemoveInOrderHelper(root->left, N, ids);
        ids.push_back(root->id);
        RemoveInOrderHelper(root->right, N, ids);

    }

public: //Functions that can be called in main

    ~AVLTree()  //Destructor
    {
        vector<TreeNode *> v;
        DeleteNodes(root, v);
        for (int i = 0; i < v.size(); i++)  //Delete all nodes in vector with all nodes
        {
            delete v[i];
        }
    }

    //Insert into AVL
    TreeNode *insert(const string &name, const int &id) {
        if (root == nullptr)    //If no nodes in Tree
        {
            cout << "successful" << endl;
            return root = new TreeNode(name, id, 0);
        }
        return root = Insert(root, name, id, root->height);
    }

    //Search for ID and delete node and update tree accordingly
    void remove(int id) {
        //Call helper functions to find the node to be removed
        TreeNode *nodeRemoving = FindID(root, id);
        if (nodeRemoving == nullptr) {cout << "unsuccessful" << endl;}  //If ID is not in tree
        else {cout << "successful" << endl;}

        //If Node has no children, change parents pointer to nullptr and delete node
        if (nodeRemoving->left == nullptr and
            nodeRemoving->right == nullptr)
        {
            TreeNode *parent = FindIDParent(root, id);  // Find parent of the Node Removed
            if (parent->right->id ==
                id) { parent->right = nullptr; } //If parents right is the node Removed, change its pointer to nullptr
            else { parent->left = nullptr; }  //Other case
            delete nodeRemoving;    //Delete the Node

        }
            //If Node has 1 child on the left
        else if (nodeRemoving->left != nullptr and nodeRemoving->right == nullptr)
        {
            TreeNode *parent = FindIDParent(root, id);  // Find parent of the Node Removed
            if (parent->right->id == id) { parent->right = nodeRemoving->left; }
            else {parent->left = nodeRemoving->left;}
            delete nodeRemoving;
        }
            //If Node has 1 child on the right
        else if (nodeRemoving->left == nullptr and nodeRemoving->right != nullptr)
        {
            TreeNode *parent = FindIDParent(root, id);  // Find parent of the Node Removed
            if (parent->right->id == id) { parent->right = nodeRemoving->right; }
            else {parent->left = nodeRemoving->right;}
            delete nodeRemoving;
        }

            //If Node has 2 children
        else if (nodeRemoving->left != nullptr and nodeRemoving->right != nullptr)
        {
            TreeNode* successor = GetInOrderSuccessor(nodeRemoving->right); //Get InOrder Successor to replace the removed node
            TreeNode *parentOfSuccessor = FindIDParent(root, successor->id);    //Get parent of Successor to change its pointers to nulllptr
            nodeRemoving->id = successor->id;   //Update information
            nodeRemoving->name = successor->name;
            if (parentOfSuccessor->left->id == nodeRemoving->id) {parentOfSuccessor->left = nullptr;}
            else {parentOfSuccessor->right = nullptr;}
            delete successor;   //Delete the successor after replacing node removed with it
        }
    }

    //Search for ID
    void search(int id)
    {
        TreeNode* idNode = SearchIDHelper(root,id);
        if (idNode != nullptr) {cout << idNode->id << endl;}
        else { cout << "unsuccessful" << endl;}
    }

    //Search for Name, using Preorder traversal
    void search(string name)
    {
        vector<TreeNode*> nodes;    //vector to store nodes with found name
        SearchNamePreOrderHelper(root, name, nodes);
        if (nodes.size() == 0) {cout << "unsuccessful" << endl;}
        else{ for (int i = 0; i < nodes.size(); i++) {cout << nodes[i]->id << endl;} }  //print out all names in vector
    }

    //print names of nodes using Inorder traversal
    void printInorder()
    {
        string s = "";
        PrintNamesHelperInOrder(root, s);
        s.pop_back();
        s.pop_back();
        cout << s << endl;
    }

    //prints names using PostOrder traversal
    void printPostorder()
    {
        string s = "";
        PrintNamesHelperPostOrder(root, s);
        s.pop_back();
        s.pop_back();
        cout << s << endl;
    }

    //Print Level count of tree
    void printLevelCount()
    {
        if (root == nullptr) {cout << "0" << endl;}
        else { cout << root->height + 1 << endl;}
    }

    //Remove the Nth GatorID from the inorder traversal of the tree
    void removeInorder(int N)
    {
        vector<int> ids;
        RemoveInOrderHelper(root, N, ids);
        if (ids.size() == 0) {cout << "unsuccessful" << endl;}
        else {cout << "successful" << endl;
            remove(ids[N]);} //Remove Nth node vector of ids that is already in correct order

    }
};

