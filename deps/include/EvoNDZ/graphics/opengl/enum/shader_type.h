#pragma once
#include "../types.h"

namespace evo::ogl
{
	struct ShaderType {
	public:
		gl_int_t count() const noexcept { return m_count; }
		gl_enum_t type() const noexcept { return m_type; }
		gl_sizei_t size() const noexcept { return m_size; }

		static ShaderType Bool();
		static ShaderType Int();
		static ShaderType UInt();
		static ShaderType Float();
		static ShaderType Double();

		static ShaderType FVec2();
		static ShaderType FVec3();
		static ShaderType FVec4();

		static ShaderType BVec2();
		static ShaderType BVec3();
		static ShaderType BVec4();

		static ShaderType IVec2();
		static ShaderType IVec3();
		static ShaderType IVec4();

		static ShaderType UVec2();
		static ShaderType UVec3();
		static ShaderType UVec4();

		static ShaderType DVec2();
		static ShaderType DVec3();
		static ShaderType DVec4();

		template<GlShaderType T>
		static ShaderType Scalar();
		template<> static ShaderType Scalar<gl_boolean_t>() { return Bool();		}
		template<> static ShaderType Scalar<gl_int_t>()		{ return Int();		}
		template<> static ShaderType Scalar<gl_uint_t>()		{ return UInt();		}
		template<> static ShaderType Scalar<gl_float_t>()	{ return Float();	}
		template<> static ShaderType Scalar<gl_double_t>()	{ return Double();	}

		template<GlShaderType T>
		static ShaderType Vec2();
		template<> static ShaderType Vec2<gl_boolean_t>()	{ return BVec2();	}
		template<> static ShaderType Vec2<gl_int_t>()		{ return IVec2();	}
		template<> static ShaderType Vec2<gl_uint_t>()		{ return UVec2();	}
		template<> static ShaderType Vec2<gl_float_t>()		{ return FVec2();	}
		template<> static ShaderType Vec2<gl_double_t>()		{ return DVec2();	}

		template<GlShaderType T>
		static ShaderType Vec3();
		template<> static ShaderType Vec3<gl_boolean_t>()	{ return BVec3();	}
		template<> static ShaderType Vec3<gl_int_t>()		{ return IVec3();	}
		template<> static ShaderType Vec3<gl_uint_t>()		{ return UVec3();	}
		template<> static ShaderType Vec3<gl_float_t>()		{ return FVec3();	}
		template<> static ShaderType Vec3<gl_double_t>()		{ return DVec3();	}

		template<GlShaderType T>
		static ShaderType Vec4();
		template<> static ShaderType Vec4<gl_boolean_t>()	{ return BVec4();	}
		template<> static ShaderType Vec4<gl_int_t>()		{ return IVec4();	}
		template<> static ShaderType Vec4<gl_uint_t>()		{ return UVec4();	}
		template<> static ShaderType Vec4<gl_float_t>()		{ return FVec4();	}
		template<> static ShaderType Vec4<gl_double_t>()		{ return DVec4();	}

	private:
		gl_int_t m_count;
		gl_enum_t m_type;
		gl_sizei_t m_size;

		ShaderType(gl_int_t count, gl_enum_t type, gl_sizei_t size) : m_count(count), m_type(type), m_size(size) { }
	};
}