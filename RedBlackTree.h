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

#ifdef WIN32
/**
 * \brief       Used in print root methods. That way the key shows
 *          what the color being printed is. Mostly for debug purpose.
 *
 * @param s String to be colored in std::cout
 * @param color Int version of the color to use for std::cout. HEX.
 *
 * \note        This is windows only function.
 */
void printInColor(std::string &&s, int color)
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
#elif __unix__
void printInColor(std::string &&s, int color)
{
    /*
     * No color yet :/
     */
    std::cout << s;
}
#endif

/**
 * Enumerators for color of the Node.
 */
enum Color {red, black};

/**
 * \brief       Node class for acting as the nodes within the binary
 *          tree. Has helper methods for returning parent, uncle, sibling
 *          etc.
 *
 * @tparam kType Key value type.
 * @tparam dType Data value type.
 */
template <typename kType, typename dType>
class Node
{
public:
    kType key;
    dType data;
    std::shared_ptr<Node<kType, dType>> left = nullptr;
    std::shared_ptr<Node<kType, dType>> right = nullptr;
    std::shared_ptr<Node<kType, dType>> parent = nullptr;
    Color color = Color::red;

    /**
     * \brief   Returns the parent (if there is) of this node. Else std::shared_ptr(nullptr)
     * @return  std::shared_ptr<Node<kType, dType>> of the parent node.
     */
    std::shared_ptr<Node<kType, dType>> getParent();

    /**
     * \brief   Returns the uncle (if there is) of this node. Else std::shared_ptr(nullptr)
     * @return  std::shared_ptr<Node<kType, dType>> of the uncle node.
     */
    std::shared_ptr<Node<kType, dType>> getUncle();

    /**
     * \brief   Returns the sibling (if there is) of this node. Else std::shared_ptr(nullptr)
     * @return  std::shared_ptr<Node<kType, dType>> of the sibling node.
     */
    std::shared_ptr<Node<kType, dType>> getSibling();
};

template<typename kType, typename dType>
std::shared_ptr<Node<kType, dType>> Node<kType, dType>::getParent() {
    return this->parent;
}

template<typename kType, typename dType>
std::shared_ptr<Node<kType, dType>> Node<kType, dType>::getUncle() {
    if(this->parent != nullptr && this->parent->parent != nullptr) {
        if(this->parent->parent->left == this->parent)
            return this->parent->parent->right;
        else
            return this->parent->parent->left;
    }
    else
    {
        return std::shared_ptr<Node<kType, dType>>(nullptr);
    }
}

template<typename kType, typename dType>
std::shared_ptr<Node<kType, dType>> Node<kType, dType>::getSibling() {
    if(this->parent != nullptr) {
        if(this->parent->left != nullptr && this->parent->left->key == this->key)
            return this->parent->right;
        else if(this->parent->right != nullptr && this->parent->right->key == this->key)
            return this->parent->left;
        else
            return std::shared_ptr<Node<kType, dType>>(nullptr);
    }
    else
    {
        return std::shared_ptr<Node<kType, dType>>(nullptr);
    }
}

template<typename kType, typename dType>
class RedBlackTree
{
private:
    /**
     * top root of the tree
     */
    std::shared_ptr<Node<kType, dType>> root;

    /**
     * counter for totalNodes. Increments/decrements on insert/remove success.
     */
    unsigned long long totalNodes;

    /**
     * \brief       Creates a node object when called with the parameters key
     *          and data. Returns the resulting node.
     *
     * \details     Creates a node object when called with the parameters key
     *          and data which is of types kType & dType respectively with the
     *          declaration of the object type(s).
     *
     * @param key kType key value for traversing the tree.
     * @param data Data that is stored in nodes. Not part of the structure
     *          of the tree
     *
     * @return      std::shared_ptr<Node<kType, dType>> of the node created.
     */
    std::shared_ptr<Node<kType, dType>> createLeaf(kType key, dType data);

    /**
     * \brief       Private function for adjusting a tree when a new node has
     *          just been inserted. Follows Red Black Tree rules for insertion.
     *          Uses the parameter Node to adjust from there on.
     *
     * @param node std::shared_ptr<Node<kType, dType>> of the node we're adjusting
     *          the tree to.
     */
    void privateInsertAdjustTree(std::shared_ptr<Node<kType, dType>> node);

    /**
     * \brief       Recursively finds the node in which the newly created node
     *          can be inserted at. If the Node->left/right is null, insert
     *          the node param in that place.
     *
     * @param root Node we are recursively transcending down.
     * @param node Node/leaf we are inserting into the tree.
     *
     * @return Boolean if the node was successfully inserted.
     */
    bool privateInsert(std::shared_ptr<Node<kType, dType>> root, std::shared_ptr<Node<kType, dType>> node);

    /**
     * \brief       Helper function that calls other methods upon insertion
     *          of the tree.
     *
     * \details     Helper function that calls other methods upon insertion
     *          of the tree. First calls privateInsert to insert into the tree
     *          as a normal BST as a red node. Since we have the node as a ptr,
     *          we call privateInsertAdjustTree to calibrate that(and parents)
     *          node to match the rules of a red black tree.
     *
     * @param root Node that we start from.
     * @param key kType key value of the node being inserted.
     * @param data dType data value of the node being inserted.
     * @return Boolean if the node was successfully inserted.
     */
    bool privateRedBlackInsert(std::shared_ptr<Node<kType, dType>> root, kType key, dType data);

