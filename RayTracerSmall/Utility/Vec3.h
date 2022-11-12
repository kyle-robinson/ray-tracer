#pragma once
#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <fstream>

template<typename T>
class Vec3
{
public:
	T x, y, z;
	Vec3() : x( T( 0 ) ), y( T( 0 ) ), z( T( 0 ) ) {}
	Vec3( T xx ) : x( xx ), y( xx ), z( xx ) {}
	Vec3( T xx, T yy, T zz ) : x( xx ), y( yy ), z( zz ) {}
	inline Vec3& normalize()
	{
		T nor2 = length2();
		if ( nor2 > (T)0 )
		{
			T invNor = (T)1 / sqrt( nor2 );
			x *= invNor, y *= invNor, z *= invNor;
		}
		return *this;
	}

	inline Vec3<T> operator - () const noexcept { return Vec3<T>( -x, -y, -z ); }
	inline Vec3<T> operator * ( const T &f ) const noexcept { return Vec3<T>( x * f, y * f, z * f ); }
	inline Vec3<T> operator * ( const Vec3<T> &v ) const noexcept { return Vec3<T>( x * v.x, y * v.y, z * v.z ); }
	inline Vec3<T> operator - ( const Vec3<T> &v ) const noexcept { return Vec3<T>( x - v.x, y - v.y, z - v.z ); }
	inline Vec3<T> operator + ( const Vec3<T> &v ) const noexcept { return Vec3<T>( x + v.x, y + v.y, z + v.z ); }

	inline Vec3<T>& operator += ( const Vec3<T> &v ) { x += v.x, y += v.y, z += v.z; return *this; }
	inline Vec3<T>& operator *= ( const Vec3<T> &v ) { x *= v.x, y *= v.y, z *= v.z; return *this; }

	inline T length() const noexcept { return sqrt( length2() ); }
	inline T length2() const noexcept { return x * x + y * y + z * z; }
	inline T dot(const Vec3<T> &v) const noexcept { return x * v.x + y * v.y + z * v.z; }
	inline friend std::ostream & operator << ( std::ostream &os, const Vec3<T> &v )
	{
		os << "[" << v.x << " " << v.y << " " << v.z << "]";
		return os;
	}
};
typedef Vec3<float> Vec3f;

#endif