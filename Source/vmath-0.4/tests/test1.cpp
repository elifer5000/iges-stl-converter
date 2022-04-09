#include "vmath.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/qt/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

namespace tests
{
	class Vector2fTest : public CPPUNIT_NS::TestFixture
	{

		CPPUNIT_TEST_SUITE(Vector2fTest);
		CPPUNIT_TEST(testAritV2f);
		CPPUNIT_TEST_SUITE_END();
		Vector2f a2f,b2f,c2f;
		Matrix4f am4f,bm4f;
		
	public:
		
		void setUp()
		{
			a2f = Vector2f(-1,4);
			b2f = Vector2f(-1,3);
			c2f = Vector2f(-2,7);
			
		}
		
		void tearDown(){}
		
	protected:
		void testAritV2f()
		{
			
			CPPUNIT_ASSERT(a2f + b2f == c2f);
			CPPUNIT_ASSERT(a2f != c2f);
			
		}
			
		
	};

	CPPUNIT_TEST_SUITE_REGISTER(Vector2fTest);
}



int main(int argc, char** argv)
{
	
	CppUnit::TestCaller<tests::Vector2fTest> test1("Vector2f aritmetic", &tests::Vector2fTest::testAritV2f);
	CppUnit::TestResult result;
	
	test1.run(&result);
	
	
	return 0;
}

