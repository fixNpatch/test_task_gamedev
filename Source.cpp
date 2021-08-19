#pragma once

#include "Source.h"
#include <iostream>


// ---------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------


Cell::Cell(int SomeData, Cell* prev, Cell* next)
{
	m_SomeData = SomeData;
	m_prev = prev;
	m_next = next;
}

void Cell::SetPointers(Cell* prev, Cell* next)
{
	m_next = next;
	m_prev = prev;
}

void Cell::SetData(int data)
{
	m_SomeData = data;
}

int Cell::GetData()
{
	return m_SomeData;
}

Cell* Cell::GetPrev()
{
	return m_prev;
}

Cell* Cell::GetNext()
{
	return m_prev;
}

Cell* Cell::addr()
{
	return this;
}

// ---------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------

void Cluster::pushBack(int data, Cluster* prev)
{
	size++;		

	Cell* NewCell = new Cell(data, nullptr, nullptr);

	if (cl_head == nullptr) { // дополнительная проверка (99% не зайдет сюда)
		cl_head = NewCell;
		return;
	}
	else if (cl_tail == nullptr) { // если еще не было указано tail
		cl_tail = NewCell;
		cl_head->m_next = cl_tail;
		cl_tail->m_prev = cl_head;
	}
	else {
		NewCell->m_prev = cl_tail;
		cl_tail->m_next = NewCell;
		cl_tail = NewCell;
	}
}


// ---------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------

// Функция отображения данных
void Container::LoadOut()
{
	std::cout << "Addr\t||\tPrev\t||\tNext\t||\tVal\t||\tCluster" << "\n";
	std::cout << "===========================================================================" << "\n";
	Cluster* currentCluster = head;
	Cell* currentCell;
	while (true) {
		currentCell = currentCluster->cl_head;
		for (auto i = 0; i < currentCluster->size; i++) {
			std::cout << currentCell << "||    " << currentCell->m_prev << "\t||    " << currentCell->m_next << "\t||\t" << currentCell->m_SomeData << "\t||     " << currentCluster << " " << currentCluster->cl_next << "\n";
			if (currentCell->m_next == nullptr) break;
			currentCell = currentCell->m_next;
		}
		
		if (currentCluster->cl_next == nullptr) break;
		currentCluster = currentCluster->cl_next;
	}

}

// Функция получения указателя на ячейку и указателя на связанный с ней кластер
CellClusterRef Container::GetNthCell(int index)
{
	CellClusterRef result;
	result.cell = nullptr;
	result.cluster = nullptr;

	// если нет ни одного кластера, то нет смысла искать индекс
	if (head == nullptr) return result;

	// делаем огромные шаги по кластерам
	int counter = 0;
	Cluster* currentCluster = head;
	while (counter + currentCluster->size < index) {
		counter += head->size;
		if (currentCluster->cl_next == nullptr) break;
		currentCluster = currentCluster->cl_next;
	}

	// бежим по найденному кластеру
	Cell* currentElem = currentCluster->cl_head;
	while (counter < index) {
		if (currentElem->m_next == nullptr) break;
		currentElem = currentElem->m_next;
		counter++;
	}

	// если счетчик дошел до нужного значения, значит все нашли
	if (counter == index) {
		result.cell = currentElem;
		result.cluster = currentCluster;
		return result;
	}

	return result;
}

// Insert by pointer function
void Container::pushAfterPointer(int data, Cell* cellptr, Cluster* clusterptr)
{
	Cell* NewCell = new Cell(data, nullptr, nullptr); // создаем референс на новую ячейку

	if (cellptr == nullptr || clusterptr == nullptr) return;

	// если размер кластера позволяет, то пишем в него
	if (clusterptr->size + 1 <= MAX_CLUSTER_SIZE) {
		NewCell->m_prev = cellptr;
		NewCell->m_next = cellptr->m_next;
		cellptr->m_next->m_prev = NewCell; 
		cellptr->m_next = NewCell;
		clusterptr->size++;
	}
	else { // если не позволяет размер рассматриваемого
		// смотрим размер следующего кластера
		if (clusterptr->cl_next != nullptr && clusterptr->cl_next->size + 1 <= MAX_CLUSTER_SIZE) { // 1. либо пишем в него
			NewCell->m_prev = cellptr;
			NewCell->m_next = cellptr->m_next;
			cellptr->m_next = NewCell;
			cellptr->m_next->m_prev = NewCell;
			clusterptr->cl_next->cl_head = NewCell;
			clusterptr->cl_next->size++;
		}
		else { // 2. либо создаем еще один кластер между рассмотренными двумя
			Cluster* NewCluster = new Cluster(nullptr, nullptr, NewCell);
			NumberOfClusters++;
			
			NewCluster->cl_prev = clusterptr;
			NewCluster->cl_next = clusterptr->cl_next;
			clusterptr->cl_next = NewCluster;
			clusterptr->cl_next->cl_prev = NewCluster;

			NewCell->m_prev = cellptr;
			NewCell->m_next = cellptr->m_next;
			cellptr->m_next = NewCell;
			cellptr->m_next->m_prev = NewCell;
		}
	}

}


void Container::pushAfterIndex(int data, int index)
{
	CellClusterRef CellInfo = this->GetNthCell(index);
	pushAfterPointer(data, CellInfo.cell, CellInfo.cluster);
}



void Container::pushBack(int data)
{
	Cell* NewCell = new Cell(data, nullptr, nullptr); // создаем референс на новую ячейку

	if (head == nullptr) { // если нет ни одного кластера
		Cluster* NewCluster = new Cluster(nullptr, nullptr, NewCell);
		NumberOfClusters++;
		head = NewCluster;
		return;
	}
	else { // если есть хотя бы один
		if (tail == nullptr) { // проверяем есть ли tail
			if (head->size < MAX_CLUSTER_SIZE) { // если tail'а нет, то смотрим на размер head_cluster
				head->pushBack(data, nullptr);
			}
			else { // если в head добавить нельзя, создаем новый кластер и присваиваем его tail'у
				Cluster* NewCluster = new Cluster(head, nullptr, NewCell);
				NumberOfClusters++;
				NewCell->m_prev = head->cl_tail;
				head->cl_tail->m_next = NewCell;
				tail = NewCluster;
				head->cl_next = tail;
			}
		}
		else { // если уже есть tail
			if (tail->size < MAX_CLUSTER_SIZE) {
				tail->pushBack(data, tail->cl_prev);
			}
			else {
				Cluster* NewCluster = new Cluster(tail, nullptr, NewCell);
				NumberOfClusters++;
				NewCell->m_prev = tail->cl_tail;
				tail->cl_tail->m_next = NewCell;
				tail->cl_next = NewCluster;
				tail = NewCluster;
			}
		}
	}

	return;
}
