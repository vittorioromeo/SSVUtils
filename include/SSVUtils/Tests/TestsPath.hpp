// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSPATH
#define SSVU_TESTS_TESTSPATH

SSVUT_TEST(PathTests)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::FileSystem;

	Path path{"/usr"};
	if(path.exists<ssvufs::Type::Folder>())
	{
		SSVUT_EXPECT_OP(path.getStr(), ==, "/usr/");
		SSVUT_EXPECT_OP(path.getFolderName(), ==, "usr");
	}

	path = "/usr.txt";
	SSVUT_EXPECT(path.getStr() == "/usr.txt");
	SSVUT_EXPECT(path.getFolderName() == "");

	path = "/usr.txt/banana/.log";
	SSVUT_EXPECT(path.getStr() == "/usr.txt/banana/.log");
	SSVUT_EXPECT(path.getParent().getFolderName() == "banana");
	SSVUT_EXPECT(path.getExtension() == "");
	SSVUT_EXPECT(path.getAllExtensions() == "");
	SSVUT_EXPECT(path.getFileName() == ".log");
	SSVUT_EXPECT(path.getFileNameNoExtensions() == ".log");

	path = "///////////usr//////";
	SSVUT_EXPECT(path.getStr() == "/usr/");
	SSVUT_EXPECT(path.getFolderName() == "usr");

	path = "/////\\//\\usr///\\//test//test2/////test4.png";
	SSVUT_EXPECT(path.getStr() == "/usr/test/test2/test4.png");
	SSVUT_EXPECT(path.getExtension() == ".png");
	SSVUT_EXPECT(path.getAllExtensions() == ".png");
	SSVUT_EXPECT(path.getFileName() == "test4.png");
	SSVUT_EXPECT(path.getFileNameNoExtensions() == "test4");

	path = "/////\\//\\usr///\\//test//test2/////test4.png.bak";
	SSVUT_EXPECT(path.getStr() == "/usr/test/test2/test4.png.bak");
	SSVUT_EXPECT(path.getExtension() == ".bak");
	SSVUT_EXPECT(path.getAllExtensions() == ".png.bak");
	SSVUT_EXPECT(path.getFileName() == "test4.png.bak");
	SSVUT_EXPECT(path.getFileNameNoExtensions() == "test4");
}

#endif
