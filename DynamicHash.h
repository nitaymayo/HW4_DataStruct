
#ifndef DYNAMICHASH_H
#define DYNAMICHASH_H
#include <memory>
#include <stdexcept>

#define DEFAULT_CAPACITY 16
using namespace std;

template<class T>

struct Node{
    shared_ptr<T> data;
    shared_ptr<Node<T>> next;

    Node(): data(nullptr), next(nullptr){}
    explicit Node(shared_ptr<T> data):
         data(data), next(nullptr){}
};


template<class T>
class DynamicHash {
private:
    unique_ptr<shared_ptr<Node<T>>[]> arr;
    int size;
    int amount;
public:
    DynamicHash(){
        arr = make_unique<shared_ptr<Node<T>>[]>(DEFAULT_CAPACITY);
        size = DEFAULT_CAPACITY;
        amount = 0;
    }
    shared_ptr<T> search(int key){
        int i = hash(key);
        shared_ptr<Node<T>> current = arr[i];
        while (current != nullptr){
            if (current->data->key() == key){
                return current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    bool insert(shared_ptr<T> obj){            
        int key = obj->key();
        if (search(key) != nullptr){
            return false;
        }
        int i = hash(key);
        shared_ptr<Node<T>> node = make_shared<Node<T>>(obj);
        node->next = arr[i];  
        arr[i] = node;
        amount++;
        if(amount >= size){
            multiplyHash();
        }
        return true;
    }

    void multiplyHash(){
        unique_ptr<shared_ptr<Node<T>>[]> temp_arr = move(this->arr);
        this->size = this->size*2;
        arr = make_unique<shared_ptr<Node<T>>[]>(this->size);
        for (int i = 0; i < size/2; i++){
            shared_ptr<Node<T>> current = temp_arr[i];
            while (current != nullptr && current->data != nullptr)
            {
                shared_ptr<Node<T>> next = current->next;
                int i = hash(current->data->key());
                current->next = arr[i];
                arr[i] = current;
                current = next;
            }
        }
        return;
    }

    int hash(int key) const{
        return key % this->size;
    }
};


#endif //DYNAMICHASH_H
