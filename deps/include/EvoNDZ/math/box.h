#pragma once
#include "../util/util.h"
#include "segment.h"
#include "vector.h"
#include "ball.h"

namespace evo::math
{
	template<std::floating_point T, unsigned Dimension>
	struct Box {
		using vector_type = evo::Vector<T, Dimension>;

		vector_type min;
		vector_type max;

		Box() noexcept = default;
		constexpr Box(const vector_type min, const vector_type max) noexcept : min(min), max(max) { }
		
		constexpr explicit Box(const Ball<T, Dimension>& ball) noexcept 
			: min(ball.center - ball.radius), max(ball.center + ball.radius) { }

		constexpr explicit Box(const Segment<T, Dimension>& line) noexcept {
			*this = Between(line.a, line.b);
		}

		constexpr static Box Between(const vector_type a, const vector_type b) noexcept {
			const auto [m, M] = vector_type::MinMax(a, b);
			return { m, M };
		}

		constexpr vector_type centroid() const noexcept {
			return ( min + max ) * T(0.5l);
		}
		constexpr vector_type size() const noexcept {
			return max - min;
		}

		constexpr bool contains(const vector_type p) const noexcept {
			return min <= p && p <= max;
		}
		constexpr bool contains(const Box& other) const  noexcept {
			return min <= other.min && other.max <= max;
		}
		constexpr bool contains(const Segment<T, Dimension>& segm) const noexcept {
			return contains(segm.a) && contains(segm.b);
		}

		constexpr Box enclose(const vector_type point) const noexcept {
			return { vector_type::Min(min, point), vector_type::Max(max, point) };
		}
		constexpr Box enclose(const Box& other) const noexcept {
			return { vector_type::Min(min, other.min), vector_type::Max(max, other.max) };
		}
		constexpr Box enclose(const Segment<T, Dimension>& segm) const noexcept {
			return enclose(segm.a).enclose(segm.b);
		}
		constexpr Box enclose(const Ball<T, Dimension>& ball) const noexcept {
			return enclose(Box(ball));
		}
	};

	template<std::floating_point T>
	using Box2 = Box<T, 2>;

	using Box2f = Box2<float>;
	using Box2d = Box2<double>;
	using Box2l = Box2<long double>;

	template<std::floating_point T>
	using Box3 = Box<T, 3>;

	using Box3f = Box3<float>;
	using Box3d = Box3<double>;
	using Box3l = Box3<long double>;
}

/*


constexpr std::array<Line2<T>, 4> borders() const noexcept {
			const auto box_l1 = Line2<T> { upper_left(),   upper_right()  };
			const auto box_l2 = Line2<T> { upper_right(),  bottom_right() };
			const auto box_l3 = Line2<T> { bottom_right(), bottom_left()  };
			const auto box_l4 = Line2<T> { bottom_left(),  upper_left()   };
			return { box_l1, box_l2, box_l3, box_l4 };
		}

		constexpr static bool Intersect(
			std::array<std::pair<Line2<T>, typename Line2<T>::Equation>, 4> boxLines,
			const Line2<T>& line, typename Line2<T>::Equation lineEquation
		) noexcept {
			return Line2<T>::Intersect(line, lineEquation, boxLines[0].first, boxLines[0].second)
				|| Line2<T>::Intersect(line, lineEquation, boxLines[1].first, boxLines[1].second)
				|| Line2<T>::Intersect(line, lineEquation, boxLines[2].first, boxLines[2].second)
				|| Line2<T>::Intersect(line, lineEquation, boxLines[3].first, boxLines[3].second);
		}

		constexpr static bool Intersect(const Box& box, const Line2<T>& line) {
			return Intersect(
				transform_array(box.borders(), [](const Line2<T>& bl) { return std::make_pair(bl, bl.equation()); }),
				line, line.equation()
			);
		}


		constexpr vector_type upper_left() const noexcept {
			return { a.x, b.y };
		}
		constexpr vector_type bottom_left() const noexcept {
			return a;
		}
		constexpr vector_type upper_right() const noexcept {
			return b;
		}
		constexpr vector_type bottom_right() const noexcept {
			return { b.x, a.y };
		}
		constexpr T left() const noexcept {
			return a.x;
		}
		constexpr T right() const noexcept {
			return b.x;
		}
		constexpr T bottom() const noexcept {
			return a.y;
		}
		constexpr T top() const noexcept {
			return b.y;
		}

*/