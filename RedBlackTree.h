//
// Created by steve on 3/28/2021.
//
#include <memory>
#include <iomanip>
#include <iostream>
#include <string>
#include <windows.h>

#ifndef REDBLACKTREE_REDBLACKTREE_H
#define REDBLACKTREE_REDBLACKTREE_H

void printInColor(std::string &s, int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD oldColor;
    CONSOLE_SCREEN_BUFFER_INFO csbiData;

    GetConsoleScreenBufferInfo(hConsole, &csbiData);
    oldColor = csbiData.wAttributes;

    SetConsoleTextAttribute(hConsole, color);
    std::cout << s;

    SetConsoleTextAttribute(hConsole, oldColor);

}

// Enum for Node type.
enum Color {red, black};

template <typename type>
struct Node
{
    type data;
    std::shared_ptr<Node<type>> left = nullptr;
    std::shared_ptr<Node<type>> right = nullptr;
    std::shared_ptr<Node<type>> parent = nullptr;
    Color color = Color::red;
};

template<typename type>
class RedBlackTree
{
private:
    std::shared_ptr<Node<type>> root;

    std::shared_ptr<Node<type>> createLeaf(type val);
    void privateAdjustTree(std::shared_ptr<Node<type>> &node);
    bool privateInsert(std::shared_ptr<Node<type>> &root, std::shared_ptr<Node<type>> &node);
    bool privateRedBlackInsert(std::shared_ptr<Node<type>> &root, type val);

    // rotate functions
    void privateLeftRotate(std::shared_ptr<Node<type>> &root);
    void privateRightRotate(std::shared_ptr<Node<type>> &root);

    // print related methods
    void privatePrintInorder(std::shared_ptr<Node<type>> root);

    void privatePrintTreeFromRoot(std::shared_ptr<Node<type>> root);

public:
    RedBlackTree();
    RedBlackTree(type rootValue);
    bool insert(type insertValue);

    void printInorder();
    void printTreeFromRoot(type rootVal);
};

template<typename type>
RedBlackTree<type>::RedBlackTree(type rootValue)
{
    this->root = std::make_shared<Node<type>>();
    this->root->data = rootValue;
    this->root->color = Color::black;
}

template<typename type>
RedBlackTree<type>::RedBlackTree()
{

}


template<typename type>
std::shared_ptr<Node<type>> RedBlackTree<type>::createLeaf(type val)
{
    auto leaf = std::make_shared<Node<type>>();
    leaf->data = val;
    return leaf;
}

template<typename type>
void RedBlackTree<type>::privateAdjustTree(std::shared_ptr<Node<type>> &node)
{
    while(node != this->root)
    {
        // Set uncle of node to null, set uncle if exists.
        auto parent =       std::shared_ptr<Node<type>>(nullptr);
        auto grandparent =  std::shared_ptr<Node<type>>(nullptr);
        auto uncle =        std::shared_ptr<Node<type>>(nullptr);

        if(node != nullptr)
            parent = node->parent;
        if(parent != nullptr)
            grandparent = parent->parent;
        if(grandparent != nullptr)
            uncle = grandparent->left == parent ? grandparent->right : grandparent->left;

        // Case that parent and child both are red
        if(node->color == Color::red && node->parent->color == Color::red)
        {
            // Case that child is left pointed to by parent.
            if(parent->left == node)
            {
                // If uncle is a nullptr, we need to rotate.
                if(uncle == nullptr || uncle->color == Color::black) {
                    // right left case
                    if(node->parent->parent->right == parent){
                        privateRightRotate(node->parent);
                        privateLeftRotate(node->parent);
                        node->color = Color::black;
                        node->right->color = Color::red;
                        node->left->color = Color::red;
                    }
                    // left left case
                    else if(node->parent->parent->left == parent)
                    {
                        privateRightRotate(node->parent->parent);
                        node->parent->color = Color::black;
                        node->parent->right->color = Color::red;
                        node->parent->left->color = Color::red;
                    }
                }
                else if(uncle->color == Color::red)
                {
                    uncle->color = Color::black;
                    node->parent->color = Color::black;
                    node->parent->parent->color = Color::red;
                }
            }

            // Case that child is right pointed to by parent.
            else if(node->parent->right == node)
            {
                // Check if uncle exists or uncle color is black.
                if(uncle == nullptr || uncle->color == Color::black)
                {
                    // Left Right Case.
                    if(node->parent->parent->left == parent) {
                        privateLeftRotate(node->parent);
                        privateRightRotate(node->parent);
                        node->color = Color::black;
                        node->right->color = Color::red;
                        node->left->color = Color::red;
                    }
                    // right right case
                    else if(node->parent->parent->right == parent)
                    {
                        privateLeftRotate(node->parent->parent);
                        node->parent->color = Color::black;
                        node->parent->right->color = Color::red;
                        node->parent->left->color = Color::red;
                    }
                }
                else if(uncle->color == Color::red)
                {
                    uncle->color = Color::black;
                    node->parent->color = Color::black;
                    node->parent->parent->color = Color::red;
                }
            }
        }

        // Increment if can
        if(node != this->root)
        {
            node = node->parent;
        }
    }

    /*
    // If root is red, pass black down to children.
    if(this->root != nullptr && this->root->color == Color::red)
    {
        if(this->root->right != nullptr)
            this->root->right->color = Color::red;
        if(this->root->left != nullptr)
            this->root->left->color = Color::red;
    }*/

    // Set root to black!
    this->root->color = Color::black;
    return;
}

