#include <iostream>
#include <deque>
#include <time.h>
#include "RedBlackTree.h"

int main() {
    RedBlackTree<int, int> T(13, 1);
    T.insert(8, 1);
    T.insert(17, 1);
    T.insert(1, 1);
    T.insert(11,1);
    T.insert(15,1);
    T.insert(25,1);
    T.insert(22, 1);
    T.insert(27, 1);
    T.insert(6, 1);

    T.printTreeFromRoot();
    std::cout << std::endl;


    T.remove(25);

    T.printTreeFromRoot();
    std::cout << std::endl;



    RedBlackTree<int, int> T2(7, 1);
    T2.insert(3, 1);
    T2.insert(18, 1);
    T2.insert(10, 1);
    T2.insert(22, 1);
    T2.insert(8, 1);
    T2.insert(11, 1);
    T2.insert(26, 1);
    T2.printTreeFromRoot();
    std::cout << std::endl;

    T2.remove(18);
    T2.printTreeFromRoot();
    std::cout << std::endl;



    RedBlackTree<int, int> T4(5, 1);
    T4.debugInsert(2, 1, red);
    T4.debugInsert(8, 1, black);
    T4.debugInsert(1, 1, black);
    T4.debugInsert(4, 1, black);
    T4.debugInsert(7, 1, red);
    T4.debugInsert(9, 1, red);
    T4.printTreeFromRoot();
    std::cout << std::endl;

    T4.remove(2);

    T4.printTreeFromRoot();
    std::cout << std::endl;


    RedBlackTree<int, int> T5(13, 1);
    T5.insert(8, 1);
    T5.insert(17, 1);
    T5.insert(1, 1);
    T5.insert(11,1);
    T5.insert(15,1);
    T5.insert(25,1);
    T5.insert(22, 1);
    T5.insert(27, 1);
    T5.insert(6, 1);

    T5.printTreeFromRoot();
    std::cout << std::endl;

    T5.remove(13);

    T5.printTreeFromRoot();
    std::cout << std::endl;

    RedBlackTree<int, int> T6(13, 1);
    T6.insert(8, 1);
    T6.insert(17, 1);
    T6.insert(1, 1);
    T6.insert(11,1);
    T6.insert(15,1);
    T6.insert(25,1);
    T6.insert(22, 1);
    T6.insert(27, 1);
    T6.insert(6, 1);

    T6.printTreeFromRoot();
    std::cout << std::endl;

    T6.remove(8);

    T6.printTreeFromRoot();
    std::cout << std::endl;


    RedBlackTree<int, int> T9(7, 1);
    T9.debugInsert(3, 1, black);
    T9.debugInsert(18, 1, red);
    T9.debugInsert(10, 1, black);
    T9.debugInsert(22, 1, black);
    T9.debugInsert(8, 1, red);
    T9.debugInsert(11, 1, red);
    T9.debugInsert(26, 1, red);

    T9.printTreeFromRoot();
    std::cout << std::endl;

    T9.remove(3);

    T9.printTreeFromRoot();
    std::cout << std::endl;

    RedBlackTree<int, int> T10(13, 1);
    T10.insert(8, 1);
    T10.insert(17, 1);
    T10.insert(1, 1);
    T10.insert(11,1);
    T10.insert(15,1);
    T10.insert(25,1);
    T10.insert(22, 1);
    T10.insert(27, 1);
    T10.insert(6, 1);

    T10.printTreeFromRoot();
    std::cout << std::endl;

    T10.remove(11);

    T10.printTreeFromRoot();
    std::cout << std::endl;

    const unsigned N = 20;
    srand(time(nullptr));

    std::deque<int> keys;

    RedBlackTree<int, int> FL1(500, 1);
    keys.push_back(500);
    for(unsigned i = 0; i < N; i++)
    {
        auto r = rand() % N + 1;
        if(FL1.insert(r, r)){
            if(r % 2 == 0)
                keys.push_back(r);
            else
                keys.push_front(r);
            std::cout << "Inserting: " << r << std::endl;
        }



        //std::cout << "Inserted: " << r << std::endl;
    }

    std::cout << "Total Nodes Inserted: " << FL1.getTotalSize() << std::endl;
    std::cout << "Keys Size: " << keys.size() << std::endl;

    for(unsigned i = 0; i < keys.size() -4 ; i++)
    {
        FL1.remove(keys[i]);
        //std::cout << "Deleted: " << keys[i] << std::endl;
        std::cout << "Deleting: " << keys[i] << std::endl;
    }

    std::cout << "Total Nodes Left: " << FL1.getTotalSize() << std::endl;

    FL1.printTreeFromRoot();
    std::cout << std::endl;

    RedBlackTree<int, int> DB(500,1);
    DB.insert(2,1);
    DB.insert(13,1);
    DB.insert(1,1);
    DB.insert(14,1);
    DB.insert(6,1);
    DB.insert(10,1);
    DB.insert(12,1);
    DB.insert(7,1);
    DB.insert(11,1);
    DB.insert(15,1);
    DB.insert(19,1);
    DB.insert(17,1);

    DB.printTreeFromRoot();
    std::cout << std::endl;

    DB.remove(17);

    DB.printTreeFromRoot();
    std::cout << std::endl;

    DB.remove(19);

    DB.printTreeFromRoot();
    std::cout << std::endl;

    DB.remove(15);
    DB.remove(11);
    DB.remove(7);
    DB.remove(1);
    DB.remove(13);
    DB.remove(500);
    DB.remove(2);

    DB.printTreeFromRoot();
    std::cout << std::endl;

    return 0;
}
