// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEST_INTERNAL_IMPL_ENABLED
#define SSVU_TEST_INTERNAL_IMPL_ENABLED

namespace ssvu
{
	namespace Test
	{
		namespace Internal
		{
			struct Runner
			{
				template<typename T> inline Runner(const T& mAction) { mAction(); }
			};

			struct TestBase
			{
				const std::string name, line, file;

				inline TestBase(std::string mName, std::string mLine, std::string mFile) : name{std::move(mName)}, line{std::move(mLine)}, file{std::move(mFile)} { }
				inline virtual ~TestBase() { }
				inline virtual void run() const { }
			};

			struct TestFailException final : std::exception
			{
				const TestBase* test;
				const std::string expr, line, current, expected;

				inline TestFailException(const TestBase* mTest, std::string mExpr, std::string mLine, std::string mCurrent = "", std::string mExpected = "")
					: test{mTest}, expr{std::move(mExpr)}, line{std::move(mLine)}, current(std::move(mCurrent)), expected(std::move(mExpected)) { }
			};

			using TestStorage = VecUPtr<TestBase>;
			using TestExecMap = std::map<std::string, bool>;

			inline auto& getTestStorage() noexcept	{ static TestStorage result; return result; }
			inline auto& getTestExecMap() noexcept	{ static TestExecMap result; return result; }

			inline bool wasTestExecuted(const std::string& mKey) noexcept	{ return getTestExecMap()[mKey]; }
			inline void setTestExecuted(const std::string& mKey) noexcept	{ getTestExecMap()[mKey] = true; }

			inline void runAllTests()
			{
				static bool	done{false};
				if(done) return;

				done = true;

				bool failure{false};

				for(const auto& t : getTestStorage())
				{
					try
					{
						t->run();
					}
					catch(TestFailException& mFail)
					{
						failure = true;

						auto& test(*mFail.test);

						lo("Test") << "Test failure\n\n"
							<< "Test:\t<"		<< test.name << ">\n"
							<< "Line:\t<"		<< test.line << ">\n"
							<< "File:\t<"		<< test.file << ">\n\n"
							<< "Expression:\n"	<< mFail.expr << "\n"
							<< "at line: "		<< mFail.line << "\n";

						if(mFail.current != "") lo() << "\nExpression result: " << mFail.current << "\n";
						if(mFail.expected != "") lo() <<  "Expected result:   " << mFail.expected << "\n";

						lo() << hr() << "\n\n";
					}
				}

				if(!failure) lo("Test") << "All " << getTestStorage().size() << " tests passed!\n";

				getTestStorage().clear();
				getTestExecMap().clear();

				lo().flush();
			}
		}
	}
}

#endif
