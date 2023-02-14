#include <mutex>
#include "../includes/externs.h"
#include "../includes/logger.h"
#include "../includes/baker.h"
using namespace std;

Baker::Baker(int id) :
		id(id) {
}

Baker::~Baker() {
}

void Baker::bake_and_box(ORDER &anOrder) {
	int numDonuts;
	int currentBox;
	numDonuts = anOrder.number_donuts;
	while (numDonuts > 0) {
		if (numDonuts >= 12) {
			currentBox = 12;
		} else {
			currentBox = numDonuts;
		}
		Box filledBox;
		DONUT donut;
		for (int i = 0; i < currentBox; i++) {
			filledBox.addDonut(donut);
		}
		anOrder.boxes.push_back(filledBox);
		numDonuts = numDonuts - currentBox;
	}
	order_out_Vector.push_back(anOrder);
}

void Baker::beBaker() {
	ORDER nextOrder;
	unique_lock<mutex> lock(mutex_order_inQ);
	while (true) {
		cv_order_inQ.wait(lock);
		while (order_in_Q.size() > 0 || b_WaiterIsFinished) {
			if (order_in_Q.size() == 0 && b_WaiterIsFinished) {
				mutex_order_inQ.unlock();
				return;
			}
			nextOrder = order_in_Q.front();
			order_in_Q.pop();
			bake_and_box(nextOrder);
			mutex_order_inQ.unlock();
		}
	}
}
