
#include "DynamicHash.h"
#include <string>
#include <iostream>

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
    

    return 1;
}