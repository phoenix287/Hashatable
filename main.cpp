// Demo: insert, find, remove, and resize a HashTable<std::string>.
#include <cassert>
#include <iostream>
#include <memory>
#include <string>

#include "HashTable.h"

int main() {
    HashTable<std::string> table;

    // Insert enough elements to force at least one resize (starts at
    // tableSize 10).
    for (int i = 0; i < 15; i++) {
        auto value = std::make_shared<std::string>("value_" + std::to_string(i));
        table.insert(i, value);
    }
    std::cout << "After 15 inserts: " << table.getNumElements()
              << " elements, table size " << table.getTableSize() << std::endl;
    assert(table.getNumElements() == 15);
    assert(table.getTableSize() > 10);  // confirms it grew

    auto found = table.findElement(7);
    assert(found != nullptr && *(found->value) == "value_7");
    std::cout << "find(7) -> " << *(found->value) << std::endl;

    assert(table.findElement(999) == nullptr);
    std::cout << "find(999) -> not found (as expected)" << std::endl;

    bool removed = table.remove(7);
    assert(removed);
    assert(table.findElement(7) == nullptr);
    std::cout << "remove(7) -> removed, find(7) now returns nullptr" << std::endl;

    assert(table.remove(7) == false);  // already gone
    std::cout << "remove(7) again -> false (as expected)" << std::endl;

    std::cout << "\nFinal contents:" << std::endl;
    table.print();

    std::cout << "\nAll checks passed." << std::endl;
    return 0;
}
