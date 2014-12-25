#pragma once
#include <Windows.h>
#include <SimpleMath.h>


class MathHelper
{
public:
	static float ATan2(float y, float x)
	{
		return atan2f(y,x);
	}
	static DirectX::SimpleMath::Matrix CreateScale( const  float scale)
	{
		return  DirectX::SimpleMath::Matrix::CreateScale(scale);
	}
	static DirectX::SimpleMath::Matrix CreateTranslation( const  DirectX::SimpleMath::Vector3& position)
	{
		
		return  DirectX::SimpleMath::Matrix::CreateTranslation(position);
	}

	static DirectX::SimpleMath::Vector3 CrossVector3(DirectX::SimpleMath::Vector3& v1, DirectX::SimpleMath::Vector3& v2)
	{
		return v1.Cross(v2);
	}

	static void NormalizeVector3(DirectX::SimpleMath::Vector3& v)
	{
		v.Normalize();
	}

	static DirectX::SimpleMath::Matrix MatrixMultiply(DirectX::SimpleMath::Matrix& mat1,DirectX::SimpleMath::Matrix mat2)
	{
		return mat1 * mat2;
	}
	// returns random float in [0,1).
	static float RandF()
	{
		return (float) ( rand() ) / (float)RAND_MAX;
	}

	// returns random float in [a,b)
	static float RandF(float a, float b)
	{
		return a + RandF() * (b-a);
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{ 
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a: b;
	}

	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + ( b - a ) * t;
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);

	static DirectX::SimpleMath::Matrix InverseTranspose(DirectX::SimpleMath::Matrix M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		DirectX::XMMATRIX A = M;
		A.r[3] = DirectX::SimpleMath::Vector4(0.0f,0.0f,0.0f,1.0f);

		DirectX::XMVECTOR det = XMMatrixDeterminant(A);
		return XMMatrixTranspose(XMMatrixInverse(&det, A));
	}

	static DirectX::XMVECTOR RandUnitVec3();
	static DirectX::XMVECTOR RandHemisphereUnitVec3(DirectX::XMVECTOR n);

	static const float Infinity;
	static const float Pi;


};

