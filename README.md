# BlockAllocator
This is a single-header block allocator that I wrote as an excercise.
There are definitively better options out there, but it might be good for educational purposes.
<br><br>
# Usage
This allocator is a standard block/pool allocator that, when created, allocates a block of memory from the heap. You can then store objects in this block using the member method ```alloc()```. If you run out of memory in the current block, another block will be allocated. How many elements can be stored in one block, and the maximum total amount of blocks that can be allocated is defined by the user. A too small block size will result in a lot of dynamic allocation, so it's recommended to set these values accordingly.

### Creation
This example creates an allocator that stores objects of the type ```Foo```, where one block can hold 8 elements, and a total of 2 blocks can be allocated.
```cpp
// Like this for custom values
BlockAllocator<Foo, 8, 2> allocator;
// Or like this for a default of 32 and 4
BlockAllocator<Foo> allocator;
```
### Allocation
This stores the object on the heap and returns the address. The object is sent as a reference, and is then copied onto the heap.
```cpp
Foo* objPtr = allocator.insert(Foo(1, 2, 3));
```
This constructs the object directly onto the heap, taking the object's constructor arguments. 
```cpp
Foo* objPtr = allocator.alloc(1, 2, 3);
```
Both these examples assume that ```Foo``` has a constructor that takes 3 integers.
### Deallocation
This removes the element from the block, and frees up its spot. 
```cpp
allocator.dealloc(objPtr);
```
If invoking this method empties a block, this block will <b>not</b> be deallocated. If a block has been created it will stay until the allocator itself is destroyed. This makes it so that we don't have to allocate a new block if we want to add that element again.<br>

### Fragmentation Prevention
This allocator does not defragment, since this would require the use of smart pointers. It does however prevent fragmentation to some extent.<br>
In this example, ```c``` will simply fill in ```a```'s old position. And when we allocate ```d``` it skips to the next free slot.
```cpp
BlockAllocator<Foo, 3, 1> allocator;  // [-, -, -]
Foo* a = allocator.alloc(Foo());      // [A, -, -]
Foo* b = allocator.alloc(Foo());      // [A, B, -]
allocator.dealloc(a);                 // [-, B, -]
Foo* c = allocator.alloc(Foo());      // [C, B, -]
Foo* d = allocator.alloc(Foo());      // [C, B, D]
```
