# Hash Table (Separate Chaining)

A hash table in C++, templated over the stored value type, mapping `int`
keys to `shared_ptr<T>` values. Collisions are handled with separate
chaining — each bucket is a singly linked list of nodes.

## How it works

`insert`, `findElement`, and `remove` are all `O(1)` on average: the key is
hashed into a bucket index, and the bucket's short linked list is walked
to find the matching node. The table doubles in size whenever the load
factor reaches 1 (`numElements == tableSize`) and halves whenever it drops
to 1/4, down to a minimum size of 10, so the average bucket length stays
small as the table grows or shrinks.

## API

```cpp
HashTable<T> table;
table.insert(id, value);        // value: shared_ptr<T>
auto node = table.findElement(id);   // nullptr if not present
table.remove(id);               // true if removed, false if not present
table.print();                  // prints every (index, id) pair
```

## Run it

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o ht_demo
./ht_demo
```

`main.cpp` inserts 15 elements (forcing a resize from the initial table
size of 10), finds an existing and a missing key, removes a key and
confirms it's gone, and prints the final table.

## Notes

This started as a course assignment and had a few issues I fixed while
cleaning it up: `insert` returned a reference into the internal array,
which would dangle after a later resize freed that array, so it now
returns by value. `print()` built an "id: " line but never actually
printed the id. There was no copy constructor or assignment operator,
so copying a table would double-free its internal array on destruction;
copying is now explicitly disabled instead. And `elementRemoved()`
(which shrinks the table) existed but nothing called it, because
deletion itself wasn't implemented — `remove()` is a completed version
of that missing piece.
