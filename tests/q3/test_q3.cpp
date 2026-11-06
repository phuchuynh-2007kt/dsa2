#include "tests/TestUtils.h"
#include "BKMove.h"
#include <string>
#include <vector>

static void sample(BKMove& app) {
    CHECK(app.addStop({"S4", "Dai hoc Bach Khoa", "dai hoc bach khoa"}));
    CHECK(app.addStop({"S2", "Ben Xe Mien Tay", "ben xe mien tay"}));
    CHECK(app.addStop({"S1", "Ben Thanh", "ben thanh"}));
    CHECK(app.addStop({"S3", "Ben Thanh", "ben thanh"}));
    CHECK(app.addStop({"S5", "Ben xe Mien Dong", "ben xe mien dong"}));
}
int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q3 - Bus Stop Finder");
    suite.add("Empty directory", [] {
        BKMove app;
        CHECK(app.getStopCount() == 0);
        CHECK(app.findStops("", 0, 10).empty());
        StopInfo out{"sentinel", "keep", "keep"};
        CHECK(!app.findStop("missing", out) && out.stopId == "sentinel");
        CHECK(!app.removeStop("missing"));
    });
    suite.add("Invalid stop records are rejected", [] {
        BKMove app;
        CHECK(!app.addStop({"", "Bad", "bad"}));
        CHECK(!app.addStop({"S1", "", "bad"}));
        CHECK(!app.addStop({"S1", "Bad", ""}));
        CHECK(app.getStopCount() == 0);
    });
    suite.add("Add and find by stop ID", [] {
        BKMove app; sample(app);
        CHECK(app.getStopCount() == 5);
        StopInfo out;
        CHECK(app.findStop("S4", out));
        CHECK(out.stopId == "S4" && out.displayName == "Dai hoc Bach Khoa");
        CHECK(out.searchKey == "dai hoc bach khoa");
    });
    suite.add("Duplicate stop ID is not inserted", [] {
        BKMove app;
        CHECK(app.addStop({"S1", "First", "first"}));
        CHECK(!app.addStop({"S1", "Second", "second"}));
        CHECK(app.getStopCount() == 1);
        StopInfo out;
        CHECK(app.findStop("S1", out) && out.displayName == "First");
    });
    suite.add("Prefix search ordering and duplicate names", [] {
        BKMove app; sample(app);
        auto rows = app.findStops("ben", 0, 10);
        CHECK(rows.size() == 4);
        CHECK(rows[0].stopId == "S1" && rows[1].stopId == "S3");
        CHECK(rows[2].stopId == "S5" && rows[3].stopId == "S2");
    });
    suite.add("Arbitrary page and partial last page", [] {
        BKMove app; sample(app);
        auto rows = app.findStops("ben", 1, 2);
        CHECK(rows.size() == 2 && rows[0].stopId == "S5" && rows[1].stopId == "S2");
        rows = app.findStops("", 2, 2);
        CHECK(rows.size() == 1 && rows[0].stopId == "S4");
    });
    suite.add("Empty prefix matches all stops", [] {
        BKMove app; sample(app);
        auto rows = app.findStops("", 0, 10);
        CHECK(rows.size() == 5);
        CHECK(rows.front().stopId == "S1" && rows.back().stopId == "S4");
    });
    suite.add("Invalid pages and missing prefix", [] {
        BKMove app; sample(app);
        CHECK(app.findStops("ben", 99, 2).empty());
        CHECK(app.findStops("ben", -1, 2).empty());
        CHECK(app.findStops("ben", 0, 0).empty());
        CHECK(app.findStops("ben", 0, -2).empty());
        CHECK(app.findStops("missing", 0, 10).empty());
    });
    suite.add("Remove updates ID and name indexes", [] {
        BKMove app; sample(app);
        CHECK(app.removeStop("S1"));
        CHECK(!app.removeStop("S1"));
        CHECK(app.getStopCount() == 4);
        auto rows = app.findStops("ben thanh", 0, 10);
        CHECK(rows.size() == 1 && rows[0].stopId == "S3");
        StopInfo out{"sentinel", "keep", "keep"};
        CHECK(!app.findStop("S1", out) && out.stopId == "sentinel");
    });
    suite.add("Many stops, distant page and reinsert", [] {
        BKMove app;
        for (int i = 0; i < 160; ++i) {
            const std::string id = "S" + std::to_string(1000 + i);
            CHECK(app.addStop({id, id, "stop"}));
        }
        CHECK(app.getStopCount() == 160);
        auto page = app.findStops("stop", 15, 10);
        CHECK(page.size() == 10 && page[0].stopId == "S1150" && page[9].stopId == "S1159");
        CHECK(app.removeStop("S1150"));
        CHECK(app.addStop({"S1150", "Readded", "stop"}));
        page = app.findStops("stop", 15, 10);
        CHECK(page.size() == 10 && page[0].stopId == "S1150");
    });
    return suite.run(argc, argv);
}
