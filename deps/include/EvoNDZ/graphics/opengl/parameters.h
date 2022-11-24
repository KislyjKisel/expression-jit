#pragma once
#include <vector>
#include <array>
#include <span>
#include "types.h"
#include "enum/capability.h"

namespace evo::ogl
{
	class Parameters final {
	public:
		Parameters();

		gl_boolean_t is_enabled(Capability);
		gl_boolean_t is_enabled(Capability, gl_uint_t index);

#pragma region Static parameter getters
		std::span<const int> compressed_texture_formats() const noexcept { return msp_compressed_texture_formats; }
		std::span<const int> program_binary_formats() const noexcept { return msp_program_binary_formats; }
		std::array<gl_float_t, 2> aliased_line_width_range() const noexcept { return msp_aliased_line_width_range; }
		gl_int_t max_compute_shader_storage_blocks() const noexcept { return msp_max_compute_shader_storage_blocks; }
		gl_int_t max_combined_shader_storage_blocks() const noexcept { return msp_max_combined_shader_storage_blocks; }
		gl_int_t max_compute_uniform_blocks() const noexcept { return msp_max_compute_uniform_blocks; }
		gl_int_t max_compute_texture_image_units() const noexcept { return msp_max_compute_texture_image_units; }
		gl_int_t max_compute_uniform_components() const noexcept { return msp_max_compute_uniform_components; }
		gl_int_t max_compute_atomic_counters() const noexcept { return msp_max_compute_atomic_counters; }
		gl_int_t max_compute_atomic_counter_buffers() const noexcept { return msp_max_compute_atomic_counter_buffers; }
		gl_int_t max_combined_compute_uniform_components() const noexcept { return msp_max_combined_compute_uniform_components; }
		gl_int_t max_compute_work_group_count_x() const noexcept { return msp_max_compute_work_group_count_x; }
		gl_int_t max_compute_work_group_count_y() const noexcept { return msp_max_compute_work_group_count_y; }
		gl_int_t max_compute_work_group_count_z() const noexcept { return msp_max_compute_work_group_count_z; }
		gl_int_t max_compute_work_group_size_x() const noexcept { return msp_max_compute_work_group_size_x; }
		gl_int_t max_compute_work_group_size_y() const noexcept { return msp_max_compute_work_group_size_y; }
		gl_int_t max_compute_work_group_size_z() const noexcept { return msp_max_compute_work_group_size_z; }
		gl_int_t max_debug_group_stack_depth() const noexcept { return msp_max_debug_group_stack_depth; }
		gl_int_t context_flags() const noexcept { return msp_context_flags; }
		gl_boolean_t doublebuffer() const noexcept { return msp_doublebuffer; }
		gl_int_t implementation_color_read_format() const noexcept { return msp_implementation_color_read_format; }
		gl_int_t implementation_color_read_type() const noexcept { return msp_implementation_color_read_type; }
		gl_int_t layer_provoking_vertex() const noexcept { return msp_layer_provoking_vertex; }
		gl_int_t api_major_version() const noexcept { return msp_api_major_version; }
		gl_int_t max_3d_texture_size() const noexcept { return msp_max_3d_texture_size; }
		gl_int_t max_array_texture_layers() const noexcept { return msp_max_array_texture_layers; }
		gl_int_t max_clip_distances() const noexcept { return msp_max_clip_distances; }
		gl_int_t max_color_texture_samples() const noexcept { return msp_max_color_texture_samples; }
		gl_int_t max_combined_atomic_counters() const noexcept { return msp_max_combined_atomic_counters; }
		gl_int_t max_combined_fragment_uniform_components() const noexcept { return msp_max_combined_fragment_uniform_components; }
		gl_int_t max_combined_geometry_uniform_components() const noexcept { return msp_max_combined_geometry_uniform_components; }
		gl_int_t max_combined_texture_image_units() const noexcept { return msp_max_combined_texture_image_units; }
		gl_int_t max_combined_uniform_blocks() const noexcept { return msp_max_combined_uniform_blocks; }
		gl_int_t max_combined_vertex_uniform_components() const noexcept { return msp_max_combined_vertex_uniform_components; }
		gl_int_t max_cube_map_texture_size() const noexcept { return msp_max_cube_map_texture_size; }
		gl_int_t max_depth_texture_samples() const noexcept { return msp_max_depth_texture_samples; }
		gl_int_t max_draw_buffers() const noexcept { return msp_max_draw_buffers; }
		gl_int_t max_dual_source_draw_buffers() const noexcept { return msp_max_dual_source_draw_buffers; }
		gl_int_t max_elements_indices() const noexcept { return msp_max_elements_indices; }
		gl_int_t max_elements_vertices() const noexcept { return msp_max_elements_vertices; }
		gl_int_t max_fragment_atomic_counters() const noexcept { return msp_max_fragment_atomic_counters; }
		gl_int_t max_fragment_shader_storage_blocks() const noexcept { return msp_max_fragment_shader_storage_blocks; }
		gl_int_t max_fragment_input_components() const noexcept { return msp_max_fragment_input_components; }
		gl_int_t max_fragment_uniform_components() const noexcept { return msp_max_fragment_uniform_components; }
		gl_int_t max_fragment_uniform_vectors() const noexcept { return msp_max_fragment_uniform_vectors; }
		gl_int_t max_fragment_uniform_blocks() const noexcept { return msp_max_fragment_uniform_blocks; }
		gl_int_t max_framebuffer_width() const noexcept { return msp_max_framebuffer_width; }
		gl_int_t max_framebuffer_height() const noexcept { return msp_max_framebuffer_height; }
		gl_int_t max_framebuffer_layers() const noexcept { return msp_max_framebuffer_layers; }
		gl_int_t max_framebuffer_samples() const noexcept { return msp_max_framebuffer_samples; }
		gl_int_t max_geometry_atomic_counters() const noexcept { return msp_max_geometry_atomic_counters; }
		gl_int_t max_geometry_shader_storage_blocks() const noexcept { return msp_max_geometry_shader_storage_blocks; }
		gl_int_t max_geometry_input_components() const noexcept { return msp_max_geometry_input_components; }
		gl_int_t max_geometry_output_components() const noexcept { return msp_max_geometry_output_components; }
		gl_int_t max_geometry_texture_image_units() const noexcept { return msp_max_geometry_texture_image_units; }
		gl_int_t max_geometry_uniform_blocks() const noexcept { return msp_max_geometry_uniform_blocks; }
		gl_int_t max_geometry_uniform_components() const noexcept { return msp_max_geometry_uniform_components; }
		gl_int_t max_integer_samples() const noexcept { return msp_max_integer_samples; }
		gl_int_t min_map_buffer_alignment() const noexcept { return msp_min_map_buffer_alignment; }
		gl_int_t max_label_length() const noexcept { return msp_max_label_length; }
		gl_int_t max_program_texel_offset() const noexcept { return msp_max_program_texel_offset; }
		gl_int_t min_program_textel_offset() const noexcept { return msp_min_program_textel_offset; }
		gl_int_t max_rectangle_texture_size() const noexcept { return msp_max_rectangle_texture_size; }
		gl_int_t max_renderbuffer_size() const noexcept { return msp_max_renderbuffer_size; }
		gl_int_t max_sample_mask_words() const noexcept { return msp_max_sample_mask_words; }
		gl_int_t max_server_wait_timeout() const noexcept { return msp_max_server_wait_timeout; }
		gl_int_t max_shader_storage_buffer_bindings() const noexcept { return msp_max_shader_storage_buffer_bindings; }
		gl_int_t max_tess_control_atomic_counters() const noexcept { return msp_max_tess_control_atomic_counters; }
		gl_int_t max_tess_evaluation_atomic_counters() const noexcept { return msp_max_tess_evaluation_atomic_counters; }
		gl_int_t max_tess_control_shader_storage_blocks() const noexcept { return msp_max_tess_control_shader_storage_blocks; }
		gl_int_t max_tess_evaluation_shader_storage_blocks() const noexcept { return msp_max_tess_evaluation_shader_storage_blocks; }
		gl_int_t max_texture_buffer_size() const noexcept { return msp_max_texture_buffer_size; }
		gl_int_t max_texture_image_units() const noexcept { return msp_max_texture_image_units; }
		gl_int_t max_texture_lod_bias() const noexcept { return msp_max_texture_lod_bias; }
		gl_int_t max_texture_size() const noexcept { return msp_max_texture_size; }
		gl_int_t max_uniform_buffer_bindings() const noexcept { return msp_max_uniform_buffer_bindings; }
		gl_int_t max_uniform_block_size() const noexcept { return msp_max_uniform_block_size; }
		gl_int_t max_uniform_locations() const noexcept { return msp_max_uniform_locations; }
		gl_int_t max_varying_components() const noexcept { return msp_max_varying_components; }
		gl_int_t max_varying_vectors() const noexcept { return msp_max_varying_vectors; }
		gl_int_t max_varying_floats() const noexcept { return msp_max_varying_floats; }
		gl_int_t max_vertex_atomic_counters() const noexcept { return msp_max_vertex_atomic_counters; }
		gl_int_t max_vertex_attribs() const noexcept { return msp_max_vertex_attribs; }
		gl_int_t max_vertex_shader_storage_blocks() const noexcept { return msp_max_vertex_shader_storage_blocks; }
		gl_int_t max_vertex_texture_image_units() const noexcept { return msp_max_vertex_texture_image_units; }
		gl_int_t max_vertex_uniform_components() const noexcept { return msp_max_vertex_uniform_components; }
		gl_int_t max_vertex_uniform_vectors() const noexcept { return msp_max_vertex_uniform_vectors; }
		gl_int_t max_vertex_output_components() const noexcept { return msp_max_vertex_output_components; }
		gl_int_t max_vertex_uniform_blocks() const noexcept { return msp_max_vertex_uniform_blocks; }
		gl_int_t max_viewport_dims() const noexcept { return msp_max_viewport_dims; }
		gl_int_t max_viewports() const noexcept { return msp_max_viewports; }
		gl_int_t api_minor_version() const noexcept { return msp_api_minor_version; }
		gl_int_t num_compressed_texture_formats() const noexcept { return msp_num_compressed_texture_formats; }
		gl_int_t num_extensions() const noexcept { return msp_num_extensions; }
		gl_int_t num_program_binary_formats() const noexcept { return msp_num_program_binary_formats; }
		gl_int_t num_shader_binary_formats() const noexcept { return msp_num_shader_binary_formats; }
		gl_float_t point_size_granularity() const noexcept { return msp_point_size_granularity; }
		std::array<gl_float_t, 2> point_size_range() const noexcept { return msp_point_size_range; }
		gl_boolean_t shader_compiler() const noexcept { return msp_shader_compiler; }
		std::array<gl_float_t, 2> smooth_line_width_range() const noexcept { return msp_smooth_line_width_range; }
		gl_float_t smooth_line_width_granularity() const noexcept { return msp_smooth_line_width_granularity; }
		gl_int_t subpixel_bits() const noexcept { return msp_subpixel_bits; }
		gl_int_t max_vertex_attrib_relative_offset() const noexcept { return msp_max_vertex_attrib_relative_offset; }
		gl_int_t max_vertex_attrib_bindings() const noexcept { return msp_max_vertex_attrib_bindings; }
		std::array<gl_int_t, 2> viewport_bounds_range() const noexcept { return msp_viewport_bounds_range; }
		gl_int_t viewport_index_provoking_vertex() const noexcept { return msp_viewport_index_provoking_vertex; }
		gl_int_t viewport_subpixel_bits() const noexcept { return msp_viewport_subpixel_bits; }
		gl_int_t max_element_index() const noexcept { return msp_max_element_index; }
		gl_int_t max_color_attachments() const noexcept { return msp_max_color_attachments; }
#pragma endregion
#pragma region Dynamic parameter getters
		gl_int_t active_texture() const;
		gl_int_t array_buffer_binding() const;
		gl_boolean_t blend() const;
		gl_boolean_t blend(gl_uint_t index) const;
		std::array<gl_float_t, 4> blend_color() const;
		gl_int_t blend_dst_alpha() const;
		gl_int_t blend_dst_rgb() const;
		gl_int_t blend_equation_rgb() const;
		gl_int_t blend_equation_alpha() const;
		gl_int_t blend_src_alpha() const;
		gl_int_t blend_src_rgb() const;
		std::array<gl_float_t, 4> color_clear_value() const;
		gl_boolean_t color_logic_op() const;
		std::array<gl_boolean_t, 4> color_writemask() const;
		std::array<gl_boolean_t, 4> color_writemask(gl_uint_t index) const;
		gl_int_t dispatch_indirect_buffer_binding() const;
		gl_int_t debug_group_stack_depth() const;
		gl_boolean_t cull_face() const;
		gl_int_t cull_face_mode() const;
		gl_int_t current_program() const;
		gl_double_t depth_clear_value() const;
		gl_int_t depth_func() const;
		std::array<gl_double_t, 2> depth_range() const;
		gl_boolean_t depth_test() const;
		gl_boolean_t depth_writemask() const;
		gl_boolean_t dither() const;
		gl_int_t draw_buffer(gl_uint_t index) const;
		gl_int_t draw_framebuffer_binding() const;
		gl_int_t read_framebuffer_binding() const;
		gl_int_t element_array_buffer_binding() const;
		gl_int_t fragment_shader_derivative_hint() const;
		gl_boolean_t line_smooth() const;
		gl_int_t line_smooth_hint() const;
		gl_float_t line_width() const;
		gl_int_t logic_op_mode() const;
		gl_int_t pack_alignment() const;
		gl_int_t pack_image_height() const;
		gl_boolean_t pack_lsb_first() const;
		gl_int_t pack_row_length() const;
		gl_int_t pack_skip_images() const;
		gl_int_t pack_skip_pixels() const;
		gl_int_t pack_skip_rows() const;
		gl_boolean_t pack_swap_bytes() const;
		gl_int_t pixel_pack_buffer_binding() const;
		gl_int_t pixel_unpack_buffer_binding() const;
		gl_float_t point_fade_threshold_size() const;
		gl_int_t primitive_restart_index() const;
		gl_int_t program_pipeline_binding() const;
		gl_boolean_t program_point_size() const;
		gl_int_t provoking_vertex() const;
		gl_float_t point_size() const;
		gl_float_t polygon_offset_factor() const;
		gl_float_t polygon_offset_units() const;
		gl_boolean_t polygon_offset_fill() const;
		gl_boolean_t polygon_offset_line() const;
		gl_boolean_t polygon_offset_point() const;
		gl_boolean_t polygon_smooth() const;
		gl_int_t polygon_smooth_hint() const;
		gl_int_t read_buffer() const;
		gl_int_t renderbuffer_binding() const;
		gl_int_t sample_buffers() const;
		gl_float_t sample_coverage_value() const;
		gl_boolean_t sample_coverage_invert() const;
		gl_int_t sampler_binding() const;
		gl_int_t samples() const;
		std::array<gl_float_t, 4> scissor_box() const;
		gl_boolean_t scissor_test() const;
		gl_boolean_t scissor_test(gl_uint_t index) const;
		gl_int_t shader_storage_buffer_offset_alignment() const;
		gl_int_t shader_storage_buffer_binding() const;
		gl_int_t shader_storage_buffer_binding(gl_uint_t index) const;
		gl_int64_t shader_storage_buffer_start() const;
		gl_int64_t shader_storage_buffer_start(gl_uint_t index) const;
		gl_int64_t shader_storage_buffer_size() const;
		gl_int64_t shader_storage_buffer_size(gl_uint_t index) const;
		gl_int_t stencil_back_fail() const;
		gl_int_t stencil_back_func() const;
		gl_int_t stencil_back_pass_depth_fail() const;
		gl_int_t stencil_back_pass_depth_pass() const;
		gl_int_t stencil_back_ref() const;
		gl_int_t stencil_back_value_mask() const;
		gl_int_t stencil_back_writemask() const;
		gl_int_t stencil_clear_value() const;
		gl_int_t stencil_fail() const;
		gl_int_t stencil_func() const;
		gl_int_t stencil_pass_depth_fail() const;
		gl_int_t stencil_pass_depth_pass() const;
		gl_int_t stencil_ref() const;
		gl_boolean_t stencil_test() const;
		gl_int_t stencil_value_mask() const;
		gl_int_t stencil_writemask() const;
		gl_boolean_t stereo() const;
		gl_int_t texture_binding_1_d() const;
		gl_int_t texture_binding_1_d_array() const;
		gl_int_t texture_binding_2_d() const;
		gl_int_t texture_binding_2_d_array() const;
		gl_int_t texture_binding_2_d_multisample() const;
		gl_int_t texture_binding_2_d_multisample_array() const;
		gl_int_t texture_binding_3_d() const;
		//gl_int_t texture_binding_buffer() const;
		gl_int_t texture_binding_cube_map() const;
		gl_int_t texture_binding_rectangle() const;
		gl_int_t texture_compression_hint() const;
		gl_int_t texture_binding_buffer() const;
		gl_int_t texture_buffer_offset_alignment() const;
		gl_int64_t timestamp() const;
		gl_int_t transform_feedback_buffer_binding() const;
		gl_int_t transform_feedback_buffer_binding(gl_uint_t index) const;
		gl_int64_t transform_feedback_buffer_start(gl_uint_t index) const;
		gl_int64_t transform_feedback_buffer_size(gl_uint_t index) const;
		gl_int_t uniform_buffer_binding() const;
		gl_int_t uniform_buffer_binding(gl_uint_t index) const;
		gl_int_t uniform_buffer_offset_alignment() const;
		gl_int64_t uniform_buffer_size(gl_uint_t index) const;
		gl_int64_t uniform_buffer_start(gl_uint_t index) const;
		gl_int_t unpack_alignment() const;
		gl_int_t unpack_image_height() const;
		gl_boolean_t unpack_lsb_first() const;
		gl_int_t unpack_row_length() const;
		gl_int_t unpack_skip_images() const;
		gl_int_t unpack_skip_pixels() const;
		gl_int_t unpack_skip_rows() const;
		gl_boolean_t unpack_swap_bytes() const;
		gl_int_t vertex_array_binding() const;
		gl_int_t vertex_binding_divisor(gl_uint_t index) const;
		gl_int_t vertex_binding_offset(gl_uint_t index) const;
		gl_int_t vertex_binding_stride(gl_uint_t index) const;
		gl_int_t vertex_binding_buffer(gl_uint_t index) const;
		std::array<gl_int_t, 4> viewport() const;
#pragma endregion
	private:
#pragma region Static parameters
		std::vector<int> msp_compressed_texture_formats; //GL_COMPRESSED_TEXTURE_FORMATS <- GL_NUM_COMPRESSED_TEXTURE_FORMATS
		std::vector<int> msp_program_binary_formats; //GL_PROGRAM_BINARY_FORMATS <- GL_NUM_PROGRAM_BINARY_FORMATS
		std::array<gl_float_t, 2> msp_aliased_line_width_range; //GL_ALIASED_LINE_WIDTH_RANGE
		gl_int_t msp_max_compute_shader_storage_blocks; //GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS
		gl_int_t msp_max_combined_shader_storage_blocks; //GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS
		gl_int_t msp_max_compute_uniform_blocks; //GL_MAX_COMPUTE_UNIFORM_BLOCKS
		gl_int_t msp_max_compute_texture_image_units; //GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS
		gl_int_t msp_max_compute_uniform_components; //GL_MAX_COMPUTE_UNIFORM_COMPONENTS
		gl_int_t msp_max_compute_atomic_counters; //GL_MAX_COMPUTE_ATOMIC_COUNTERS
		gl_int_t msp_max_compute_atomic_counter_buffers; //GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS
		gl_int_t msp_max_combined_compute_uniform_components;//GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS
		gl_int_t msp_max_compute_work_group_count_x;//GL_MAX_COMPUTE_WORK_GROUP_COUNT 0
		gl_int_t msp_max_compute_work_group_count_y;//GL_MAX_COMPUTE_WORK_GROUP_COUNT 1
		gl_int_t msp_max_compute_work_group_count_z;//GL_MAX_COMPUTE_WORK_GROUP_COUNT 2
		gl_int_t msp_max_compute_work_group_size_x;//GL_MAX_COMPUTE_WORK_GROUP_SIZE 0
		gl_int_t msp_max_compute_work_group_size_y;//GL_MAX_COMPUTE_WORK_GROUP_SIZE 1
		gl_int_t msp_max_compute_work_group_size_z;//GL_MAX_COMPUTE_WORK_GROUP_SIZE 2
		gl_int_t msp_max_debug_group_stack_depth;//GL_MAX_DEBUG_GROUP_STACK_DEPTH
		gl_int_t msp_context_flags;//GL_CONTEXT_FLAGS
		gl_boolean_t msp_doublebuffer;//GL_DOUBLEBUFFER
		gl_int_t msp_implementation_color_read_format; // GL_IMPLEMENTATION_COLOR_READ_FORMAT
		gl_int_t msp_implementation_color_read_type;//GL_IMPLEMENTATION_COLOR_READ_TYPE
		gl_int_t msp_layer_provoking_vertex;//GL_LAYER_PROVOKING_VERTEX int
		gl_int_t msp_api_major_version;//GL_MAJOR_VERSION
		gl_int_t msp_max_3d_texture_size;//GL_MAX_3D_TEXTURE_SIZE
		gl_int_t msp_max_array_texture_layers;//GL_MAX_ARRAY_TEXTURE_LAYERS
		gl_int_t msp_max_clip_distances;//GL_MAX_CLIP_DISTANCES
		gl_int_t msp_max_color_texture_samples;//GL_MAX_COLOR_TEXTURE_SAMPLES
		gl_int_t msp_max_combined_atomic_counters;//GL_MAX_COMBINED_ATOMIC_COUNTERS
		gl_int_t msp_max_combined_fragment_uniform_components;//GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS
		gl_int_t msp_max_combined_geometry_uniform_components;//GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS
		gl_int_t msp_max_combined_texture_image_units;//GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS //separate ?
		gl_int_t msp_max_combined_uniform_blocks;//GL_MAX_COMBINED_UNIFORM_BLOCKS
		gl_int_t msp_max_combined_vertex_uniform_components;//GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS
		gl_int_t msp_max_cube_map_texture_size;//GL_MAX_CUBE_MAP_TEXTURE_SIZE
		gl_int_t msp_max_depth_texture_samples;//GL_MAX_DEPTH_TEXTURE_SAMPLES
		gl_int_t msp_max_draw_buffers;//GL_MAX_DRAW_BUFFERS
		gl_int_t msp_max_dual_source_draw_buffers;//GL_MAX_DUAL_SOURCE_DRAW_BUFFERS
		gl_int_t msp_max_elements_indices;//GL_MAX_ELEMENTS_INDICES
		gl_int_t msp_max_elements_vertices;//GL_MAX_ELEMENTS_VERTICES
		gl_int_t msp_max_fragment_atomic_counters;//GL_MAX_FRAGMENT_ATOMIC_COUNTERS
		gl_int_t msp_max_fragment_shader_storage_blocks;//GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS
		gl_int_t msp_max_fragment_input_components;//GL_MAX_FRAGMENT_INPUT_COMPONENTS
		gl_int_t msp_max_fragment_uniform_components;//GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
		gl_int_t msp_max_fragment_uniform_vectors;//GL_MAX_FRAGMENT_UNIFORM_VECTORS
		gl_int_t msp_max_fragment_uniform_blocks;//GL_MAX_FRAGMENT_UNIFORM_BLOCKS
		gl_int_t msp_max_framebuffer_width;//GL_MAX_FRAMEBUFFER_WIDTH
		gl_int_t msp_max_framebuffer_height;//GL_MAX_FRAMEBUFFER_HEIGHT
		gl_int_t msp_max_framebuffer_layers;//GL_MAX_FRAMEBUFFER_LAYERS
		gl_int_t msp_max_framebuffer_samples;//GL_MAX_FRAMEBUFFER_SAMPLES
		gl_int_t msp_max_geometry_atomic_counters;//GL_MAX_GEOMETRY_ATOMIC_COUNTERS
		gl_int_t msp_max_geometry_shader_storage_blocks;//GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS
		gl_int_t msp_max_geometry_input_components;//GL_MAX_GEOMETRY_INPUT_COMPONENTS
		gl_int_t msp_max_geometry_output_components;//GL_MAX_GEOMETRY_OUTPUT_COMPONENTS
		gl_int_t msp_max_geometry_texture_image_units;//GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS
		gl_int_t msp_max_geometry_uniform_blocks;//GL_MAX_GEOMETRY_UNIFORM_BLOCKS
		gl_int_t msp_max_geometry_uniform_components;//GL_MAX_GEOMETRY_UNIFORM_COMPONENTS
		gl_int_t msp_max_integer_samples;//GL_MAX_INTEGER_SAMPLES
		gl_int_t msp_min_map_buffer_alignment;//GL_MIN_MAP_BUFFER_ALIGNMENT
		gl_int_t msp_max_label_length;//GL_MAX_LABEL_LENGTH
		gl_int_t msp_max_program_texel_offset;//GL_MAX_PROGRAM_TEXEL_OFFSET
		gl_int_t msp_min_program_textel_offset;//GL_MIN_PROGRAM_TEXEL_OFFSET int
		gl_int_t msp_max_rectangle_texture_size;//GL_MAX_RECTANGLE_TEXTURE_SIZE
		gl_int_t msp_max_renderbuffer_size;//GL_MAX_RENDERBUFFER_SIZE
		gl_int_t msp_max_sample_mask_words;//GL_MAX_SAMPLE_MASK_WORDS
		gl_int_t msp_max_server_wait_timeout;//GL_MAX_SERVER_WAIT_TIMEOUT
		gl_int_t msp_max_shader_storage_buffer_bindings;//GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS
		gl_int_t msp_max_tess_control_atomic_counters;//GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS
		gl_int_t msp_max_tess_evaluation_atomic_counters;//GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS
		gl_int_t msp_max_tess_control_shader_storage_blocks;//GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS
		gl_int_t msp_max_tess_evaluation_shader_storage_blocks;//GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS
		gl_int_t msp_max_texture_buffer_size;//GL_MAX_TEXTURE_BUFFER_SIZE
		gl_int_t msp_max_texture_image_units;//GL_MAX_TEXTURE_IMAGE_UNITS
		gl_int_t msp_max_texture_lod_bias;//GL_MAX_TEXTURE_LOD_BIAS
		gl_int_t msp_max_texture_size;//GL_MAX_TEXTURE_SIZE
		gl_int_t msp_max_uniform_buffer_bindings;//GL_MAX_UNIFORM_BUFFER_BINDINGS
		gl_int_t msp_max_uniform_block_size;//GL_MAX_UNIFORM_BLOCK_SIZE
		gl_int_t msp_max_uniform_locations;//GL_MAX_UNIFORM_LOCATIONS
		gl_int_t msp_max_varying_components;//GL_MAX_VARYING_COMPONENTS
		gl_int_t msp_max_varying_vectors;//GL_MAX_VARYING_VECTORS
		gl_int_t msp_max_varying_floats;//GL_MAX_VARYING_FLOATS
		gl_int_t msp_max_vertex_atomic_counters;//GL_MAX_VERTEX_ATOMIC_COUNTERS
		gl_int_t msp_max_vertex_attribs;//GL_MAX_VERTEX_ATTRIBS
		gl_int_t msp_max_vertex_shader_storage_blocks;//GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS
		gl_int_t msp_max_vertex_texture_image_units;//GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
		gl_int_t msp_max_vertex_uniform_components;//GL_MAX_VERTEX_UNIFORM_COMPONENTS
		gl_int_t msp_max_vertex_uniform_vectors;//GL_MAX_VERTEX_UNIFORM_VECTORS
		gl_int_t msp_max_vertex_output_components;//GL_MAX_VERTEX_OUTPUT_COMPONENTS
		gl_int_t msp_max_vertex_uniform_blocks;//GL_MAX_VERTEX_UNIFORM_BLOCKS
		gl_int_t msp_max_viewport_dims;//GL_MAX_VIEWPORT_DIMS
		gl_int_t msp_max_viewports;//GL_MAX_VIEWPORTS
		gl_int_t msp_api_minor_version;//GL_MINOR_VERSION
		gl_int_t msp_num_compressed_texture_formats; //GL_NUM_COMPRESSED_TEXTURE_FORMATS
		gl_int_t msp_num_extensions;//GL_NUM_EXTENSIONS
		gl_int_t msp_num_program_binary_formats;//GL_NUM_PROGRAM_BINARY_FORMATS
		gl_int_t msp_num_shader_binary_formats;//GL_NUM_SHADER_BINARY_FORMATS
		gl_float_t msp_point_size_granularity; //GL_POINT_SIZE_GRANULARITY
		std::array<gl_float_t, 2> msp_point_size_range; //GL_POINT_SIZE_RANGE
		gl_boolean_t msp_shader_compiler; //GL_SHADER_COMPILER
		std::array<gl_float_t, 2> msp_smooth_line_width_range; //GL_SMOOTH_LINE_WIDTH_RANGE
		gl_float_t msp_smooth_line_width_granularity; //GL_SMOOTH_LINE_WIDTH_GRANULARITY
		gl_int_t msp_subpixel_bits; //GL_SUBPIXEL_BITS
		gl_int_t msp_max_vertex_attrib_relative_offset;//GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET
		gl_int_t msp_max_vertex_attrib_bindings;//GL_MAX_VERTEX_ATTRIB_BINDINGS
		std::array<gl_int_t, 2> msp_viewport_bounds_range; //GL_VIEWPORT_BOUNDS_RANGE int2
		gl_int_t msp_viewport_index_provoking_vertex; //GL_VIEWPORT_INDEX_PROVOKING_VERTEX
		gl_int_t msp_viewport_subpixel_bits; //GL_VIEWPORT_SUBPIXEL_BITS
		gl_int_t msp_max_element_index;//GL_MAX_ELEMENT_INDEX
		gl_int_t msp_max_color_attachments;//GL_MAX_COLOR_ATTACHMENTS
#pragma endregion
	};
}