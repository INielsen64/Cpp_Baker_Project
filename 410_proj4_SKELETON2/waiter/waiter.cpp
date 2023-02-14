#include <string>
#include "stdlib.h"
#include "../includes/externs.h"
#include "../includes/logger.h"
#include "../includes/waiter.h"
#include <iostream>

using namespace std;

Waiter::Waiter(int id, std::string filename) :
		id(id), myIO(filename) {
}

Waiter::~Waiter() {
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder) {
	return myIO.getNext(anOrder);
}

void Waiter::beWaiter() {
	b_WaiterIsFinished = false;
	ORDER nextOrder;

	while (getNext(nextOrder) == SUCCESS) {
		mutex_order_inQ.lock();
		order_in_Q.push(nextOrder);
		mutex_order_inQ.unlock();
		cv_order_inQ.notify_all();

	}
	b_WaiterIsFinished = true;
	cv_order_inQ.notify_all();

}

