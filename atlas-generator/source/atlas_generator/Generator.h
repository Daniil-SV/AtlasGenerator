#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <stdint.h>
#include <numeric>

#include "Config.h"
#include "Item/Item.h"
#include "PackagingException.h"

namespace wk {
	namespace AtlasGenerator
	{
		class Generator
		{
		public:

		public:
			Generator(const Config& config);
			virtual ~Generator() = default;

		public:
			size_t generate(Container<Item>& items);

			cv::Mat& get_atlas(uint8_t atlas);

		private:
			virtual bool pack_items();

			void place_image_to(cv::Mat& src, uint8_t atlas_index, uint16_t x, uint16_t y);

		private:
			const Config m_config;

			Container<Item*> m_items;
			Container<size_t> m_duplicate_indices;

			Container<cv::Mat> m_atlases;
		};
	}
}