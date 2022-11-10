#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Vec3.h"

class Sphere
{
public:
	Vec3f center; /// position of the sphere
	float radius, radius2; /// sphere radius and radius^2
	Vec3f surfaceColor, emissionColor; /// surface color and emission (light)
	float transparency, reflection; /// surface transparency and reflectivity
	Sphere()
		:
		radius( 1.0f ),
		radius2( 1.0f ),
		reflection( 1.0f ),
		transparency( 1.0f )
	{}
	Sphere(
		const Vec3f &c,
		const float &r,
		const Vec3f &sc,
		const float &refl = 0.0f,
		const float &transp = 0.0f,
		const Vec3f &ec = 0 )
		:
		center( c ),
		radius( r ),
		radius2( r * r ),
		surfaceColor( sc ),
		emissionColor( ec ),
		transparency( transp ),
		reflection( refl )
	{}
	//[comment]
	// Compute a ray-sphere intersection using the geometric solution
	//[/comment]
	inline bool intersect( const Vec3f &rayorig, const Vec3f &raydir, float &t0, float &t1 ) const noexcept
	{
		Vec3f l = center - rayorig;
		float tca = l.dot( raydir );
		if ( tca < 0.0f ) return false;

		float d2 = l.dot( l ) - tca * tca;
		if ( d2 > radius2 ) return false;

		float thc = sqrt( radius2 - d2 );
		t0 = tca - thc;
		t1 = tca + thc;

		return true;
	}
};

#endif