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
		SSVUT_EXPECT(n.as<unsigned char>() == static_cast<unsigned char>(10)); \
		SSVUT_EXPECT(n.as<unsigned int>() == 10u); \
		SSVUT_EXPECT(n.as<unsigned long int>() == 10ul); \
		SSVUT_EXPECT(n.as<float>() == 10.f); \
		SSVUT_EXPECT(n.as<double>() == 10.);

	{
		Num n;

		n.set(10);
		SSVUT_EXPECT(n.getRepr() == Num::Repr::IntS);
		EXEC_NUM_TESTS()

		n.set(10u);
		SSVUT_EXPECT(n.getRepr() == Num::Repr::IntU);
		EXEC_NUM_TESTS()

		n.set(10.f);
		SSVUT_EXPECT(n.getRepr() == Num::Repr::Real);
		EXEC_NUM_TESTS()
	}

	#undef EXEC_NUM_TESTS

	Num ns{15}, nsu{15u}, nsul{15ul}, nf{15.f}, nd{15.};
	SSVUT_EXPECT(ns == nsu);
	SSVUT_EXPECT(nsu == nsul);
	SSVUT_EXPECT(nsul == nf);
	SSVUT_EXPECT(nf == nd);
}

#define EXEC_TEST_BASIC_IMPL(mType, mVal, mRepr) \
	{ \
		Val v0, v1, v2; \
		v0 = mVal; \
		SSVUT_EXPECT(v0.is<mRepr>()); \
		v1 = v0; \
		SSVUT_EXPECT(v1.is<mRepr>()); \
		SSVUT_EXPECT(v0.as<Val>() == v0); \
		SSVUT_EXPECT(v0.as<mType>() == mVal); \
		SSVUT_EXPECT(v1.as<mType>() == mVal); \
		SSVUT_EXPECT(v0 == v1); \
		v2 = std::move(v1); \
		SSVUT_EXPECT(v2.is<mRepr>()); \
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

#define EXEC_TEST_BASIC(mType, mVal) EXEC_TEST_BASIC_IMPL(mType, mVal, mType)

SSVUT_TEST(SSVUJsonValTests1)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	EXEC_TEST_BASIC(bool, true)
	EXEC_TEST_BASIC(bool, false)
	EXEC_TEST_BASIC_IMPL(char, 'a', IntS)
	EXEC_TEST_BASIC_IMPL(int, 10, IntS)
	EXEC_TEST_BASIC_IMPL(long int, 10l, IntS)
}

SSVUT_TEST(SSVUJsonValTests2)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	EXEC_TEST_BASIC_IMPL(unsigned char, static_cast<unsigned char>('a'), IntU)
	EXEC_TEST_BASIC_IMPL(unsigned int, 10u, IntU)
	EXEC_TEST_BASIC_IMPL(unsigned long int, 10ul, IntU)
	EXEC_TEST_BASIC_IMPL(float, 10.f, Real)
	EXEC_TEST_BASIC_IMPL(double, 10., Real)
}

SSVUT_TEST(SSVUJsonValTests3)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

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

	{
		using PP = std::pair<std::string, double>;
		PP p{"coolpair", 205.5};
		EXEC_TEST_BASIC(PP, p)
	}

	{
		using PP = std::pair<std::pair<int, std::string>, std::pair<float, double>>;
		PP p{std::pair<int, std::string>{10, "coolpair2"}, std::pair<float, double>{15.f, 205.5}};
		EXEC_TEST_BASIC(PP, p)
	}

	{
		using PP = std::tuple<int>;
		PP p{10};
		EXEC_TEST_BASIC(PP, p)
	}

	{
		using PP = std::tuple<int, float, std::string>;
		PP p{10, 15.5f, "swag"s};
		EXEC_TEST_BASIC(PP, p)
	}

	{
		using PP = std::tuple<int, float, std::string, int, float>;
		PP p{10, 15.5f, "more swag"s, 22, 0.f};
		EXEC_TEST_BASIC(PP, p)
	}

	{
		using PP = std::tuple<int, float, std::string, int, std::pair<double, int>, float, std::tuple<int, int, int>>;
		PP p{10, 15.5f, "ULTRA SWAG"s, 22, std::pair<double, int>{10.5, 99}, 0.f, std::tuple<int, int, int>{0, 1, 2}};
		EXEC_TEST_BASIC(PP, p)
	}
}

