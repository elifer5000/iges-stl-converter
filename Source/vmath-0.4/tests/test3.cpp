////////////////////////////////////////////////////////////////////////////////
//
// VMath library testing suite
//
////////////////////////////////////////////////////////////////////////////////
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/Test.h>
#include <cppunit/ui/text/TestRunner.h>
#include "vmath.h"

class Vector2fTest : public CppUnit::TestCase
{
   private:
      Vector2f v1,v2,v3,v4,v5,v6;
		



   public:
      //Vector2fTest(std::string name) : CppUnit::TestCase(name){}
		
      void setUp()
      {
	 v1 = Vector2f(1,0.5);
	 v2 = Vector2f(-1, 0.75);
	 v3 = Vector2f(0, 1.25);
	 v4 = Vector2f(2, -0.25);
	 v5 = Vector2f(3, 1.5);
	 v6 = Vector2f(0.5, 0.25);
      }
		
      void tearDown()
      {
      }
		
      void testAddition()
      {
	 CPPUNIT_ASSERT(v1 + v2 == v3);
      }
		
      void testSubstraction()
      {
	 CPPUNIT_ASSERT(v1 - v2 == v4);
      }
		
      void testMultiplication()
      {
	 CPPUNIT_ASSERT(v1 * 3 == v5);
      }
		
      void testDivision()
      {
	 CPPUNIT_ASSERT(v1 / 2 == v6);
      }

      void testNormalize()
      {
	 v5.normalize();
	 CPPUNIT_ASSERT(std::abs(v5.length() - 1.0 ) < EPSILON);
			
      }
		
      static CppUnit::Test* suite()
      {
	 CppUnit::TestSuite* suite = new CppUnit::TestSuite("Vector2f test");
			
	 suite->addTest( new CppUnit::TestCaller<Vector2fTest>(
			    "addition", &Vector2fTest::testAddition));
	 suite->addTest( new CppUnit::TestCaller<Vector2fTest>(
			    "substraction", &Vector2fTest::testSubstraction));
	 suite->addTest( new CppUnit::TestCaller<Vector2fTest>(
			    "multiplication", &Vector2fTest::testMultiplication));
	 suite->addTest( new CppUnit::TestCaller<Vector2fTest>(
			    "division", &Vector2fTest::testDivision));
	 suite->addTest( new CppUnit::TestCaller<Vector2fTest>(
			    "normalize", &Vector2fTest::testNormalize));
			
	 return suite;
      }
};

class Matrix4fTest : public CppUnit::TestFixture
{
   private:
      Matrix4f m1,m2,m3;
      Vector4f vx, vy, vz;
      Vector4f v1,v2,v3;
      Matrix4f mrx;
      Matrix4f mry;
      Matrix4f mrz;
      Vector4f v4;
      Vector3f v5;
      Matrix4f mrot;
      Matrix4f mtran;
      Vector3f v6;
      Vector4f v7;
      Matrix4f mtrans;
      Matrix4f minv;


   public:
	
      void setUp()
      {
	 vx = Vector4f(1,0,0,0);
	 vy = Vector4f(0,1,0,0);
	 vz = Vector4f(0,0,1,0);
	 v4 = Vector4f(-1, 0.56, 9.3, 1);
	 v5 = Vector3f(v4.x, v4.y, v4.z);
		
      }
	
      void tearDown(){}
	
      void testMultiplication()
      {
	 m2.at(1,3) = 5;
	 m2.at(3,1) = 7;
	 m2.at(2,2) = 0;
	 m2.at(0,2) = 11;
	 m2.at(2,0) = 13;
	 m2.at(3,3) = 99;
	 m2.at(3,0) = -1;
	 m2.at(0,3) = 1;
		
	 m3 = m1 * m2 * m1;
	 /*std::cout << "M1:" << std::endl << m1;
	   std::cout << "M2:" << std::endl << m2;
	   std::cout << "M3 = M1 * M1 * M2:" << std::endl << m3;
	   std::cout << "M1 * M1:" << std::endl << m1 * m1;*/
	 CPPUNIT_ASSERT(m3 == m2);
      }
	
      void testRotation()
      {
	 mrx = Matrix4f::createRotationAroundAxis(-M_PI/2, 0, 0);
	 mry = Matrix4f::createRotationAroundAxis(0, -M_PI/2, 0);
	 mrz = Matrix4f::createRotationAroundAxis(0, 0, -M_PI/2);
		
	 v1 = mrx * vy;
	 v2 = mry * vz;
	 v3 = mrz * vx;
	
	 //std::cout << v1 << v2 << v3 << std::endl;
	 CPPUNIT_ASSERT(v1 == vz);
	 CPPUNIT_ASSERT(v2 == vx);
	 CPPUNIT_ASSERT(v3 == vy);
      }
	
