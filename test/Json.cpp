// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Json/Json.hpp"
#include "./utils/test_utils.hpp"

SSVJ_CNV_NAMESPACE()
{
    struct __ssvjTestStruct
    {
        SSVJ_CNV_FRIEND();

        int f0{10};
        float f1{5.5f};
        std::vector<std::pair<float, double>> f2{
            {5.f, 10.}, {5.5f, 10.5}, {5.f, 10.}};
        std::string f3{"yo"};
        Tpl<std::string, int, int> f4{"hey", 5, 10};

        inline bool operator==(const __ssvjTestStruct& mT) const noexcept
        {
            return f0 == mT.f0 && f1 == mT.f1 && f2 == mT.f2 && f3 == mT.f3 &&
                   f4 == mT.f4;
        }
    };

    template <>
    SSVJ_CNV(__ssvjTestStruct, mV, mX)
    {
        ssvj::cnvArr(mV, mX.f0, mX.f1, mX.f2, mX.f3, mX.f4);
    }
    SSVJ_CNV_END()
}
SSVJ_CNV_NAMESPACE_END()

int main()
{


    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

#define EXEC_NUM_TESTS()                                                     \
    TEST_ASSERT_NS(n.as<char>() == char{10});                                \
    TEST_ASSERT_NS(n.as<int>() == 10);                                       \
    TEST_ASSERT_NS(n.as<long int>() == 10l);                                 \
    TEST_ASSERT_NS(n.as<unsigned char>() == ssvu::toNum<unsigned char>(10)); \
    TEST_ASSERT_NS(n.as<unsigned int>() == 10u);                             \
    TEST_ASSERT_NS(n.as<unsigned long int>() == 10ul);                       \
    TEST_ASSERT_NS(n.as<float>() == 10.f);                                   \
    TEST_ASSERT_NS(n.as<double>() == 10.);

        {
            Num n;

            n.set(10);
            TEST_ASSERT_NS(n.getRepr() == Num::Repr::IntS);
            EXEC_NUM_TESTS()

            n.set(10u);
            TEST_ASSERT_NS(n.getRepr() == Num::Repr::IntU);
            EXEC_NUM_TESTS()

            n.set(10.f);
            TEST_ASSERT_NS(n.getRepr() == Num::Repr::Real);
            EXEC_NUM_TESTS()
        }

#undef EXEC_NUM_TESTS

        Num ns{15}, nsu{15u}, nsul{15ul}, nf{15.f}, nd{15.};
        TEST_ASSERT_NS(ns == nsu);
        TEST_ASSERT_NS(nsu == nsul);
        TEST_ASSERT_NS(nsul == nf);
        TEST_ASSERT_NS(nf == nd);
    }

#define EXEC_TEST_BASIC_IMPL(mType, mVal, mRepr) \
    {                                            \
        Val v0, v1, v2;                          \
        v0 = mVal;                               \
        TEST_ASSERT_NS(v0.is<mRepr>());          \
        v1 = v0;                                 \
        TEST_ASSERT_NS(v1.is<mRepr>());          \
        TEST_ASSERT_NS(v0.as<Val>() == v0);      \
        TEST_ASSERT_NS(v0.as<mType>() == mVal);  \
        TEST_ASSERT_NS(v1.as<mType>() == mVal);  \
        TEST_ASSERT_NS(v0 == v1);                \
        v2 = mv(v1);                             \
        TEST_ASSERT_NS(v2.is<mRepr>());          \
        TEST_ASSERT_NS(v2.as<mType>() == mVal);  \
        v0 = Obj{};                              \
        v0["inner"] = v2;                        \
        TEST_ASSERT_NS(v0["inner"] == v2);       \
        auto sv0(v0.getWriteToStr());            \
        auto sv2(v2.getWriteToStr());            \
        auto osv0(fromStr(sv0));                 \
        auto osv2(fromStr(sv2));                 \
        TEST_ASSERT_NS(v0 == osv0);              \
        TEST_ASSERT_NS(v2 == osv2);              \
        TEST_ASSERT_NS(v0["inner"] == osv2);     \
        TEST_ASSERT_NS(osv0["inner"] == v2);     \
        TEST_ASSERT_NS(osv0["inner"] == osv2);   \
    }

