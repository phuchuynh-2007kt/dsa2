#include "tests/TestUtils.h"
#include "BKMove.h"

static void stops(BKMove& app) {
    CHECK(app.addStop({"S1", "Bach Khoa", "bach khoa"}));
    CHECK(app.addStop({"S2", "Ben Thanh", "ben thanh"}));
}
int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q4 - Live Arrival Board");
    suite.add("Empty arrival boards and output preservation", [] {
        BKMove app; stops(app);
        ArrivalEvent out{"sentinel", "keep", "keep", 777};
        CHECK(!app.peekNextArrival("S1", out));
        CHECK(!app.processNextArrival("S1", out));
        CHECK(!app.peekNextArrival("missing", out));
        CHECK(out.eventId == "sentinel" && out.arrivalTime == 777);
    });
    suite.add("Invalid arrival records are rejected", [] {
        BKMove app; stops(app);
        CHECK(!app.scheduleArrival({"", "S1", "08", 10}));
        CHECK(!app.scheduleArrival({"X", "missing", "08", 10}));
        CHECK(!app.scheduleArrival({"X", "S1", "", 10}));
        CHECK(!app.scheduleArrival({"X", "S1", "08", -1}));
        ArrivalEvent out;
        CHECK(!app.peekNextArrival("S1", out));
    });
    suite.add("Schedule and peek do not consume arrivals", [] {
        BKMove app; stops(app);
        CHECK(app.scheduleArrival({"E1", "S1", "08", 10}));
        ArrivalEvent out;
        CHECK(app.peekNextArrival("S1", out) && out.eventId == "E1");
        CHECK(app.peekNextArrival("S1", out) && out.eventId == "E1");
        CHECK(out.stopId == "S1" && out.routeId == "08" && out.arrivalTime == 10);
    });
    suite.add("Same time uses event ID tie-break", [] {
        BKMove app; stops(app);
        CHECK(app.scheduleArrival({"E2", "S1", "08", 10}));
        CHECK(app.scheduleArrival({"E1", "S1", "19", 10}));
        ArrivalEvent out;
        CHECK(app.peekNextArrival("S1", out) && out.eventId == "E1");
        CHECK(app.processNextArrival("S1", out) && out.eventId == "E1");
        CHECK(app.processNextArrival("S1", out) && out.eventId == "E2");
    });
    suite.add("Arrival boards are isolated by stop", [] {
        BKMove app; stops(app);
        CHECK(app.scheduleArrival({"E1", "S1", "08", 30}));
        CHECK(app.scheduleArrival({"E2", "S2", "14", 1}));
        ArrivalEvent out;
        CHECK(app.peekNextArrival("S1", out) && out.eventId == "E1");
        CHECK(app.peekNextArrival("S2", out) && out.eventId == "E2");
    });
    suite.add("Pending event IDs are unique across stops", [] {
        BKMove app; stops(app);
        CHECK(app.scheduleArrival({"E", "S1", "08", 3}));
        CHECK(!app.scheduleArrival({"E", "S2", "14", 1}));
        ArrivalEvent out;
        CHECK(!app.peekNextArrival("S2", out));
        CHECK(app.peekNextArrival("S1", out) && out.routeId == "08");
    });
    suite.add("Reschedule earlier and later", [] {
        BKMove app; stops(app);
        CHECK(app.scheduleArrival({"E1", "S1", "08", 10}));
        CHECK(app.scheduleArrival({"E2", "S1", "19", 12}));
        ArrivalEvent out;
        CHECK(app.rescheduleArrival("E2", 5));
        CHECK(app.peekNextArrival("S1", out) && out.eventId == "E2" && out.arrivalTime == 5);
        CHECK(app.rescheduleArrival("E2", 20));
        CHECK(app.peekNextArrival("S1", out) && out.eventId == "E1");
    });
    suite.add("Invalid reschedule leaves board unchanged", [] {
        BKMove app; stops(app);
        CHECK(app.scheduleArrival({"E", "S1", "08", 10}));
        CHECK(!app.rescheduleArrival("missing", 3));
        CHECK(!app.rescheduleArrival("E", -5));
        ArrivalEvent out;
        CHECK(app.peekNextArrival("S1", out) && out.arrivalTime == 10);
    });
    suite.add("Cancel arbitrary event and preserve the rest", [] {
        BKMove app; stops(app);
        for (int i = 1; i <= 5; ++i) CHECK(app.scheduleArrival({"E" + std::to_string(i), "S1", "08", i}));
        CHECK(app.cancelArrival("E3"));
        CHECK(!app.cancelArrival("E3"));
        ArrivalEvent out;
        for (int i : {1, 2, 4, 5}) {
            CHECK(app.processNextArrival("S1", out));
            CHECK(out.eventId == "E" + std::to_string(i));
        }
        CHECK(!app.processNextArrival("S1", out));
    });
    suite.add("Process and cancel release event IDs for reuse", [] {
        BKMove app; stops(app);
        CHECK(app.scheduleArrival({"E1", "S1", "08", 10}));
        ArrivalEvent out;
        CHECK(app.processNextArrival("S1", out) && out.eventId == "E1");
        CHECK(app.scheduleArrival({"E1", "S2", "14", 7}));
        CHECK(app.cancelArrival("E1"));
        CHECK(app.scheduleArrival({"E1", "S1", "08", 6}));
    });
    suite.add("Cannot delete stop until pending arrivals finish", [] {
        BKMove app; stops(app);
        CHECK(app.scheduleArrival({"E1", "S1", "08", 10}));
        CHECK(app.scheduleArrival({"E2", "S1", "19", 15}));
        CHECK(app.scheduleArrival({"E3", "S2", "14", 12}));
        CHECK(!app.removeStop("S1"));
        ArrivalEvent out;
        CHECK(app.processNextArrival("S1", out) && out.eventId == "E1");
        CHECK(!app.removeStop("S1")); // E2 is still pending at S1
        CHECK(app.cancelArrival("E2"));
        CHECK(app.removeStop("S1"));
        CHECK(!app.scheduleArrival({"E4", "S1", "08", 12}));
        CHECK(app.peekNextArrival("S2", out) && out.eventId == "E3");
    });
    suite.add("Many arrivals, update and process in order", [] {
        BKMove app; stops(app);
        for (int i = 100; i >= 1; --i) {
            CHECK(app.scheduleArrival({"E" + std::to_string(1000 + i), "S1", "08", i}));
        }
        CHECK(app.rescheduleArrival("E1100", 0));
        ArrivalEvent out;
        CHECK(app.processNextArrival("S1", out) && out.eventId == "E1100" && out.arrivalTime == 0);
        for (int i = 1; i < 100; ++i) {
            CHECK(app.processNextArrival("S1", out) && out.arrivalTime == i);
        }
        CHECK(!app.processNextArrival("S1", out));
    });
    return suite.run(argc, argv);
}