    /**
     * \brief       Recursive standard BST function to the find the node we want to delete.
     *
     * @param root Node from where to start from.
     */
    void privateDeleteBST(std::shared_ptr<Node<kType, dType>> root);

    /**
     * \brief       Function that is called with the node we want to delete.
     *
     * \details     Function called with the param root, this root is the node
     *          that we are deleting. Upon deleting, we proceed with the standard
     *          delete case in a BST. Before the leaf node is replaced/deleted, we
     *          set the appropriate variables like replacedColor and deletedColor
     *          to find the best case.
     *          Initial Steps:
     *          -If deleted Node has 2 Null children, x->nullptr
     *          -If deleted Node has 1 Null children, x->non-nullptr child.
     *          -If deleted Node has 0 Null childrem, x->replacement->right(could be null)
     *
     *          Other Steps:
     *          -If the node we deleted is red and its replacement is red or nullptr,
     *          we are done.
     *          -If the node we deleted is red and its replacement is black, color the
     *          replacement red and proceed to CASES 0-4.
     *          -If the node we deleted is black and its replacement is red, color the
     *          replacement black, we are done.
     *          -If the node we deleted is black and its replacement is null or black,
     *          proceed to cases 0-4.
     *
     *          Cases:
     *          -Case Zero: Node x is red.
     *          -Case One: Node x is black and sibling is red
     *          -Case Two: Node x is black and sibling is black and both children of
     *          sibling is black.
     *          -Case Three: Node x is black and sibling is black with:
     *              -If x is left child, siblings left child is red and right is black.
     *              -If x is right child, siblings right child is red and left is black.
     *          -Case Four: Node x is black and sibling is black with:
     *              -If x is left child, sibling right is red.
     *              -If x is right childm sibling left is red.
     * @param root
     */
    void privateDelete(std::shared_ptr<Node<kType, dType>> root);

    /**
     * \brief       Recursive function to the find the node we want to delete.
     *          Upon finding the node to delete, call privateDelete.
     *
     * @param root Node from where to start from.
     */
    bool privateRemove(std::shared_ptr<Node<kType, dType>> root, kType key);

    /**
     * \brief       Performs a left rotate on the root node. Sets the
     *          pivot node to roots->right child.
     *
     * @param root Node to perform left rotate on.
     */
    void privateLeftRotate(std::shared_ptr<Node<kType, dType>> root);

    /**
     * \brief       Performs a right rotate on the root node. Sets the
     *          pivot node to roots->left child.
     *
     * @param root Node to perform right rotate on.
     */
    void privateRightRotate(std::shared_ptr<Node<kType, dType>> root);

    /**
     * \brief Debugging print inorder method.
     * @param root To start from.
     */
    void privatePrintInorder(std::shared_ptr<Node<kType, dType>> root);
    void privatePrintTreeFromRoot(std::shared_ptr<Node<kType, dType>> root);

    /**
     * \brief       Finds the largest value from the root Node.
     *
     * \details     Recursively transcends the tree to find the largest
     *          value from the root Node.
     *
     * @param root Node starting point.
     *
     * @return std::shared_ptr<Node<KType, dType>> of the largest Node.
     */
    std::shared_ptr<Node<kType, dType>> privateFindLargest(std::shared_ptr<Node<kType, dType>> root);

    /**
     * \brief       Finds the smalled value from the root Node.
     *
     * \details     Recursively transcends the tree to find the smallest
     *          value from the root Node.
     *
     * @param root Node starting point.
     *
     * @return std::shared_ptr<Node<KType, dType>> of the smallest Node.
     */
    std::shared_ptr<Node<kType, dType>> privateFindSmallest(std::shared_ptr<Node<kType, dType>> root);

    /**
     * \brief       Finds the key value from the root Node. If the node doesn't
     *          contain the key value, recusively move left or right depending
     *          on whether it is less than or greater than the search val.
     *
     * @param root Node starting point.
     *
     * @return std::shared_ptr<Node<KType, dType>> of the search Node.
     */
    std::shared_ptr<Node<kType, dType>> privateSearch(std::shared_ptr<Node<kType, dType>> root, const kType& val);

    /**
     * \brief       Checks if Case Zero is applicable. Returns true if so.
     *          Else false.
     *
     * \details     Returns true if x is red.
     *
     * @param x Node x to check if Case Zero.
     *
     * @return Boolean true/false if is case zero.
     */
    bool privateCheckCaseZero(std::shared_ptr<Node<kType, dType>> x);

    /**
     * \brief       Checks if Case One is applicable. Returns true if so.
     *          Else false.
     *
     * \details     Returns true if x is black and sibling w is red.
     *
     * @param x Node x to check if black.
     * @param w Node w to check if red.
     *
     * @return Boolean true/false if is case one.
     */
    bool privateCheckCaseOne(std::shared_ptr<Node<kType, dType>> x, std::shared_ptr<Node<kType, dType>> w);

    /**
     * \brief       Check if Case Two is applicable. Returns true if so.
     *          Else false
     *
     * \details     Returns true if x is black and sibling w is black
     *          and sibling w's childed are both black.
     *
     * @param x Node x to check if black.
     * @param w Node w to check if black and children are black.
     *
     * @return Boolean true/false if is case two.
     */
    bool privateCheckCaseTwo(std::shared_ptr<Node<kType, dType>> x, std::shared_ptr<Node<kType, dType>> w);

