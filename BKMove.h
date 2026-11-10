#ifndef BKMOVE_A2_H
#define BKMOVE_A2_H

#include <string>
#include <vector>
#include "AVL.h"

struct StopInfo {
    std::string stopId;
    std::string displayName;
    std::string searchKey;

    friend std::ostream& operator<<(std::ostream& os, const StopInfo& s) {
        os << s.stopId << ":" << s.displayName;
        return os;
    }
};

struct ArrivalEvent {
    std::string eventId;
    std::string stopId;
    std::string routeId;
    int arrivalTime;
};

class BKMove {
public:
    BKMove();
    ~BKMove();

    BKMove(const BKMove&) = delete;
    BKMove& operator=(const BKMove&) = delete;

    // Q3: Bus Stop Finder
    bool addStop(const StopInfo& stop);
    bool removeStop(const std::string& stopId);
    bool findStop(const std::string& stopId, StopInfo& out) const;
    int getStopCount() const;
    std::vector<StopInfo> findStops(
        const std::string& prefix, int page, int pageSize) const;

    // Q4: Live Arrival Board
    bool scheduleArrival(const ArrivalEvent& event);
    bool peekNextArrival(const std::string& stopId,
                         ArrivalEvent& out) const;
    bool processNextArrival(const std::string& stopId,
                            ArrivalEvent& out);
    bool rescheduleArrival(const std::string& eventId,
                           int newArrivalTime);
    bool cancelArrival(const std::string& eventId);
private:
    // Q3/Q4: Students propose their own private data members and helpers.
    // BKMove must manage registered stops and pending arrival events.
    // You may add classes/structs, extend BST/AVL/Heap, or use inheritance.
    // Keep the public API and the provided data-record fields unchanged.
    // All implementations must stay within the five permitted source files.
    struct stopKey
    {
        string searchKey;
        string stopId;
        bool operator<(const stopKey& other) const 
        {
            if(searchKey != other.searchKey) return searchKey < other.searchKey;
            return stopId < other.stopId;
        }
        bool operator>(const stopKey& other) const 
        {
            if (searchKey != other.searchKey) return searchKey > other.searchKey;
            return stopId > other.stopId;
        }
        bool operator==(const stopKey& other) const
        {
            return searchKey == other.searchKey && stopId == other.stopId;
        }
        friend std::ostream& operator<<(std::ostream& os, const stopKey& k) {
            os << k.searchKey << ", " << k.stopId;
            return os;
        }
    };

    AVL<std::string, StopInfo*> bkmoveStop;
    AVL<stopKey, StopInfo*> findingStop;
    int stopCount;

    static void identifyStops(
        const BST<stopKey, StopInfo*>::Node* root, const std::string& prefix, 
        int start, int end, int &curr, vector<StopInfo>& result);
};

#endif  // BKMOVE_A2_H