      void testInversion()
      {
	 mrot = Matrix4f::createRotationAroundAxis(M_PI/4, -3*M_PI/7, M_PI * 0.43);
	 mtran = Matrix4f::createTranslation(10.0f, -6.0f, 7.0f, 1);
	 v6 = mtran * mrot * v5;
	 v7 = mtran * v4;
	 mtrans = mtran * mrot;
	 minv = mtrans.invertTransform();
		
	 /*std::cout << "V5 = " << v5 << std::endl;
	   std::cout << "V7 = " << v7 << std::endl;
	   std::cout << "V6 = " << v6 << std::endl;
	   std::cout << "V4 = " << v4 << std::endl;
	   std::cout << "Mtr = " << std::endl << mtran;
	   std::cout << "Mrot = " << std::endl << mrot;
	   std::cout << "Mtrans = " << std::endl << mtrans;
	   std::cout << "Minv = " << std::endl << minv;*/
	 CPPUNIT_ASSERT(v5 == v7);
	 CPPUNIT_ASSERT(minv * v6 == v4); 
		
      }
	
      static CppUnit::Test* suite()
      {
	 CppUnit::TestSuite* suite = new CppUnit::TestSuite("Matrix4f test");
	 suite->addTest(new CppUnit::TestCaller<Matrix4fTest>(
			   "multiplication", &Matrix4fTest::testMultiplication));
	 suite->addTest(new CppUnit::TestCaller<Matrix4fTest>(
			   "rotation", &Matrix4fTest::testRotation));
	 /*suite->addTest(new CppUnit::TestCaller<Matrix4fTest>(
	   "inversion", &Matrix4fTest::testInversion));*/
			
	 return suite;
      }
};

class QuatTest : public CppUnit::TestFixture
{
   private:
      Vector3f v1;
      Quatf q1;
      Matrix3f rot;
      Vector3f w1;
      Vector3f r1;
      Vector4f v2;
      Quatf q2,q3,q4;
      Matrix4f rot2;
      Vector4f w2;
      Vector4f r2;

   public:

      void setUp()
      {
	 v1 = Vector3f(1,0,0);
	 r1 = Vector3f(0,0,1);
	 v2 = Vector4f(15,0,0,1);
	 r2 = Vector4f(0,-15,0,1);
      }
	
      void tearDown(){}
	
      void testRotAxis()
      {
	 Vector3f v1(1,0,0);
	 q1 = Quatf::fromAxisRot(Vector3f(0, 1, 0), -90);
	 rot = q1.rotMatrix();
	 w1 = rot * v1;
		
	 /*std::cout << "V1 = " << v1 << std::endl;
	   std::cout << "W1 = " << w1 << std::endl;
	   std::cout << "R1 = " << r1 << std::endl;
	   std::cout << "Rot = " << std::endl << rot;*/
	 CPPUNIT_ASSERT(r1 == w1);	
      }

      void testTransform()
      {
	 q2 = Quatf::fromAxisRot(Vector3f(0,1,0), 180);
	 q3 = Quatf::fromAxisRot(Vector3f(0,0,-1), 90);
	 //q4 = q2 + q3;
	 //q4.normalize();
	 Matrix4f m1,m2;
	 m1 = q2.transform();
	 m2 = q3.transform();
	 rot2 = m1 * m2;//q3.transform();//q2.transform();// * q3.transform();//q4.transform();
	 w2 = Vector4f(1,0,0,1);
	 r2 = Vector4f(0,-1,0,1);
	 Vector4f v2 = rot2 * w2;
	 q4 = q2 * q3;
	 Vector4f v3 = q4.transform() * w2; 

	 /*std::cout << "W2 = " << w2 << std::endl;
	   std::cout << "R2 = " << r2 << std::endl;
	   std::cout << "Rot2 = " << std::endl << rot2;
	   std::cout << "V2 = " << v2 << std::endl;
	   std::cout << "V3 = " << v3 << std::endl;*/
	 CPPUNIT_ASSERT(r2 == v2);
	 CPPUNIT_ASSERT(r2 == v3);
      }
	