#define EXEC_TEST_BASIC(mType, mVal) EXEC_TEST_BASIC_IMPL(mType, mVal, mType)

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

        EXEC_TEST_BASIC(bool, true)
        EXEC_TEST_BASIC(bool, false)
        EXEC_TEST_BASIC_IMPL(char, 'a', IntS)
        EXEC_TEST_BASIC_IMPL(int, 10, IntS)
        EXEC_TEST_BASIC_IMPL(long int, 10l, IntS)
    }

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

        EXEC_TEST_BASIC_IMPL(
            unsigned char, ssvu::toNum<unsigned char>('a'), IntU)
        EXEC_TEST_BASIC_IMPL(unsigned int, 10u, IntU)
        EXEC_TEST_BASIC_IMPL(unsigned long int, 10ul, IntU)
        EXEC_TEST_BASIC_IMPL(float, 10.f, Real)
        EXEC_TEST_BASIC_IMPL(double, 10., Real)
    }

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

        EXEC_TEST_BASIC(Str, "hello")
        EXEC_TEST_BASIC(Str, "hello"s)

        {
            Arr a{"hello", "bye"s, 10, 15.f, 'u'};
            EXEC_TEST_BASIC(Arr, a)
        }

        {
            Obj o{{"hello", "bye"}, {"welcome"s, "goodbye"s}, {"banana", 15},
                {"best letter", 'v'}};
            EXEC_TEST_BASIC(Obj, o)
        }

        {
            using PP = std::pair<std::string, double>;
            PP p{"coolpair", 205.5};
            EXEC_TEST_BASIC(PP, p)
        }

        {
            using PP = std::pair<std::pair<int, std::string>,
                std::pair<float, double>>;
            PP p{std::pair<int, std::string>{10, "coolpair2"},
                std::pair<float, double>{15.f, 205.5}};
            EXEC_TEST_BASIC(PP, p)
        }

        {
            using PP = Tpl<int>;
            PP p{10};
            EXEC_TEST_BASIC(PP, p)
        }

        {
            using PP = Tpl<int, float, std::string>;
            PP p{10, 15.5f, "swag"s};
            EXEC_TEST_BASIC(PP, p)
        }

        {
            using PP = Tpl<int, float, std::string, int, float>;
            PP p{10, 15.5f, "more swag"s, 22, 0.f};
            EXEC_TEST_BASIC(PP, p)
        }

        {
            using PP = Tpl<int, float, std::string, int, std::pair<double, int>,
                float, Tpl<int, int, int>>;
            PP p{10, 15.5f, "ULTRA SWAG"s, 22, std::pair<double, int>{10.5, 99},
                0.f, Tpl<int, int, int>{0, 1, 2}};
            EXEC_TEST_BASIC(PP, p)
        }
    }

// Equality with C-style arrays doesn't work
#define EXEC_TEST_C_ARRAY(mType, mVal)         \
    {                                          \
        Val v0, v1, v2;                        \
        v0 = mVal;                             \
        TEST_ASSERT_NS(v0.is<mType>());        \
        v1 = v0;                               \
        TEST_ASSERT_NS(v1.is<mType>());        \
        TEST_ASSERT_NS(v0 == v1);              \
        v2 = mv(v1);                           \
        TEST_ASSERT_NS(v2.is<mType>());        \
        v0 = Obj{};                            \
        v0["inner"] = v2;                      \
        TEST_ASSERT_NS(v0["inner"] == v2);     \
        auto sv0(v0.getWriteToStr());          \
        auto sv2(v2.getWriteToStr());          \
        auto osv0(fromStr(sv0));               \
        auto osv2(fromStr(sv2));               \
        TEST_ASSERT_NS(v0 == osv0);            \
        TEST_ASSERT_NS(v2 == osv2);            \
        TEST_ASSERT_NS(v0["inner"] == osv2);   \
        TEST_ASSERT_NS(osv0["inner"] == v2);   \
        TEST_ASSERT_NS(osv0["inner"] == osv2); \
        auto s = v0.getWriteToStr();           \
        auto fs = fromStr(s);                  \
        TEST_ASSERT_NS(fs == v0);              \
    }

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

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
            using PP = Tpl<std::vector<std::string>,
                std::pair<std::vector<int>, std::vector<float>>, int,
                std::vector<int>>;
            PP p{std::vector<std::string>{"10", "20", "35"},
                std::pair<std::vector<int>, std::vector<float>>{
                    std::vector<int>{1, 2}, std::vector<float>{1.f, 2.f}},
                20, std::vector<int>{}};
            EXEC_TEST_BASIC(PP, p)
        }
    }

