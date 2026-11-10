#include "BKMove.h"

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
    StopInfo* removePtr = bkmoveStop.remove(stopId);
    if(removePtr != nullptr) delete removePtr;

    StopInfo* p = findingStop.remove(stopKey{sKey, stopId});
    if(p != nullptr) delete p;

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

void BKMove::identifyStops(
    const BST<stopKey, StopInfo*>::Node* root, const string& prefix,
    int start, int end, int &curr, vector<StopInfo>& result, bool dropLeft) {
    if(root == nullptr || static_cast<int>(result.size()) >= (end - start))
        return;

    const std::string& key = root->data.key.searchKey;
    bool containPrefix = false;
    if(key.rfind(prefix, 0) != std::string::npos) containPrefix = true;

    if(key >= prefix) {
        int leftS = (root->pLeft == nullptr) ? 0 : root->pLeft->subtreeSize;

        if(dropLeft && containPrefix && (curr + leftS <= start))
            curr += leftS;
        else
            identifyStops(root->pLeft, prefix, start, end, curr, result, dropLeft);
            
    }
    if(static_cast<int>(result.size()) >= (end - start))
        return;

    if(containPrefix) {
        if(curr >= start && curr < end) {
            if(root->data.value != nullptr)
                result.push_back(*(root->data.value));
        }
        ++curr;
    }

    if(static_cast<int>(result.size()) >= (end - start))
        return;

    if(containPrefix || key < prefix)
        identifyStops(root->pRight, prefix, start, end, curr, result, (key >= prefix));
}

std::vector<StopInfo> BKMove::findStops(
    const std::string& prefix, int page, int pageSize) const {
    if(page < 0 || pageSize <= 0 || stopCount == 0) return {};

    long long startLL = 1LL * page * pageSize;
    if(startLL >= stopCount) return {};

    std::vector<StopInfo> result;
    int start = static_cast<int>(startLL);
    int end = start + pageSize;
    int curr = 0;

    identifyStops(findingStop.getRoot(), prefix, start, end, curr, result);
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
