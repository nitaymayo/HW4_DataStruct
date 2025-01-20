
#pragma once
#include <memory>
#include <stdexcept>
// #include <stdlib.h>
#include "Herd.h"
#include "UnionFind.h"

#define DEFAULT_CAPACITY 16
using namespace std;
template <class T>
struct RecordsNode{
    private:
    weak_ptr<RecordsNode<T>> previous;
    shared_ptr<T> data;
    shared_ptr<RecordsNode<T>> next;
    int m_record = 0;


public:
    RecordsNode(): data(nullptr), next(nullptr){}
    explicit RecordsNode(shared_ptr<T> data):
         previous(), data(data), next(nullptr){}
    ~RecordsNode(){
        RecordsNode<T>* current = this;
        while (current != nullptr){
            shared_ptr<RecordsNode<T>> next = current->getNext();
            current->data.reset();
            current->next.reset();
            current = next.get();
        }
    }

    shared_ptr<T> getData() {
        return data;
    }
    void setData(shared_ptr<T> data) {
        this->data = data;
    }

    shared_ptr<RecordsNode<T>> getNext() {
        return next;
    }
    void setNext(shared_ptr<RecordsNode<T>> next) {
        this->next = next;
    }

    int getRecord() const {return m_record;}
    void setRecord(int record) {
        m_record = record;
    }
    shared_ptr<RecordsNode<T>> getPrevious(){return previous.lock();}
    void resetPrevious() {
        previous.reset();
    }
    void setPrevious(weak_ptr<RecordsNode<T>> previous) {
        this->previous = previous;
    }
};
template <class T>
struct Set;
struct Pairs //for returning to values in amount function
{
    shared_ptr<Set<Herd>> first;
    shared_ptr<Set<Herd>> second;
};


class DynamicRecordsHash{
private:
    shared_ptr<RecordsNode<Set<Herd>>>* arr;
    int size;
    int amount;
public:
    DynamicRecordsHash(){
        arr = new shared_ptr<RecordsNode<Set<Herd>>>[DEFAULT_CAPACITY];
        size = DEFAULT_CAPACITY;
        amount = 0;
    }
    ~DynamicRecordsHash(){
        delete[] arr;
    }
    shared_ptr<Set<Herd>> search(int record){
        int i = hash(abs(record));
        shared_ptr<RecordsNode<Set<Herd>>> current = arr[i];
        while (current != nullptr){
            if (current->getRecord() == record){
                return current->getData();
            }
            current = current->getNext();
        }
        return nullptr;
    }
    // returns the herd with the positive record, if the amount of groups
    //  is not precisly one negative and one positive returns nullptr
    Pairs Amount(int record){
        if (record >= size) {
            Pairs temp;
            return temp;
        }
        int i = hash(record);
        Pairs temp;
        shared_ptr<RecordsNode<Set<Herd>>> current = arr[i];
        int positive_counter = 0;
        int negative_counter = 0;
        while (current != nullptr && positive_counter < 2 && negative_counter < 2 ){
            if (current->getRecord() == record){
                temp.first = current->getData();
                positive_counter++;
            }else if(current->getRecord() == -record){
                temp.second = current->getData();
                negative_counter++;
            }   
            current = current->getNext();
        }
        if (positive_counter != 1 || negative_counter != 1)
        {
            temp.first.reset();
            temp.second.reset();
        }

        return temp;
    }

    shared_ptr<RecordsNode<Set<Herd>>> insert(const int record, shared_ptr<Set<Herd>> obj){
        int i = hash(abs(record));
        shared_ptr<RecordsNode<Set<Herd>>> node =
             make_shared<RecordsNode<Set<Herd>>>(obj);
        node->setRecord(record);
        node->setNext(arr[i]);
        node->resetPrevious();
        if (arr[i] != nullptr )
        {
            arr[i]->setPrevious(node);
        }
        arr[i] = node;
        amount++;
        if(amount >= size){
            multiplyHash();
        }
        return node;
    }

    void multiplyHash(){
        auto temp_arr = this->arr;
        this->size = this->size*2;

        arr = new shared_ptr<RecordsNode<Set<Herd>>>[this->size];
        for (int i = 0; i < size/2; i++){
            shared_ptr<RecordsNode<Set<Herd>>> current = temp_arr[i];
            while (current != nullptr && current->getData()){
                shared_ptr<RecordsNode<Set<Herd>>> next = current->getNext();
                int i = hash(current->getRecord());
                current->setNext(arr[i]);
                if (arr[i]) {
                    arr[i]->setPrevious(current);
                }
                arr[i] = current;
                current->resetPrevious();
                current = next;
            }
            temp_arr[i].reset();
        }
        delete[] temp_arr;
    }
    shared_ptr<RecordsNode<Set<Herd>>> increaseRecord(int num,
         shared_ptr<RecordsNode<Set<Herd>>>  m_record){
            return this->insert(num + m_record->getRecord(), m_record->getData());
    }

    // for deleting herds from records in O(1)
    void deleteNode(shared_ptr<RecordsNode<Set<Herd>>> node){
        int i = hash(abs(node->getRecord()));
        if (arr[i] == node)
        {
            arr[i] = node->getNext();
        }
        if (node->getPrevious())
        {
            node->getPrevious()->setNext(node->getNext());
        }
        if (node->getNext())
        {
            node->getNext()->setPrevious(node->getPrevious());
        }
        this->amount--;
        node->getNext().reset();
        node->resetPrevious();
        node.reset();
    }

    int hash(int record) const{
        return abs(record % this->size);
    }
};