template<typename type>
bool RedBlackTree<type>::privateRedBlackInsert(std::shared_ptr<Node<type>> &root, type val)
{
    // Create Node we want to insert.
    auto node = createLeaf(val);

    bool itemInserted = privateInsert(root, node);
    /*
     * TODO: Add color to node. Modify the rest of the tree.
     */
    privateAdjustTree(node);



    return itemInserted;
}

template<typename type>
bool RedBlackTree<type>::privateInsert(std::shared_ptr<Node<type>> &root, std::shared_ptr<Node<type>> &node)
{
    // If root is empty, then insert node into here. Return true since we have inserted a new item.
    if(root == nullptr) {
        root = node;
        return true;
    }
    // Else we transcend down.
    else if(root->data != node->data) {
        // Set Node Parent to this root.
        node->parent = root;
        if(root->data < node->data)
        {
            if(root->right != nullptr) {
                return privateInsert(root->right, node);
            }
            else {
                root->right = node;
                return true;
            }
        }
        else
        {
            if(root->left != nullptr) {
                return privateInsert(root->left, node);
            }
            else
            {
                root->left = node;
                return true;
            }
        }
    }
    // Else, the value exists already, do not insert.
    else
    {
        return false;
    }
}

template<typename type>
bool RedBlackTree<type>::insert(type val)
{
    return privateRedBlackInsert(this->root, val);
}

template<typename type>
void RedBlackTree<type>::privateLeftRotate(std::shared_ptr<Node<type>> &root)
{
    /*
    // Temp of current root. As root parent will take right
    auto z = root;
    auto y = root->right;

    // If z parent exists and the parent points to the right, set the right point to y
    if(z->parent != nullptr && z->parent->right == z)
        z->parent->right = y;
    // Else if parent exists and the parent points to the left, set the left point to y
    else if(z->parent != nullptr && z->parent->left == z)
        z->parent->left = y;
    // else z->parent does not exist, T.F. do not modify.

    // Set y->parent to z->parent
    y->parent = z->parent;

    // Set Z parent to y now. Z points to Y's left.
    z->parent = y;
    z->right = y->left;

    // Y's left now points to z.
    y->left = z;

    // Now check if this points to Z
    if(z == this->root)
        this->root = y;*/
    auto pivot = root->right;

    if(pivot != nullptr)
    {
        root->right = pivot->left;

        pivot->left = root;
        pivot->parent = root->parent;

        if(root->parent != nullptr && root->parent->left == root)
            root->parent->left = pivot;
        else if(root->parent != nullptr && root->parent->right == root)
            root->parent->right = pivot;

        root->parent = pivot;

    }
}