    /**
     * \brief       Check if Case Three is applicable. Returns true if so.
     *          Else false.
     *
     * \details     Returns true if x is black and sibling w is black along with:
     *          -If x is left child, and w->left is red and w->right is black.
     *          -If x is right child, and w->right is red and w->left is black.
     *
     * @param x Node x to check if black.
     * @param w Node w to check if black and one child is red.
     * @param parent Parent of x and W. Used to find if the child is left or right.
     *
     * @return Boolean true/false if the case is three.
     */
    bool privateCheckCaseThree(std::shared_ptr<Node<kType, dType>> x, std::shared_ptr<Node<kType, dType>> w, std::shared_ptr<Node<kType, dType>> parent);

    /**
     * \brief       Check if Case Four is applicable. Returns true if so.
     *          Else false.
     *
     * \details     Returns true if x is black and sibling w is black along with:
     *          -If x is left child, w->right is red.
     *          -If x is right child, w->left is red.
     *
     * @param x Node x to check if black.
     * @param w Node w to check if black and one child is red.
     * @param parent Parent of x and W. Used to find if the child is left or right.
     *
     * @return Boolean true/false if the case is four.
     */
    bool privateCheckCaseFour(std::shared_ptr<Node<kType, dType>> x, std::shared_ptr<Node<kType, dType>> w, std::shared_ptr<Node<kType, dType>> parentw);

    /**
     * \brief       Adjusts x Node to fit case zero.
     *
     * \details     Colors x Node black. Does not branch to other cases.
     *
     * @param x Node that is of case zero.
     */
    void privateCaseZero(std::shared_ptr<Node<kType, dType>> x);

    /**
     * \brief       Adjusts x Node, w Node, and parent Node to match that of
     *          case one to a Red Black BST.
     *
     * \details     Node x is black and w is red. Color w black. Color x parent red.
     *          Rotate parent depending on if x is left child, then left rotation, etc.
     *          Adjust w to the new sibling. Branch to next case 2-4 depending on results.
     *
     * @param x Node that is of case one.
     * @param w Sibling of Node x.
     * @param parent Node that is parent of x. X can be null. Parent is needed.
     */
    void privateCaseOne(std::shared_ptr<Node<kType, dType>> x, std::shared_ptr<Node<kType, dType>> w, std::shared_ptr<Node<kType, dType>> parent);

    /**
     * \brief       Adjusts x Node, w Node, and Parent Node to match that of
     *          case two to a Red Black BST.
     *
     * \details     Node x is black, w is black, and both children of w are black.
     *          Color w red. Set x to parent. If x is red, color black. Else proceed
     *          to cases 1-4 based on result.
     *
     * @param x Node that is of case one.
     * @param w Sibling of Node x.
     * @param parent Node that is parent of x. X can be null. Parent is needed.
     */
    void privateCaseTwo(std::shared_ptr<Node<kType, dType>> x, std::shared_ptr<Node<kType, dType>> w, std::shared_ptr<Node<kType, dType>> parent);

    /**
     * \brief       Adjusts x Node, w Node, and parent Node to match that of
     *          case three to a Red Black BST.
     *
     * \details     Node x is black, w is black, and one child of w is red.
     *          If x is left child, then w's left child is red and right is black. Else
     *          conversely with x is right child, then w's right child is red and left
     *          is black. Color the red child black, color w red. Rotate w based on left
     *          or right. Adjust w to the new w since the rotate. Proceed to case 4.
     *
     * @param x Node that is of case one.
     * @param w Sibling of Node x.
     * @param parent Node that is parent of x. X can be null. Parent is needed.
     */
    void privateCaseThree(std::shared_ptr<Node<kType, dType>> x, std::shared_ptr<Node<kType, dType>> w, std::shared_ptr<Node<kType, dType>> parent);

    /**
     * \brief       Adjusts x Node, w Node, and parent Node to match that of
     *          case four to a Red Black BST.
     *
     * \details     Node x is black, w is black and w's right child is red with x
     *          being pointed to by the left. Conversily flipped as well. w gets set
     *          to parents color. Parent gets colored black. Color w's child black.
     *          Rotate the parent left if x is left child. Right if else. Then we are
     *          done, don't branch to another case.
     *
     * @param x Node that is of case one.
     * @param w Sibling of Node x.
     * @param parent Node that is parent of x. X can be null. Parent is needed.
     */
    void privateCaseFour(std::shared_ptr<Node<kType, dType>> x, std::shared_ptr<Node<kType, dType>> w, std::shared_ptr<Node<kType, dType>> parent);
public:
    /**
     * \brief       Constructs an empty tree.
     *
     * \details     Constructs an empty tree.
     */
    RedBlackTree();

    /**
     * \brief       Constructs a tree with the following two params.
     *
     * \details     Constucts a tree with one initial node. rootKey is the starting
     *          key value for the root of the tree, and rootData as the data value
     *          for that key.
     *
     * @param rootKey Key value of the data entry.
     * @param rootData Data value for that key entry.
     */
    RedBlackTree(kType rootKey, dType rootData);

    /**
     * \brief       Returns the total entries of the tree.
     *
     * @return Unsigned long long size of the total nodes in the tree.
     */
    unsigned long long getTotalSize() {return this->totalNodes;}

