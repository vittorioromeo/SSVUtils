#include <SSVUtils/Core/Core.hpp>

using namespace std;
using namespace ssvu;

constexpr SizeT argCount{128};
constexpr SizeT arithCount{256};
constexpr SizeT foreachCount{128};
constexpr SizeT tplCount{64};

std::ostringstream output;

void mkFill(SizeT i)
{
	output << "#define SSVPP_IMPL_TPL_FILL_" << i << "(m0)";
	
	if(i != 0) output << "	SSVPP_TPL_EXPLODE(m0),	";
	else output << "				";

	for(auto j(0u); j < tplCount - i - 1; ++j) output << "0, ";
	output << "0\n";
}

void mkFillReverse(SizeT i)
{
	output << "#define SSVPP_IMPL_TPL_FILL_REVERSE_" << i << "(m0)	";
	
	for(auto j(0u); j < tplCount - i - 1; ++j) output << "0, ";
	output << "0";
	
	if(i != 0) output << ", SSVPP_TPL_EXPLODE(m0)";
	
	output << "\n";
}

void mkTupleElem(SizeT i)
{
	output << "#define SSVPP_IMPL_TPL_ELEM_" << i << "(";
	
	for(auto j(0u); j < tplCount - 1; ++j) output << "m" << j << ", ";
	output << "m" << tplCount - 1 << ") m" << i << "\n";
}

void mkTupleElemReverse(SizeT i)
{
	output << "#define SSVPP_IMPL_TPL_ELEM_REVERSE_" << i << "(";
	
	for(auto j(0u); j < tplCount - 1; ++j) output << "m" << j << ", ";
	output << "m" << tplCount - 1 << ") m" << tplCount - i - 1 << "\n";
}

void mkCat(SizeT i)
{
	output << "#define SSVPP_IMPL_TPL_CAT_" << i << "(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_" << i - 1 << "(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)\n";
}

void genTuple()
{
	for(auto i(0u); i < tplCount; ++i) mkFill(i);
	output << "#define SSVPP_IMPL_TPL_FILL_" << tplCount << "(m0)	SSVPP_TPL_EXPLODE(m0)\n";
	output << "\n";

	// tpl fill reverse
	for(auto i(0u); i < tplCount; ++i) mkFillReverse(i);
	output << "#define SSVPP_IMPL_TPL_FILL_REVERSE_" << tplCount << "(m0)	SSVPP_TPL_EXPLODE(m0)\n";
	output << "\n";

	// tpl elem
	for(auto i(0u); i < tplCount; ++i) mkTupleElem(i);	
	output << "\n";

	// tpl elem reverse
	for(auto i(0u); i < tplCount; ++i) mkTupleElemReverse(i);	
	output << "\n";

	// tpl cat
	output << "#define SSVPP_IMPL_TPL_CAT_0()			SSVPP_EMPTY()\n";
	output << "#define SSVPP_IMPL_TPL_CAT_1(m0)			m0\n";
	output << "#define SSVPP_IMPL_TPL_CAT_2(m0, m1)		SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1))\n";
	for(auto i(3u); i < tplCount * 2 + 1; ++i) mkCat(i);	
	output << "\n";

	output << "#define SSVPP_TPL_MAX_SIZE " << tplCount << "\n";
	output << "\n";
}

void genArithmetic()
{
	// decrement
	output << "#define SSVPP_IMPL_DECREMENT_0 0\n";
	output << "#define SSVPP_IMPL_DECREMENT_1 0\n";
	output << "#define SSVPP_IMPL_DECREMENT_2 1\n";
	for(auto i(3u); i < arithCount; ++i) output << "#define SSVPP_IMPL_DECREMENT_" << i << " " << i - 1 << "\n";
	output << "\n";

	// increment
	for(auto i(0u); i < arithCount - 1; ++i) output << "#define SSVPP_IMPL_INCREMENT_" << i << " " << i + 1 << "\n";
	output << "#define SSVPP_IMPL_INCREMENT_" << arithCount - 1 << " " << arithCount << "\n";
	output << "#define SSVPP_IMPL_INCREMENT_" << arithCount << " " << arithCount  << "\n";
	output << "\n";

	// bool
	output << "#define SSVPP_IMPL_BOOL_0 0\n";
	for(auto i(1u); i < arithCount; ++i) output << "#define SSVPP_IMPL_BOOL_" << i << " 1\n";
	output << "\n";
}

void genArgCount()
{
	// nseq
	output << "#define SSVPP_IMPL_NSEQ(	";
	for(auto i(0u); i < argCount - 1; ++i) output << "m" << i + 1 << ", ";
	output << "mN, ...) mN\n";
	output << "\n";

	// rseq
	output << "#define SSVPP_IMPL_RSEQ()	";
	for(auto i(argCount - 1); i > 0; --i) output << i << ", ";
	output << "0\n";

	// cseq
	output << "#define SSVPP_IMPL_CSEQ()	";
	for(auto i(argCount - 2); i > 0; --i) output << 1 << ", ";
	output << "0, 0\n";

	// cat
	output << "#define SSVPP_IMPL_CAT_0()			SSVPP_EMPTY()\n";
	output << "#define SSVPP_IMPL_CAT_1(m0)			SSVPP_EXPAND(m0)\n";
	output << "#define SSVPP_IMPL_CAT_2(m0, m1)		SSVPP_IMPL_CAT_1(SSVPP_TKNCAT_2(m0, m1))\n";
	for(auto i(3u); i < argCount; ++i) output << "#define SSVPP_IMPL_CAT_" << i << "(m0, m1, ...)	SSVPP_IMPL_CAT_" << i - 1 << "(SSVPP_TKNCAT_2(m0, m1), __VA_ARGS__)\n";
	output << "\n";
}

void genForeach()
{
	output << "#define SSVPP_IMPL_FOREACH_1(mAction, mData, mA0)				mAction(0, mData, mA0)\n";
	output << "#define SSVPP_IMPL_FOREACH_2(mAction, mData, mA0, mA1)			mAction(1, mData, mA0) SSVPP_IMPL_FOREACH_1(mAction, mData, mA1)\n";

	for(auto i(2u); i < foreachCount; ++i)
	{
		output << "#define SSVPP_IMPL_FOREACH_" << i + 1;
		output << "(mAction, mData, mA0, mA1, ...)		";
		output << "mAction(" << i << ", mData, mA0) SSVPP_IMPL_FOREACH_" << i << "(mAction, mData, mA1, __VA_ARGS__)";
		output << "\n";
	}
}

int main()
{
	output << 	"// Copyright (c) 2013-2014 Vittorio Romeo\n"
				"// License: Academic Free License (\"AFL\") v. 3.0\n"
				"// AFL License page: http://opensource.org/licenses/AFL-3.0\n"
				"// (auto-generated file)\n\n"
				"#ifndef SSVU_CORE_PREPROCESSOR_INTERNAL_GENERATED\n"
				"#define SSVU_CORE_PREPROCESSOR_INTERNAL_GENERATED\n\n";

	genArithmetic(); 
	output << "\n\n";
	
	genArgCount(); 
	output << "\n\n";	

	genTuple();
	output << "\n\n";

	genForeach();
	output << "\n";

	output << "#endif";

	std::cout << output.str() << std::endl;

	return 0;
}