// Equality with C-style arrays doesn't work
#define EXEC_TEST_C_ARRAY(mType, mVal) \
{ \
	Val v0, v1, v2; \
	v0 = mVal; \
	SSVUT_EXPECT(v0.is<mType>()); \
	v1 = v0; \
	SSVUT_EXPECT(v1.is<mType>()); \
	SSVUT_EXPECT(v0 == v1); \
	v2 = std::move(v1); \
	SSVUT_EXPECT(v2.is<mType>()); \
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
	auto s = v0.getWriteToStr(); \
	auto fs = Val::fromStr(s); \
	SSVUT_EXPECT(fs == v0); \
}

SSVUT_TEST(SSVUJsonValTests4)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	{
		using PP = int[3];
		PP p{10, 20, 35};
		EXEC_TEST_C_ARRAY(PP, p)
	}

	{
		using PP = std::vector<int>;
		PP p{10, 20, 35};
		EXEC_TEST_BASIC(PP, p)
	}

	{
		using PP = std::vector<std::string>;
		PP p{"10", "20", "35"};
		EXEC_TEST_BASIC(PP, p)
	}

	{
		using PP = std::tuple<std::vector<std::string>, std::pair<std::vector<int>, std::vector<float>>, int, std::vector<int>>;
		PP p{std::vector<std::string>{"10", "20", "35"}, std::pair<std::vector<int>, std::vector<float>>{std::vector<int>{1, 2}, std::vector<float>{1.f, 2.f}}, 20, std::vector<int>{}};
		EXEC_TEST_BASIC(PP, p)
	}
}

#undef EXEC_TEST_BASIC
#undef EXEC_TEST_BASIC_IMPL
#undef EXEC_TEST_C_ARRAY

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

	SSVUT_EXPECT_OP(v["a"], ==, 1);
	SSVUT_EXPECT(v["b"] == Nll{});
	SSVUT_EXPECT_OP(v["c"], ==, "//");
	SSVUT_EXPECT_OP(v["d"], ==, true);
	SSVUT_EXPECT_OP(v["e"], ==, "//\"//");
}

SSVUT_TEST(SSVUJsonWriteTests)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	auto testSrc(R"(
	{
		"a": 15,
		"b": { "c": null },
		"c": ["x", "y", 10.5]
	}
	)");

	auto v(Val::fromStr(testSrc));

	SSVUT_EXPECT(v["a"] == 15);
	SSVUT_EXPECT(v["b"]["c"] == Nll{});
	SSVUT_EXPECT(v["c"][0] == "x");
	SSVUT_EXPECT(v["c"][1] == "y");
	SSVUT_EXPECT(v["c"][2] == 10.5);

	auto minified(v.getWriteToStr<WSMinified>());

	SSVUT_EXPECT(minified == R"({"a":15,"b":{"c":null},"c":["x","y",10.5]})");
}

#define EXEC_CV_TEST(mType, mBV) \
	{ \
		using Type = mType; \
		Val vIn; \
		const Type& in = mBV; \
		cnv(vIn, in); \
		SSVUT_EXPECT(vIn.as<Type>() == mBV); \
		const Val& vOut{static_cast<Type>(mBV)}; \
		Type out; \
		cnv(vOut, out); \
		SSVUT_EXPECT(out == mBV); \
		auto s = vOut.getWriteToStr(); \
		auto fs = Val::fromStr(s); \
		SSVUT_EXPECT(fs == vOut); \
		SSVUT_EXPECT(fs.as<Type>() == out); \
		SSVUT_EXPECT(fs.as<Type>() == mBV); \
		Val eaVal; \
		arch(eaVal, mBV); \
		SSVUT_EXPECT(eaVal.as<Type>() == mBV); \
		Type eaOut; \
		extr(eaVal, eaOut); \
		SSVUT_EXPECT(eaOut == mBV); \
		auto getEaVal(getArch(mBV)); \
		SSVUT_EXPECT(getEaVal.as<Type>() == mBV); \
		auto getEaOut(getExtr<Type>(getEaVal)); \
		SSVUT_EXPECT(getEaOut == mBV); \
	}