    /**
     * \details     Attempts an insertion into the tree with the two
     *          given params. Calls the privateInsert function to build
     *          onto the tree. Returns false if inserting the node results
     *          in a collision.
     *
     * @param key Key value of node being inserted.
     * @param data Data value of node being inserted.
     * @return Bool if inserting into the tree was successful.
     */
    bool insert(kType key, dType data);

    /**
     * \details     Attempts to remove an item from the tree.
     * @param key
     * @return
     */
    bool remove(kType key);

    /**
     * \details     Searches the tree for search key parameter and if
     *          if exists in the tree, returns true. Else returns false
     *          since entry is not in tree. dataPtr param is a pointer
     *          to an object that is to be filled with what is from
     *          the tree.
     *
     * @param sKey Search Key to be searched against in the tree.
     * @param dataPtr Pointer to data type that is to be copied into.
     * @return Bool depending if search key is in the tree.
     */
    bool search(const kType& sKey, dType* dataPtr);

    void printInorder();
    void printTreeFromRoot(kType rootVal);
    void printTreeFromRoot();


    /**
     * Debugging puposes only.
     */
    void debugInsertRecursive(std::shared_ptr<Node<kType, dType>> &root, std::shared_ptr<Node<kType, dType>> &node);
    void debugInsert(kType key, dType data, Color color);
};

template<typename kType, typename dType>
RedBlackTree<kType, dType>::RedBlackTree(kType rootKey, dType rootData)
{
    this->root = std::make_shared<Node<kType, dType>>();
    this->root->key = rootKey;
    this->root->data = rootData;
    this->root->color = Color::black;
    this->totalNodes = 1;
}

template<typename kType, typename dType>
RedBlackTree<kType, dType>::RedBlackTree() {}


template<typename kType, typename dType>
std::shared_ptr<Node<kType, dType>> RedBlackTree<kType, dType>::createLeaf(kType key, dType data)
{
    auto leaf = std::make_shared<Node<kType, dType>>();
    leaf->key = key;
    leaf->data = data;
    return leaf;
}

