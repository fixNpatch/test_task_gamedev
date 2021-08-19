#pragma once

const int MAX_CLUSTER_SIZE = 10;


// ---------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------
class Cell
{
public:
	int m_SomeData;
	Cell* m_prev;
	Cell* m_next;

public:
	Cell* addr();
	Cell(int data, Cell* prev, Cell* next);
	void SetPointers(Cell* m_prev, Cell* m_next);
	void SetData(int data);
	int GetData();
	Cell* GetPrev();
	Cell* GetNext();
};

// ---------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------


class Cluster {
public:
	int size;
	Cell* cl_head;
	Cell* cl_tail;
	Cluster* cl_prev;
	Cluster* cl_next;
	Cluster(Cluster* prev, Cluster* next, Cell* head) { cl_prev = prev; cl_next = next; cl_head = head; cl_tail = nullptr; size = 1; };

public:
	void pushBack(int data, Cluster* prev);
};


// ---------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------

struct CellClusterRef {
	Cell* cell;
	Cluster* cluster;
};

class Container {
private:
	Cluster* head;
	Cluster* tail;
	
public: 
	int NumberOfClusters;
	Container() {
		head = nullptr;
		tail = nullptr;
		NumberOfClusters = 0;
	};

public:
	void LoadOut();
	CellClusterRef GetNthCell(int index);

	void pushAfterPointer(int data, Cell* cellptr, Cluster* clusterptra);
	void pushAfterIndex(int data, int index);
	void pushBack(int data);


};

