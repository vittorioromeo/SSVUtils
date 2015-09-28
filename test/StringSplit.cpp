// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"


#include "./utils/test_utils.hpp"
int main()
{
    using namespace std;
    using namespace ssvu;
    string s1{"test a b c d e"};
    string s2{"test,a,b,c,d,e"};
    string s3{"test##a##b##c##d##e"};

    auto sp1 = getSplit(s1, ' ');
    auto sp2 = getSplit(s2, ',');
    auto sp3 = getSplit(s3, "##");

    auto sps1 = getSplit(s1, " "s);
    auto sps2 = getSplit(s2, ","s);
    auto sps3 = getSplit(s3, "##"s);

    auto spks1 = getSplit<Split::TrailingSeparator>(s1, ' ');
    auto spks2 = getSplit<Split::TrailingSeparator>(s2, ',');
    auto spks3 = getSplit<Split::TrailingSeparator>(s3, "##");

    auto spksat1 = getSplit<Split::TokenizeSeparator>(s1, ' ');
    auto spksat2 = getSplit<Split::TokenizeSeparator>(s2, ',');
    auto spksat3 = getSplit<Split::TokenizeSeparator>(s3, "##");

    TEST_ASSERT((sp1.size() == sp2.size()) == (sp3.size() == 6));
    TEST_ASSERT((sps1.size() == sps2.size()) == (sps3.size() == 6));
    TEST_ASSERT((spks1.size() == spks2.size()) == (spks3.size() == 6));
    TEST_ASSERT((spksat1.size() == spksat2.size()) == (spksat3.size() == 11));

    TEST_ASSERT((sp1[0] == sp2[0]) == (sp3[0] == "test"));
    TEST_ASSERT((sp1[1] == sp2[1]) == (sp3[1] == "a"));
    TEST_ASSERT((sp1[2] == sp2[2]) == (sp3[2] == "b"));
    TEST_ASSERT((sp1[3] == sp2[3]) == (sp3[3] == "c"));
    TEST_ASSERT((sp1[4] == sp2[4]) == (sp3[4] == "d"));
    TEST_ASSERT((sp1[5] == sp2[5]) == (sp3[5] == "e"));

    TEST_ASSERT((sps1[0] == sps2[0]) == (sps3[0] == "test"));
    TEST_ASSERT((sps1[1] == sps2[1]) == (sps3[1] == "a"));
    TEST_ASSERT((sps1[2] == sps2[2]) == (sps3[2] == "b"));
    TEST_ASSERT((sps1[3] == sps2[3]) == (sps3[3] == "c"));
    TEST_ASSERT((sps1[4] == sps2[4]) == (sps3[4] == "d"));
    TEST_ASSERT((sps1[5] == sps2[5]) == (sps3[5] == "e"));

    TEST_ASSERT((spksat1[0] == spksat2[0]) == (spksat3[0] == "test"));
    TEST_ASSERT((spksat1[2] == spksat2[2]) == (spksat3[2] == "a"));
    TEST_ASSERT((spksat1[4] == spksat2[4]) == (spksat3[4] == "b"));
    TEST_ASSERT((spksat1[6] == spksat2[6]) == (spksat3[6] == "c"));
    TEST_ASSERT((spksat1[8] == spksat2[8]) == (spksat3[8] == "d"));
    TEST_ASSERT((spksat1[10] == spksat2[10]) == (spksat3[10] == "e"));

    TEST_ASSERT(spks1[2] == "b ");
    TEST_ASSERT(spks2[2] == "b,");
    TEST_ASSERT(spks3[2] == "b##");

    string s4{"test##a,b##c,d##efg"};
    vector<string> splits{"##", ","};

    auto spv1 = getSplit(s4, splits);
    auto spv2 = getSplit<Split::TrailingSeparator>(s4, splits);
    auto spv3 = getSplit<Split::TokenizeSeparator>(s4, splits);

    TEST_ASSERT(spv1.size() == 6);
    TEST_ASSERT(spv2.size() == 6);
    TEST_ASSERT(spv3.size() == 11);

    TEST_ASSERT((spv1[0] == "test") && spv2[0] == "test##");
    TEST_ASSERT((spv1[1] == "a") && spv2[1] == "a,");
    TEST_ASSERT((spv1[2] == "b") && spv2[2] == "b##");
    TEST_ASSERT((spv1[3] == "c") && spv2[3] == "c,");
    TEST_ASSERT((spv1[4] == "d") && spv2[4] == "d##");
    TEST_ASSERT((spv1[5] == "efg") && spv2[5] == "efg");

    TEST_ASSERT(spv3[0] == "test");
    TEST_ASSERT(spv3[1] == "##");
    TEST_ASSERT(spv3[2] == "a");
    TEST_ASSERT(spv3[3] == ",");
    TEST_ASSERT(spv3[4] == "b");
    TEST_ASSERT(spv3[5] == "##");
    TEST_ASSERT(spv3[6] == "c");
    TEST_ASSERT(spv3[7] == ",");
    TEST_ASSERT(spv3[8] == "d");
    TEST_ASSERT(spv3[9] == "##");
    TEST_ASSERT(spv3[10] == "efg");
}
