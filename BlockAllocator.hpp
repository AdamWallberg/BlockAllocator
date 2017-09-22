/**
	@file BlockAllocator.hpp
	@author Adam Wallberg
	@date 22/09/2017
*/

#pragma once
#include <cstdlib>
#include <cstring>
#include <assert.h>

template<class T, unsigned int BLOCK_SIZE = 32, unsigned int MAX_NUM_BLOCKS = 4>
class BlockAllocator
{
private:
	static_assert(BLOCK_SIZE > 0, "Block size should not be less than 1!");
	static_assert(BLOCK_SIZE > 0, "Max number of blocks should not be less than 1!");

	struct Block
	{
		bool filledFlags[BLOCK_SIZE];
		T* first;
		T* last;
		T* current;
	};
public:
	BlockAllocator()
		: currentBlock(0)
		, numBlocks(0)
	{
		blocks[0] = allocBlock();
	}

	~BlockAllocator()
	{
		for (unsigned int i = 0; i < numBlocks; i++)
		{
			free(blocks[i].first);
		}
	}

	T* alloc(const T&  object)
	{
		Block* block = &blocks[currentBlock];
		int position = block->current - block->first;

		while (true)
		{
			// Check if we are out of space in current block.
			if (block->current > block->last)
			{
				// Check if we have to allocate a new block,
				// or if we already have an empty one.
				currentBlock++;
				if (currentBlock >= numBlocks)
					blocks[currentBlock] = allocBlock();
				
				// Update pointer
				block = &blocks[currentBlock];
				position = block->current - block->first;
			}
			// Check if the current address is filled.
			else if (block->filledFlags[position])
			{
				block->current++;
				position = block->current - block->first;
			}
			else
			{
				// We have found a free address.
				break;
			}
		}

		// Store result in address.
		T* result = block->current;
		*result = object;
		block->filledFlags[position] = true;
		block->current++;
		return result;
	}

	void dealloc(T* object)
	{
		for (unsigned int i = 0; i < numBlocks; i++)
		{
			Block* block = &blocks[i];
			// Check if address is in range of this block
			if (object >= block->first && object <= block->last)
			{
				assert(
					block->filledFlags[object - block->first] 
					&& "This item is already deallocated!"
				);
				memset(object, 0, sizeof(T));
				block->filledFlags[object - block->first] = false;
				// Since we now have a free address, we set the "current"
				// pointer here, so we don't get a fragmented block.
				if (i <= currentBlock)
				{
					currentBlock = i;
					if (object <= block->current)
					{
						block->current = object;
					}
				}

				return;
			}
		}
	}
	
	unsigned int getNumAllocatedBlocks() const { return numBlocks; }
private:
	Block allocBlock()
	{
		assert(
			numBlocks < MAX_NUM_BLOCKS
			&& "Out of memory blocks! Please increase block capacity!"
		);

		Block block;

		// Allocate block memory.
		block.current = block.first = (T*)malloc(sizeof(T) * BLOCK_SIZE);
		
		block.last = block.first + BLOCK_SIZE - 1;
		memset(block.first, 0, sizeof(T) * BLOCK_SIZE);
		
		// Set flags to false.
		memset(&block.filledFlags[0], false, sizeof(bool) * BLOCK_SIZE);
		
		numBlocks++;
		return block;
	}

	Block blocks[MAX_NUM_BLOCKS];
	unsigned int currentBlock;
	unsigned int numBlocks;
};