#define EXEC_CV_TEST_ARR(mType, mBV0, mBV1, mBV2) \
	{ \
		using Type = mType; \
		Val vIn; \
		const Type& in0 = mBV0; \
		const Type& in1 = mBV1; \
		const Type& in2 = mBV2; \
		cnvArr(vIn, in0, in1, in2); \
		SSVUT_EXPECT(vIn[0].as<Type>() == mBV0); \
		SSVUT_EXPECT(vIn[1].as<Type>() == mBV1); \
		SSVUT_EXPECT(vIn[2].as<Type>() == mBV2); \
		const Val& vOut(vIn); \
		Type out0, out1, out2; \
		cnvArr(vOut, out0, out1, out2); \
		SSVUT_EXPECT(out0 == mBV0); \
		SSVUT_EXPECT(out1 == mBV1); \
		SSVUT_EXPECT(out2 == mBV2); \
		auto s = vOut.getWriteToStr(); \
		auto fs = Val::fromStr(s); \
		SSVUT_EXPECT(fs == vOut); \
	}

#define EXEC_CV_TEST_OBJ(mType, mBV0, mBV1, mBV2) \
	{ \
		using Type = mType; \
		Val vIn; \
		const Type& in0 = mBV0; \
		const Type& in1 = mBV1; \
		const Type& in2 = mBV2; \
		cnvObj(vIn, "k0", in0, "k1", in1, "k2", in2); \
		SSVUT_EXPECT(vIn["k0"].as<Type>() == mBV0); \
		SSVUT_EXPECT(vIn["k1"].as<Type>() == mBV1); \
		SSVUT_EXPECT(vIn["k2"].as<Type>() == mBV2); \
		const Val& vOut(vIn); \
		Type out0, out1, out2; \
		cnvObj(vOut, "k0", out0, "k1", out1, "k2", out2); \
		SSVUT_EXPECT(out0 == mBV0); \
		SSVUT_EXPECT(out1 == mBV1); \
		SSVUT_EXPECT(out2 == mBV2); \
		auto s = vOut.getWriteToStr(); \
		auto fs = Val::fromStr(s); \
		SSVUT_EXPECT(fs == vOut); \
	}

SSVUT_TEST(SSVUJsonConvertTests1)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	EXEC_CV_TEST(bool, true)
	EXEC_CV_TEST(bool, false)
	EXEC_CV_TEST(char, 'a')
	EXEC_CV_TEST(int, 10)
	EXEC_CV_TEST(long int, 10l)
	EXEC_CV_TEST(unsigned char, 'a')
	EXEC_CV_TEST(unsigned int, 10u)
	EXEC_CV_TEST(unsigned long int, 10ul)
	EXEC_CV_TEST(float, 10.f)
	EXEC_CV_TEST(double, 10.)
}

SSVUT_TEST(SSVUJsonConvertTests2)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	EXEC_CV_TEST_ARR(bool, true, true, false)
	EXEC_CV_TEST_ARR(bool, false, false, true)
	EXEC_CV_TEST_ARR(char, 'a', 'b', 'c')
	EXEC_CV_TEST_ARR(int, 10, 20, 30)
	EXEC_CV_TEST_ARR(long int, 10l, 0l, 2l)
	EXEC_CV_TEST_ARR(unsigned char, 'a', 'b', 'x')
	EXEC_CV_TEST_ARR(unsigned int, 10u, 22u, 6553u)
	EXEC_CV_TEST_ARR(unsigned long int, 10ul, 22u, 314u)
	EXEC_CV_TEST_ARR(float, 10.f, 5.f, 0.f)
	EXEC_CV_TEST_ARR(double, 10., 5.5, 1.2)
}

SSVUT_TEST(SSVUJsonConvertTests3)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	EXEC_CV_TEST_OBJ(bool, true, true, false)
	EXEC_CV_TEST_OBJ(bool, false, false, true)
	EXEC_CV_TEST_OBJ(char, 'a', 'b', 'c')
	EXEC_CV_TEST_OBJ(int, 10, 20, 30)
	EXEC_CV_TEST_OBJ(long int, 10l, 0l, 2l)
	EXEC_CV_TEST_OBJ(unsigned char, 'a', 'b', 'x')
	EXEC_CV_TEST_OBJ(unsigned int, 10u, 22u, 6553u)
	EXEC_CV_TEST_OBJ(unsigned long int, 10ul, 22u, 314u)
	EXEC_CV_TEST_OBJ(float, 10.f, 5.f, 0.f)
	EXEC_CV_TEST_OBJ(double, 10., 5.5, 1.2)
}

#undef EXEC_CV_TEST
#undef EXEC_CV_TEST_ARR
#undef EXEC_CV_TEST_OBJ

