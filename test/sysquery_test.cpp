#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "sysquery.hpp"


//Global?
sqlite::database db("sysmap.db");

TEST_CASE("Test if amount of hosts is right", "[Sysquery]"){
    auto hostMap = getHostIDs(db);
    REQUIRE(hostMap.size() == 5);
}

TEST_CASE("Test if content of hostMap is right", "[Sysquery]"){
    auto hostMap = getHostIDs(db);
    /* std::cout << hostMap[1] << std::endl; */
    std::vector<int> v1{1,2,3,4};
    std::vector<int> v2{5,6,7,8};
    std::vector<int> v3{9,10,11,12};
    std::vector<int> v4{13,14,15,16};
    std::vector<int> v5{17,18,19,20};
    REQUIRE(hostMap[1] == v1);
    REQUIRE(hostMap[2] == v2);
    REQUIRE(hostMap[3] == v3);
    REQUIRE(hostMap[4] == v4);
    REQUIRE(hostMap[5] == v5);
}

TEST_CASE("Test if content of specialized hostMap is right", "[Sysquery]"){
    std::vector<int> vec{1};
    auto hostMap = getHostIDs(db, {"taurusi6469"});
    REQUIRE(hostMap.size() == 1);
    std::vector<int> v1{1,2,3,4};
    REQUIRE(hostMap[1] == v1);
}

TEST_CASE("Second Test if content of specialized hostMap is right", "[Sysquery]"){
    std::vector<int> vec{1};
    auto hostMap = getHostIDs(db, {"taurusi6469", "taurusi6473"});
    REQUIRE(hostMap.size() == 2);
    std::vector<int> v1{1,2,3,4};
    std::vector<int> v2{13,14,15,16};
    REQUIRE(hostMap[1] == v1);
    REQUIRE(hostMap[4] == v2);
}

TEST_CASE("Test if returned EIDs of getEIDs(...) are right", "[Sysquery]"){
    auto eids = getEIDs(db);
    REQUIRE(eids.size() == 4);
    std::vector<int> vec{1,2,3,4};
    REQUIRE(eids == vec);
    eids = getEIDs(db, {"disks", "machineinfo"});
    REQUIRE(eids.size() == 2);
    vec = {1,2};
    REQUIRE(eids == vec);
}

TEST_CASE("Test if returned JSON of queryToJSON(...) is right", "[Sysquery]"){
    auto hostMap = getHostIDs(db, {"taurusi6469"});
    auto eids = getEIDs(db, {"disks"});
    outwrapper out;
    std::stringstream str;
    queryToJSON(str, db, hostMap, eids);
    std::cout << str.str() << std::endl;
    REQUIRE(str.str().length() == 123);
    REQUIRE(str.str() ==  "{\"taurusi6469\":{\"disks\":[{\"device_number\":\"8:0\",\"model\":\"Micron_M510_MTFD\",\"name\":\"sda\",\"size\":250069680,\"vendor\":\"ATA\"}]}}");
}
