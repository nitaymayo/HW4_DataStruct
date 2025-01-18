//
// Created by nitaymayo on 1/12/25.
//

#ifndef RIDER_H
#define RIDER_H
#include "Herd.h"
#include <memory>

#include "UnionFind.h"


class Rider {
private:
    int m_id;
    int record;
    std::shared_ptr<RevTreeNode<Herd>> m_herd;

public:
    Rider(int id, std::shared_ptr<RevTreeNode<Herd>> herd): m_id(id), record(0), m_herd(herd) {};

    int getID() const {
        return m_id;
    }
    int HerdID() const {
        return m_herd->key();
    }
    std::shared_ptr<RevTreeNode<Herd>> getHerd() const {
        return m_herd;
    }
    int key() const {
        return m_id;
    }
    void won() {
        record++;
    }
    void lost() {
        record--;
    }
    int getRecord() const {
        return record;
    }
};



#endif //RIDER_H