      void testRotEuler()
      {
	 q2 = Quatf::fromEulerAngles(0,0,-90);
	 rot2 = q2.transform();
	 w2 = rot2 * v2;
		
	
	 /*std::cout << "V2 = " << v2 << std::endl;
	   std::cout << "W2 = " << w2 << std::endl;
	   std::cout << "R2 = " << r2 << std::endl;
	   std::cout << "Rot2 = " << std::endl << rot2;*/
	 CPPUNIT_ASSERT(r2 == w2);
      }
	
      static CppUnit::Test* suite()
      {
	 CppUnit::TestSuite* suite = new CppUnit::TestSuite("Quaternion test");
	 suite->addTest(new CppUnit::TestCaller<QuatTest>(
			   "Rotation around axis", &QuatTest::testRotAxis));
	 suite->addTest(new CppUnit::TestCaller<QuatTest>(
			   "Roation from euler angles", &QuatTest::testRotEuler));
	 suite->addTest(new CppUnit::TestCaller<QuatTest>(
			   "Rotation around axis using transform()", &QuatTest::testTransform));
		
	 return suite;
      }
};

class ConvTest : public CppUnit::TestFixture
{
   private:
      Vector2<char> c2;
      Vector2<int> i2;
      Vector2<float> f2;
      Vector2<double> d2;

      Vector3<char> c3;
      Vector3<int> i3;
      Vector3<float> f3;
      Vector3<double> d3;

      Vector4<char> c4;
      Vector4<int> i4;
      Vector4<float> f4;
      Vector4<double> d4;

      Matrix3<char> cm3;
      Matrix3<int> im3;
      Matrix3<float> fm3;
      Matrix3<double> dm3;

      Matrix4<char> cm4;
      Matrix4<int> im4;
      Matrix4<float> fm4;
      Matrix4<double> dm4;

      Quaternion<char> cq;
      Quaternion<int> iq;
      Quaternion<float> fq;
      Quaternion<double> dq;

   public:
      void setUp()
      {
	 c2 = Vector2<char>(1,2);
	 c3 = Vector3<char>(1,2,3);
	 c4 = Vector4<char>(0,1,2,3);
	 cm3.identity();
	 cm4.identity();
	 cq = Quaternion<char>(1, Vector3<char>(1,2,3));
      }

      void tearDown()
      {
      }

      void testVector2()
      {
	 i2 = c2; f2 = i2; d2 = f2;
	 CPPUNIT_ASSERT(c2 == d2);
      }

      void testVector3()
      {
	 i3 = c3; f3 = i3; d3 = f3;
	 CPPUNIT_ASSERT(c3 == d3);
      }

      void testVector4()
      {
	 i4 = c4; f4 = i4; d4 = f4;
	 CPPUNIT_ASSERT(c4 == d4);
      }

      void testMatrix3()
      {
	 im3 = cm3; fm3 = im3; dm3 = fm3;
	 CPPUNIT_ASSERT(cm3 == dm3);
      }

      void testMatrix4()
      {
	 im4 = cm4; fm4 = im4; dm4 = fm4;
	 CPPUNIT_ASSERT(cm4 == dm4);
      }

      void testQuaternion()
      {
	 iq = cq; fq = iq; dq = fq;
	 CPPUNIT_ASSERT(cq == dq);
      }


      static CppUnit::Test* suite()
      {
	 CppUnit::TestSuite* suite = new CppUnit::TestSuite("Conversion test");
	 suite->addTest(new CppUnit::TestCaller<ConvTest>(
			   "Vector2 conversion", &ConvTest::testVector2));
	 suite->addTest(new CppUnit::TestCaller<ConvTest>(
			   "Vector3 conversion", &ConvTest::testVector3));
	 suite->addTest(new CppUnit::TestCaller<ConvTest>(
			   "Vector4 conversion", &ConvTest::testVector4));
	 suite->addTest(new CppUnit::TestCaller<ConvTest>(
			   "Matrix3 conversion", &ConvTest::testMatrix3));
	 suite->addTest(new CppUnit::TestCaller<ConvTest>(
			   "Matrix4 conversion", &ConvTest::testMatrix4));
	 suite->addTest(new CppUnit::TestCaller<ConvTest>(
			   "Quaternion conversion", &ConvTest::testQuaternion));

		
	 return suite;
      }






};

int main(int argc, char** argv)
{
	
   CppUnit::TextUi::TestRunner runner;
   runner.addTest(Vector2fTest::suite());
   runner.addTest(Matrix4fTest::suite());
   runner.addTest(QuatTest::suite());
   runner.addTest(ConvTest::suite());
   runner.run();

   return 0;	
}

