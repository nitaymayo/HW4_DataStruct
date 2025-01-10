//
// Created by nitaymayo on 1/10/25.
//

#ifndef DYNAMICHASH_H
#define DYNAMICHASH_H
#include <memory>

#define DEFAULT_CAPACITY 16

template<class T>

struct Node{
    T* data;
    Node<T>* next;

    Node(): data(nullptr), next(nullptr){}
    explicit Node(T* data): data(data), next(nullptr){}
    ~Node(){
        delete data;
    }
};


template<class T>
class DynamicHash {
    private:
        Node<T>** arr;
        int size;
        int amount;
    public:
        DynamicHash(){
            arr = new Node<T>*[DEFAULT_CAPACITY]{};
            size = DEFAULT_CAPACITY;
            amount = 0;
        }
        Node<T>* search(int key){
            int i = hash(key);
            Node<T>* current = arr[i];
            while (current != nullptr){
                if (*(current->data) == key){
                    return current;
                }
                current = current->next;
            }
            return nullptr;
        }

        bool insert(int key,T* obj){            
            if (search(key) != nullptr){
                return false;
            }
            int i = hash(key);
            Node<T>* node = new Node<T>(obj);
            node->next = arr[i];  
            arr[i] = node;
            amount++;
            if(amount >= size){
                multiplyHash();
            }
            return true;
        }

        void multiplyHash(){
            Node<T>** temp_arr = this->arr;
            this->size = this->size*2;
            arr = new Node<T>*[size]{};
            for (int i = 0; i < size/2; i++){
                Node<T>* current = temp_arr[i];
                while (current != nullptr && current->data != nullptr)
                {
                    Node<T>* next = current->next;
                    int i = hash(*(current->data));
                    current->next = arr[i];
                    arr[i] = current;
                    current = next;
                }
            }
            delete temp_arr;
            return;
        }

        int hash(int key) const{
            return key % this->size;
        }

        ~DynamicHash(){
            for (int i = 0; i < size; i++){
                Node<T>* current = arr[i];
                while (current != nullptr)
                {
                    Node<T>* next = current->next;
                    delete current;
                    current = next;
                }
            }
            delete[] arr;
        }
};


#endif //DYNAMICHASH_H
