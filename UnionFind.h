//
// Created by nitaymayo on 1/10/25.
//

#ifndef UNIONFIND_H
#define UNIONFIND_H
#include "DynamicArray.h"
#include "DynamicHash.h"
#include <memory>
#include "Herd.h"
using namespace std;

template <typename T>
struct Set;

template<class T>
struct RevTreeNode{
  private:
    shared_ptr<T> m_data;
    shared_ptr<RevTreeNode<T>> m_parent;
    shared_ptr<Set<T>> m_set;
    public:
      RevTreeNode(shared_ptr<T> data, shared_ptr<RevTreeNode<T>> parent, shared_ptr<Set<T>> set):
        m_data(data), m_parent(parent), m_set(set){};

      ~RevTreeNode() = default;

//      int getID(){
//        return m_ID;
//      }
//      void setID(int ID){
//        this->m_ID = ID;
//      }
      shared_ptr<T> getData(){
        return m_data;
      }
      int key(){return m_data->key();}
      void setParent(shared_ptr<RevTreeNode<T>> parent){
        this->m_parent = parent;
      }
      bool isRoot() const{
       	return (m_set != nullptr);
      }
      void clearSet(){
        m_set = nullptr;
      }
      shared_ptr<Set<T>> getSet(){
        return m_set;
      }
};


template<class T>
struct Set{
  private:
    int m_size;
    int m_ID;
    shared_ptr<RevTreeNode<T>> head;
    bool deleted;
  public:
    Set(int ID, shared_ptr<RevTreeNode<T>> root): m_size(1), m_ID(ID), head(root), deleted(false){};

    ~Set() = default;

    int getSize() const {
      return m_size;
    }
    void increaseSize(int size){
      this->m_size += size;
    }
    int key() const {return getID();};
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
class UnionFind {
  private:
    DynamicHash<Set<T>> sets;
    DynamicHash<RevTreeNode<T>> nodes;
    int m_size;

  public:
    UnionFind(): sets(), nodes(), m_size(0) {};

    ~UnionFind() = default;

    shared_ptr<Set<T>> makeSet(shared_ptr<T> data){
      auto root = make_shared<RevTreeNode<T>>(data, nullptr, nullptr);
      shared_ptr<Set<T>> set = make_shared<Set<T>>(m_size, data);
      auto node = make_shared<RevTreeNode<T>>(data, set);
  	  sets.insert(m_size++, set.get());
  	  nodes.insert(data->getID(), root.get());
      return set;
    }

    void Union(Set<T> &left, Set<T> &right){
      shared_ptr<RevTreeNode<T>> smallerSet;
      shared_ptr<Set<T>> largerSet;

      //unite into larger set
      if (left.getSize() > right.getSize()){
        smallerSet = right.getHead();
        largerSet = left;
      } else {
        smallerSet = left.getHead();
        largerSet = right;
    	}

       smallerSet.getHead()->setParent(largerSet.getHead());
       smallerSet.getHead()->clearSet();
       largerSet.increaseSize(smallerSet.getSize());
    }

    shared_ptr<Set<T>> Find(T data){
    	 auto nodePtr = nodes.search(data->getID());
         auto nodePtrUpdater = nodePtr;
         //find root
         while (!nodePtr.isRoot()){
            nodePtr = nodePtr.getParent();
         }

         // update nodes parents
         while (!nodePtrUpdater.isRoot()){
           auto temp = nodePtrUpdater;
           nodePtrUpdater = nodePtrUpdater.getParent();
           temp.setParent(nodePtr);
         }

         return nodePtr.getSet();
    }

};

#endif //UNIONFIND_H