SSVUT_TEST(SSVUJsonConvertTests4)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;


	// Simulation of converting a struct
	int f0{10};
	float f1{5.5f};
	std::vector<std::pair<float, double>> f2
	{
		{5.f, 10.}, {5.5f, 10.5}, {5.f, 10.}
	};
	std::string f3{"yo"};
	std::tuple<std::string, int, int> f4{"hey", 5, 10};

	Val vIn;
	cnvObj(vIn, "f0", f0, "f1", f1, "f2", f2, "f3", f3, "f4", f4);
	SSVUT_EXPECT(f0 == vIn["f0"].as<decltype(f0)>());
	SSVUT_EXPECT(f1 == vIn["f1"].as<decltype(f1)>());
	SSVUT_EXPECT(f2 == vIn["f2"].as<decltype(f2)>());
	SSVUT_EXPECT(f3 == vIn["f3"].as<decltype(f3)>());
	SSVUT_EXPECT(f4 == vIn["f4"].as<decltype(f4)>());
	const Val& vOut(vIn);
	decltype(f0) of0;
	decltype(f1) of1;
	decltype(f2) of2;
	decltype(f3) of3;
	decltype(f4) of4;
	cnvObj(vOut, "f0", of0, "f1", of1, "f2", of2, "f3", of3, "f4", of4);
	SSVUT_EXPECT(f0 == of0);
	SSVUT_EXPECT(f1 == of1);
	SSVUT_EXPECT(f2 == of2);
	SSVUT_EXPECT(f3 == of3);
	SSVUT_EXPECT(f4 == of4);
}

SSVJ_CNV_NAMESPACE()
{
	struct __ssvjTestStruct
	{
		SSVJ_CNV_FRIEND();

		int f0{10};
		float f1{5.5f};
		std::vector<std::pair<float, double>> f2
		{
			{5.f, 10.}, {5.5f, 10.5}, {5.f, 10.}
		};
		std::string f3{"yo"};
		std::tuple<std::string, int, int> f4{"hey", 5, 10};

		inline bool operator==(const __ssvjTestStruct& mT) const noexcept
		{
			return f0 == mT.f0
				&& f1 == mT.f1
				&& f2 == mT.f2
				&& f3 == mT.f3
				&& f4 == mT.f4;
		}
	};

	template<> SSVJ_CNV(__ssvjTestStruct, mV, mX)
	{
		ssvj::cnvArr(mV, mX.f0, mX.f1, mX.f2, mX.f3, mX.f4);
	}
	SSVJ_CNV_END()
}
SSVJ_CNV_NAMESPACE_END()

SSVUT_TEST(SSVUJsonCnvTest)
{
	using namespace ssvj;
	using Type = ssvu::Json::Internal::__ssvjTestStruct;

	Type s1;
	Val k = s1;
	SSVUT_EXPECT(k.as<Type>() == s1);
	Val k2 = k;
	SSVUT_EXPECT(k == k2);
	SSVUT_EXPECT(k2.as<Type>() == s1);
	SSVUT_EXPECT(k.is<Type>());
	SSVUT_EXPECT(k2.is<Type>());

	auto s = k.getWriteToStr();
	auto fs = Val::fromStr(s);
	SSVUT_EXPECT(fs == k);
	SSVUT_EXPECT(fs == k2);
	SSVUT_EXPECT(fs.as<Type>() == s1);
}

#define EXEC_TEST_ITR_NUM_ARR(mType) \
	{ \
		using TT = mType; \
		TT acc = TT(0); \
		Val v{Arr{}}; \
		v.emplace(TT(0)); \
		v.emplace(TT(1)); \
		v.emplace(TT(2)); \
		v.emplace(TT(3)); \
		v.emplace(TT(4)); \
		acc = 0; \
		for(const auto& i : v.forArr()) acc += i.as<TT>(); \
		SSVUT_EXPECT_OP(acc, ==, 10); \
		acc = 0; \
		for(const auto& i : v.forArrAs<TT>()) acc += i; \
		SSVUT_EXPECT_OP(acc, ==, 10); \
		acc = 0; \
		for(const auto& i : v.forUncheckedArr()) acc += i.as<TT>(); \
		SSVUT_EXPECT_OP(acc, ==, 10); \
		acc = 0; \
		for(const auto& i : v.forUncheckedArrAs<TT>()) acc += i; \
		SSVUT_EXPECT_OP(acc, ==, 10); \
	} \

