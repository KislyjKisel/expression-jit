#pragma once
#include <cstdint>
#include "../../util/concepts.h"

namespace evo::ogl {

	using gl_int_t			= int32_t;
	using gl_uint_t			= uint32_t;
	using gl_int64_t		= int64_t;
	using gl_enum_t			= uint32_t;
	using gl_boolean_t		= unsigned char;
	using gl_byte_t			= signed char;
	using gl_ubyte_t		= unsigned char;
	using gl_short_t		= int16_t;
	using gl_ushort_t		= uint16_t;
	using gl_sizei_t		= int32_t;
	using gl_bitfield_t		= uint32_t;
	using gl_clampf_t		= float;
	using gl_float_t		= float;
	using gl_double_t		= double;
	using gl_char_t			= char;
	using gl_sizeiptr_t		= signed long long;
	using gl_intptr_t		= signed long long;

	inline constexpr gl_boolean_t gl_false = 0; 
	inline constexpr gl_boolean_t gl_true = 1; 

	inline constexpr gl_uint_t gl_invalid = ~0ui32;

	template<typename T>
	concept GlShaderType = AnyOf<T, gl_boolean_t, gl_int_t, gl_uint_t, gl_float_t, gl_double_t>;
}