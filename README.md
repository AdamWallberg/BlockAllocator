# BlockAllocator
This is a single-header block allocator that I wrote as an excercise.
There are definitively better options out there, but it might be good for educational purposes.
<br><br>
# Usage
This allocator is a standard block/pool allocator that, when created, allocates a block of memory from the heap. You can then store objects in this block using the member method ```alloc()```. If you run out of memory in the current block, another block will be allocated. How many elements can be stored in one block, and the maximum total amount of blocks that can be allocated is defined by the user. A too small block size will result in a lot of dynamic allocation, so it's recommended to set these values accordingly.

<b>Creation</b><br>
This example an allocator that stores objects of the type ```Foo```. One block can hold 32 elements, and a total of 8 blocks can be allocated.
```cpp
BlockAllocator<Foo, 32, 8> allocator;
```
<b>Allocation</b><br>
This stores the object on the heap and returns the address. The object is sent as a reference, and is then copied onto the heap.
```cpp
Foo* objPtr = allocator.alloc(Foo());
```
<b>Deallocation</b><br>
This removes the element from the block, and frees up its spot. 
```cpp
allocator.dealloc(objPtr);
```
If invoking this method empties a block, this block will <b>not</b> be deallocated. If a block has been created it will stay until the allocator itself is destroyed. This makes it so that we don't have to allocate a new block if we want to add that element again.<br>
