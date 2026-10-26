#include "BKMove.h"

// Q3 + Q4: implement the public methods below.
// BKMove.h intentionally leaves the private section unspecified: design your
// own member variables and helpers for registered stops and pending arrivals.
// You may add classes and extend BST/AVL/Heap within the permitted files.
// Do not change the public API or the supplied StopInfo/ArrivalEvent fields.
// Do not use global storage shared by unrelated BKMove objects.

BKMove::BKMove() = default;
BKMove::~BKMove() = default;

// Q3 -- Bus Stop Finder
bool BKMove::addStop(const StopInfo& stop) {
    (void)stop;
    return false; // TODO Q3
}

bool BKMove::removeStop(const std::string& stopId) {
    (void)stopId;
    return false; // TODO Q3
}

bool BKMove::findStop(const std::string& stopId, StopInfo& out) const {
    (void)stopId; (void)out;
    return false; // TODO Q3; do not modify out on failure
}

int BKMove::getStopCount() const {
    return 0; // TODO Q3
}

std::vector<StopInfo> BKMove::findStops(
    const std::string& prefix, int page, int pageSize) const {
    (void)prefix; (void)page; (void)pageSize;
    return {}; // TODO Q3
}

// Q4 -- Live Arrival Board
bool BKMove::scheduleArrival(const ArrivalEvent& event) {
    (void)event;
    return false; // TODO Q4
}

bool BKMove::peekNextArrival(
    const std::string& stopId, ArrivalEvent& out) const {
    (void)stopId; (void)out;
    return false; // TODO Q4; do not modify out on failure
}

bool BKMove::processNextArrival(
    const std::string& stopId, ArrivalEvent& out) {
    (void)stopId; (void)out;
    return false; // TODO Q4; do not modify out on failure
}

bool BKMove::rescheduleArrival(
    const std::string& eventId, int newArrivalTime) {
    (void)eventId; (void)newArrivalTime;
    return false; // TODO Q4
}

bool BKMove::cancelArrival(const std::string& eventId) {
    (void)eventId;
    return false; // TODO Q4
}
