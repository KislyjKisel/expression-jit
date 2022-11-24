#pragma once
#include <optional>
#include "../data/limited_stack.h"
#include "vector.h"
#include "figure.h"
#include "line_equation.h"

namespace evo::math
{
	template<std::floating_point T, unsigned D>
	constexpr Vector<T, D> midpoint(const Vector<T, D>& a, const Vector<T, D>& b) noexcept {
		return Segment<T, D>(a, b).centroid();
	}

#pragma region General distances 

	template<std::floating_point T, unsigned D>
	constexpr T distance(const Vector<T, D>& a, const Vector<T, D>& b) noexcept {
		return ( a - b ).length();
	}

	template<std::floating_point T, unsigned D>
	constexpr T distance(const Ball<T, D>& ball, const Vector<T, D> point) noexcept {
		return std::max(T(0), ( point - ball.center ).length() - ball.radius);
	}

	template<std::floating_point T, unsigned D>
	constexpr T distance(const Vector<T, D> point, const Ball<T, D>& ball) noexcept {
		return distance(ball, point);
	}

	template<std::floating_point T, unsigned D>
	constexpr T distance(const Ball<T, D>& a, const Ball<T, D>& b) noexcept {
		return std::max(T(0), distance(a.center, b.center) - ( a.radius + b.radius ));
	}

	template<std::floating_point T>
	constexpr T distance(const Line2Equation<T>& line, const Vector2<T> point) noexcept {
		return line.eval(point) / (sqrt(sqr(line.a) + sqr(line.b)));
	}

	template<std::floating_point T>
	constexpr T distance(const Vector2<T> point, const Line2Equation<T>& line) noexcept {
		return distance(line, point);
	}

	template<std::floating_point T>
	constexpr T distance(const Segment2<T>& segm, const Vector2<T> point) noexcept {
		const auto ap = point - segm.a;
		const auto ab = segm.b - segm.a;
		const auto dp = Vector2<T>::Dot(ap, ab);
		if (T(0) < dp && dp <= ab.sqrlen()) {
			return distance(Line2Equation<T>(Line2<T>(segm)), point);
		}
		else {
			return std::min(distance(segm.a, point), distance(segm.b, point));
		}
	}

	template<std::floating_point T>
	constexpr T distance(const Vector2<T> point, const Segment2<T>& segm) noexcept {
		return distance(segm, point);
	}

	template<std::floating_point T>
	constexpr T distance(const Ray2<T>& ray, const Vector2<T> point) noexcept {
		const auto ap = point - ray.origin;
		const auto dp = Vector2<T>::Dot(ap, ray.direction);
		if (T(0) < dp) {
			return distance(Line2Equation<T>(Line2<T>(ray)), point);
		}
		else {
			return distance(ray.origin, point);
		}
	}

	template<std::floating_point T>
	constexpr T distance(const Vector2<T> point, const Ray2<T>& ray) noexcept {
		return distance(ray, point);
	}

#pragma endregion

#pragma region General intersections

	template<std::floating_point T, unsigned D>
	constexpr bool intersect(const Ball<T, D>& a, const Ball<T, D>& b) noexcept {
		return math::sqr(a.radius + b.radius) <= ( a.center - b.center ).sqrlen();
	}

	template<std::floating_point T, unsigned D>
	constexpr bool intersect(const Box<T, D>& a, const Box<T, D>& b) noexcept {
		return a.min <= b.max && b.min <= a.max;
	}

	template<std::floating_point T, unsigned D>
	constexpr Box<T, D> intersection(const Box<T, D>& a, const Box<T, D>& b) noexcept {
		using Vec = Vector<T, D>;
		return { Vec::Max(a.min, b.min), Vec::Min(a.max, b.max) };
	}

	template<std::floating_point T, unsigned D>
	constexpr bool intersect(const Box<T, D>& box, const Ball<T, D>& ball) noexcept {
		using Vec = Vector<T, D>;
		return math::sqr(ball.radius) > ( Vec::Max(box.min, Vec::Min(ball.center, box.max)) - ball.center ).sqrlen();
	}

