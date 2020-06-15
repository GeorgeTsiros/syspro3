#include <iostream>
#include "../inc/functs.h"
#include "../inc/heapPair.h"
#include "../inc/heap.h"

int PriorityQueue::PARENT(int i) 
{ 
	return (i - 1) / 2; 
}

int PriorityQueue::LEFT(int i) 
{ 
	return (2 * i + 1); 
}

int PriorityQueue::RIGHT(int i) 
{ 
	return (2 * i + 2); 
}

void PriorityQueue::heapify_down(int i) {
	// get left and right child of node at index i
	int left = LEFT(i);
	int right = RIGHT(i);

	int largest = i;

	// compare A[i] with its left and right child
	// and find largest value
	if (left < size() && A[left].count > A[i].count)
		largest = left;

	if (right < size() && A[right].count > A[largest].count)
		largest = right;


	if (largest != i) {
		pairSwap(A[i], A[largest]);
		heapify_down(largest);
	}
}
 

void PriorityQueue::heapify_up(int i) {

	if (i && A[PARENT(i)].count < A[i].count) {
		
		pairSwap(A[i], A[PARENT(i)]);
	
		heapify_up(PARENT(i));
	}
}

unsigned int PriorityQueue::size()
{
	return A.size();
}

// function to check if heap is empty or not
bool PriorityQueue::empty()
{
	return size() == 0;
}

// insert key into the heap
void PriorityQueue::push(heapPair hPair)
{
	//insert the new element to the end of the vector
	A.add(hPair);

	// get element index and call heapify-up procedure
	int index = size() - 1;
	heapify_up(index);
}

// function to remove element with highest priority (present at root)
void PriorityQueue::pop()
{
	if (size() == 0) {
		std::cerr << "Heap Underflow" << std::endl;
		return;
	}
		
	int end = A.size() - 1;
	A[0] = A[end];
	A.remove(end);

	// call heapify-down on root node
	heapify_down(0);
	
}

heapPair PriorityQueue::top()
{
	
	if(size() == 0) {
		std::cerr << "Index out of range" << std::endl;
		exit(1);
	}

	return A[0];
}


