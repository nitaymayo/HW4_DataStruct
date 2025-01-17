
#include <memory>
#include <stdexcept>
#include <stdlib.h>
#include "Herd.h"
#include "UnionFind.h"

#define DEFAULT_CAPACITY 16
using namespace std;

template <class T>
struct RecordsNode{
    shared_ptr<T> data;
    shared_ptr<RecordsNode<T>> next;
    shared_ptr<RecordsNode<T>> previous;
    int m_record = 0;

    RecordsNode(): data(nullptr), next(nullptr){}
    RecordsNode(shared_ptr<T> data):
         data(data), next(nullptr), previous(nullptr){}
};

struct Pairs //for returning to values in amount function
{
    shared_ptr<RevTreeNode<Herd>> first;
    shared_ptr<RevTreeNode<Herd>> second;

};


class DynamicRecordsHash{
private:
    unique_ptr<shared_ptr<RecordsNode<RevTreeNode<Herd>>>[]> arr;
    int size;
    int amount;
public:
    DynamicRecordsHash(){
        arr = make_unique<shared_ptr<RecordsNode<RevTreeNode<Herd>>>[]>(DEFAULT_CAPACITY);
        size = DEFAULT_CAPACITY;
        amount = 0;
    }
    shared_ptr<RevTreeNode<Herd>> search(int record){
        int i = hash(abs(record));
        shared_ptr<RecordsNode<RevTreeNode<Herd>>> current = arr[i];
        while (current != nullptr){
            if (current->m_record == record){
                return current->data;
            }
            current = current->next;
        }
        return nullptr;
    }
    // returns the herd with the positive record, if the amount of groups
    //  is not precisly one negative and one positive returns nullptr
    Pairs Amount(int record){
        record = abs(record);
        int i = hash(record);
        Pairs temp;
        shared_ptr<RecordsNode<RevTreeNode<Herd>>> current = arr[i];
        int positive_counter = 0;
        int negative_counter = 0;
        while (current != nullptr && positive_counter < 2 && negative_counter < 2 ){
            if (current->m_record == record){
                temp.first = current->data;
                positive_counter++;
            }else if(current->m_record == -record){
                temp.second = current->data;
                negative_counter++;
            }   
            current = current->next;
        }
        if (positive_counter != 1 || negative_counter != 1)
        {
            temp.first = nullptr;
            temp.second = nullptr;
        }

        return temp;
    }

    bool insert(int record, shared_ptr<RevTreeNode<Herd>> obj){            
        int i = hash(record);
        shared_ptr<RecordsNode<RevTreeNode<Herd>>> node =
             make_shared<RecordsNode<RevTreeNode<Herd>>>(obj);
        node->m_record = record;
        node->next = arr[i];  
        arr[i] = node;
        amount++;
        if(amount >= size){
            multiplyHash();
        }
        return true;
    }

    void multiplyHash(){
        unique_ptr<shared_ptr<RecordsNode<RevTreeNode<Herd>>>[]> temp_arr = move(this->arr);
        this->size = this->size*2;
        arr = make_unique<shared_ptr<RecordsNode<RevTreeNode<Herd>>>[]>(this->size);
        for (int i = 0; i < size/2; i++){
            shared_ptr<RecordsNode<RevTreeNode<Herd>>> current = temp_arr[i];
            while (current != nullptr && current->data != nullptr)
            {
                shared_ptr<RecordsNode<RevTreeNode<Herd>>> next = current->next;
                int i = hash(current->m_record);
                current->next = arr[i];
                arr[i] = current;
                current = next;
            }
        }
        return;
    }

    // for deleting herds from records in O(1)
    void deleteNode(shared_ptr<RecordsNode<RevTreeNode<Herd>>> node){
        int i = hash(node->m_record);
        if (arr[i] == node)
        {
            arr[i] = node->next;
        }
        if (node->previous != nullptr)
        {
            node->previous->next = node->next;
        }
        if (node->next != nullptr)
        {
            node->next->previous = node->previous;
        }
        this->amount--;
    }

    int hash(int record) const{
        return record % this->size;
    }
};

