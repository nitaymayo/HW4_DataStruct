//
// Created by nitaymayo on 1/10/25.
//

#ifndef UNIONFIND_H
#define UNIONFIND_H
#include "DynamicArray.h"
#include "DynamicHash.h"
#include "DynamicRecordsHash.h"
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
        m_set.reset();
      }
      shared_ptr<Set<T>> getSet(){
        return m_set;
      }
      void setSet(shared_ptr<Set<T>> set){
      this->m_set = set;
      }
};


template<class T>
struct Set{
  private:
    int m_size;
    int m_ID;
    shared_ptr<RevTreeNode<T>> head;
    bool deleted;
    shared_ptr<RecordsNode<Set<T>>> m_record;
  public:
    Set(const int ID, shared_ptr<RevTreeNode<T>> root): m_size(1), m_ID(ID),
     deleted(false), m_record(nullptr){
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
    return m_record->getRecord();
    }
    shared_ptr<RecordsNode<Set<Herd>>> getNodeRecord(){
      return m_record;;
    }
    void setRecord(shared_ptr<RecordsNode<Set<Herd>>> record){
      m_record = record;
    }
    void increaseRecord(int num){
      m_record->m_record += num;
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
      head = member;
    }
    void deleteSet(){
    deleted = true;
      head->getData()->markDelete();
      head.reset();
      m_record.reset();
      m_size = -1;
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
      shared_ptr<Set<Herd>> set = make_shared<Set<Herd>>(data.getId(), root);
      root->setSet(set);
  	  sets.insert(set);
  	  nodes.insert(root);
      m_size++;
      return set;
    }

    void Union(const Herd& left, const Herd& right, DynamicRecordsHash& records){
      shared_ptr<Set<Herd>> leftSet = Find(left), rightSet = Find(right);
      if (!leftSet || !rightSet) {
        throw std::invalid_argument("No such element");
      }
      //unite into larger set
      if (leftSet->getSize() > rightSet->getSize()){
        rightSet->getHead()->setParent(leftSet->getHead());
        if (leftSet->getRecord() < rightSet->getRecord()){
            setLeadingSet(rightSet, leftSet, records);
        } else {
            leftSet->increaseRecord(rightSet->getRecord());
            leftSet->increaseSize(rightSet->getSize());
            rightSet->deleteSet();
        }
      } else {
        leftSet->getHead()->setParent(rightSet->getHead());
        if (leftSet->getRecord() >= rightSet->getRecord()){
            setLeadingSet(leftSet, rightSet, records);
        } else {
            rightSet->increaseRecord(leftSet->getRecord());
            rightSet->increaseSize(leftSet->getSize());
            leftSet->deleteSet();
        }
    	}
    }
    void setLeadingSet(const shared_ptr<Set<Herd>>& lead,
       const shared_ptr<Set<Herd>>& follow, DynamicRecordsHash& records){
        auto initalLeadHead = lead->getHead();
        lead->setHead(follow->getHead());
        lead->getHead()->setSet(lead);
        lead->increaseSize(follow->getSize());
        // lead->increaseRecord(follow->getRecord());
        shared_ptr<RecordsNode<Set<Herd>>> loseRecord = follow->getNodeRecord();
        shared_ptr<RecordsNode<Set<Herd>>> winRecord = lead->getNodeRecord();
        records.deleteNode(loseRecord);
        records.deleteNode(winRecord);
        shared_ptr<RecordsNode<Set<Herd>>> record = 
        records.insert(loseRecord->m_record + winRecord->m_record , lead);
        lead->setRecord(record);

        initalLeadHead->clearSet();
        follow->deleteSet();
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

         return sets.search(nodePtr->getSet()->key());
    }

};

#endif //UNIONFIND_H
