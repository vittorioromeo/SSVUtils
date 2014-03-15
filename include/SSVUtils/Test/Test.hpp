// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEST_HPP
#define SSVU_TEST_HPP

#include <map>
#include <string>
#include <stdexcept>
#include "SSVUtils/Core/Core.hpp"

#define SSVUT_IMPL_GET_NAME_TYPE(mName) 		SSVPP_CAT(SSVUTTestUnique, mName, __LINE__)
#define SSVUT_IMPL_GET_NAME_RUNNER(mName)		SSVPP_CAT(SSVUT_IMPL_GET_NAME_TYPE(mName), runner)
#define SSVUT_IMPL_GET_NAME_INSTANCE(mName) 	SSVPP_CAT(SSVUT_IMPL_GET_NAME_TYPE(mName), instance)
#define SSVUT_IMPL_GET_KEY(mName)				SSVPP_TOSTR(SSVUT_IMPL_GET_NAME_TYPE(mName))

#define SSVUT_IMPL_GENERATE_STRUCT(mName) \
	struct SSVUT_IMPL_GET_NAME_TYPE(mName) : public ::ssvu::Test::Internal::TestBase \
	{ \
		inline SSVUT_IMPL_GET_NAME_TYPE(mName) () : ::ssvu::Test::Internal::TestBase{SSVPP_TOSTR(mName), SSVPP_TOSTR(__LINE__), SSVPP_TOSTR(__FILE__)} { } \
		virtual void run() const override; \
	}; \
	static SSVUT_IMPL_GET_NAME_TYPE(mName) SSVUT_IMPL_GET_NAME_INSTANCE(mName);

#define SSVUT_IMPL_GENERATE_RUNNER(mName) \
	static ::ssvu::Test::Internal::Runner SSVUT_IMPL_GET_NAME_RUNNER(mName) {[] \
	{ \
		if(::ssvu::Test::Internal::wasTestExecuted(SSVUT_IMPL_GET_KEY(mName))) return; \
		::ssvu::Test::Internal::setTestExecuted(SSVUT_IMPL_GET_KEY(mName)); \
		ssvu::getEmplaceUptr<SSVUT_IMPL_GET_NAME_TYPE(mName)>(::ssvu::Test::Internal::getTestStorage(), SSVUT_IMPL_GET_NAME_INSTANCE(mName)); \
	}};

#ifndef SSVUT_DISABLE
	#define SSVUT_TEST(mName) \
		SSVUT_IMPL_GENERATE_STRUCT(mName) \
		SSVUT_IMPL_GENERATE_RUNNER(mName) \
		inline void SSVUT_IMPL_GET_NAME_TYPE(mName)::run() const

	#define SSVUT_EXPECT(mExpr) \
		while(true) \
		{ \
			if(!(mExpr)) throw ::ssvu::Test::Internal::TestFailException{this, SSVPP_TOSTR_SEP(",", SSVPP_EMPTY(), mExpr)}; \
			break; \
		}

	#define SSVUT_RUN() ::ssvu::Test::Internal::runAllTests()
#else
	#define SSVUT_TEST(mName) inline void SSVPP_CAT(SSVUT_IMPL_GET_NAME_TYPE(mName), unused) ()
	#define SSVUT_EXPECT(...)
	#define SSVUT_RUN() while(false){ }
#endif

namespace ssvu
{
	namespace Test
	{
		namespace Internal
		{
			#ifndef SSVUT_DISABLE
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

					for(const auto& t : ::ssvu::Test::Internal::getTestStorage())
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
								<< "_________________________________________\n\n";
						}
					}

					if(!failure) ssvu::lo("Test") << "All tests passed!\n";

					ssvu::lo().flush();
				}
			#endif
		}
	}
}

#endif
