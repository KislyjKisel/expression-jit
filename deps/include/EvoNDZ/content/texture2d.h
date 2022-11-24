#pragma once
#include <memory>
#include "../color.h"
#include "../error.h"

namespace evo::data {
	class Texture2D {
	public:
		Texture2D(size_t width, size_t height) :
			m_width(width), m_height(height), m_color(std::make_unique<evo::Color[]>((size_t)width * height)) { }
		Texture2D(Texture2D&& t) noexcept : m_width(t.m_width), m_height(t.m_height), m_color(t.m_color.release()) { }
		//todo: from another texture
		~Texture2D() { }

		Texture2D& operator=(Texture2D&& other) noexcept
		{
			if (this != &other) {
				m_color.reset(other.m_color.release());
				m_width = other.m_width;
				m_height = other.m_height;
			}
			return *this;
		}
		size_t width() const {
			return m_width;
		}
		size_t height() const {
			return m_height;
		}
		evo::Color& operator[](size_t i) {
			EVO_ASSERT(i < (m_width * m_height));
			return m_color[i];
		}
		const evo::Color& operator[](size_t i) const {
			EVO_ASSERT(i < (m_width* m_height));
			return m_color[i];
		}
		evo::Color& operator()(size_t x, size_t y) {
			EVO_ASSERT(x < m_width && y < m_height && x >= 0 && y >= 0);
			return m_color[y * m_width + x];
		}
		const evo::Color& operator()(size_t x, size_t y) const {
			EVO_ASSERT(x < m_width&& y < m_height&& x >= 0 && y >= 0);
			return m_color[y * m_width + x];
		}

		const unsigned char* data() const {
			return (unsigned char*)m_color.get();
		}

		static Texture2D Load(unsigned char* data, uint64_t length) {
			size_t width = (size_t)data[0] * 256u + data[1];
			size_t height = (size_t)data[2] * 256u + data[3];
			size_t textureSize = width * height;
			Texture2D res = Texture2D(width, height);
			size_t pi = 4;
			for (size_t i = 0; i < textureSize; ++i) {
				res[i].b = data[pi++];
				res[i].g = data[pi++];
				res[i].r = data[pi++];
				res[i].a = data[pi++];
			}
			return res;
		}

	private:
		size_t m_width;
		size_t m_height;
		std::unique_ptr<evo::Color[]> m_color;

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
	};
}