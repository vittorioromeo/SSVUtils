// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEST_INTERNAL_IMPL_ENABLED
#define SSVU_TEST_INTERNAL_IMPL_ENABLED

namespace ssvu
{
	namespace Test
	{
		namespace Impl
		{
			struct Runner final
			{
				template<typename T> inline Runner(const T& mAction)
					noexcept(noexcept(mAction()))
				{
					mAction();
				}
			};

			struct TestBase
			{
				const char* name;
				const char* line;
				const char* file;

				inline TestBase(const char* mName, const char* mLine, const char* mFile) : name{mName}, line{mLine}, file{mFile} { }
				inline virtual ~TestBase() { }
				inline virtual void run() const { }
			};

			using TestStorage = VecUPtr<TestBase>;
			using TestExecMap = std::map<const char*, bool>;

			inline auto& getTestStorage() noexcept	{ static TestStorage result; return result; }
			inline auto& getTestExecMap() noexcept	{ static TestExecMap result; return result; }
			inline auto& getFailure() noexcept		{ static bool failure{false}; return failure; }

			inline bool wasTestExecuted(const char* mKey) noexcept	{ return getTestExecMap()[mKey]; }
			inline void setTestExecuted(const char* mKey) noexcept	{ getTestExecMap()[mKey] = true; }

			inline void testFailure(const TestBase* mTest, const char* mExpr, const char* mLine, std::string mCurrent = "", std::string mExpected = "")
			{
				if(getFailure()) return;
				getFailure() = true;

				lo("Test") << "Test failure\n\n"
					<< "Test:\t<"		<< mTest->name << ">\n"
					<< "Line:\t<"		<< mTest->line << ">\n"
					<< "File:\t<"		<< mTest->file << ">\n\n"
					<< "Expression:\n"	<< mExpr << "\n"
					<< "at line: "		<< mLine << "\n";

				if(!mCurrent.empty()) lo() << "\nExpression result: " << mCurrent << "\n";
				if(!mExpected.empty()) lo() <<  "Expected result:   " << mExpected << "\n";

				lo() << hr() << "\n\n";
			}

			inline void runAllTests()
			{
				static bool done{false};
				if(done) return;

				done = true;

				for(const auto& t : getTestStorage()) t->run();

				if(!getFailure()) lo("Test") << "All " << getTestStorage().size() << " tests passed!\n";

				getTestStorage().clear();
				getTestExecMap().clear();

				lo().flush();
			}
		}
	}
}

#endif
