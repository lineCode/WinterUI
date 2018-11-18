#pragma once

#include "def.hh"

#include <vector>
#include <iris/vec3.hh>
#include <iris/misc.hh>

/// Raytracing tools
namespace Beast
{
	namespace Graphics
	{
		namespace Raytracing
		{
			template <typename T> struct RayHit
			{
				bool hit = false;
				T distance = 0;
				IR::vec3<T> hitPoint{0};
			};
			
			template<typename T> struct Triangle
			{
				IR::vec3<T> normal, vert1, vert2, vert3;
				T distance;
				
				/// Construct a triangle with world coordinates
				/// \param a The first vertex of the triangle
				/// \param b The second vertex of the triangle
				/// \param c The third vertex of the triangle
				inline Triangle(IR::vec3<T> const &a, IR::vec3<T> const &b, IR::vec3<T> const &c)
				{
					this->vert1 = a;
					this->vert2 = b;
					this->vert3 = c;
					this->normal = IR::crossVec3<T>(this->vert1 - this->vert2, this->vert1 - this->vert3);
					this->normal.normalize();
					this->distance = IR::dotVec3<T>(this->vert1, normal);
					if(this->distance < 0) this->normal = -this->normal;
				}
				
				/// Construct a triangle with local coordinates and a model matrix
				/// \param model The matrix describing the translation rotation and scale of the mesh this triangle belongs to
				/// \param a The first vertex of the triangle
				/// \param b The second vertex of the triangle
				/// \param c The third vertex of the triangle
				inline Triangle(IR::mat4x4<T> model, IR::vec3<T> const &a, IR::vec3<T> const &b, IR::vec3<T> const &c)
				{
					this->vert1 = IR::vec3<T>{model * IR::vec4<T>{a, 1}};
					this->vert2 = IR::vec3<T>{model * IR::vec4<T>{b, 1}};
					this->vert3 = IR::vec3<T>{model * IR::vec4<T>{c, 1}};
					this->normal = IR::crossVec3<T>(this->vert1 - this->vert2, this->vert1 - this->vert3);
					this->normal.normalize();
					this->distance = IR::dotVec3<T>(this->vert1, normal);
					if(this->distance < 0) this->normal = -this->normal;
				}
			};
			
			/// Describes a ray in 3D space with an origin point and a direction
			template <typename T> struct Ray
			{
				IR::vec3<T> origin, direction;
				
				inline Ray(IR::vec3<T> const &origin, IR::vec3<T> const &direction) : origin(origin), direction(direction) {}
				
				/// Test intersection between this ray and the given triangle
				inline RayHit<T> testIntersection(Triangle<T> const &triangle)
				{
					T dot = IR::dotVec3<T>(triangle.normal * triangle.distance - this->origin, triangle.normal) / IR::dotVec3<T>(this->direction, triangle.normal);
					IR::vec3<T> hitPos = this->origin + this->direction * dot;
					
					//barycentric inside-outside test
					IR::vec3<T> u = triangle.vert2 - triangle.vert1, v = triangle.vert3 - triangle.vert1, w = hitPos - triangle.vert1;
					IR::vec3<T> vXw = IR::crossVec3<T>(v, w), vXu = IR::crossVec3<T>(v, u);
					if(IR::dotVec3<T>(vXw, vXu) < 0) return {};
					IR::vec3<T> uXw = IR::crossVec3<T>(u, w), uXv = IR::crossVec3<T>(u, v);
					if(IR::dotVec3<T>(uXw, uXv) < 0) return {};
					T denom = uXv.mag(), r = vXw.mag() / denom, t = uXw.mag() / denom;
					if(r + t <= 1) return {true, IR::distanceVec3<T>(this->origin, hitPos), hitPos};
					else return {};
				}
				
				/// Test intersection between this ray and all given triangles
				inline std::vector<RayHit<T>> testIntersections(std::vector<Triangle<T>> const &triangles, bool filterMisses)
				{
					std::vector<RayHit<T>> out;
					for(auto const &triangle : triangles)
					{
						if(filterMisses)
						{
							RayHit<T> hit = this->testIntersection(triangle);
							if(hit.hit) out.push_back(hit);
						}
						else out.push_back(this->testIntersection(triangle));
					}
					return out;
				}
				
				/// Find a ray direction pointing from the given origin to the point the user clicked on, projected into the world
				/// \param origin The origin of the ray, usually the camera's position
				/// \param clickX The X position on the OpenGL context the user clicked, in pixels
				/// \param clickY The Y position on the OpenGL context the user clicked, in pixels
				/// \param view The view matrix describing the camera being rendered
				/// \param projection The projection matrix describing the OpenGL viewport being rendered to
				/// \param contextWidth The width of the OpenGL context being rendered
				/// \param contextHeight The height of the OpenGL context being rendered
				inline static Ray get(IR::vec3<T> const &origin, int const clickX, int const clickY, IR::mat4x4<T> view, IR::mat4x4<T> projection, uint32_t const contextWidth, uint32_t const contextHeight)
				{
					//FIXME ray's direction becomes more wrong the farther you are from the origin
					IR::vec3<T> rayEnd = IR::mouseToWorld<T>(clickX, contextHeight - clickY, view, projection, contextWidth, contextHeight);
					//IR::vec3<T> rayEnd2 = IRC::GLMtoIRvec3<T>(glm::unProject(glm::vec3{clickX, contextHeight - clickY, 1.0f}, IRC::IRtoGLMmat4<T>(view), IRC::IRtoGLMmat4<T>(projection), glm::vec4{0.0f, 0.0f, contextWidth, contextHeight}));
					return Ray<T>{origin, IR::vec3<T>{rayEnd - origin}.normalized()};
				}
			};
		}
	}
}
