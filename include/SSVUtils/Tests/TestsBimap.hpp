// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSBIMAP
#define SSVU_TESTS_TESTSBIMAP

SSVUT_TEST(BimapTests)
{
	ssvu::Bimap<int, std::string> sb;

	SSVUT_EXPECT(sb.empty());
	SSVUT_EXPECT(!sb.has(10));
	SSVUT_EXPECT(!sb.has("banana"));
	SSVUT_EXPECT(sb.count(10) == 0);
	SSVUT_EXPECT(sb.count("banana") == 0);

	sb.emplace(10, "banana");

	SSVUT_EXPECT(!sb.empty());
	SSVUT_EXPECT(sb.size() == 1);
	SSVUT_EXPECT(sb.has(10));
	SSVUT_EXPECT(sb.has("banana"));
	SSVUT_EXPECT(sb.count(10) == 1);
	SSVUT_EXPECT(sb.count("banana") == 1);
	SSVUT_EXPECT(sb.at(10) == "banana");
	SSVUT_EXPECT(sb[10] == "banana");
	SSVUT_EXPECT(sb.at("banana") == 10);
	SSVUT_EXPECT(sb["banana"] == 10);

	sb["banana"] = 25;

	SSVUT_EXPECT(!sb.empty());
	SSVUT_EXPECT(sb.size() == 1);
	SSVUT_EXPECT(!sb.has(10));
	SSVUT_EXPECT(sb.has(25));
	SSVUT_EXPECT(sb.has("banana"));
	SSVUT_EXPECT(sb.count(10) == 0);
	SSVUT_EXPECT(sb.count(25) == 1);
	SSVUT_EXPECT(sb.count("banana") == 1);
	SSVUT_EXPECT(sb.at(25) == "banana");
	SSVUT_EXPECT(sb[25] == "banana");
	SSVUT_EXPECT(sb.at("banana") == 25);
	SSVUT_EXPECT(sb["banana"] == 25);

	sb["banana"] = 15;
	sb[15] = "melon";
	sb.emplace(10, "cucumber");

	SSVUT_EXPECT(!sb.empty());
	SSVUT_EXPECT(sb.size() == 2);
	SSVUT_EXPECT(sb.has(10));
	SSVUT_EXPECT(sb.has(15));
	SSVUT_EXPECT(!sb.has(25));
	SSVUT_EXPECT(sb.has("melon"));
	SSVUT_EXPECT(sb.has("cucumber"));
	SSVUT_EXPECT(!sb.has("banana"));
	SSVUT_EXPECT(sb.count(10) == 1);
	SSVUT_EXPECT(sb.count(15) == 1);
	SSVUT_EXPECT(sb.count(25) == 0);
	SSVUT_EXPECT(sb.count("melon") == 1);
	SSVUT_EXPECT(sb.count("cucumber") == 1);
	SSVUT_EXPECT(sb.count("banana") == 0);
	SSVUT_EXPECT(sb.at(10) == "cucumber");
	SSVUT_EXPECT(sb[10] == "cucumber");
	SSVUT_EXPECT(sb.at("cucumber") == 10);
	SSVUT_EXPECT(sb["cucumber"] == 10);
	SSVUT_EXPECT(sb.at(15) == "melon");
	SSVUT_EXPECT(sb[15] == "melon");
	SSVUT_EXPECT(sb.at("melon") == 15);
	SSVUT_EXPECT(sb["melon"] == 15);

	sb.clear();

	SSVUT_EXPECT(sb.empty());
	SSVUT_EXPECT(sb.size() == 0);
	SSVUT_EXPECT(!sb.has(10));
	SSVUT_EXPECT(!sb.has(15));
	SSVUT_EXPECT(!sb.has(25));
	SSVUT_EXPECT(!sb.has("melon"));
	SSVUT_EXPECT(!sb.has("cucumber"));
	SSVUT_EXPECT(!sb.has("banana"));

	sb["yolo"] = 15;

	SSVUT_EXPECT(!sb.empty());
	SSVUT_EXPECT(sb.size() == 1);
	SSVUT_EXPECT(!sb.has(10));
	SSVUT_EXPECT(sb.has(15));
	SSVUT_EXPECT(!sb.has("melon"));
	SSVUT_EXPECT(sb.has("yolo"));
	SSVUT_EXPECT(sb["yolo"] == 15);

	sb["yolo"] = 25;

	SSVUT_EXPECT(sb.has("yolo"));
	SSVUT_EXPECT(!sb.has(15));
	SSVUT_EXPECT(sb.has(25));
	SSVUT_EXPECT(sb["yolo"] == 25);

	sb.erase("yolo");

	SSVUT_EXPECT(!sb.has("yolo"));
	SSVUT_EXPECT(!sb.has(15));
	SSVUT_EXPECT(!sb.has(25));
	SSVUT_EXPECT(sb.empty());
}

#endif
