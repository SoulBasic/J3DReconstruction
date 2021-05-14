#ifndef _VIEWER_IMAGE_H_
#define _VIEWER_IMAGE_H_

namespace VIEWER {

	class Image
	{
	public:
		typedef CLISTDEFIDX(Image, uint32_t) ImageArr;
		enum {
			IMG_NULL = 0,
			IMG_LOADING,
			IMG_VALID
		};
		union ImagePtrInt {
			cv::Mat* pImage;
			int_t ptr;
			inline ImagePtrInt() : ptr(IMG_NULL) {}
			inline ImagePtrInt(cv::Mat* p) : pImage(p) {}
			inline operator cv::Mat* () const { return pImage; }
			inline operator cv::Mat*& () { return pImage; }
		};

	public:
		MVS::IIndex idx; // image index in the current scene
		int width, height;
		GLuint texture;
		double opacity;
		ImagePtrInt pImage;

	public:
		Image(MVS::IIndex = NO_ID);
		~Image();

		void Release();
		void ReleaseImage();
		inline bool IsValid() const { return texture > 0; }
		inline bool IsImageEmpty() const { return pImage.ptr == IMG_NULL; }
		inline bool IsImageLoading() const { return pImage.ptr == IMG_LOADING; }
		inline bool IsImageValid() const { return pImage.ptr >= IMG_VALID; }

		void SetImageLoading();
		void AssignImage(cv::InputArray);
		bool TransferImage();

		void SetImage(cv::InputArray);
		void GenerateMipmap() const;
		void Bind() const;

	protected:
	};
	typedef Image::ImageArr ImageArr;
	/*----------------------------------------------------------------*/

} // namespace VIEWER

#endif // _VIEWER_IMAGE_H_