#undef EXEC_TEST_BASIC
#undef EXEC_TEST_BASIC_IMPL
#undef EXEC_TEST_C_ARRAY

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

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

        auto v(fromStr(testSrc));

        TEST_ASSERT_NS_OP(v["a"], ==, 1);
        TEST_ASSERT_NS(v["b"] == Nll{});
        TEST_ASSERT_NS_OP(v["c"], ==, "//");
        TEST_ASSERT_NS_OP(v["d"], ==, true);
        TEST_ASSERT_NS_OP(v["e"], ==, "//\"//");
    }

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

        auto testSrc(R"(
    {
        "a": 15,
        "b": { "c": null },
        "c": ["x", "y", 10.5]
    }
    )");

        auto v(fromStr(testSrc));

        TEST_ASSERT_NS(v["a"] == 15);
        TEST_ASSERT_NS(v["b"]["c"] == Nll{});
        TEST_ASSERT_NS(v["c"][0] == "x");
        TEST_ASSERT_NS(v["c"][1] == "y");
        TEST_ASSERT_NS(v["c"][2] == 10.5);

        auto minified(v.getWriteToStr<WSMinified>());

        TEST_ASSERT_NS(minified ==
                       R"({"a":15,"b":{"c":null},"c":["x","y",10.5]})");
    }

#define EXEC_CV_TEST(mType, mBV)                    \
    {                                               \
        using Type = mType;                         \
        Val vIn;                                    \
        const Type& in = mBV;                       \
        cnv(vIn, in);                               \
        TEST_ASSERT_NS(vIn.as<Type>() == mBV);      \
        const Val& vOut{static_cast<Type>(mBV)};    \
        Type out;                                   \
        cnv(vOut, out);                             \
        TEST_ASSERT_NS(out == mBV);                 \
        auto s = vOut.getWriteToStr();              \
        auto fs = fromStr(s);                       \
        TEST_ASSERT_NS(fs == vOut);                 \
        TEST_ASSERT_NS(fs.as<Type>() == out);       \
        TEST_ASSERT_NS(fs.as<Type>() == mBV);       \
        Val eaVal;                                  \
        arch(eaVal, mBV);                           \
        TEST_ASSERT_NS(eaVal.as<Type>() == mBV);    \
        Type eaOut;                                 \
        extr(eaVal, eaOut);                         \
        TEST_ASSERT_NS(eaOut == mBV);               \
        auto getEaVal(getArch(mBV));                \
        TEST_ASSERT_NS(getEaVal.as<Type>() == mBV); \
        auto getEaOut(getExtr<Type>(getEaVal));     \
        TEST_ASSERT_NS(getEaOut == mBV);            \
    }

#define EXEC_CV_TEST_ARR(mType, mBV0, mBV1, mBV2)  \
    {                                              \
        using Type = mType;                        \
        Val vIn;                                   \
        const Type& in0 = mBV0;                    \
        const Type& in1 = mBV1;                    \
        const Type& in2 = mBV2;                    \
        cnvArr(vIn, in0, in1, in2);                \
        TEST_ASSERT_NS(vIn[0].as<Type>() == mBV0); \
        TEST_ASSERT_NS(vIn[1].as<Type>() == mBV1); \
        TEST_ASSERT_NS(vIn[2].as<Type>() == mBV2); \
        const Val& vOut(vIn);                      \
        Type out0, out1, out2;                     \
        cnvArr(vOut, out0, out1, out2);            \
        TEST_ASSERT_NS(out0 == mBV0);              \
        TEST_ASSERT_NS(out1 == mBV1);              \
        TEST_ASSERT_NS(out2 == mBV2);              \
        auto s = vOut.getWriteToStr();             \
        auto fs = fromStr(s);                      \
        TEST_ASSERT_NS(fs == vOut);                \
    }