template<typename type>
void RedBlackTree<type>::privateRightRotate(std::shared_ptr<Node<type>> &root)
{
    /*
    // Temp of current root. As root parent will take right/
    auto z = root;
    auto y = root->left;

    // If z parent exists and the parent points to the right, set the right point to y
    if(z->parent != nullptr && z->parent->right == z)
        z->parent->right = y;
    // Else if parent exists and the parent points to the left, set the left point to y
    else if(z->parent != nullptr && z->parent->left == z)
        z->parent->left = y;
    // else z->parent does not exist, T.F. do not modify.

    // Set y->parent to z->parent
    y->parent = z->parent;
    // Set Z parent to y now.
    z->parent = y;

    z->left = y->right;

    // Y's left now points to z.
    y->right = z;

    // Now check if this points to Z
    if(z == this->root)
        this->root = y;*/
    auto pivot = root->left;

    if(pivot != nullptr)
    {
        root->left = pivot->right;

        pivot->right = root;
        pivot->parent = root->parent;

        if(root->parent != nullptr && root->parent->left == root)
            root->parent->left = pivot;
        else if(root->parent != nullptr && root->parent->right == root)
            root->parent->right = pivot;

        root->parent = pivot;

    }
}

template<typename type>
void RedBlackTree<type>::privatePrintInorder(std::shared_ptr<Node<type>> root) {
    if(root != nullptr) {
        if(root->left != nullptr)
        {
            privatePrintInorder(root->left);
        }

        std::cout << root->data << " ";

        if(root->right != nullptr)
        {
            privatePrintInorder(root->right);
        }
    }
    return;
}
template<typename type>
void RedBlackTree<type>::printInorder()
{
    privatePrintInorder(this->root);
    std::cout << std::endl;
}

template <typename type>
void RedBlackTree<type>::privatePrintTreeFromRoot(std::shared_ptr<Node<type>> root)
{
    const int CENTER_PADDING = 40;
    const int NULL_COLOR = 0x00;
    const int BLACK = 0x05;
    const int RED = 0x04;
    /*
     * Root Node
     */
    if(root != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING);
        printInColor( "( " + std::to_string(root->data) + " )", root->color == Color::red ? RED : BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /*
     * 1 Height from Root
     */
    if(root != nullptr && root->left != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor("( " + std::to_string(root->left->data) + " )", root->left->color == Color::red ? RED: BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING);
        printInColor("( " + std::to_string(root->right->data) + " )", root->right->color == Color::red ? RED: BLACK );
        std::cout << std::setw(CENTER_PADDING/2);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
        std::cout << std::setw(CENTER_PADDING/2);
    }
    std::cout << std::endl;

    /*
     * 2 Height from root
     */
    if(root != nullptr && root->left != nullptr && root->left->left != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->left->left->data) + " )", root->left->left->color == Color::red ? RED: BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->left != nullptr && root->left->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor("( " + std::to_string(root->left->right->data) + " )", root->left->right->color == Color::red ? RED: BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->left != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor("( " + std::to_string(root->right->left->data) + " )", root->right->left->color == Color::red ? RED: BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor("( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor("( " + std::to_string(root->right->right->data) + " )", root->right->right->color == Color::red ? RED: BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor("( " + std::string(".") + " )", NULL_COLOR);
    }
    std::cout << std::endl;

    /*
     * 3 Height from root
     */
    if(root != nullptr && root->left != nullptr && root->left->left != nullptr && root->left->left->left != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/8);
        printInColor("( " + std::to_string(root->left->left->left->data) + " )", root->left->left->left->color == Color::red ? RED: BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/8);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->left != nullptr && root->left->left != nullptr && root->left->left->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->left->left->right->data) + " )", root->left->left->right->color == Color::red ? RED: BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->left != nullptr && root->left->right != nullptr && root->left->right->left != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->left->right->left->data) + " )", root->left->right->left->color == Color::red ? RED: BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->left != nullptr && root->left->right != nullptr && root->left->right->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->left->right->right->data) + " )", root->left->right->right->color == Color::red ? RED: BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->left != nullptr && root->right->left->left != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->right->left->left->data) + " )", root->right->left->left->color == Color::red ? RED: BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->left != nullptr && root->right->left->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->right->left->right->data) + " )", root->right->left->right->color == Color::red ? RED: BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->right != nullptr && root->right->right->left != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->right->right->left->data) + " )", root->right->right->left->color == Color::red ? RED: BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->right != nullptr && root->right->right->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->right->right->right->data) + " )", root->right->right->right->color == Color::red ? RED: BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::string(".") + " )", NULL_COLOR);
    }

}

template <typename type>
void RedBlackTree<type>::printTreeFromRoot(type rootVal)
{
    privatePrintTreeFromRoot(this->root);
}


#endif //REDBLACKTREE_REDBLACKTREE_H
