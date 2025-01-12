//
// Created by nitaymayo on 1/12/25.
//

#ifndef RIDER_H
#define RIDER_H
#include "Herd.h"
#include <memory>


class Rider {
private:
    int m_id;
    int record;
    std::shared_ptr<Herd> m_herd;

public:
    Rider(int id, std::shared_ptr<Herd> herd): m_id(id), record(0), m_herd(nullptr) {};

    int getID() const {
        return m_id;
    }
    int getRecord() const {
        return record;
    }
};



#endif //RIDER_H