#define EXEC_CV_TEST_OBJ(mType, mBV0, mBV1, mBV2)         \
    {                                                     \
        using Type = mType;                               \
        Val vIn;                                          \
        const Type& in0 = mBV0;                           \
        const Type& in1 = mBV1;                           \
        const Type& in2 = mBV2;                           \
        cnvObj(vIn, "k0", in0, "k1", in1, "k2", in2);     \
        TEST_ASSERT_NS(vIn["k0"].as<Type>() == mBV0);     \
        TEST_ASSERT_NS(vIn["k1"].as<Type>() == mBV1);     \
        TEST_ASSERT_NS(vIn["k2"].as<Type>() == mBV2);     \
        const Val& vOut(vIn);                             \
        Type out0, out1, out2;                            \
        cnvObj(vOut, "k0", out0, "k1", out1, "k2", out2); \
        TEST_ASSERT_NS(out0 == mBV0);                     \
        TEST_ASSERT_NS(out1 == mBV1);                     \
        TEST_ASSERT_NS(out2 == mBV2);                     \
        auto s = vOut.getWriteToStr();                    \
        auto fs = fromStr(s);                             \
        TEST_ASSERT_NS(fs == vOut);                       \
    }

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

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

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

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

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

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

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;


        // Simulation of converting a struct
        int f0{10};
        float f1{5.5f};
        std::vector<std::pair<float, double>> f2{
            {5.f, 10.}, {5.5f, 10.5}, {5.f, 10.}};
        std::string f3{"yo"};
        Tpl<std::string, int, int> f4{"hey", 5, 10};

        Val vIn;
        cnvObj(vIn, "f0", f0, "f1", f1, "f2", f2, "f3", f3, "f4", f4);
        TEST_ASSERT_NS(f0 == vIn["f0"].as<decltype(f0)>());
        TEST_ASSERT_NS(f1 == vIn["f1"].as<decltype(f1)>());
        TEST_ASSERT_NS(f2 == vIn["f2"].as<decltype(f2)>());
        TEST_ASSERT_NS(f3 == vIn["f3"].as<decltype(f3)>());
        TEST_ASSERT_NS(f4 == vIn["f4"].as<decltype(f4)>());
        const Val& vOut(vIn);
        decltype(f0) of0;
        decltype(f1) of1;
        decltype(f2) of2;
        decltype(f3) of3;
        decltype(f4) of4;
        cnvObj(vOut, "f0", of0, "f1", of1, "f2", of2, "f3", of3, "f4", of4);
        TEST_ASSERT_NS(f0 == of0);
        TEST_ASSERT_NS(f1 == of1);
        TEST_ASSERT_NS(f2 == of2);
        TEST_ASSERT_NS(f3 == of3);
        TEST_ASSERT_NS(f4 == of4);
    }



    {
        using namespace ssvj;
        using Type = ssvu::Json::Impl::__ssvjTestStruct;

        Type s1;
        Val k = s1;
        TEST_ASSERT_NS(k.as<Type>() == s1);
        Val k2 = k;
        TEST_ASSERT_NS(k == k2);
        TEST_ASSERT_NS(k2.as<Type>() == s1);
        TEST_ASSERT_NS(k.is<Type>());
        TEST_ASSERT_NS(k2.is<Type>());

        auto s = k.getWriteToStr();
        auto fs = fromStr(s);
        TEST_ASSERT_NS(fs == k);
        TEST_ASSERT_NS(fs == k2);
        TEST_ASSERT_NS(fs.as<Type>() == s1);
    }

