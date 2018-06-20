#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sysquery.hpp"


//Global?
sqlite::database db("sysmap.db");

TEST_CASE("Test if amount of hosts is right", "[Sysquery]"){
    const auto hostMap = getHostIDs(db);
    REQUIRE(hostMap.size() == 4);
}

TEST_CASE("Test if content of hostMap is right", "[Sysquery]"){
    auto hostMap = getHostIDs(db);
    /* std::cout << hostMap[1] << std::endl; */
    const std::vector<int> v1{1,2,3,4,5,6,7,8};
    const std::vector<int> v2{1,2,11,4,13,14,15,16};
    const std::vector<int> v3{1,2,19,4,5,22,23,24};
    const std::vector<int> v4{1,2,27,4,29,30,31,32};
    REQUIRE(hostMap[1] == v1);
    REQUIRE(hostMap[2] == v2);
    REQUIRE(hostMap[3] == v3);
    REQUIRE(hostMap[4] == v4);
}

TEST_CASE("Test if content of specialized hostMap is right", "[Sysquery]"){
    const std::vector<int> vec{1};
    auto hostMap = getHostIDs(db, {"nonexistent"});
    REQUIRE(hostMap.size() == 0);
    hostMap = getHostIDs(db, {"taurusi6487"});
    REQUIRE(hostMap.size() == 1);
    const std::vector<int> v1{1,2,3,4,5,6,7,8};
    REQUIRE(hostMap[1] == v1);
}

TEST_CASE("Second Test if content of specialized hostMap is right", "[Sysquery]"){
    const std::vector<int> vec{1};
    auto hostMap = getHostIDs(db, {"nonexistent", "taurusi6487"});
    REQUIRE(hostMap.size() == 1);
    hostMap = getHostIDs(db, {"taurusi6487", "taurusi6493"});
    REQUIRE(hostMap.size() == 2);
    const std::vector<int> v1{1,2,3,4,5,6,7,8};
    const std::vector<int> v2{1,2,27,4,29,30,31,32};
    REQUIRE(hostMap[1] == v1);
    REQUIRE(hostMap[4] == v2);
}

TEST_CASE("Test if returned EIDs of getEIDs(...) are right", "[Sysquery]"){
    auto eids = getEIDs(db);
    REQUIRE(eids.size() == 8);
    std::vector<int> vec{1,2,3,4,5,6,7,8};
    REQUIRE(eids == vec);
    eids = getEIDs(db, {"disks", "machineinfo", "partitions"});
    REQUIRE(eids.size() == 3);
    vec = {1,3,7};
    REQUIRE(eids == vec);
}

TEST_CASE("Test if returned JSON of queryToJSON(...) is right", "[Sysquery]"){
    auto hostMap = getHostIDs(db, {"nonexistent"});
    auto eids = getEIDs(db, {"nonexistent"});
    std::stringstream str, str2;
    queryToJSON(str, db, hostMap, eids);
    REQUIRE(str.str().length() == 2);

    hostMap = getHostIDs(db, {"taurusi6493"});
    eids = getEIDs(db, {"disks"});
    queryToJSON(str2, db, hostMap, eids);
    REQUIRE(str2.str().length() == 123);

    //hash of the string
    const size_t hashed_str = 14598195260041437121;
    REQUIRE(std::hash<std::string>{}(str2.str()) == hashed_str);
}

TEST_CASE("Test if returned JSON of many Hosts is right", "[Sysquery]"){
    auto hostMap = getHostIDs(db, {"taurusi6487", "taurusi6488", "taurusi6489", "taurusi6493"});
    auto eids = getEIDs(db, {"disks", "machineinfo", "memoryinfo"});
    std::stringstream str;
    queryToJSON(str, db, hostMap, eids);
    REQUIRE(str.str().length() == 3441);

    //hash of the string
    const size_t hashed_str = 439064883875412482;
    REQUIRE(std::hash<std::string>{}(str.str()) == hashed_str);
}
