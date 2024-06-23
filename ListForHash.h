
#ifndef WET2_LISTFORHASH_H
#define WET2_LISTFORHASH_H


template<class Object>
class Node{
    Object obj;
    int key;
    Node* next;

public:
    Node(Object obj, int key):obj(obj), key(key),next(nullptr){};
    ~Node() = default;

    Object getObj(){
        return obj;
    }
    int getKey(){
        return key;
    }
    Node* getNext(){
        return next;
    }

    void setObj(Object new_obj){
        obj = new_obj;
    }
    void setKey(int new_key){
        key = new_key;
    }
    void setNext(Node* new_next){
        next = new_next;
    }
};


template<class Object>
class List{
    Node<Object>* head;
    int size;
public:
    List():head(nullptr), size(0){};
    ~List();
    Node<Object>* getHead();
    int getSize();
    void setHead(Node<Object>* new_head);
    void setSize(int new_size);
    Node<Object>* findNodeInList(int key_to_find);
    void pushBack(Object new_obj,int new_key);
    void removeFromList(int key_to_remove);

};
template<class Object>
List<Object>::~List(){
    while(head){
        removeFromList(head->getKey());
    }
    delete head;
}

template<class Object>
Node<Object>* List<Object>::getHead(){
    return this->head;
}

template<class Object>
int List<Object>::getSize(){
    return size;
}

template<class Object>
void List<Object>::setHead(Node<Object>* new_head){
    head = new_head;
}

template<class Object>
void List<Object>::setSize(int new_size) {
    size = new_size;
}

template<class Object>
Node<Object>* List<Object>::findNodeInList(int key_to_find){
    Node<Object>* curr_node = head;
    while(curr_node!= nullptr){
        if(curr_node->getKey() == key_to_find){
            return curr_node;
        }
        curr_node = curr_node->getNext();
    }
    return nullptr;
}
template<class Object>
void List<Object>::pushBack(Object new_obj,int new_key){
    if(findNodeInList(new_key) != nullptr) return;

    Node<Object>* node_to_add = new (std::nothrow) Node<Object>(new_obj,new_key);
    if(node_to_add == nullptr){
        return;
    }

    if(head == nullptr){
        this->setHead(node_to_add);
        size++;
        return;
    }

    Node<Object>* curr_node = head;
    while(curr_node->getNext()!= nullptr){
        curr_node = curr_node->getNext();
    }
    curr_node->setNext(node_to_add);
    size++;
}
template<class Object>
void List<Object>::removeFromList(int key_to_remove){
    if(findNodeInList(key_to_remove) == nullptr) return;

    Node<Object>* curr_node = head;

    if(curr_node->getKey() == key_to_remove){
        this->setHead(curr_node->getNext());
        size--;
        delete curr_node;
        return;
    }

    Node<Object>* prev_node = nullptr;
    while(curr_node!= nullptr){
        if(curr_node->getKey() == key_to_remove){
            prev_node->setNext(curr_node->getNext());
            size--;
            delete curr_node;
            return;
        }
        prev_node = curr_node;
        curr_node = curr_node->getNext();
    }
}


#endif //WET2_LISTFORHASH_H
