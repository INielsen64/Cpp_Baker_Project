#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>

#include "../includes/box.h"
#include "../includes/constants.h"
#include "../includes/waiter.h"
#include "../includes/baker.h"
#include "../includes/datastructs.h"
#include "../includes/PRINT.h"

using namespace std;

//*************************************************
//NOTE:  most of these globals are needed
//by both bakers and waiters, so be sure to include
//externs.h in appropriate cpp implementations
//*************************************************
//the mutexes
mutex mutex_order_inQ;
mutex mutex_order_outQ;

//the condition variable
condition_variable cv_order_inQ;

//when true its time for the baker to quit
bool b_WaiterIsFinished = false;	

//where orders are stored
queue<ORDER> order_in_Q;
vector<ORDER> order_out_Vector;

//used to make PRINT statements work properly
mutex printMutex;

//*************************************************
//runs waiter until orders all read and placed
//on order_in_Q then exits
void doWaiter(int id,string fn) {
	Waiter myWaiter(id,fn);
	myWaiter.beWaiter();
}

//takes orders from order_in_Q, processes
//them and then puts them on order_out_Vector
void doBaker(int id) {
	Baker myBaker(id);
	myBaker.beBaker();
}

//prints what is in order_out_Vector
//DO NOT CALL THIS WHEN MULTIPLE THREADS ARE ACCESSING order_out_Vector
void audit_results() {
	std::vector<ORDER>::iterator itOrder;
	std::vector<Box>::iterator itBox;

	int total_donuts = 0;
	int total_orders = 0;
	int total_boxes  = 0;

	//first go through every order
	for (itOrder = order_out_Vector.begin(); itOrder != order_out_Vector.end(); itOrder++) {
		int numDonuts = 0;
		total_orders++;

		//for each order go through all the boxes and add up all the donuts
		for (itBox = (itOrder->boxes).begin(); itBox != (itOrder->boxes).end(); ++itBox) {
			total_boxes++;
			numDonuts += itBox->size();
			total_donuts += itBox->size();
		}

		//if one order was screwed up say so
		if (numDonuts != itOrder->number_donuts) 
			PRINT6("ERROR Order", itOrder->order_number, " Expected ", itOrder->number_donuts, " found ", numDonuts);
	}

	PRINT2("Total donuts made   = ", total_donuts);
	PRINT2("Total number boxes  = ", total_boxes);
	PRINT2("Total orders filled = ", total_orders);
}

int main()
{
	thread baker1(doBaker, 0);
	thread baker2(doBaker, 1);
	thread waiter1(doWaiter, 0, "in3.txt");
	waiter1.join();
	baker1.join();
	baker2.join();
	cout << endl;
	cout << "Order summary:" << endl;
	for(int i = 0; i<order_out_Vector.size(); i++){
		cout << "Order Number: " << order_out_Vector[i].order_number << " Number of donuts: " << order_out_Vector[i].number_donuts << endl;
		for(int j = 0; j<order_out_Vector[i].boxes.size(); j++){
			cout << "Box size: " << order_out_Vector[i].boxes[j].size() << endl;
		}
	}
	return SUCCESS;
}
