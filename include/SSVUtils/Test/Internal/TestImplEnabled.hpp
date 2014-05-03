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

			class TestBase
			{
				private:
					const std::string name, line, file;

				public:
					inline TestBase(std::string mName, std::string mLine, std::string mFile) : name{std::move(mName)}, line{std::move(mLine)}, file{std::move(mFile)} { }
					inline virtual ~TestBase() { }
					inline virtual void run() const { }

					inline const std::string& getName() const	{ return name; }
					inline const std::string& getLine() const	{ return line; }
					inline const std::string& getFile() const	{ return file; }
			};

			struct TestFailException : std::exception
			{
				const TestBase* test;
				const std::string expr;
				inline TestFailException(const TestBase* mTest, std::string mExpr) : test{mTest}, expr{std::move(mExpr)} { }
			};

			using TestStorage = std::vector<ssvu::Uptr<TestBase>>;
			using TestExecMap = std::map<std::string, bool>;

			inline TestStorage& getTestStorage() noexcept 	{ static TestStorage result; return result; }
			inline TestExecMap& getTestExecMap() noexcept	{ static TestExecMap result; return result; }

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

						ssvu::lo("Test") << "Test failure\n\n"
							<< "Test:\t<"		<< test.getName() << ">\n"
							<< "Line:\t<"		<< test.getLine() << ">\n"
							<< "File:\t<"		<< test.getFile() << ">\n\n"
							<< "Expression:\n"	<< mFail.expr << "\n"
							<< ssvu::hr() << "\n\n";
					}
				}

				if(!failure) ssvu::lo("Test") << "All tests passed!\n";

				ssvu::lo().flush();
			}
		}
	}
}

#endif
