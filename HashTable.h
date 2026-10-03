// HashTable.h
//
// A hash table with separate chaining, mapping int keys to shared_ptr<T>
// values. Buckets are singly linked lists of Node<T>; a new node is always
// inserted at the head of its bucket.
//
// Resizing: the table doubles when load factor reaches 1 (numElements ==
// tableSize) and halves when it drops to 1/4, so insert/find/remove are
// O(1) amortized.
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <memory>
#include <iostream>

template <class T>
class Node {
   public:
    int id;
    std::shared_ptr<T> value;
    std::shared_ptr<Node> next;
    Node() = delete;
    explicit Node(int id, const std::shared_ptr<T>& value = nullptr)
        : id(id), value(value), next(nullptr) {}
};

template <class T>
class HashTable {
    int numElements;  // n
    int tableSize;    // m
    std::shared_ptr<Node<T>>* table;
    const static int RESIZE_FACTOR = 2;

    void elementRemoved();

   public:
    HashTable() : numElements(0), tableSize(10) {
        table = new std::shared_ptr<Node<T>>[tableSize]();
    }
    ~HashTable();

    // Deep-copying isn't implemented; disable copying so the
    // compiler-generated versions (which would shallow-copy the raw
    // `table` array and double-free it) can't be used by accident.
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    int applyHashFunc(int id) const;
    // Inserts (id, element) at the head of its bucket and returns the new
    // node. Does not check for an existing id -- inserting a duplicate id
    // adds a second node rather than replacing the first; findElement()
    // will then return whichever of the two was inserted most recently.
    std::shared_ptr<Node<T>> insert(int id, const std::shared_ptr<T>& element);
    void resize(int new_size);
    int getNumElements() const { return numElements; }
    int getTableSize() const { return tableSize; }
    std::shared_ptr<Node<T>> findElement(int id) const;
    // Removes the first node found with this id. Returns true if a node
    // was removed, false if no node with this id existed.
    bool remove(int id);
    void print() const;
};

template <class T>
int HashTable<T>::applyHashFunc(int id) const {
    int toReturn = id % tableSize;
    if (toReturn < 0) {
        toReturn += tableSize;
    }
    return toReturn;
}

template <class T>
std::shared_ptr<Node<T>> HashTable<T>::insert(int id, const std::shared_ptr<T>& element) {
    if (numElements == tableSize) {
        resize(tableSize * RESIZE_FACTOR);
    }
    std::shared_ptr<Node<T>> toAdd = std::make_shared<Node<T>>(id, element);
    int index = applyHashFunc(id);
    toAdd->next = table[index];
    table[index] = toAdd;
    numElements++;
    return toAdd;
}

template <class T>
void HashTable<T>::resize(int new_size) {
    int oldSize = tableSize;
    tableSize = new_size;
    std::shared_ptr<Node<T>>* newTable = new std::shared_ptr<Node<T>>[tableSize]();

    for (int i = 0; i < oldSize; i++) {
        std::shared_ptr<Node<T>> tmp = table[i];
        while (tmp) {
            int index = applyHashFunc(tmp->id);
            std::shared_ptr<Node<T>> toMove = tmp;
            tmp = tmp->next;
            toMove->next = newTable[index];  // insert at the head of the list
            newTable[index] = toMove;
        }
    }

    delete[] table;
    table = newTable;
}

template <class T>
std::shared_ptr<Node<T>> HashTable<T>::findElement(int id) const {
    int index = applyHashFunc(id);
    std::shared_ptr<Node<T>> p = table[index];
    while (p) {
        if (p->id == id) {
            return p;
        }
        p = p->next;
    }
    return nullptr;
}

template <class T>
void HashTable<T>::elementRemoved() {
    numElements--;
    if (tableSize > 10 && numElements * 4 <= tableSize) {
        resize(std::max(10, tableSize / RESIZE_FACTOR));
    }
}

template <class T>
bool HashTable<T>::remove(int id) {
    int index = applyHashFunc(id);
    std::shared_ptr<Node<T>> prev = nullptr;
    std::shared_ptr<Node<T>> curr = table[index];

    while (curr) {
        if (curr->id == id) {
            if (prev) {
                prev->next = curr->next;
            } else {
                table[index] = curr->next;
            }
            elementRemoved();
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

template <class T>
HashTable<T>::~HashTable() {
    delete[] table;
}

template <class T>
void HashTable<T>::print() const {
    std::cout << "number of elements: " << numElements << std::endl;
    std::cout << "table size: " << tableSize << std::endl;
    for (int i = 0; i < tableSize; i++) {
        std::shared_ptr<Node<T>> p = table[i];
        while (p) {
            std::cout << "index: " << i << " id: " << p->id << std::endl;
            p = p->next;
        }
    }
}

#endif  // HASHTABLE_H