	template<std::floating_point T, unsigned D>
	constexpr T intersect(const Box<T, D>& box, const Ray<T, D>& ray) noexcept {
		using Vec = Vector<T, D>;
		Vec t1 = ( box.min - ray.origin ) / ray.direction;
		Vec t2 = ( box.max - ray.origin ) / ray.direction;
		const auto [tmin, tmax] = Vec::MinMax(t1, t2);
		return tmin.max() <= tmax.min();
	}

#pragma endregion

#pragma region Intersection of Ball and Line/Ray/Segment

	namespace detail 
	{
		template<std::floating_point T, unsigned D>
		constexpr LimitedStack<T, 2> intersection_ball_line_impl(
			const Vector<T, D> ballCenter, const T ballSqrRadius, 
			const Vector<T, D> linePoint, const Vector<T, D> lineDirection, const T lineDirSqrlen
		) {
			using Vec = Vector<T, D>;
			const Vec CO = linePoint - ballCenter;
			const T A = lineDirSqrlen;
			const T B = T(2) * Vec::Dot(lineDirection, CO);
			const T C = CO.sqrlen() - ballSqrRadius;
			const T Discriminant = B * B - T(4) * A * C;

			if (math::is_zero(Discriminant)) [[unlikely]] {
				const T t = -B / ( T(2) * A );
				return { t };
			}
			else if (Discriminant < T(0)) [[likely]] return { };
			else {
				const T itwoA = T(1) / ( T(2) * A );
				const T negB = -B;
				const T t1 = ( negB + sqrt(Discriminant) ) * itwoA;
				const T t2 = ( negB - sqrt(Discriminant) ) * itwoA;
				return { t1, t2 };
			}
		}
	}
	
	template<std::floating_point T, unsigned D>
	constexpr LimitedStack<Vector<T, D>, 2> intersection_ball_line(
		const Vector<T, D> ballCenter, const T ballSqrRadius,
		const Vector<T, D> linePoint, const Vector<T, D> lineDirection, const T lineDirSqrlen
	) { 
		const auto ts = detail::intersection_ball_line_impl<T, D>
			(ballCenter, ballSqrRadius, linePoint, lineDirection, lineDirSqrlen);

		LimitedStack<Vector<T, D>, 2> r;
		for (size_t i = 0; i < ts.size(); ++i) r.push_back(linePoint + lineDirection * ts[i]);
		return r;
	}

	template<std::floating_point T, unsigned D>
	constexpr LimitedStack<Vector<T, D>, 2> intersection(const Ball<T, D>& ball, const Line<T, D>& line) noexcept {
		const auto direction = line.b - line.a;
		return intersection_ball_line<T, D>(ball.center, ball.radius * ball.radius, line.a, direction, direction.sqrlen());
	}

	template<std::floating_point T, unsigned D>
	constexpr auto intersection(const Line<T, D>& line, const Ball<T, D>& ball) noexcept {
		return intersection(ball, line);
	}

	template<std::floating_point T, unsigned D>
	constexpr LimitedStack<T, 2> intersection_distance_ball_ray(
		const Vector<T, D> ballCenter, const T ballSqrRadius,
		const Vector<T, D> rayOrigin, const Vector<T, D> rayDirection, const T rayDirSqrlen
	) {
		return detail::intersection_ball_line_impl<T, D>(ballCenter, ballSqrRadius, rayOrigin, rayDirection, rayDirSqrlen);
	}

	template<std::floating_point T, unsigned D>
	constexpr LimitedStack<Vector<T, D>, 2> intersection_ball_ray(
		const Vector<T, D> ballCenter, const T ballSqrRadius,
		const Vector<T, D> rayOrigin, const Vector<T, D> rayDirection, const T rayDirSqrlen
	) {
		const auto ts = detail::intersection_ball_line_impl<T, D>
			(ballCenter, ballSqrRadius, rayOrigin, rayDirection, rayDirSqrlen);

		LimitedStack<Vector<T, D>, 2> r;
		for (size_t i = 0; i < ts.size(); ++i) if(T(0) <= ts[i]) r.push_back(rayOrigin + rayDirection * ts[i]);
		return r;
	}