#define EXEC_TEST_ITR_NUM_ARR(mType)                                \
    {                                                               \
        using TT = mType;                                           \
        TT acc = TT(0);                                             \
        Val v{Arr{}};                                               \
        v.emplace(TT(0));                                           \
        v.emplace(TT(1));                                           \
        v.emplace(TT(2));                                           \
        v.emplace(TT(3));                                           \
        v.emplace(TT(4));                                           \
        acc = 0;                                                    \
        for(const auto& i : v.forArr()) acc += i.as<TT>();          \
        TEST_ASSERT_NS_OP(acc, ==, 10);                             \
        acc = 0;                                                    \
        for(const auto& i : v.forArrAs<TT>()) acc += i;             \
        TEST_ASSERT_NS_OP(acc, ==, 10);                             \
        acc = 0;                                                    \
        for(const auto& i : v.forUncheckedArr()) acc += i.as<TT>(); \
        TEST_ASSERT_NS_OP(acc, ==, 10);                             \
        acc = 0;                                                    \
        for(const auto& i : v.forUncheckedArrAs<TT>()) acc += i;    \
        TEST_ASSERT_NS_OP(acc, ==, 10);                             \
    }

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

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
            TEST_ASSERT_NS_OP(acc, ==, "hi my name is meow");
        }
    }

#undef EXEC_TEST_ITR_NUM_ARR

#define EXEC_TEST_ITR_NUM_OBJ(mType)                   \
    {                                                  \
        using TT = mType;                              \
        std::string keyacc = "";                       \
        TT acc = TT(0);                                \
        Val v{Obj{}};                                  \
        v["0"] = TT(0);                                \
        v["1"] = TT(1);                                \
        v["2"] = TT(2);                                \
        v["3"] = TT(3);                                \
        v["4"] = TT(4);                                \
        keyacc = "";                                   \
        acc = 0;                                       \
        for(const auto& i : v.forObj())                \
        {                                              \
            keyacc += i.key;                           \
            acc += i.value.as<TT>();                   \
        }                                              \
        TEST_ASSERT_NS_OP(keyacc, ==, "01234");        \
        TEST_ASSERT_NS_OP(acc, ==, 10);                \
        keyacc = "";                                   \
        acc = 0;                                       \
        for(const auto& i : v.forObjAs<TT>())          \
        {                                              \
            keyacc += i.key;                           \
            acc += i.value;                            \
        }                                              \
        TEST_ASSERT_NS_OP(keyacc, ==, "01234");        \
        TEST_ASSERT_NS_OP(acc, ==, 10);                \
        keyacc = "";                                   \
        acc = 0;                                       \
        for(const auto& i : v.forUncheckedObj())       \
        {                                              \
            keyacc += i.key;                           \
            acc += i.value.as<TT>();                   \
        }                                              \
        TEST_ASSERT_NS_OP(keyacc, ==, "01234");        \
        TEST_ASSERT_NS_OP(acc, ==, 10);                \
        keyacc = "";                                   \
        acc = 0;                                       \
        for(const auto& i : v.forUncheckedObjAs<TT>()) \
        {                                              \
            keyacc += i.key;                           \
            acc += i.value;                            \
        }                                              \
        TEST_ASSERT_NS_OP(keyacc, ==, "01234");        \
        TEST_ASSERT_NS_OP(acc, ==, 10);                \
    }

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

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

    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

        {
            Val v{Obj{}};
            auto k1 = v.getIfHas<int>("k", 10);
            TEST_ASSERT_NS_OP(k1, ==, 10);
            v["k"] = 5;
            k1 = v.getIfHas<int>("k", 10);
            TEST_ASSERT_NS_OP(k1, ==, 5);

            const int& def0(20);
            int def1(25);
            auto kyj = v.getIfHas<int>("kyj", def0);
            TEST_ASSERT_NS_OP(kyj, ==, def0);
            kyj = v.getIfHas<int>("kyj", def1);
            TEST_ASSERT_NS_OP(kyj, ==, def1);
        }

        {
            Val v{Arr{}};
            auto k1 = v.getIfHas<int>(0, 10);
            TEST_ASSERT_NS_OP(k1, ==, 10);
            v.emplace(5);
            k1 = v.getIfHas<int>(0, 10);
            TEST_ASSERT_NS_OP(k1, ==, 5);
        }
    }


    {
        using namespace ssvu;
        using namespace ssvu::Json;
        using namespace ssvu::Json::Impl;

        {
            using Bts = std::bitset<10>;
            Val v;
            Bts b{"1000101111"};

            v = b;
            TEST_ASSERT_NS_OP(v.as<Bts>(), ==, b);
            TEST_ASSERT_NS_OP(v.is<Bts>(), ==, true);
        }
    }
}