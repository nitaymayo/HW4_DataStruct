//
// Created by nitaymayo on 1/10/25.
//

#ifndef UNIONFIND_H
#define UNIONFIND_H
#include "DynamicArray.h"
#include "DynamicHash.h"
#include <memory>
#include <utility>
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
      shared_ptr<RevTreeNode<T>> getParent(){
      return m_parent;
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
      void setSet(shared_ptr<Set<T>> set){
      this->m_set = std::move(set);
      }
};


template<class T>
struct Set{
  private:
    int m_size;
    int m_ID;
    shared_ptr<RevTreeNode<T>> head;
    bool deleted;
    int m_record;
  public:
    Set(const int ID, shared_ptr<RevTreeNode<T>> root): m_size(1), m_ID(ID), deleted(false){
    m_record = root->getData()->getRecord();
      head = std::move(root);
    };

    ~Set() = default;

    int getSize() const {
      return m_size;
    }
    void increaseSize(int size){
      this->m_size += size;
    }
    int getRecord() const {
    return m_record;
    }
    int key() const {return getID();};
    int getID() const {
      return m_ID;
    }
    void setID(int ID){
      this->m_ID = ID;
    }
    shared_ptr<RevTreeNode<T>> getHead(){
      return head;
    }
    void setHead(shared_ptr<RevTreeNode<T>> member){
      head = std::move(member);
    }
    void deleteSet(){
    deleted = true;
    head = nullptr;
    }

};

class UnionFind {
  private:
    DynamicHash<Set<Herd>> sets;
    DynamicHash<RevTreeNode<Herd>> nodes;
    int m_size;

  public:
    UnionFind(): sets(), nodes(), m_size(0) {};

    ~UnionFind() = default;

    shared_ptr<Set<Herd>> makeSet(const Herd& data){
      auto newHerd = make_shared<Herd>(data);
      auto root = make_shared<RevTreeNode<Herd>>(newHerd, nullptr, nullptr);
      shared_ptr<Set<Herd>> set = make_shared<Set<Herd>>(m_size++, root);
      auto node = make_shared<RevTreeNode<Herd>>(newHerd, nullptr, set);
  	  sets.insert(set);
  	  nodes.insert(root);
      return set;
    }

    void Union(const Herd& left, const Herd& right){
      shared_ptr<Set<Herd>> leftSet = Find(left), rightSet = Find(right);
      if (!leftSet || !rightSet) {
        throw std::invalid_argument("No such element");
      }

      shared_ptr<RevTreeNode<Herd>> smallerRep;
      shared_ptr<RevTreeNode<Herd>> largerRep;

      //unite into larger set
      if (leftSet->getSize() > rightSet->getSize()){
        smallerRep = rightSet->getHead();
        largerRep = leftSet->getHead();
      } else {
        smallerRep = leftSet->getHead();
        largerRep = rightSet->getHead();
    	}

       smallerRep->setParent(largerRep);
       smallerRep->clearSet();
       largerRep->getSet()->increaseSize(smallerRep->getSet()->getSize());
      // check who has better score
      if (largerRep->getSet()->getRecord() < smallerRep->getSet()->getRecord()) {
        largerRep->clearSet();
        smallerRep->getSet()->setHead(largerRep);
        largerRep->setSet(smallerRep->getSet());
        smallerRep->clearSet();
      } else if (largerRep->getSet()->getRecord() == smallerRep->getSet()->getRecord()) {
        // clear right set and make the final ID be of the left set
        leftSet->getHead()->clearSet();
        leftSet->setHead(rightSet->getHead());
        rightSet->getHead()->setSet(leftSet);
        rightSet->deleteSet();
      }

    }

    shared_ptr<Set<Herd>> Find(const Herd& data){
    	 shared_ptr<RevTreeNode<Herd>> nodePtr = nodes.search(data.key());
      if (!nodePtr) {
        throw std::invalid_argument("No such element");
      }
         auto nodePtrUpdater = nodePtr;
         //find root
         while (nodePtr->getParent()){
            nodePtr = nodePtr->getParent();
         }

         // update nodes parents
         while (nodePtrUpdater->getParent()){
           auto temp = nodePtrUpdater;
           nodePtrUpdater = nodePtrUpdater->getParent();
           temp->setParent(nodePtr);
         }

         return sets.search(nodePtr->key());
    }

};

#endif //UNIONFIND_H
