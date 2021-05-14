#ifndef _VIEWER_CAMERA_H_
#define _VIEWER_CAMERA_H_

namespace VIEWER {

	class Camera
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

			AABB3d box;
		int width, height;
		Eigen::Quaterniond rotation;
		Eigen::Vector3d center;
		double dist;
		double radius;
		double fov;
		float scaleF;
		MVS::IIndex prevCamID, currentCamID, maxCamID;

	public:
		explicit Camera(const AABB3d& _box = AABB3d(true), double _fov = 40);
		void CopyOf(const Camera&);

		void Init(const AABB3d&);
		void Reset();
		void Resize(int _width, int _height);
		void SetFOV(double _fov);

		Eigen::Vector3d GetPosition() const;
		Eigen::Matrix4d GetLookAt() const;
		void GetLookAt(Eigen::Vector3d& eye, Eigen::Vector3d& center, Eigen::Vector3d& up) const;
		void Rotate(const Eigen::Vector2d& pos, const Eigen::Vector2d& prevPos);

	protected:
		void Project2Sphere(double radius, Eigen::Vector3d& p) const;
	};
	typedef CSharedPtr<Camera> CameraPtr;
	/*----------------------------------------------------------------*/

} // namespace VIEWER

#endif // _VIEWER_CAMERA_H_
