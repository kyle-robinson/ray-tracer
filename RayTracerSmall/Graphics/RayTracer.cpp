#include "RayTracer.h"

//[comment]
// This is the main trace function. It takes a ray as argument (defined by its origin
// and direction). We test if this ray intersects any of the geometry in the scene.
// If the ray intersects an object, we compute the intersection point, the normal
// at the intersection point, and shade this point using this information.
// Shading depends on the surface property (is it transparent, reflective, diffuse).
// The function returns a color for the ray. If the ray intersects an object that
// is the color of the object at the intersection point, otherwise it returns
// the background color.
//[/comment]
Vec3f RayTracer::Trace(
	const Vec3f& rayorig,
	const Vec3f& raydir,
	const Sphere* spheres,
	const unsigned& depth,
	const unsigned& size )
{
	//if (raydir.length() != 1) std::cerr << "Error " << raydir << std::endl;
	float tnear = INFINITY;
	const Sphere* sphere = NULL;

	// find intersection of this ray with the sphere in the scene
	for ( unsigned i = 0u; i < size; ++i )
	{
		float t0 = INFINITY, t1 = INFINITY;
		if ( spheres[i].intersect( rayorig, raydir, t0, t1 ) )
		{
			if ( t0 < 0 ) t0 = t1;
			if ( t0 < tnear )
			{
				tnear = t0;
				sphere = &spheres[i];
			}
		}
	}

	// if there's no intersection return black or background color
	if ( !sphere ) return Vec3f( 2.0f );
	Vec3f surfaceColor = 0.0f; // color of the ray/surfaceof the object intersected by the ray
	Vec3f phit = rayorig + raydir * tnear; // point of intersection
	Vec3f nhit = phit - sphere->center; // normal at the intersection point
	
	// normalize normal direction
	// If the normal and the view direction are not opposite to each other
	// reverse the normal direction. That also means we are inside the sphere so set
	// the inside bool to true. Finally reverse the sign of IdotN which we want
	// positive.
	nhit.normalize();

	float bias = 1e-4; // add some bias to the point from which we will be tracing
	bool inside = false;
	if ( raydir.dot( nhit ) > 0.0f ) nhit = -nhit, inside = true;
	if ( ( sphere->transparency > 0.0f || sphere->reflection > 0.0f ) && depth < MAX_RAY_DEPTH )
	{
		float facingratio = -raydir.dot( nhit );
		// change the mix value to tweak the effect
		float fresneleffect = Mix( powf( 1.0f - facingratio, 3.0f ), 1.0f, 0.1f );
		// compute reflection direction (not need to normalize because all vectors are already normalized)
		Vec3f refldir = raydir - nhit * 2.0f * raydir.dot( nhit );
		refldir.normalize();
		Vec3f reflection = Trace( phit + nhit * bias, refldir, spheres, depth + 1u, size );
		Vec3f refraction = 0.0f;

		// if the sphere is also transparent compute refraction ray (transmission)
		if ( sphere->transparency )
		{
			float ior = 1.1f, eta = ( inside ) ? ior : 1.0f / ior; // are we inside or outside the surface?
			float cosi = -nhit.dot( raydir );
			float k = 1.0f - eta * eta * ( 1.0f - cosi * cosi );
			Vec3f refrdir = raydir * eta + nhit * ( eta *  cosi - sqrt( k ) );
			refrdir.normalize();
			refraction = Trace( phit - nhit * bias, refrdir, spheres, depth + 1u, size );
		}
		// the result is a mix of reflection and refraction (if the sphere is transparent)
		surfaceColor = (
			reflection * fresneleffect +
			refraction * ( 1.0f - fresneleffect ) * sphere->transparency ) * sphere->surfaceColor;
	}
	else
	{
		// it's a diffuse object, no need to raytrace any further
		for ( unsigned i = size - 1u; i != 0u; --i )
		{
			if ( spheres[i].emissionColor.x > 0.0f )
			{
				// this is a light
				Vec3f transmission = 1.0f;
				Vec3f lightDirection = spheres[i].center - phit;
				lightDirection.normalize();
				for ( unsigned j = size - 1u; j != 0u; --j )
				{
					if ( i != j )
					{
						float t0, t1;
						if ( spheres[j].intersect( phit + nhit * bias, lightDirection, t0, t1 ) )
						{
							transmission = 0.0f;
							break;
						}
					}
				}
				surfaceColor += sphere->surfaceColor * transmission *
					std::max( 0.0f, nhit.dot( lightDirection ) ) * spheres[i].emissionColor;
			}
		}
	}

	return surfaceColor + sphere->emissionColor;
}

float RayTracer::Mix( const float &a, const float &b, const float &mix )
{
	return b * mix + a * ( 1.0f - mix );
}