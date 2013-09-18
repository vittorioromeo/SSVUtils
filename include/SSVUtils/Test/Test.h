// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// Adaptation of lest: https://github.com/martinmoene/lest

#ifndef SSVU_TEST
#define SSVU_TEST

#include <string>
#include <vector>
#include <stdexcept>
#include "SSVUtils/Global/Typedefs.h"
#include "SSVUtils/Log/Log.h"

#define EXPECT(expr) \
	for(;;) \
	{ \
		namespace stInternal = ssvu::Test::Internal; \
		try { if(!stInternal::suppress(expr)) throw stInternal::Fail{stInternal::Location{__FILE__, __LINE__}, #expr}; } \
		catch(const stInternal::Fail&) { throw; } \
		catch(const std::exception& e) { throw stInternal::Unexpect{stInternal::Location{__FILE__, __LINE__}, #expr, stInternal::getWithMsg(e.what())}; } \
		catch(...) { throw stInternal::Unexpect{stInternal::Location{__FILE__, __LINE__}, #expr, "of unknown type"}; } \
		break; \
	}

#define EXPECT_THROWS(expr) \
	for(;;) \
	{ \
		namespace stInternal = ssvu::Test::Internal; \
		try { stInternal::suppress(expr); } catch(...) { break; } \
		throw stInternal::Expect{stInternal::Location{__FILE__, __LINE__}, #expr}; \
	}

#define EXPECT_THROWS_AS(expr, excpt) \
	for(;;) \
	{ \
		namespace stInternal = ssvu::Test::Internal; \
		try { stInternal::suppress(expr); } catch(excpt&) { break; } catch(...) { } \
		throw stInternal::Expect{stInternal::Location{__FILE__, __LINE__}, #expr, stInternal::getOfType(#excpt)}; \
	}

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

//#define SSVU_TEST_DISABLE

#ifndef SSVU_TEST_DISABLE
	#define SSVU_TEST(name) static ssvu::Test::Internal::Runner TOKENPASTE2(Unique_, __LINE__) { []{ ssvu::Test::Internal::getTestGroups().push_back({ {name, []
	#define SSVU_TEST_END() }});}};
	#define SSVU_TEST_RUN_ALL() ssvu::Test::Internal::runAllTests()
#else
	#define SSVU_TEST(name) struct TOKENPASTE2(Unique_, __LINE__) { void f() __attribute__ ((unused)) {
	#define SSVU_TEST_END() }} __attribute__ ((unused));
	#define SSVU_TEST_RUN_ALL()
#endif

namespace ssvu
{
	namespace Test
	{
		namespace Internal
		{
			struct Test { const std::string name; const Action action; };
			struct Location
			{
				const std::string file;
				const int line;
				Location(std::string mFile, int mLine) : file{std::move(mFile)}, line{mLine} { }
			};
			struct Comment
			{
				const std::string text;
				Comment(std::string mText) : text{std::move(mText)} { }
				inline operator bool() const { return !text.empty(); }
			};

			struct Msg : std::runtime_error
			{
				const std::string type;
				const Location location;
				const Comment comment;
				Msg(std::string mType, Location mLocation, std::string mExpr, std::string mNote = "")
					: std::runtime_error{std::move(mExpr)}, type{std::move(mType)}, location{std::move(mLocation)}, comment{std::move(mNote)} { }
			};
			struct Fail : Msg		{ Fail(Location mLocation, std::string mExpr) :									Msg{"fail", std::move(mLocation), std::move(mExpr)} { } };
			struct Expect : Msg		{ Expect(Location mLocation, std::string mExpr, std::string mException = "") :	Msg{"fail: no wanted exception", std::move(mLocation), std::move(mExpr), std::move(mException)} { } };
			struct Unexpect : Msg	{ Unexpect(Location mLocation, std::string mExpr, std::string mNote) :			Msg{"unwanted exception:", std::move(mLocation), std::move(mExpr), std::move(mNote)} { } };

			inline bool suppress(bool mValue) { return mValue; }

			inline std::string getWithMsg(std::string mStr)									{ return "with msg \"" + std::move(mStr) + "\""; }
			inline std::string getOfType(std::string mStr)									{ return "of type " + std::move(mStr); }
			inline std::ostream& operator<<(std::ostream& mOs, const Comment& mComment)		{ return mOs << (mComment ? " " + mComment.text : ""); }
			inline std::ostream& operator<<(std::ostream& mOs, const Location& mLocation)	{ return mOs << mLocation.file << ":" << mLocation.line; }

			inline void report(const Msg& mMsg, std::string mTest) { ssvu::lo("ssvu::Test") << mMsg.location << std::endl << mMsg.type << mMsg.comment << ": " << std::move(mTest) << ": " << mMsg.what() << std::endl << std::endl; }
			inline void report(const std::string& mError, std::string mTest) { ssvu::lo("ssvu::Test") << mError << std::endl << ": " << std::move(mTest) << ": " << std::endl << std::endl; }

			inline int run(const std::vector<Test>& mTests)
			{
				int fails{0};

				for(auto& t : mTests)
				{
					try { t.action(); }
					catch(const Msg& mMsg) { ++fails; report(mMsg, t.name); }
					catch(...) { ++fails; report("unexpected exception", t.name); }
				}

				if(fails > 0) ssvu::lo("ssvu::Test") << "[" << fails << "/" << mTests.size() << "] " << "tests failed" << std::endl << std::endl;
				return fails;
			}

			static std::vector<std::vector<Test>>& getTestGroups() { static std::vector<std::vector<Test>> testGroups; return testGroups; }
			struct Runner
			{
				#ifndef SSVU_TEST_DISABLE
					template<typename T> Runner(const T& mAction) { mAction(); }
				#endif
			};

			inline void runAllTests()
			{
				#ifndef SSVU_TEST_DISABLE
					static bool done{false};
					if(done) return;
					bool fail{false};
					for(auto& tg : Internal::getTestGroups())
						if(Internal::run(tg) > 0)
						{
							fail = true;
							ssvu::lo("############################################################################################################") << std::endl << std::endl;
						}
					done = true;
					if(!fail) ssvu::lo("ssvu::Test") << "All tests passed!" << std::endl;
				#endif
			}
		}
	}
}

#endif