	template<std::floating_point T, unsigned D>
	constexpr LimitedStack<Vector<T, D>, 2> intersection(const Ball<T, D>& ball, const Ray<T, D>& ray) noexcept {
		return intersection_ball_ray<T, D>(ball.center, ball.radius * ball.radius, ray.origin, ray.direction, ray.direction.sqrlen());
	}

	template<std::floating_point T, unsigned D>
	constexpr LimitedStack<T, 2> intersection_distance(const Ball<T, D>& ball, const Ray<T, D>& ray) noexcept {
		return intersection_distance_ball_ray<T, D>(ball.center, ball.radius * ball.radius, ray.origin, ray.direction, ray.direction.sqrlen());
	}

	template<std::floating_point T, unsigned D>
	constexpr auto intersection(const Ray<T, D>& ray, const Ball<T, D>& ball) noexcept {
		return intersection(ball, ray);
	}

	template<std::floating_point T, unsigned D>
	constexpr LimitedStack<Vector<T, D>, 2> intersection_ball_segment(
		const Vector<T, D> ballCenter, const T ballSqrRadius,
		const Vector<T, D> segmPoint, const Vector<T, D> segmDirection, const T segDirectionSqr
	) {
		const auto ts = detail::intersection_ball_line_impl<T, D>
			(ballCenter, ballSqrRadius, segmPoint, segmDirection, segDirectionSqr);

		LimitedStack<Vector<T, D>, 2> r;
		for (size_t i = 0; i < ts.size(); ++i) if (T(0) <= ts[i] && ts[i] <= T(1)) r.push_back(segmPoint + segmDirection * ts[i]);
		return r;
	}

	template<std::floating_point T, unsigned D>
	constexpr LimitedStack<Vector <T, D>, 2> intersection(const Ball<T, D>& ball, const Segment<T, D>& segm) noexcept {
		const auto direction = segm.b - segm.a;
		return intersection_ball_segment<T, D>(ball.center, ball.radius * ball.radius, segm.a, direction, direction.sqrlen());
	}

	template<std::floating_point T, unsigned D>
	constexpr auto intersection(const Segment<T, D>& segm, const Ball<T, D>& ball) noexcept {
		return intersection(ball, segm);
	}

#pragma endregion

#pragma region Intersection of 2D lines/segments/rays
	
	namespace detail
	{
		template<std::floating_point T>
		constexpr std::array<T, 4> line_pair_equation_values(
			const Line2<T>& l1, const Line2Equation<T>& eq1, const Line2<T>& l2, const Line2Equation<T>& eq2
		) noexcept {
			return { eq1.eval(l2.a), eq1.eval(l2.b), eq2.eval(l1.a), eq2.eval(l1.b) };
		}
	}

	template<std::floating_point T>
	constexpr bool intersect(
		const Line2<T>& l1, const Line2Equation<T>& eq1, const Line2<T>& l2, const Line2Equation<T>& eq2
	) noexcept {
		const auto [v11, v12, v21, v22] = line_pair_equation_values(l1, eq1, l2, eq2);

		if (is_zero(v11) || is_zero(v12) || is_zero(v21) || is_zero(v22)) return true;
		
		const bool b1z = is_zero(eq1.b);
		const bool b2z = is_zero(eq2.b);

		if (b1z || b2z) {
			return b1z ^ b2z;
		}
		else {
			return !equal(eq1.a / eq1.b, eq2.a / eq2.b);
		}
	}

	template<std::floating_point T>
	constexpr bool intersect(const Line2<T>& a, const Line2<T>& b) noexcept {
		return intersect(a, Line2Equation<T>(a), b, Line2Equation<T>(b));
	}

