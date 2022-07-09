
#include "NodeList.h"
#include <iostream>



NodeList::NodeList(){
   this->length = 0;
}

NodeList::NodeList(int size) {
   this->length = 0;
   this->size = size;
   this->nodes = new NodePtr[size];
}

NodeList::NodeList(NodeList &other) {
   this->nodes = new NodePtr[other.size];
   this->size = other.size;
   for (int i=0; i<other.length; ++i) {
      NodePtr node = new Node(other.get(i)->getRow(), other.get(i)->getCol(), other.get(i)->getDistanceToS());
      this->addBack(node);
   }
}

NodeList::~NodeList(){
   this->clear();
}


int NodeList::getLength(){
   return this->length;
}


NodePtr NodeList::get(int i){ 
   return this->nodes[i];
}

void NodeList::addBack(NodePtr newNode){  
   this->length += 1;
   this->nodes[this->length-1] = newNode;

   // Legacy method for backup
   // bool addSuccess = false;
   // for (int i=0; i<this->size && addSuccess==false; ++i) {
   //    if (this->nodes[i] == nullptr) {
   //       this->nodes[i] = newNode;
   //       addSuccess = true;
   //    }
   // }
   // this->length += 1;
   // std::cout << newNode << std::endl;
}

bool NodeList::containsNode(NodePtr node){
   bool containsNode = false;
   for (int i=0; i<this->length && containsNode == false; ++i) {
      if (this->nodes[i]->getRow() == node->getRow() 
      && this->nodes[i]->getCol() == node->getCol()) {
         containsNode = true;
      }
   }
   return containsNode;
}

void NodeList::clear(){
   if (this->length != 0) {
      for (int i=0; i<this->length; ++i) {
         delete this->get(i);
         this->nodes[i] = nullptr;
      }
   }
}

// Overloaded form of get() that searches by coordinate instead
NodePtr NodeList::get(int row, int col) {
   NodePtr pointer = nullptr;
   for (int i=0; i<this->length; ++i) {
      if (row == this->nodes[i]->getRow() 
      && col == this->nodes[i]->getCol()) {
         pointer = this->nodes[i];
      }
   }
   return pointer;
}