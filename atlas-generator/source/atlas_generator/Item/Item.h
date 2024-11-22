#pragma once

#include <stdint.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <filesystem>

#include "Vertex.h"
#include "atlas_generator/Config.h"
#include "core/math/point.h"
#include "core/math/rect.h"

#ifdef CV_DEBUG
void ShowContour(cv::Mat& src, std::vector<wk::AtlasGenerator::Vertex>& points);
void ShowContour(cv::Mat& src, std::vector<wk::Point>& points);
void ShowContour(cv::Mat& src, std::vector<cv::Point> points);
void ShowImage(std::string name, cv::Mat& image);
#endif

namespace wk
{
	namespace AtlasGenerator
	{
		template<typename T>
		using Container = std::vector<T>;

		// Interface that represents Atlas Sprite
		class Item
		{
		public:
			class Transformation
			{
			public:
				Transformation(double rotation = 0.0, Point translation = Point(0, 0));

			public:
				// Rotation in radians
				double rotation;
				Point translation;

				template<typename T>
				void transform_point(Point_t<T>& vertex) const
				{
					T x = vertex.x;
					T y = vertex.y;

					vertex.x = (T)ceil(x * std::cos(rotation) - y * std::sin(rotation) + translation.x);
					vertex.y = (T)ceil(y * std::cos(rotation) + x * std::sin(rotation) + translation.y);
				}
			};
		public:
			enum class Status : uint8_t
			{
				Unset = 0,
				Valid,
				InvalidPolygon
			};

			enum class Type : uint8_t
			{
				Sprite,
				Sliced
			};

			enum class SlicedArea : uint8_t
			{
				BottomLeft = 1,
				BottomMiddle,
				BottomRight,
				MiddleLeft,
				Center,
				MiddleRight,
				TopLeft,
				TopMiddle,
				TopRight
			};

		public:
			Item(cv::Mat& image, Type type = Type::Sprite);
			Item(std::filesystem::path path, Type type = Type::Sprite);
			Item(cv::Scalar color, Type type = Type::Sprite);

			virtual ~Item() = default;

			// Image Info
		public:
			virtual Status status() const;
			virtual uint16_t width() const;
			virtual uint16_t height() const;

			// TODO: move to self written image class?
			virtual cv::Mat& image();

			// Generator Info
		public:
			uint8_t texture_index = 0xFF;
			Container<Vertex> vertices;

			// UV Transformation
			Transformation transform;

		public:
			virtual bool is_rectangle() const;
			virtual bool is_sliced() const;

		public:
			// XY coords bound
			Rect bound() const;
			void generate_image_polygon(const Config& config);

		public:
			void get_sliced_area(
				SlicedArea area,
				const Rect& guide,
				Rect& xy,
				RectUV& uv,
				const Transformation xy_transform = Transformation()
			) const;

		public:
			bool operator ==(Item& other);

		private:
			void image_preprocess(const Config& config);
			void alpha_preprocess();

			void get_image_contour(cv::Mat& image, Container<cv::Point>& result);

			void normalize_mask(cv::Mat& mask);

		protected:
			Type m_type;
			Status m_status = Status::Unset;
			bool m_preprocessed = false;
			cv::Mat m_image;
		};
	}
}