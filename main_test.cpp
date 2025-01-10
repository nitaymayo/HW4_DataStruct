
#include "DynamicHash.h"
#include <string>
#include <iostream>

using namespace std;


int main(){

    DynamicHash<int> table;
    for (int i = 0; i < 100; i++)
    {
        int* x = new int(i);
        table.insert(i, x);
    }
    

    return 1;
}