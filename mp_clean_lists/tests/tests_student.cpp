#include <catch2/catch_test_macros.hpp>
#include "tests_helper.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"

#include "List.h"

using namespace cs225;

// THESE TESTS ARE NOT GRADED AND ARE FOR YOU ONLY

TEST_CASE("Hello World", "") {
    std::string hello = "Hello World!";

    REQUIRE( hello == "Hello World!" );
}

TEST_CASE("StudentListIterator", "preIncrement") {
    List<int> myList;
    myList.insertBack(1);
    myList.insertBack(2);
    auto iter = myList.begin();

    REQUIRE( *++iter == 2 );
    REQUIRE( *iter == 2 );

    iter = myList.begin();

    REQUIRE( *iter++ == 1 );
    REQUIRE( *iter == 2 );
}

TEST_CASE("StudentListIteratorCool", "preIncrement") {
    List<int> myList;
    for (int i = 0; i < 10; i++) {
        myList.insertBack(i);
    }
    auto node = myList.begin();
    int index = 0;
    for (auto elem : myList) {
        REQUIRE(elem == index);
        index++;
    }
    REQUIRE(index == 10);
    // REQUIRE(node == --myList.end());
}

TEST_CASE("List::merge basic", "[part=2]") {
    // example from MP assignment
    List<int> list1;
    list1.insertBack(1);
    list1.insertBack(3);
    list1.insertBack(4);
    list1.insertBack(6);
    List<int> list2;
    list2.insertBack(2);
    list2.insertBack(5);
    list2.insertBack(7);

    list1.mergeWith(list2);
    
    stringstream s1;

    list1.print(s1);
    std::cout << s1.str() << std::endl;
    REQUIRE(list2.empty());

    // multiple consecutive list2 data elements
    List<int> list3;
    list3.insertBack(1);
    list3.insertBack(9);
    List<int> list4;
    list4.insertBack(2);
    list4.insertBack(5);
    list4.insertBack(7);

    list3.mergeWith(list4);
    
    stringstream s2;

    list3.print(s2);
    std::cout << s2.str() << std::endl;
    std::cout << "{1, 2, 5, 7, 9}" << std::endl;
    REQUIRE(list4.empty());

    // Trailing data in second list
    List<int> list5;
    list5.insertBack(1);
    list5.insertBack(3);
    List<int> list6;
    list6.insertBack(2);
    list6.insertBack(5);
    list6.insertBack(7);

    list5.mergeWith(list6);
    
    stringstream s3;

    list5.print(s3);
    std::cout << s3.str() << std::endl;
    std::cout << "{1, 2, 3, 5, 7}" << std::endl;
    REQUIRE(list6.empty());
    
    // Multiple equal values
    List<int> list7;
    list7.insertBack(1);
    list7.insertBack(2);
    list7.insertBack(2);
    list7.insertBack(3);
    List<int> list8;
    list8.insertBack(2);
    list8.insertBack(5);
    list8.insertBack(7);

    list7.mergeWith(list8);
    
    stringstream s4;

    list7.print(s4);
    std::cout << s4.str() << std::endl;
    std::cout << "{1, 2, 2, 2, 3, 5, 7}" << std::endl;
    REQUIRE(list8.empty());
}

TEST_CASE("List::merge insert second list node at beginning", "[part=2]") {
    List<int> list1;
    list1.insertBack(2);
    list1.insertBack(5);
    list1.insertBack(7);
    List<int> list2;
    list2.insertBack(1);
    list2.insertBack(3);
    list2.insertBack(4);
    list2.insertBack(6);


    list1.mergeWith(list2);
    
    stringstream s;

    list1.print(s);
    std::cout << s.str() << std::endl;
    std::cout << "{1, 2, 3, 4, 5, 6, 7}" << std::endl;
    REQUIRE(list2.empty());
}

TEST_CASE("List::sort simple case", "[part=2]") {
    List<int> list1;
    list1.insertBack(6);
    list1.insertBack(1);
    list1.insertBack(5);
    list1.insertBack(8);
    list1.insertBack(4);
    list1.insertBack(3);
    list1.insertBack(7);
    list1.insertBack(2);
    list1.insertBack(9);
    
    list1.sort();
    
    stringstream s;

    list1.print(s);
    std::cout << s.str() << std::endl;

    std::cout << "{1, 2, 3, 4, 5, 6, 7, 8, 9}" << std::endl;
}