template<typename kType, typename dType>
void RedBlackTree<kType, dType>::privateInsertAdjustTree(std::shared_ptr<Node<kType, dType>> node)
{
    while(node != this->root && node != nullptr)
    {
        // Set uncle of node to null, set uncle if exists.
        auto parent =       std::shared_ptr<Node<kType, dType>>(nullptr);
        auto grandparent =  std::shared_ptr<Node<kType, dType>>(nullptr);
        auto uncle =        std::shared_ptr<Node<kType, dType>>(nullptr);

        if(node != nullptr)
            parent = node->parent;
        if(parent != nullptr)
            grandparent = parent->parent;
        if(grandparent != nullptr)
            uncle = grandparent->left == parent ? grandparent->right : grandparent->left;

        // Case that parent and child both are red
        if(node != nullptr && node->color == Color::red && node->parent != nullptr && node->parent->color == Color::red)
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
}

template<typename kType, typename dType>
bool RedBlackTree<kType, dType>::privateRedBlackInsert(std::shared_ptr<Node<kType, dType>> root, kType key, dType data)
{
    // Create Node we want to insert.
    auto node = createLeaf(key, data);

    bool itemInserted = privateInsert(root, node);

    if(itemInserted)
        this->totalNodes++;

    /*
     * TODO: Add color to node. Modify the rest of the tree.
     */
    privateInsertAdjustTree(node);

    return itemInserted;
}

template<typename kType, typename dType>
bool RedBlackTree<kType, dType>::privateInsert(std::shared_ptr<Node<kType, dType>> root, std::shared_ptr<Node<kType, dType>> node)
{
    // If root is empty, then insert node into here. Return true since we have inserted a new item.
    if(root == nullptr) {
        root = node;
        return true;
    }
    // Else we transcend down.
    else if(root->key != node->key) {
        // Set Node Parent to this root.
        node->parent = root;
        if(root->key < node->key)
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

template<typename kType, typename dType>
bool RedBlackTree<kType, dType>::insert(kType key, dType data)
{
    return privateRedBlackInsert(this->root, key, data);
}


template<typename kType, typename dType>
void RedBlackTree<kType, dType>::debugInsertRecursive(std::shared_ptr<Node<kType, dType>> &root, std::shared_ptr<Node<kType, dType>> &node)
{
    // If root is empty, then insert node into here. Return true since we have inserted a new item.
    if(root == nullptr) {
        root = node;
        return ;
    }
        // Else we transcend down.
    else if(root->key != node->key) {
        // Set Node Parent to this root.
        node->parent = root;
        if(root->key < node->key)
        {
            if(root->right != nullptr) {
                return privateInsert(root->right, node);
            }
            else {
                root->right = node;
                return ;
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
                return ;
            }
        }
    }
        // Else, the value exists already, do not insert.
    else
    {
        return ;
    }
}

template<typename kType, typename dType>
void RedBlackTree<kType, dType>::debugInsert(kType key, dType data, Color color)
{
    auto x = createLeaf(key, data);
    x->color = color;

    privateInsert(this->root, x);
}

template<typename kType, typename dType>
bool RedBlackTree<kType, dType>::privateCheckCaseZero(std::shared_ptr<Node<kType, dType>> x)
{
    if(x != nullptr && x->color == red)
        return true;

    return false;
}

template<typename kType, typename dType>
void RedBlackTree<kType, dType>::privateCaseZero(std::shared_ptr<Node<kType, dType>> x)
{
    x->color = Color::black;
}

template<typename kType, typename dType>
bool RedBlackTree<kType, dType>::privateCheckCaseOne(std::shared_ptr<Node<kType, dType>> x,
                                                     std::shared_ptr<Node<kType, dType>> w)
{
    if((x == nullptr || x->color == Color::black) && w != nullptr && w->color == Color::red)
        return true;

    return false;
}

template<typename kType, typename dType>
bool RedBlackTree<kType, dType>::privateCheckCaseTwo(std::shared_ptr<Node<kType, dType>> x,
                                                     std::shared_ptr<Node<kType, dType>> w)
{
    if((x == nullptr || x->color == Color::black)                      &&
       (w != nullptr && w->color == Color::black)                      &&
       ( (w->color == Color::black                      &&
                         (w->right == nullptr || w->right->color == Color::black)        &&
                         (w->left == nullptr || w->left->color == Color::black) ) ) )
    {
        return true;
    }

    return false;
}

template<typename kType, typename dType>
bool RedBlackTree<kType, dType>::privateCheckCaseThree(std::shared_ptr<Node<kType, dType>> x,
                                                       std::shared_ptr<Node<kType, dType>> w,
                                                       std::shared_ptr<Node<kType, dType>> parent)
{
    if(( x == nullptr || x->color == Color::black)                      &&
        (w != nullptr && w->color == Color::black)                      &&
        ((parent != nullptr && parent->left == x && w != nullptr && w->left != nullptr && w->left->color == Color::red && (w->right == nullptr || w->right->color == Color::black))
        ||
        (parent != nullptr && parent->right == x && w != nullptr && w->right != nullptr && w->right->color == Color::red && (w->left == nullptr || w->left->color == Color::black))
        ))
    {
        return true;
    }

    return false;
}

template<typename kType, typename dType>
bool RedBlackTree<kType, dType>::privateCheckCaseFour(std::shared_ptr<Node<kType, dType>> x,
                                                      std::shared_ptr<Node<kType, dType>> w,
                                                      std::shared_ptr<Node<kType, dType>> parent)
{
    if((x == nullptr || x->color == Color::black) &&
       w != nullptr &&
       ((parent != nullptr && parent->right == x && w->left != nullptr && w->left->color == Color::red) || (parent != nullptr && parent->left == x && w->right != nullptr && w->right->color == Color::red)))
    {
        return true;
    }

    return false;
}

template<typename kType, typename dType>
void RedBlackTree<kType, dType>::privateCaseOne(std::shared_ptr<Node<kType, dType>> x,
                                                std::shared_ptr<Node<kType, dType>> w,
                                                std::shared_ptr<Node<kType, dType>> parent)
{
    // Color w black
    w->color = Color::black;

    // Color parent of x red
    if(parent != nullptr)
        parent->color = Color::red;

    if(parent->left == x) {
        privateLeftRotate(parent);
        w = parent->right;
    }
    else {
        privateRightRotate(parent);
        w = parent->left;
    }

    /*
     * Decide on cases 2,3,4 from here!
     * CASE 2.
     */
    if((x == nullptr || x->color == Color::black)                      &&
            (w == nullptr || w->color == Color::black)                      &&
            (w == nullptr || (w->color == Color::black                      &&
                              (w->right == nullptr || w->right->color == Color::black)        &&
                              (w->left == nullptr || w->left->color == Color::black) ) ) ) {
        privateCaseTwo(x, w, parent);
    }
    /* CASE 3.
     * IF X is black
     * IF W Sibling is black
     * IF X is left child, and w's left child  is red and right black
     * IF X is right child, and w's left child is black and right red
     */
    else if((x == nullptr || x->color == Color::black)                      &&
            (w == nullptr || w->color == Color::black)                      &&
            ((parent != nullptr && parent->left == x && w != nullptr && w->left != nullptr && w->left->color == Color::red && (w->right == nullptr || w->right->color == Color::black))
             ||
             (parent != nullptr && parent->right == x && w != nullptr && w->right != nullptr && w->right->color == Color::red && (w->left == nullptr || w->left->color == Color::black))
            ))
    {
        privateCaseThree(x, w, parent);
    }
    /* CASE 4. X is black/null && sibling w is black
     * &&
     * x is left child, T.F w's right child is red ||
     * x ir right child, T.F. w's left child is red
     */
    else if((x == nullptr || x->color == Color::black) &&
            w != nullptr &&
            ((w->left != nullptr && w->left->color == Color::red) || (w->right != nullptr && w->right->color == Color::red)))
    {
        privateCaseFour(x, w, parent);
    }

}

template<typename kType, typename dType>
void RedBlackTree<kType, dType>::privateCaseTwo(std::shared_ptr<Node<kType, dType>> x,
                                                std::shared_ptr<Node<kType, dType>> w,
                                                std::shared_ptr<Node<kType,dType>> parent)
{
    std::shared_ptr<Node<kType, dType>> tempX = x;
    std::shared_ptr<Node<kType, dType>> tempW = w;
    std::shared_ptr<Node<kType, dType>> tempP = parent;

    if(w != nullptr)
        w->color = Color::red;

    x = parent;

    w = x->getSibling();
    if(x != nullptr)
        parent = x->parent;

    if(x != nullptr && x->color == Color::red)
    {
        x->color = Color::black;
    }
    else
    {
        if(privateCheckCaseOne(x, w))
            privateCaseOne(x, w, parent);
        else if(privateCheckCaseTwo(x, w))
            privateCaseTwo(x, w, parent);
        else if(privateCheckCaseThree(x, w, parent))
            privateCaseThree(x, w, parent);
        else if(privateCheckCaseFour(x, w, parent))
            privateCaseFour(x, w, parent);
        else
            return;
    }
}

template<typename kType, typename dType>
void RedBlackTree<kType, dType>::privateCaseThree(std::shared_ptr<Node<kType, dType>> x,
                                                  std::shared_ptr<Node<kType, dType>> w,
                                                  std::shared_ptr<Node<kType, dType>> parent)
{
    // Color w's child black(the one thats red)
    if(parent != nullptr && parent->left == x)
        w->left->color = Color::black;
    else
        w->right->color = Color::black;

    // Color w red
    w->color = Color::red;

    if(parent != nullptr && parent->left == x)
        privateRightRotate(w);
    else
        privateLeftRotate(w);

    if(parent->left == x)
        w = parent->right;
    else
        w = parent->left;

    if(privateCheckCaseFour(x, w, parent))
        privateCaseFour(x, w, parent);
}

template<typename kType, typename dType>
void RedBlackTree<kType, dType>::privateCaseFour(std::shared_ptr<Node<kType, dType>> x,
                                                 std::shared_ptr<Node<kType, dType>> w,
                                                 std::shared_ptr<Node<kType, dType>> parent)
{
    // Color w the same color as x->parent
    if(w != nullptr && parent != nullptr)
        w->color = parent->color;

    // Color parent black
    if(parent != nullptr)
        parent->color = Color::black;

    // Color w's child black now depending on if x is left or right.
    if(w != nullptr && parent != nullptr) {
        if(parent->left == x) {
            if(w->right != nullptr)
                w->right->color = Color::black;
            privateLeftRotate(parent);
        }
        else
        {
            if(w->left != nullptr)
                w->left->color = Color::black;
            privateRightRotate(parent);
        }
    }
}

template<typename kType, typename dType>
void RedBlackTree<kType, dType>::privateDeleteBST(std::shared_ptr<Node<kType, dType>> root)
{
    if(root->left == nullptr && root->right == nullptr)
    {
        if(root->parent != nullptr) {
            if (root->parent->left == root)
                root->parent->left = nullptr;
            else
                root->parent->right = nullptr;

            root->parent = nullptr;
        }
        else
        {
            this->root = nullptr;
        }
    }
    // case that root has only one child.
    else if((root->left != nullptr && root->right == nullptr) || (root->right != nullptr && root->left == nullptr))
    {
        // case that left child exists
        if(root->left != nullptr)
        {
            root->left->parent = root->parent;

            if(root->parent != nullptr && root->parent->left == root)
                root->parent->left = root->left;
            else
                root->parent->right = root->left;

            root->parent = nullptr;
            root->left = nullptr;
        }
        else
        {
            root->right->parent = root->parent;

            if(root->parent != nullptr && root->parent->left == root)
                root->parent->left = root->right;
            else
                root->parent->right = root->right;

            root->parent = nullptr;
            root->right = nullptr;
        }
    }
}

template<typename kType, typename dType>
void RedBlackTree<kType, dType>::privateDelete(std::shared_ptr<Node<kType, dType>> root)
{
    std::shared_ptr<Node<kType, dType>> x;
    std::shared_ptr<Node<kType, dType>> xParent;
    std::shared_ptr<Node<kType, dType>> w;
    std::shared_ptr<Node<kType, dType>> replacementNode;
    Color deletedColor;
    Color replacementColor;

    // case that root is leaf!
    if(root->left == nullptr && root->right == nullptr)
    {
        xParent = root->parent;

        if(root->parent != nullptr) {
            if (root->parent->left == root)
                root->parent->left = nullptr;
            else
                root->parent->right = nullptr;

            deletedColor = root->color;
            root->parent = nullptr;
        }
        else
        {
            deletedColor = this->root->color;
            this->root = nullptr;
        }

        replacementColor = Color::black;
        x = nullptr;
    }
    // case that root has only one child.
    else if((root->left != nullptr && root->right == nullptr) || (root->right != nullptr && root->left == nullptr))
    {
        // case that left child exists
        if(root->left != nullptr)
        {
            xParent = root->parent;
            x = root->left;
            replacementNode = x;
            deletedColor = root->color;
            replacementColor = x->color;

            if(root->parent != nullptr && root->parent->left == root) {
                root->parent->left = x;
                x->parent = root->parent;
                root->left = nullptr;
                root->parent = nullptr;
            }
            else if(root->parent != nullptr && root->parent->right == root) {
                root->parent->right = x;
                x->parent = root->parent;
                root->left = nullptr;
                root->parent = nullptr;
            }
            // root is top of tree!
            else {
                this->root = x;
                root->right = nullptr;
                root->left = nullptr;
                root->parent = nullptr;
            }

        }
        else
        {
            xParent = root->parent;
            x = root->right;
            replacementNode = x;
            deletedColor = root->color;
            replacementColor = x->color;

            if(root->parent != nullptr && root->parent->left == root) {
                root->parent->left = x;
                x->parent = root->parent;
                root->left = nullptr;
                root->parent = nullptr;
            }
            else if(root->parent != nullptr && root->parent->right == root) {
                root->parent->right = x;
                x->parent = root->parent;
                root->left = nullptr;
                root->parent = nullptr;
            }
            else {
                this->root = x;
                root->right = nullptr;
                root->left = nullptr;
                root->parent = nullptr;
            }
        }

        if(deletedColor == Color::black && replacementColor == Color::red)
        {
            x->color = Color::black;
        }
    }
    // else its two children
    else
    {
        auto successor = this->privateFindSmallest(root->right);
        x = successor->right;
        deletedColor = root->color;
        replacementColor = successor->color;

        root->key = successor->key;
        root->data = successor->data;
        root->color = successor->color;

        replacementNode = root;

        xParent = successor->parent;
        privateDeleteBST(successor);
    }

    // Set W now since we're beyond delete.
    if(xParent != nullptr) {
        if(xParent->left == x)
            w = xParent->right;
        else
            w = xParent->left;
    }

    /*
     * If the node we deleted is red and its replacement is red
     * Done...
     */
    //if(x == nullptr || (replacementColor == Color::red && deletedColor == Color::red))
    if((replacementNode == nullptr || replacementColor == Color::red) && deletedColor == Color::red)
    {
        return; // we are done.
    }
    /*
     * If the node we deleted is black and the replacement is red.
     * Done...
     */
    else if( deletedColor == Color::black && replacementColor == Color::red)
    {
        replacementNode->color = Color::black;
        return;
    }
    /*
     * If the deleted color is red and replacement is black, color the replacement red
     * and proceed to appropiate case.
     */
    else if(deletedColor == Color::red && replacementColor == Color::black)
    {
        replacementNode->color = Color::red;

        // find sibling
        /*
        if(replacementNode->right == x)
            w = replacementNode->left;
        else
            w = replacementNode->right;*/

        // Case 0.
        if(privateCheckCaseZero(x)) {
            privateCaseZero(x);
        }
        // Case 1.
        else if(privateCheckCaseOne(x, w))
        {
            privateCaseOne(x, w, xParent);
        }
        /* CASE 2.
         * IF X is black.
         * IF Sibling W is black
         * IF SIBLING W's Children are both black.
         */
        else if(privateCheckCaseTwo(x, w)) {
            privateCaseTwo(x, w, xParent);
        }
        /* CASE 3.
         * IF X is black
         * IF W Sibling is black
         * IF X is left child, and w's left child  is red and right black
         * IF X is right child, and w's left child is black and right red
         */
        else if(privateCheckCaseThree(x, w, xParent))
        {
            privateCaseThree(x, w, xParent);
        }
        else if(privateCheckCaseFour(x, w, xParent))
        {
            privateCaseFour(x,w,xParent);
        }
    }
    /*
     * If the node we deleted is black and the replacement is black or null
     */
    else if(deletedColor == Color::black && (replacementNode == nullptr || replacementColor == Color::black))
    {
        // Case 0.
        if(privateCheckCaseZero(x)) {
            privateCaseZero(x);
        }
        // Case 1.
        else if(privateCheckCaseOne(x, w))
        {
            privateCaseOne(x, w, xParent);
        }
        /* CASE 2.
         * IF X is black.
         * IF Sibling W is black
         * IF SIBLING W's Children are both black.
         * */
        else if(privateCheckCaseTwo(x, w)) {
            privateCaseTwo(x, w, xParent);
        }
        /* CASE 3.
         * IF X is black
         * IF W Sibling is black
         * IF X is left child, and w's left child  is red and right black
         * IF X is right child, and w's left child is black and right red
         */
        else if(privateCheckCaseThree(x, w, xParent))
        {
            privateCaseThree(x, w, xParent);
        }
        /* CASE 4. X is black/null && sibling w is black
         * &&
         * x is left child, T.F w's right child is red ||
         * x ir right child, T.F. w's left child is red
         */
        else if(privateCheckCaseFour(x, w, xParent))
        {
            privateCaseFour(x, w, xParent);
        }
    }
}

template<typename kType, typename dType>
bool RedBlackTree<kType, dType>::privateRemove(std::shared_ptr<Node<kType, dType>> root, kType key) {
    if(root != nullptr)
    {
        if(root->key == key)
        {
            privateDelete(root);
            this->totalNodes--;
            return true;
        }
        else if(root->key < key)
        {
            return privateRemove(root->right, key);
        }
        else
        {
            return privateRemove(root->left, key);
        }
    }
    // else node is not found in the tree.
    else
    {
        return false;
    }
}
template<typename kType, typename dType>
bool RedBlackTree<kType, dType>::remove(kType key)
{
    bool removed = privateRemove(this->root, key);

    return removed;
}



template<typename kType, typename dType>
void RedBlackTree<kType, dType>::privateLeftRotate(std::shared_ptr<Node<kType, dType>> root)
{
    auto pivot = root->right;

    if(pivot != nullptr)
    {
        root->right = pivot->left;

        if(pivot->left != nullptr)
            pivot->left->parent = root;

        pivot->left = root;
        pivot->parent = root->parent;

        if(root->parent != nullptr && root->parent->left == root)
            root->parent->left = pivot;
        else if(root->parent != nullptr && root->parent->right == root)
            root->parent->right = pivot;

        root->parent = pivot;

        if(this->root == root)
        {
            this->root = pivot;
        }
    }
}

template<typename kType, typename dType>
void RedBlackTree<kType, dType>::privateRightRotate(std::shared_ptr<Node<kType, dType>> root)
{
    auto pivot = root->left;

    if(pivot != nullptr)
    {
        root->left = pivot->right;

        if(pivot->right != nullptr)
            pivot->right->parent = root;

        pivot->right = root;
        pivot->parent = root->parent;

        if(root->parent != nullptr && root->parent->left == root)
            root->parent->left = pivot;
        else if(root->parent != nullptr && root->parent->right == root)
            root->parent->right = pivot;

        root->parent = pivot;

        if(this->root == root)
        {
            this->root = pivot;
        }
    }
}

template<typename kType, typename dType>
std::shared_ptr<Node<kType, dType>> RedBlackTree<kType, dType>::privateSearch(std::shared_ptr<Node<kType, dType>> root, const kType &key) {

    if(root != nullptr)
    {
        if(root->key == key)
        {
            return root;
        }

        if(root->left != nullptr && root->key > key)
        {
            return privateSearch(root->left, key);
        }
        else
        {
            return privateSearch(root->right, key);
        }
    }
    else
    {
        return std::shared_ptr<Node<kType, dType>>(nullptr);
    }
}

template<typename kType, typename dType>
std::shared_ptr<Node<kType, dType>> RedBlackTree<kType, dType>::privateFindLargest(std::shared_ptr<Node<kType, dType>> root)
{
    if(root->right != nullptr)
    {
        return privateFindLargest(root->right);
    }
    else
    {
        return root;
    }
}

template<typename kType, typename dType>
std::shared_ptr<Node<kType, dType>> RedBlackTree<kType, dType>::privateFindSmallest(std::shared_ptr<Node<kType, dType>> root)
{
    if(root->left != nullptr)
    {
        return privateFindSmallest(root->left);
    }
    else
    {
        return root;
    }
}

template<typename kType, typename dType>
bool RedBlackTree<kType, dType>::search(const kType& sKey, dType* dataPtr)
{
    auto results = privateSearch(this->root, sKey);
    if(results == nullptr)
    {
        return false;
    }
    else
    {
        *dataPtr = results->data;
        return true;
    }
}

template<typename kType, typename dType>
void RedBlackTree<kType, dType>::privatePrintInorder(std::shared_ptr<Node<kType, dType>> root) {
    if(root != nullptr) {
        if(root->left != nullptr)
        {
            privatePrintInorder(root->left);
        }

        std::cout << root->key << " ";

        if(root->right != nullptr)
        {
            privatePrintInorder(root->right);
        }
    }
    return;
}
template<typename kType, typename dType>
void RedBlackTree<kType, dType>::printInorder()
{
    privatePrintInorder(this->root);
    std::cout << std::endl;
}

template <typename kType, typename dType>
void RedBlackTree<kType, dType>::privatePrintTreeFromRoot(std::shared_ptr<Node<kType, dType>> root)
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
        printInColor("( " + std::to_string(root->key) + " )", root->color == Color::red ? RED : BLACK);
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
        printInColor("( " + std::to_string(root->left->key) + " )", root->left->color == Color::red ? RED : BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING);
        printInColor("( " + std::to_string(root->right->key) + " )", root->right->color == Color::red ? RED : BLACK );
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
        printInColor("( " + std::to_string(root->left->left->key) + " )", root->left->left->color == Color::red ? RED : BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->left != nullptr && root->left->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor("( " + std::to_string(root->left->right->key) + " )", root->left->right->color == Color::red ? RED : BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->left != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor("( " + std::to_string(root->right->left->key) + " )", root->right->left->color == Color::red ? RED : BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor("( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/2);
        printInColor("( " + std::to_string(root->right->right->key) + " )", root->right->right->color == Color::red ? RED : BLACK);
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
        printInColor("( " + std::to_string(root->left->left->left->key) + " )", root->left->left->left->color == Color::red ? RED : BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/8);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->left != nullptr && root->left->left != nullptr && root->left->left->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->left->left->right->key) + " )", root->left->left->right->color == Color::red ? RED : BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->left != nullptr && root->left->right != nullptr && root->left->right->left != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->left->right->left->key) + " )", root->left->right->left->color == Color::red ? RED : BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->left != nullptr && root->left->right != nullptr && root->left->right->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->left->right->right->key) + " )", root->left->right->right->color == Color::red ? RED : BLACK );
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor( "( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->left != nullptr && root->right->left->left != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->right->left->left->key) + " )", root->right->left->left->color == Color::red ? RED : BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->left != nullptr && root->right->left->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->right->left->right->key) + " )", root->right->left->right->color == Color::red ? RED : BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->right != nullptr && root->right->right->left != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->right->right->left->key) + " )", root->right->right->left->color == Color::red ? RED : BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::string(".") + " )", NULL_COLOR);
    }

    if(root != nullptr && root->right != nullptr && root->right->right != nullptr && root->right->right->right != nullptr)
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::to_string(root->right->right->right->key) + " )", root->right->right->right->color == Color::red ? RED : BLACK);
    }
    else
    {
        std::cout << std::setw(CENTER_PADDING/4);
        printInColor("( " + std::string(".") + " )", NULL_COLOR);
    }

}

template <typename kType, typename dType>
void RedBlackTree<kType, dType>::printTreeFromRoot()
{
    privatePrintTreeFromRoot(this->root);
}

template <typename kType, typename dType>
void RedBlackTree<kType, dType>::printTreeFromRoot(kType rootVal)
{
    auto r = privateSearch(this->root, rootVal);
    privatePrintTreeFromRoot(r);
}

#endif //REDBLACKTREE_REDBLACKTREE_H