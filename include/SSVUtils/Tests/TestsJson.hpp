// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_JSON
#define SSVU_TESTS_JSON

SSVUT_TEST(SSVUJsonNumTests)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	#define EXEC_NUM_TESTS() \
		SSVUT_EXPECT(n.as<char>() == char{10}); \
		SSVUT_EXPECT(n.as<int>() == 10); \
		SSVUT_EXPECT(n.as<long int>() == 10l); \
		SSVUT_EXPECT(n.as<unsigned char>() == (unsigned char){10}); \
		SSVUT_EXPECT(n.as<unsigned int>() == 10u); \
		SSVUT_EXPECT(n.as<unsigned long int>() == 10ul); \
		SSVUT_EXPECT(n.as<float>() == 10.f); \
		SSVUT_EXPECT(n.as<double>() == 10.);

	{
		Num n;

		n.set(10);
		SSVUT_EXPECT(n.getType() == Num::Type::IntS);
		EXEC_NUM_TESTS()

		n.set(10u);
		SSVUT_EXPECT(n.getType() == Num::Type::IntU);
		EXEC_NUM_TESTS()

		n.set(10.f);
		SSVUT_EXPECT(n.getType() == Num::Type::Real);
		EXEC_NUM_TESTS()
	}

	#undef EXEC_NUM_TESTS

	Num ns{15}, nsu{15u}, nsul{15ul}, nf{15.f}, nd{15.0};
	SSVUT_EXPECT(ns == nsu);
	SSVUT_EXPECT(nsu == nsul);
	SSVUT_EXPECT(nsul == nf);
	SSVUT_EXPECT(nf == nd);
}

SSVUT_TEST(SSVUJsonValTests)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	#define EXEC_TEST_BASIC(mType, mVal) \
		{ \
			Val v0, v1, v2; \
			v0 = mVal; \
			v1 = v0; \
			SSVUT_EXPECT(v0.as<Val>() == v0); \
			SSVUT_EXPECT(v0.as<mType>() == mVal); \
			SSVUT_EXPECT(v1.as<mType>() == mVal); \
			SSVUT_EXPECT(v0 == v1); \
			v2 = std::move(v1); \
			SSVUT_EXPECT(v2.as<mType>() == mVal); \
			v0 = Obj{}; \
			v0["inner"] = v2; \
			SSVUT_EXPECT(v0["inner"] == v2); \
			auto sv0(v0.getWriteToStr()); \
			auto sv2(v2.getWriteToStr()); \
			auto osv0(Val::fromStr(sv0)); \
			auto osv2(Val::fromStr(sv2)); \
			SSVUT_EXPECT(v0 == osv0); \
			SSVUT_EXPECT(v2 == osv2); \
			SSVUT_EXPECT(v0["inner"] == osv2); \
			SSVUT_EXPECT(osv0["inner"] == v2); \
			SSVUT_EXPECT(osv0["inner"] == osv2); \
		}

	EXEC_TEST_BASIC(bool, true)
	EXEC_TEST_BASIC(bool, false)
	EXEC_TEST_BASIC(char, 'a')
	EXEC_TEST_BASIC(int, 10)
	EXEC_TEST_BASIC(long int, 10l)
	EXEC_TEST_BASIC(unsigned char, 'a')
	EXEC_TEST_BASIC(unsigned int, 10u)
	EXEC_TEST_BASIC(unsigned long int, 10ul)
	EXEC_TEST_BASIC(float, 10.f)
	EXEC_TEST_BASIC(double, 10.)

	EXEC_TEST_BASIC(Str, "hello")
	EXEC_TEST_BASIC(Str, "hello"s)

	{
		Arr a{"hello", "bye"s, 10, 15.f, 'u'};
		EXEC_TEST_BASIC(Arr, a)
	}

	{
		Obj o{{"hello", "bye"}, {"welcome"s, "goodbye"s}, {"banana", 15}, {"best letter", 'v'}};
		EXEC_TEST_BASIC(Obj, o)
	}
}

SSVUT_TEST(SSVUJsonReadTests)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	auto testSrc(R"(
	{
		// Hello
		"a":1,//Comment // iko
		"b":null,//
		"c":"//",//
		"d":true,//////
		"e":"//\"//",//k
		"f"//a
		://a
		{//a
		 //a

				"oo"//a
		 ://g
		 [//a
		 1,//b // h
		 2,//h // ds
		 3//l
		 ]//g
		 ,//p
		 //p //pp p p
		 "2"://k
		 2
////////
		 ///s
   //s/s/

		}//a
	}
	)");

	auto v(Val::fromStr(testSrc));

	SSVUT_EXPECT(v["a"] == 1);
	SSVUT_EXPECT(v["b"] == Nll{});
	SSVUT_EXPECT(v["c"] == "//");
	SSVUT_EXPECT(v["d"] == true);
	SSVUT_EXPECT(v["e"] == "//\"//");
}

#endif

// TODO: iteration tests, missing tests (check all source), writing minified, etc
