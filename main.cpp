#pragma once

#include "Source.h"
#include <iostream>


int main() {
	Container cont = Container();

	for (int i = 0; i < 28; i++) {
		cont.pushBack(i);
	}


	// TEST PUSH AFTER INDEX
	for (int i = 0; i < 5; i++) {
		cont.pushAfterIndex((100 + i), 19);
	}
	//cont.pushAfterIndex(101, 19);
	//cont.pushAfterIndex(102, 19);
	cont.pushAfterIndex(999, 24);

	cont.LoadOut();

	

	std::cout << std::endl;
	
	CellClusterRef result = cont.GetNthCell(12);
	std::cout << result.cell << " in cluster " << result.cluster << std::endl;

	std::cout << cont.NumberOfClusters << std::endl;


	return 0;
}