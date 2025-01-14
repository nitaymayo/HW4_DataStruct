
#include "DynamicHash.h"
#include <string>
#include <iostream>
#include "UnionFind.h"
#include "Rider.h"

using namespace std;

struct obj{
    int m_key;
    int value;

    obj(int key, int value): m_key(key), value(value){}
    int key(){ return m_key;}
};


int main(){

    DynamicHash<obj> table;
    for (int i = 0; i < 100; i++)
    {
        obj x(i, i);
        table.insert(x);
    }

    UnionFind<Rider> uf;
    auto rider1 = make_shared<Rider>(1, nullptr);
    uf.makeSet(rider1);

    return 1;
}