#include <iostream>
#include "cassert"
#include "file.hpp"

void setTest(){
    copy_file("dop.gif", "test.gif");
    CustomFileStream file("../test.gif",
                          std::ios::in | std::ios::out | std::ios::binary);
    file.set(1,1,3,3, 0);
    std::vector<std::string> a {"2","1","0","1","0","1","0","1","2"};
    for (int i = 0; i < a.size(); ++i) {
        assert(a[i] == file.pixels[i]);
    }
    copy_file("dop.gif", "test.gif");
    file.set(1,1,2,2, 0);
    a = {"1","0","0","1"};
    for (int i = 0; i < a.size(); ++i) {
        assert(a[i] == file.pixels[i]);
    }
    copy_file("dop.gif", "test.gif");
    a = {"0","1","1","0"};
    for (int i = 0; i < a.size(); ++i) {
        assert(a[i] != file.pixels[i]);
    }
    file.close();
}

void flipTest(){
    copy_file("dop.gif", "test.gif");
    CustomFileStream file("../test.gif",
                          std::ios::in | std::ios::out | std::ios::binary);
    file.flip("r");
    std::vector<std::string> a {"0","1","2","0","1","0","1","2","2","1","0","1","0","2","1","0"};
    for (int i = 0; i < a.size(); ++i) {
        assert(a[i] == file.pixels[i]);
    }
    copy_file("dop.gif", "test.gif");
    CustomFileStream file2("../test.gif",
                          std::ios::in | std::ios::out | std::ios::binary);
    file2.flip("l");
    a = {"0","1","2","0","1","0","1","2","2","1","0","1","0","2","1","0"};
    for (int i = 0; i < a.size(); ++i) {
        assert(a[i] == file2.pixels[i]);
    }
    copy_file("dop.gif", "test.gif");
    CustomFileStream file3("../test.gif",
                          std::ios::in | std::ios::out | std::ios::binary);
    file3.flip("l");
    file3.flip("l");
    a = {"0","2","1","0","2","1","0","1","1","0","1","2","0","1","2","0"};
    for (int i = 0; i < a.size(); ++i) {
        assert(a[i] == file3.pixels[i]);
    }
    file.close();
}

void joinTest(){
    copy_file("dop.gif", "test.gif");
    CustomFileStream file("../test.gif",
                          std::ios::in | std::ios::out | std::ios::binary);
    CustomFileStream dop("../in.gif",
                          std::ios::in | std::ios::out | std::ios::binary);
    file.join("r", "u", &dop);
    std::vector<std::string> a{"7" ,"7", "7", "7", "5", "5", "5", "5", "5", "4", "4", "4", "4", "4", "7", "7", "7", "7", "5", "5" ,"5" ,"5", "5", "4", "4" ,"4", "4", "4", "7", "7", "7", "7", "5", "5", "5", "5", "5", "4", "4", "4", "4", "4", "7", "7", "7", "7", "5", "5", "5", "3", "3", "3", "3", "4", "4", "4", "7", "7", "7", "7", "5", "5", "5", "3", "3", "3", "3", "4", "4", "4", "7", "7", "7", "7", "4", "4", "4", "3", "3", "3", "3", "5", "5", "5", "0", "2", "1", "0", "4", "4", "4", "3", "3", "3", "3", "5", "5", "5", "2", "1", "0", "1", "4", "4", "4", "4", "4", "5", "5", "5", "5", "5", "1", "0", "1", "2", "4", "4", "4", "4", "4", "5", "5", "5", "5", "5", "0", "1", "2", "0", "4", "4", "4", "4", "4", "5", "5", "5", "5", "5"};
    for (int i = 0; i < a.size(); ++i) {
        assert(a[i] == file.pixels[i]);
    }
    file.close();
}

void negativeTest(){
    copy_file("dop.gif", "test.gif");
    CustomFileStream file("../test.gif",
                          std::ios::in | std::ios::out | std::ios::binary);
    file.negative();
    std::vector<std::string> a{"0","2","1","0","2","1","0","1","1","0","1","2","0","1","2","0"};
    for (int i = 0; i < a.size(); ++i) {
        assert(a[i] == file.pixels[i]);
    }
    file.close();
}

void overlayTest(){
    copy_file("in.gif", "test.gif");
    CustomFileStream file("../test.gif",
                          std::ios::in | std::ios::out | std::ios::binary);
    CustomFileStream dop("../dop.gif",
                         std::ios::in | std::ios::out | std::ios::binary);
    file.overlay(2,2,&dop);
    std::vector<std::string> a{"1", "1", "1", "1", "1", "2", "2", "2", "2", "2", "1", "1", "1", "1", "1", "2", "2", "2", "2", "2", "1", "1", "1", "1", "1", "2", "2", "2", "2", "2", "1", "1", "1", "0", "0", "0", "0", "2", "2", "2", "1", "1", "1", "0", "0", "0", "0", "2", "2", "2", "2", "3", "5", "4", "3", "0", "0", "1", "1", "1", "2", "5", "4", "3", "4", "0", "0", "1", "1", "1", "2", "4", "3", "4", "5", "1", "1", "1", "1", "1", "2", "3", "4", "5", "3", "1", "1", "1", "1", "1", "2", "2", "2", "2", "2", "1", "1", "1", "1", "1"};
    for (int i = 0; i < a.size(); ++i) {
        assert(a[i] == file.pixels[i]);
    }
    file.close();
}

void init(){
    setTest();
    flipTest();
    joinTest();
    negativeTest();
    overlayTest();
}