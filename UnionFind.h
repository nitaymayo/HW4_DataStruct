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
    int m_ID;
    shared_ptr<T> head;

  public:
    Group<T>(int ID, shared_ptr<T> member): m_size(1), m_ID(ID), head(member){};

    ~Group() = default;

    int getSize(){
      return m_size;
    }
    int setSize(int size){
      this->m_size = size;
    }
    int getID(){
      return m_ID;
    }
    void setID(int ID){
      this->m_ID = ID;
    }
    shared_ptr<T> getHead(){
      return head;
    }
    void setHead(shared_ptr<T> member){
      head = member;
    }
};

template<class T>
class RevTreeNode{
  private:
    int m_ID;
    shared_ptr<T> m_data;
    RevTreeNode<T>* parent;
    public:
      RevTreeNode(int ID, shared_ptr<T> data, RevTreeNode* parent):
      m_ID(ID), m_data(data), parent(parent){};

      ~RevTreeNode() = default;

      int getID(){
        return m_ID;
      }
      void setID(int ID){
        this->m_ID = ID;
      }
      shared_ptr<T> getData(){
        return m_data;
      }
      void setParent(RevTreeNode* parent){
        this->parent = parent;
      }
};

template<class T>
class UnionFind {
  private:
    DynamicArray<Group<T>> groups;
    DynamicArray<T> nodes;

  public:
    UnionFind(int size): groups(), nodes(){}

    ~UnionFind() = default;

    bool makeSet(T data){
      Group<T> group(0, data);
      RevTreeNode<T> node = new RevTreeNode<T>(data.getID(), data, &group);
      int nodeID = nodes.insert(data), groupID = groups.insert(group);
    }


};

#endif //UNIONFIND_H
