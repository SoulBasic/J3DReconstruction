

#ifndef _VIEWER_COMMON_H_
#define _VIEWER_COMMON_H_


 // I N C L U D E S /////////////////////////////////////////////////

#include <GL/glew.h>
#include "libs/MVS/Common.h"
#include "libs/MVS/Scene.h"

#if defined(_MSC_VER)
#include <gl/GLU.h>
#elif defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <GLFW/glfw3.h>


// D E F I N E S ///////////////////////////////////////////////////


// P R O T O T Y P E S /////////////////////////////////////////////

using namespace SEACAVE;

namespace VIEWER {

	// the conversion matrix from OpenGL default coordinate system
	//  to the camera coordinate system:
	// [ 1  0  0  0] * [ x ] = [ x ]
	//   0 -1  0  0      y      -y
	//   0  0 -1  0      z      -z
	//   0  0  0  1      1       1
	static const GLfloat gs_convert[4][4] = {
		{1.f,  0.f,  0.f, 0.f},
		{0.f, -1.f,  0.f, 0.f},
		{0.f,  0.f, -1.f, 0.f},
		{0.f,  0.f,  0.f, 1.f} };

	/// given rotation matrix R and translation vector t,
	/// column-major matrix m is equal to:
	/// [ R11 R12 R13 t.x ]
	/// | R21 R22 R23 t.y |
	/// | R31 R32 R33 t.z |
	/// [ 0.0 0.0 0.0 1.0 ]
	//
	// World to Local
	inline Eigen::Matrix4d TransW2L(const Eigen::Matrix3d& R, const Eigen::Vector3d& t)
	{
		Eigen::Matrix4d m(Eigen::Matrix4d::Identity());
		m.block(0, 0, 3, 3) = R;
		m.block(0, 3, 3, 1) = t;
		return m;
	}
	// Local to World
	// same as above, but with the inverse of the two
	inline Eigen::Matrix4d TransL2W(const Eigen::Matrix3d& R, const Eigen::Vector3d& t)
	{
		Eigen::Matrix4d m(Eigen::Matrix4d::Identity());
		m.block(0, 0, 3, 3) = R.transpose();
		m.block(0, 3, 3, 1) = -t;
		return m;
	}
	/*----------------------------------------------------------------*/

} // namespace MVS

#endif // _VIEWER_COMMON_H_