	template<std::floating_point T>
	constexpr bool intersect(
		const Segment2<T>& l1, const Line2Equation<T>& eq1, const Segment2<T>& l2, const Line2Equation<T>& eq2
	) noexcept {
		const auto [v11, v12, v21, v22] = line_pair_equation_values(l1, eq1, l2, eq2);

		if (math::is_zero(v11) || math::is_zero(v12) || math::is_zero(v21) || math::is_zero(v22)) {
			return ( l2.b.x <= l1.a.x && l1.a.x <= l2.b.x ) || ( l2.b.x <= l1.b.x && l1.b.x <= l2.b.x );
		}
		return ( v11 > T(0) ^ v12 > T(0) ) && ( v21 > T(0) ^ v22 > T(0) );
	}

	template<std::floating_point T>
	constexpr bool intersect(const Segment2<T>& a, const Segment2<T>& b) noexcept {
		return intersect(a, Line2Equation<T>(Line2<T>(a)), b, Line2Equation<T>(Line2<T>(b)));
	}

	template<std::floating_point T>
	constexpr std::optional<Vector2<T>> intersection(
		const Line2<T>& a, const Line2Equation<T>& e1, const Line2<T>& b, const Line2Equation<T>& e2
	) noexcept {
		if (is_zero(e1.a)) {
			const T d = e1.b * e2.a; // e1.b * e2.a - e2.b * e1.a;
			if (is_zero(d)) return std::nullopt;
			const T n = e1.c * e2.b - e2.c * e1.b;
			const T x = n / d;
			const T y = -e1.c / e1.b; // (-e1.a * x - e1.c) / e1.b;
			return Vector2<T>(x, y);
		}
		else {
			const T d = e1.a * e2.b - e2.a * e1.b;
			if (is_zero(d)) return std::nullopt;
			const T n = e1.c * e2.a - e2.c * e1.a;
			const T y = n / d;
			const T x = ( -e1.b * y - e1.c ) / e1.a;
			return Vector2<T>(x, y);
		}
	}

	template<std::floating_point T>
	constexpr auto intersection(const Line2<T>& a, const Line2<T>& b) noexcept {
		return intersection(a, Line2Equation<T>(a), b, Line2Equation<T>(b));
	}

	template<std::floating_point T>
	constexpr std::optional<Vector2<T>> intersection(
		const Segment2<T>& a, const Line2Equation<T>& e1, const Segment2<T>& b, const Line2Equation<T>& e2
	) noexcept {
		auto lineIntersection = intersection(Line2<T>(a), e1, Line2<T>(b), e2);
		if (lineIntersection.has_value()) {
			const auto& p = lineIntersection.value();
			if (is_zero(e1.b)) [[unlikely]] {
				if (is_zero(e2.b)) [[unlikely]] {
					if (std::min(b.a.y, b.b.y) <= p.y && p.y <= std::max(b.a.y, b.b.y) && 
						std::min(a.a.y, a.b.y) <= p.y && p.y <= std::max(a.a.y, a.b.y)) return lineIntersection;
					else return std::nullopt;
				}
				if (std::min(b.a.x, b.b.x) <= p.x && p.x <= std::max(b.a.x, b.b.x)) return lineIntersection;
				else return std::nullopt;
			}
			if (std::min(b.a.x, b.b.x) <= p.x && p.x <= std::max(b.a.x, b.b.x) && 
				std::min(a.a.x, a.b.x) <= p.x && p.x <= std::max(a.a.x, a.b.x)) return lineIntersection;
			else return std::nullopt;
		}
		return std::nullopt;
	}

	template<std::floating_point T>
	constexpr auto intersection(const Segment2<T>& a, const Segment2<T>& b) noexcept {
		return intersection(a, Line2Equation<T>(Line2<T>(a)), b, Line2Equation<T>(Line2<T>(b)));
	}

#pragma endregion
}