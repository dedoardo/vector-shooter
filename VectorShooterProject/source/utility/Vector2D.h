/////////////////////////////////////////////////////////////////////////////////
// The zlib/libpng License
// Copyright (c) 2012 Edoardo Dominici
// 
// This software is provided 'as-is', without any express or implied 
// warranty. In no event will the authors be held liable for any damages 
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute 
// it freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must 
// not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must 
// not be misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// With this class you can perform all kind of operations with vectors
// even if they are not of the same type, the type that results is the one of the left
// operator. the result is static_casted to the type of the left operator
////////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR2D_H_INCLUDED
#define VECTOR2D_H_INCLUDED

// Including the matrix class for transform operations
#include "Matrix2.h"

namespace sp
{
	template <typename T>
	class Vector2D
	{
	public :
		/////////////////////////////////////
		// CTORS
		/////////////////////////////////////
		Vector2D();

		Vector2D(T X,T Y);

		/////////////////////////////////////
		// MEMBER OPERATIONS
		/////////////////////////////////////

		inline Vector2D<T> getNormal()
		{
			return Vector2D<T>(-y,x);
		}

		inline float getMagnitude()
		{
			return sqrt(x^2 + y^2);
		}

		template <typename U>
		inline Vector2D<T> Transform(const Matrix2<U>& transformMatrix)
		{
			return Vector2D<T>(static_cast<T>(x * transformMatrix.m0 + y * transformMatrix.m2),
							   static_cast<T>(x * transformMatrix.m1 + y * transformMatrix.m3));
		}

		template <typename U>
		float dotProduct(const Vector2D<U>& right)
		{
			return (x * right.x + y * right.y);
		}

		/////////////////////////////////////
		// OVERLOADING OPERATORS
		/////////////////////////////////////
		// Unary
		template <typename U>
		Vector2D<T> operator +(const Vector2D<U>& right)
		{
			return Vector2D<T>(static_cast<T>(x + right.x),
							   static_cast<T>(y + right.y));
		}

		template <typename U>
		Vector2D<T> operator -(const Vector2D<U>& right)
		{
			return Vector2D<T>(static_cast<T>(x - right.x),
							   static_cast<T>(y - right.y));
		}

		template <typename U>
		Vector2D<T> operator *(const Vector2D<U>& right)
		{
			return Vector2D<T>(static_cast<T>(x * right.x),
							   static_cast<T>(y * right.y));
		}

		template <typename U>
		Vector2D<T> operator /(const Vector2D<U>& right)
		{
			return Vector2D<T>(static_cast<T>(x / right.x),
							   static_cast<T>(y / right.y));
		}

		template <typename U>
		Vector2D<T>& operator +=(const Vector2D<U>& right)
		{
			x = static_cast<T> (x + right.x);
			y = static_cast<T> (y + right.y);
			
			return *this;
		}

		template <typename U>
		Vector2D<T>& operator -=(const Vector2D<U>& right)
		{
			x = static_cast<T> (x - right.x);
			y = static_cast<T> (y - right.y);

			return *this;
		}

		template <typename U>
		Vector2D<T>& operator *=(const Vector2D<U>& right)
		{
			x = static_cast<T> (x * right.x);
			y = static_cast<T> (y * right.y);

			return *this;
		}

		template <typename U>
		Vector2D<T>& operator /=(const Vector2D<U>& right)
		{
			x = static_cast<T> (x / right.x);
			y = static_cast<T> (y / right.y);

			return *this;
		}

		/////////////////////////////////////
		// FEW MORE OPERATORS FOR COMMON FUNCTIONS
		/////////////////////////////////////
		
		template <typename U>
		Vector2D<T> operator *(U factor)
		{
			return Vector2D<T>(static_cast<T>(x * factor),
							   static_cast<T>(y * factor));
		}

		template <typename U>
		Vector2D<T>& operator *=(U factor)
		{
			x = static_cast<T>(x * factor);
			y = static_cast<T>(y * factor);

			return *this;
		}
		
		/////////////////////////////////////
		// MEMBER VARIABLES
		/////////////////////////////////////
		T	x;
		T	y;

	};

	// Defining commonly used types
	typedef Vector2D<int>		Vector2Di;
	typedef Vector2D<double>	Vector2Dd;
	typedef Vector2D<float>		Vector2Df;

};

#endif