#pragma once
#include "../util/concepts.h"
#include "vector.h"
#include "segment.h"
#include "line.h"
#include "box.h"
#include "ray.h"
#include "ball.h"

namespace evo::math
{
	template<typename T>
	struct figure_traits { using type = void; inline static constexpr unsigned Dimension = 0; };

	template<template<typename, unsigned> typename T, std::floating_point V, unsigned D>
	struct figure_traits<T<V, D>> { using type = V; inline static constexpr unsigned Dimension = D; };

	template<typename T>
	using figure_value_t = typename figure_traits<T>::type;

	template<typename T>
	constexpr unsigned dimension_of = figure_traits<T>::Dimension;

	template<typename Base, template<typename, unsigned> typename Template>
	using equivalent_figure_t = Template<figure_value_t<Base>, dimension_of<Base>>;
}