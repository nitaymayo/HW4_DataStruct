
#include "DynamicHash.h"
#include <string>
#include <iostream>
#include "UnionFind.h"
#include "Rider.h"

using namespace std;


int main(){

    DynamicHash<int> table;
    for (int i = 0; i < 100; i++)
    {
        int* x = new int(i);
        table.insert(i, x);
    }

    UnionFind<Rider> uf;
    auto rider1 = make_shared<Rider>(1, nullptr);
    uf.makeSet(rider1);

    return 1;
}