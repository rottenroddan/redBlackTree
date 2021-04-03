#include <iostream>
#include "RedBlackTree.h"

int main() {
    RedBlackTree<int> T(1000);
    T.insert(600);
    T.insert(1200);
    T.insert(1100);
    std::cout << std::endl; T.printInorder();
    T.printTreeFromRoot(10);
    T.insert(1050);
    T.insert(1030);
    T.insert(1035);
    std::cout << std::endl; T.printInorder();
    T.printTreeFromRoot(10);
    T.insert(1040);
    std::cout << std::endl; T.printInorder();
    T.printTreeFromRoot(10);
//    T.insert(1038);

    std::cout << std::endl; T.printInorder();
    T.printTreeFromRoot(10);


    RedBlackTree<int> X;
    while (!std::cin.fail())
    {
        int input;
        std::cout << std::endl << "Enter a number: ";
        std::cin >> input;
        std::cout << std::endl;

        X.insert(input);
        X.printInorder();
        X.printTreeFromRoot(10);
    }
    return 0;
}
