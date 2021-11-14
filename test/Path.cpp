// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"


#include "./utils/test_utils.hpp"
int main()
{
    using namespace std;
    using namespace ssvu;
    using namespace ssvu::FileSystem;

    Path path{"/usr"};
    if(path.exists<ssvufs::Type::Folder>())
    {
        TEST_ASSERT_OP(path.getStr(), ==, "/usr/");
        TEST_ASSERT_OP(path.getFolderName(), ==, "usr");
    }

    path = "/usr.txt";
    TEST_ASSERT(path.getStr() == "/usr.txt");
    TEST_ASSERT(path.getFolderName() == "");

    path = "/usr.txt/banana/.log";
    TEST_ASSERT(path.getStr() == "/usr.txt/banana/.log");
    TEST_ASSERT(path.getParent().getFolderName() == "banana");
    TEST_ASSERT(path.getExtension() == "");
    TEST_ASSERT(path.getAllExtensions() == "");
    TEST_ASSERT(path.getFileName() == ".log");
    TEST_ASSERT(path.getFileNameNoExtensions() == ".log");

    path = "///////////usr//////";
    TEST_ASSERT(path.getStr() == "/usr/");
    TEST_ASSERT(path.getFolderName() == "usr");

    path = "/////\\//\\usr///\\//test//test2/////test4.png";
    TEST_ASSERT(path.getStr() == "/usr/test/test2/test4.png");
    TEST_ASSERT(path.getExtension() == ".png");
    TEST_ASSERT(path.getAllExtensions() == ".png");
    TEST_ASSERT(path.getFileName() == "test4.png");
    TEST_ASSERT(path.getFileNameNoExtensions() == "test4");

    path = "/////\\//\\usr///\\//test//test2/////test4.png.bak";
    TEST_ASSERT(path.getStr() == "/usr/test/test2/test4.png.bak");
    TEST_ASSERT(path.getExtension() == ".bak");
    TEST_ASSERT(path.getAllExtensions() == ".png.bak");
    TEST_ASSERT(path.getFileName() == "test4.png.bak");
    TEST_ASSERT(path.getFileNameNoExtensions() == "test4.png");
}
