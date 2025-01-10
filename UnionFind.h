//
// Created by nitaymayo on 1/10/25.
//

#ifndef UNIONFIND_H
#define UNIONFIND_H
#include "DynamicArray.h"
#include <memory>
using namespace std;
template<class T>
class Group{
  private:
    int m_size;
    int ID;
    shared_ptr<T> head;

  public:
    Group<T>(int ID, shared_ptr<T> member): m_size(1), ID(ID), head(member){};

    ~Group() = default;

    int getSize(){
      return m_size;
    }
    int setSize(int size){
      this->m_size = size;
    }
    int getID(){
      return ID;
    }
    void setID(int ID){
      this->ID = ID;
    }
    shared_ptr<T> getHead(){
      return head;
    }
    void setHead(shared_ptr<T> member){
      head = member;
    }
};

template<class T>
class UnionFind {
  private:
    DynamicArray<T> groups;
    DynamicArray<T> members;

  public:



}

#endif //UNIONFIND_H