SSVUT_TEST(SSVUJsonArrIterationTests)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	EXEC_TEST_ITR_NUM_ARR(char)
	EXEC_TEST_ITR_NUM_ARR(int)
	EXEC_TEST_ITR_NUM_ARR(long int)
	EXEC_TEST_ITR_NUM_ARR(unsigned char)
	EXEC_TEST_ITR_NUM_ARR(unsigned int)
	EXEC_TEST_ITR_NUM_ARR(unsigned long int)
	EXEC_TEST_ITR_NUM_ARR(float)
	EXEC_TEST_ITR_NUM_ARR(double)

	{
		using TT = std::string;
		Val v{Arr{}};
		v.emplace("hi");
		v.emplace(" my");
		v.emplace(" name");
		v.emplace(" is");
		v.emplace(" meow");
		TT acc;
		for(const auto& i : v.forArr()) acc += i.as<TT>();
		SSVUT_EXPECT_OP(acc, ==, "hi my name is meow");
	}
}

#undef EXEC_TEST_ITR_NUM_ARR

#define EXEC_TEST_ITR_NUM_OBJ(mType) \
	{ \
		using TT = mType; \
		std::string keyacc = ""; \
		TT acc = TT(0); \
		Val v{Obj{}}; \
		v["0"] = TT(0); \
		v["1"] = TT(1); \
		v["2"] = TT(2); \
		v["3"] = TT(3); \
		v["4"] = TT(4); \
		keyacc = ""; \
		acc = 0; \
		for(const auto& i : v.forObj()) \
		{ \
			keyacc += i.key; \
			acc += i.value.as<TT>(); \
		} \
		SSVUT_EXPECT_OP(keyacc, ==, "01234"); \
		SSVUT_EXPECT_OP(acc, ==, 10); \
		keyacc = ""; \
		acc = 0; \
		for(const auto& i : v.forObjAs<TT>()) \
		{ \
			keyacc += i.key; \
			acc += i.value; \
		} \
		SSVUT_EXPECT_OP(keyacc, ==, "01234"); \
		SSVUT_EXPECT_OP(acc, ==, 10); \
		keyacc = ""; \
		acc = 0; \
		for(const auto& i : v.forUncheckedObj()) \
		{ \
			keyacc += i.key; \
			acc += i.value.as<TT>(); \
		} \
		SSVUT_EXPECT_OP(keyacc, ==, "01234"); \
		SSVUT_EXPECT_OP(acc, ==, 10); \
		keyacc = ""; \
		acc = 0; \
		for(const auto& i : v.forUncheckedObjAs<TT>()) \
		{ \
			keyacc += i.key; \
			acc += i.value; \
		} \
		SSVUT_EXPECT_OP(keyacc, ==, "01234"); \
		SSVUT_EXPECT_OP(acc, ==, 10); \
	} \

SSVUT_TEST(SSVUJsonObjIterationTests)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	EXEC_TEST_ITR_NUM_OBJ(char)
	EXEC_TEST_ITR_NUM_OBJ(int)
	EXEC_TEST_ITR_NUM_OBJ(long int)
	EXEC_TEST_ITR_NUM_OBJ(unsigned char)
	EXEC_TEST_ITR_NUM_OBJ(unsigned int)
	EXEC_TEST_ITR_NUM_OBJ(unsigned long int)
	EXEC_TEST_ITR_NUM_OBJ(float)
	EXEC_TEST_ITR_NUM_OBJ(double)
}

#undef EXEC_TEST_ITR_NUM_OBJ

SSVUT_TEST(SSVUJsonGetIfHas)
{
	using namespace ssvu;
	using namespace ssvu::Json;
	using namespace ssvu::Json::Internal;

	{
		Val v{Obj{}};
		auto k1 = v.getIfHas<int>("k", 10);
		SSVUT_EXPECT_OP(k1, ==, 10);
		v["k"] = 5;
		k1 = v.getIfHas<int>("k", 10);
		SSVUT_EXPECT_OP(k1, ==, 5);

		const int& def0(20);
		int def1(25);
		auto kyj = v.getIfHas<int>("kyj", def0);
		SSVUT_EXPECT_OP(kyj, ==, def0);
		kyj = v.getIfHas<int>("kyj", def1);
		SSVUT_EXPECT_OP(kyj, ==, def1);
	}

	{
		Val v{Arr{}};
		auto k1 = v.getIfHas<int>(0, 10);
		SSVUT_EXPECT_OP(k1, ==, 10);
		v.emplace(5);
		k1 = v.getIfHas<int>(0, 10);
		SSVUT_EXPECT_OP(k1, ==, 5);
	}
}

#endif
