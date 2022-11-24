#pragma once
#include "../types.h"

namespace evo::ogl
{
	class State;

	struct BlendFactor {
		gl_enum_t value() const noexcept { return m_value; }

		static BlendFactor Zero()					noexcept;
		static BlendFactor One()					noexcept;   
		static BlendFactor SrcColor()				noexcept;   
		static BlendFactor OneMinusSrcColor()		noexcept;   
		static BlendFactor DstColor()				noexcept;   
		static BlendFactor OneMinusDstColor()		noexcept;
		static BlendFactor SrcAlpha()				noexcept;   
		static BlendFactor OneMinusSrcAlpha()		noexcept;   
		static BlendFactor DstAlpha()				noexcept;   
		static BlendFactor OneMinusDstAlpha()		noexcept;   
		static BlendFactor ConstantColor()			noexcept;   
		static BlendFactor OneMinusConstantColor()	noexcept;
		static BlendFactor ConstantAlpha()			noexcept;   
		static BlendFactor OneMinusConstantAlpha()	noexcept;   
		static BlendFactor SrcAlphaSaturate()		noexcept;   
		static BlendFactor Src1Color()				noexcept;   
		static BlendFactor OneMinusSrc1Color()		noexcept;   
		static BlendFactor Src1Alpha()				noexcept;
		static BlendFactor OneMinusSrc1Alpha()		noexcept;

		bool operator==(const BlendFactor&) const = default;
		bool operator!=(const BlendFactor&) const = default;

		friend class State;
		
	private:
		gl_enum_t m_value;

		constexpr BlendFactor(gl_enum_t value) : m_value(value) { }
	};

	struct BlendFunction {
		BlendFactor srcRGB, srcA, dstRGB, dstA;

		BlendFunction()
			: srcRGB(BlendFactor::Zero()), srcA(BlendFactor::Zero()), dstRGB(BlendFactor::Zero()), dstA(BlendFactor::Zero()) { }

		constexpr BlendFunction(BlendFactor src, BlendFactor dst)
			: srcRGB(src), srcA(src), dstRGB(dst), dstA(dst) { }

		constexpr BlendFunction(BlendFactor srcRGB, BlendFactor srcA, BlendFactor dstRGB, BlendFactor dstA)
			: srcRGB(srcRGB), srcA(srcA), dstRGB(dstRGB), dstA(dstA) { }

		bool operator==(const BlendFunction&) const = default;
		bool operator!=(const BlendFunction&) const = default;
	};
}