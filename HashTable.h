//
// Created by shaha on 23/12/2022.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

#include "ListForHash.h"
#include <iostream>
using std::cout;
using std::endl;

#define INIT_SIZE_OF_TABLE 3
#define MAX_FACTOR 1

template<class Data>
class HashTable{
    int table_size;
    int num_of_elements;
    List<Data>** table;

public:
    HashTable();
    ~HashTable();
    int getTableSize();
    int getNumOfElements();
    List<Data>** getTable();
    void setTableSize(int new_size);
    void setNumOfElements(int new_num_of_elements);
    void setTable(List<Data>** table_to_set);
    int hashFunction(int key);
    Node<Data>* findElement(int key);
    void insertNewElement(Data data, int key);
    void insertNewElementAfterSizeChange(Data data, int key);
    void removeElement(int key);
    void changeSizeIfNeeded();
    void printHash() const;
};
template<class Data>
HashTable<Data>::HashTable():table_size(INIT_SIZE_OF_TABLE),num_of_elements(0) {
    table = new List<Data>*[INIT_SIZE_OF_TABLE];

    for(int i=0; i<INIT_SIZE_OF_TABLE;i++){
        table[i] =  new List<Data>();
    }
}

template<class Data>
HashTable<Data>::~HashTable(){
    for(int i = 0; i < table_size; i++){
       delete table[i];
    }
    delete[] table;
}

template<class Data>
int HashTable<Data>::getTableSize(){
    return table_size;
}
template<class Data>
int HashTable<Data>::getNumOfElements(){
    return num_of_elements;
}
template<class Data>
List<Data>** HashTable<Data>::getTable(){
    return table;
}

template<class Data>
void HashTable<Data>::setTableSize(int new_size){
    table_size = new_size;
}
template<class Data>
void HashTable<Data>::setNumOfElements(int new_num_of_elements){
    num_of_elements = new_num_of_elements;
}
template<class Data>
void HashTable<Data>::setTable(List<Data>** table_to_set){
    table = table_to_set;
}
template<class Data>
int HashTable<Data>::hashFunction(int key){
    return key%table_size;
}
template<class Data>
Node<Data>*  HashTable<Data>::findElement(int key){
    int index = hashFunction(key);
    List<Data>* list_of_element = table[index];
    return list_of_element->findNodeInList(key);
}

template<class Data>
void  HashTable<Data>::insertNewElement(Data data, int key){
    int index = hashFunction(key);
    List<Data>* list_of_element = table[index];
    list_of_element->pushBack(data,key);
    num_of_elements++;
    changeSizeIfNeeded();
}

template<class Data>
void  HashTable<Data>::insertNewElementAfterSizeChange(Data data, int key){
    int index = hashFunction(key);
    List<Data>* list_of_element = table[index];
    list_of_element->pushBack(data,key);
    num_of_elements++;
}

template<class Data>
void HashTable<Data>::removeElement(int key){
    int index = hashFunction(key);
    List<Data>* list_of_element = table[index];
    list_of_element->removeFromList(key);
    num_of_elements--;
}

template<class Data>
void HashTable<Data>::changeSizeIfNeeded() {
    int load_factor = num_of_elements/table_size;
    if(load_factor<MAX_FACTOR) return;

    int old_size = table_size;
    int new_size = (table_size*2)+1;
    List<Data>** old_table = table;
    List<Data>** new_table = new List<Data>*[new_size];
    for(int i=0; i<new_size;i++)
    {
        new_table[i] =  new List<Data>();
    }
    table = new_table;
    num_of_elements = 0;
    table_size = new_size;
    for(int i=0; i<old_size;i++){
        List<Data>* curr_old_list = old_table[i];
        Node<Data>* curr_old_node = curr_old_list->getHead();
        while(curr_old_node!= nullptr){
            insertNewElementAfterSizeChange(curr_old_node->getObj(),curr_old_node->getKey());
            curr_old_node = curr_old_node->getNext();
        }
    }
    for(int i=0;i<old_size;i++){
        delete old_table[i];
    }
    delete[] old_table;

}

template<class Data>
void HashTable<Data>::printHash() const {
    cout << "Printing Hash: " << endl;
    for (int i = 0; i < table_size; i++)
    {
        cout << "|" << i << "|";
        Node<Data> *curr_node = table[i]->getHead();
        while (curr_node != nullptr) {
            cout << "-->" << curr_node->getKey();
            curr_node = curr_node->getNext();
        }
        cout << endl;
    }
}



#endif //WET2_HASHTABLE_H
