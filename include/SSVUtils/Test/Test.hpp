// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// Adaptation of lest: https://github.com/martinmoene/lest

#ifndef SSVU_TEST
#define SSVU_TEST

#include <string>
#include <vector>
#include <stdexcept>
#include "SSVUtils/Global/Common.hpp"
#include "SSVUtils/Log/Log.hpp"
#include "SSVUtils/Utils/Macros.hpp"
#include "SSVUtils/Preprocessor/Preprocessor.hpp"

#define EXPECT(mExpr) \
	while(true) \
	{ \
		try { if(!ssvu::Test::Internal::suppress(mExpr)) throw ssvu::Test::Internal::Fail{ssvu::Test::Internal::Location{__FILE__, __LINE__}, #mExpr}; } \
		catch(const ssvu::Test::Internal::Fail&) { throw; } \
		catch(const std::exception& e) { throw ssvu::Test::Internal::Unexpect{ssvu::Test::Internal::Location{__FILE__, __LINE__}, #mExpr, ssvu::Test::Internal::getWithMsg(e.what())}; } \
		catch(...) { throw ssvu::Test::Internal::Unexpect{ssvu::Test::Internal::Location{__FILE__, __LINE__}, #mExpr, "of unknown type"}; } \
		break; \
	}

#define EXPECT_THROWS(mExpr) \
	while(true) \
	{ \
		try { ssvu::Test::Internal::suppress(mExpr); } catch(...) { break; } \
		throw ssvu::Test::Internal::Expect{ssvu::Test::Internal::Location{__FILE__, __LINE__}, #mExpr}; \
	}

#define EXPECT_THROWS_AS(mExpr, mException) \
	while(true) \
	{ \
		try { ssvu::Test::Internal::suppress(mExpr); } catch(mException&) { break; } catch(...) { } \
		throw ssvu::Test::Internal::Expect{ssvu::Test::Internal::Location{__FILE__, __LINE__}, #mExpr, ssvu::Test::Internal::getOfType(#mException)}; \
	}

#ifndef SSVU_TEST_DISABLE
	#define SSVU_TEST(name) static ssvu::Test::Internal::Runner SSVPP_CAT(Unique_, name, __LINE__) { []{ \
		if(ssvu::Test::Internal::isRunnerExecuted(SSVPP_STRINGIFY(SSVPP_CAT(Unique_, name, __LINE__)))) return; \
		ssvu::Test::Internal::setRunnerExecuted(SSVPP_STRINGIFY(SSVPP_CAT(Unique_, name, __LINE__))); \
		ssvu::Test::Internal::getTestGroups().push_back({ {SSVPP_STRINGIFY(name), []

	#define SSVU_TEST_END() }});}}
	#define SSVU_TEST_RUN_ALL() ssvu::Test::Internal::runAllTests()
#else
	#define SSVU_TEST(name) struct SSVPP_CAT(Unique_,__LINE__) { void f() __attribute__ ((unused)) {
	#define SSVU_TEST_END() }} __attribute__ ((unused));
	#define SSVU_TEST_RUN_ALL() { }
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
			struct Fail : Msg		{ Fail(Location mLocation, std::string mExpr) :									Msg{"fail",							std::move(mLocation), std::move(mExpr)							} { } };
			struct Expect : Msg		{ Expect(Location mLocation, std::string mExpr, std::string mException = "") :	Msg{"fail: no wanted exception",	std::move(mLocation), std::move(mExpr), std::move(mException)	} { } };
			struct Unexpect : Msg	{ Unexpect(Location mLocation, std::string mExpr, std::string mNote) :			Msg{"unwanted exception:",			std::move(mLocation), std::move(mExpr), std::move(mNote)		} { } };

			inline bool suppress(bool mValue) { return mValue; }

			inline std::string getWithMsg(std::string mStr)									{ return "with msg \"" + std::move(mStr) + "\""; }
			inline std::string getOfType(std::string mStr)									{ return "of type " + std::move(mStr); }
			inline std::ostream& operator<<(std::ostream& mOs, const Comment& mComment)		{ return mOs << (mComment ? " " + mComment.text : ""); }
			inline std::ostream& operator<<(std::ostream& mOs, const Location& mLocation)	{ return mOs << mLocation.file << ":" << mLocation.line; }

			inline void report(const Msg& mMsg, std::string mTest) { ssvu::lo("ssvu::Test") << mMsg.location << "\n" << mMsg.type << mMsg.comment << ": " << std::move(mTest) << ": " << mMsg.what() << "\n" << std::endl; }
			inline void report(const std::string& mError, std::string mTest) { ssvu::lo("ssvu::Test") << mError << "\n" << ": " << std::move(mTest) << ": \n" << std::endl; }

			inline int run(const std::vector<Test>& mTests)
			{
				int fails{0};

				for(auto& t : mTests)
				{
					try { t.action(); }
					catch(const Msg& mMsg) { ++fails; report(mMsg, t.name); }
					catch(...) { ++fails; report("unexpected exception", t.name); }
				}

				if(fails > 0) ssvu::lo("ssvu::Test") << "[" << fails << "/" << mTests.size() << "] " << "tests failed\n\n";
				return fails;
			}

			inline std::map<std::string, bool>& getRunnerMap() noexcept			{ static std::map<std::string, bool> result; return result; }
			inline bool isRunnerExecuted(const std::string& mKey) noexcept		{ return getRunnerMap()[mKey]; }
			inline void setRunnerExecuted(const std::string& mKey) noexcept		{ getRunnerMap()[mKey] = true; }

			inline std::vector<std::vector<Test>>& getTestGroups() { static std::vector<std::vector<Test>> testGroups; return testGroups; }
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
					done = true;

					bool fail{false};

					for(auto& tg : Internal::getTestGroups())
						if(Internal::run(tg) > 0)
						{
							fail = true;
							ssvu::lo("########################################################################################################\n\n");
						}

					if(!fail) ssvu::lo("ssvu::Test") << "All tests passed!\n";

					ssvu::lo().flush();
				#endif
			}
		}
	}
}

#endif
