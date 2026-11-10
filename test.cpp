#include "test.h"

// Q3 + Q4: implement the public methods below.
// BKMove.h intentionally leaves the private section unspecified: design your
// own member variables and helpers for registered stops and pending arrivals.
// You may add classes and extend BST/AVL/Heap within the permitted files.
// Do not change the public API or the supplied StopInfo/ArrivalEvent fields.
// Do not use global storage shared by unrelated BKMove objects.

BKMove::BKMove() : stopCount(0) {};
BKMove::~BKMove() = default;

// Q3 -- Bus Stop Finder
bool BKMove::addStop(const StopInfo& stop) {
    if(stop.stopId.empty() || stop.displayName.empty() || stop.searchKey.empty())
        return false;
    
    bool found = false;
    bkmoveStop.search(stop.stopId, found);
    
    if(found) return false;

    bkmoveStop.add(stop.stopId, new StopInfo(stop));
    findingStop.add(stopKey{stop.searchKey, stop.stopId}, new StopInfo(stop));
    ++stopCount;
    return true;
}

bool BKMove::removeStop(const std::string& stopId) {
    bool found = false;
    StopInfo* stopInf = bkmoveStop.search(stopId, found); //pending for arrival time

    if(!found || stopInf == nullptr) return false;

    std::string sKey = stopInf->searchKey;
    StopInfo* p1 = bkmoveStop.remove(stopId);
    if(p1 != nullptr) delete p1;

    StopInfo* p2 = findingStop.remove(stopKey{sKey, stopId});
    if(p2 != nullptr) delete p2;

    --stopCount;
    return true;
}

bool BKMove::findStop(const std::string& stopId, StopInfo& out) const {
    bool found = false;
    StopInfo* retStop = bkmoveStop.search(stopId, found);
    if(found && retStop != nullptr) {
        out = *retStop;
        return true;
    }
    return false;
}

int BKMove::getStopCount() const {
    return stopCount;
}

void BKMove::collectStops(
    const BST<stopKey, StopInfo*>::Node* node,
    const std::string& prefix,
    int startIdx,
    int endIdx,
    int& currentMatch,
    std::vector<StopInfo>& result) {
    if (node == nullptr || (int)result.size() >= (endIdx - startIdx))
        return;

    const std::string& key = node->data.key.searchKey;
    bool startsWithPrefix = (key.rfind(prefix, 0) == 0);

    // If key >= prefix, left subtree might have prefix matches
    if (key >= prefix) {
        collectStops(node->pLeft, prefix, startIdx, endIdx, currentMatch, result);
    }

    if ((int)result.size() >= (endIdx - startIdx))
        return;

    if (startsWithPrefix) {
        if (currentMatch >= startIdx && currentMatch < endIdx) {
            if (node->data.value != nullptr) {
                result.push_back(*(node->data.value));
            }
        }
        currentMatch++;
    }

    if ((int)result.size() >= (endIdx - startIdx))
        return;

    // If key starts with prefix OR key < prefix, right subtree might have prefix matches
    if (startsWithPrefix || key < prefix) {
        collectStops(node->pRight, prefix, startIdx, endIdx, currentMatch, result);
    }
}

std::vector<StopInfo> BKMove::findStops(
    const std::string& prefix, int page, int pageSize) const {
    if (page < 0 || pageSize <= 0)
        return {};

    std::vector<StopInfo> result;
    int startIdx = page * pageSize;
    int endIdx = startIdx + pageSize;
    int currentMatch = 0;

    collectStops(findingStop.getRoot(), prefix, startIdx, endIdx, currentMatch, result);
    return result;
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
