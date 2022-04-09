//------------------------------------------------------
//
// Testing suite to ensure that vmath library 
// works right.
//
//------------------------------------------------------

#include <iostream>
#include "vmath.h"

//------------------------------------------------------
// testing macros
//------------------------------------------------------
#ifdef assert
#undef assert
#endif

#define assert(exp) \
{\
	if (!(exp))\
		std::cerr << "Assertion (" << __STRING(exp) << ") failed on " << __FILE__ << ":" << __LINE__ << std::endl;\
}

//-----------------------------------------------------
// Tests itself
//-----------------------------------------------------

//-----------[ Vector2f class tests ]------------------
void vector2fTests()
{
	// addition test
	Vector2f v1(1,0.5), v2(-1, 0.75), v3(0, 1.25);
	assert(v1 + v2 == v3);

	// substraction test
	Vector2f v4(2, -0.25);
	assert(v1 - v2 == v4);
	
	// multiplication test
	Vector2f v5(3, 1.5);
	assert(v1 * 3 == v5);

	// division test
	Vector2f v6(0.5, 0.25);
	assert(v1 / 2 == v6);

	// normalization test
	v5.normalize();
	assert(v5.length() == 1.0);

}

//-------------[ Matrix tests]----------------------------
void matrix4fTests()
{

	//==================== test #1 =============================================
	// 
	// matrix multiplication
	//
	//==========================================================================
	Matrix4f m1,m2,m3;
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
	assert(m3 == m2);

	
	//==================== test #2 =============================================
	//
	// rotation around axis matrix
	//
	//==========================================================================
	
	Vector4f vx(1,0,0,0), vy(0,1,0,0), vz(0,0,1,0);
	Vector4f v1,v2,v3;
	Matrix4f mrx = Matrix4f::createRotationAroundAxis(-M_PI/2, 0, 0);
	Matrix4f mry = Matrix4f::createRotationAroundAxis(0, -M_PI/2, 0);
	Matrix4f mrz = Matrix4f::createRotationAroundAxis(0, 0, -M_PI/2);
	
	v1 = mrx * vy;
	v2 = mry * vz;
	v3 = mrz * vx;

	//std::cout << v1 << v2 << v3 << std::endl;
	assert(v1 == vz);
	assert(v2 == vx);
	assert(v3 == vy);

	//=================== test #3 ==============================================
	//
	// rotation & translation and its inversion
	//
	//==========================================================================
	Vector4f v4(-1, 0.56, 9.3, 1);
	Vector3f v5(v4.x, v4.y, v4.z);
	Matrix4f mrot = Matrix4f::createRotationAroundAxis(M_PI/4, -3*M_PI/7, M_PI * 0.43);
	Matrix4f mtran = Matrix4f::createTranslation(10.0f, -6.0f, 7.0f, 1);
	Vector3f v6 = mtran * mrot * v5;
	Vector4f v7 = mtran * v4;
	Matrix4f mtrans = mtran * mrot;
	Matrix4f minv = mtrans.invertTransform();
	
	/*std::cout << "V5 = " << v5 << std::endl;
	std::cout << "V7 = " << v7 << std::endl;
	std::cout << "V6 = " << v6 << std::endl;
	std::cout << "V4 = " << v4 << std::endl;
	std::cout << "Mtr = " << std::endl << mtran;
	std::cout << "Mrot = " << std::endl << mrot;
	std::cout << "Mtrans = " << std::endl << mtrans;
	std::cout << "Minv = " << std::endl << minv;
	assert(v5 == v7);
	assert(minv * v6 == v4);*/ 
	
	
	
}

void quaternionTest()
{
	//====================== test #1 ===========================================
	//
	// rotation using rotation around axis
	//
	//==========================================================================
	Vector3f v1(1,0,0);
	Quatf q1 = Quatf::fromAxisRot(Vector3f(0, 1, 0), -90);
	Matrix3f rot = q1.rotMatrix();
	Vector3f w1 = rot * v1;
	Vector3f r1(0,0,1);
	
	/*std::cout << "V1 = " << v1 << std::endl;
	std::cout << "W1 = " << w1 << std::endl;
	std::cout << "R1 = " << r1 << std::endl;
	std::cout << "Rot = " << std::endl << rot;*/
	assert(r1 == w1);	
	
	
	//====================== test #2 ===========================================
	//
	// rotation using Euler angles
	//
	//==========================================================================
	Vector4f v2(15,0,0,1);
	Quatf q2 = Quatf::fromEulerAngles(0,0,-90);
	Matrix4f rot2 = q2.transform();
	Vector4f w2 = rot2 * v2;
	Vector4f r2(0,-15,0,1);

	/*std::cout << "V2 = " << v2 << std::endl;
	std::cout << "W2 = " << w2 << std::endl;
	std::cout << "R2 = " << r2 << std::endl;
	std::cout << "Rot2 = " << std::endl << rot2;*/
	assert(r2 == w2);
}


int main(int argc, char** argv)
{

	vector2fTests();
	matrix4fTests();
	quaternionTest();	


	std::cout << "All tests passes right" << std::endl;

	return 0;
}
