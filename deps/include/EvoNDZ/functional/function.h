#pragma once
#include <functional>
#include "../meta/type_pack.h"
#include "../util/concepts.h"
#include "../util/util.h"

namespace evo
{
	namespace detail
	{
		template<typename Func, typename Arg, typename... Args>
		struct curried_res {
		public:
			using result_type = std::invoke_result_t<Func, Arg, Args...>;

			constexpr curried_res(const Func& function, const Arg& first) : m_function(function), m_first(first) { }

			constexpr result_type operator()(const Args&... args) const {
				return m_function(m_first, args...);
			}

		private:
			std::decay_t<Func> m_function;
			Arg m_first;
		};

		template<typename Func, typename Arg, typename... Args>
		struct curried_poly {
		public:
			using result_type = std::invoke_result_t<Func, Arg, Args...>;

			constexpr curried_poly(const Func& function) : m_function(function) { }

			constexpr auto operator()(const Arg& x) const {
				return curried_res<Func, Arg, Args...>(m_function, x);
			}

		private:
			std::decay_t<Func> m_function;
		};
	}

	template<typename F, typename... Args>
	class function {
	public:
		using result_type = std::invoke_result_t<F, Args...>;
		using func_t = std::decay_t<F>;

		// todo: useless? function() constructor: usage function<void(*)(arg), arg>() <- verbose. Usage - static array? mb
		constexpr function() requires std::same_as<result_type, void> && std::convertible_to<decltype(noop_fn_ptr<Args...>), F> 
			: m_function(noop_fn_ptr) {}


		constexpr function(const function& f) noexcept(std::is_nothrow_copy_constructible_v<F>) : m_function(f.m_function) { }
		constexpr function(function&& f) noexcept( std::is_nothrow_move_constructible_v<F> ) 
			: m_function(std::move(f.m_function)) { }

		explicit constexpr function(const func_t& function) noexcept( std::is_nothrow_copy_constructible_v<F> )
			: m_function(function) { }
		explicit constexpr function(func_t&& function) noexcept( std::is_nothrow_move_constructible_v<F> )
			: m_function(std::move(function)) { }

		template<typename... TArgs> requires (sizeof...(TArgs) == sizeof...(Args) &&
			std::same_as<
				typename TypePack<TArgs...>::template Morph<std::remove_cvref_t>,
				typename TypePack<Args...>::template Morph<std::remove_cvref_t>>)
		constexpr result_type operator()(TArgs&&... args) 
			const noexcept( std::is_nothrow_invocable_r_v<result_type, F, TArgs...> ) 
		{
			return m_function(std::forward<TArgs>(args)...);
		}

		template<typename TArg> requires ( std::convertible_to<TArg, indexed_t<0, Args...>> && sizeof...( Args ) > 1 )
		constexpr auto operator()(TArg&& arg) const;

		template<typename... TArgs> requires ( 1 < sizeof...( TArgs ) && sizeof...( TArgs ) < sizeof...( Args ) &&
			std::same_as<
			typename TypePack<TArgs...>::template Morph<std::remove_cvref_t>,
			typename TypePack<Args...>::template Take<sizeof...(TArgs)>::template Morph<std::remove_cvref_t>> )
		constexpr auto operator()(TArgs&&... args) const;

	private:
		func_t m_function;
	};

	template<typename R, typename... Args, typename F = R(*)(Args...)>
	constexpr function<F, Args...> to_function(R( * f)( Args... )) {
		return function<F, Args...>(f);
	}

	template<typename R, typename... Args, typename F = std::function<R(Args...)>>
	function<F, Args...> to_function(const std::function<R(Args...)>& f) {
		return function(f);
	}

	template<typename R, typename... Args, typename F = std::function<R(Args...)>>
	function<F, Args...> to_function(std::function<R(Args...)>&& f) {
		return function(std::move(f));
	}

	template<typename F, typename... Args>
	constexpr function<F, Args...> to_function(const function<F, Args...>& f) {
		return function<F, Args...>(f);
	}

	template<typename... Args, typename F>
	constexpr function<F, Args...> pick_function(F&& f) {
		return function<F, Args...>(std::forward<F>(f));
	}

	template<typename Fc, typename... FArgs, typename ChF>
	constexpr function<ChF, FArgs...> pick_function_similar(ChF&& pick_from, const function<Fc, FArgs...>& similar_to) {
		return pick_function<FArgs...>(std::forward<ChF>(pick_from));
	}

	template<typename... Args, typename CFunc, typename CArg>
	constexpr auto to_function(const detail::curried_res<CFunc, CArg, Args...>& f) {
		return pick_function<Args...>(f);
	}

	template<typename... Args, typename CFunc, typename CArg>
	constexpr auto to_function(const detail::curried_poly<CFunc, CArg, Args...>& f) {
		return pick_function<CArg>(f);
	}

	template<typename Arg, typename... Args, typename F>
	constexpr auto curry(const function<F, Arg, Args...>& f) {
		return to_function(detail::curried_poly<function<F, Arg, Args...>, Arg, Args...>(f));
	}

	template<typename T>
	concept Function = requires( T f ) {
		{ to_function(f) } -> Specialization<function>;
	};

	template<typename T>
	concept NonUniFunction = Function<T> && !is_specialization_v<T, function>;


	template<NonUniFunction F>
	constexpr auto curry(const F& f) {
		return curry(to_function(f));
	}

	template<typename F, typename... Args>
	template<typename TArg> requires ( std::convertible_to<TArg, indexed_t<0, Args...>> && sizeof...( Args ) > 1 )
	constexpr auto function<F, Args...>::operator()(TArg&& arg) const {
		return to_function(curry(*this)(std::forward<TArg>(arg)));
	}

	template<Function F, Function G>
	constexpr auto compose(F f, G g) {
		return pick_function_similar(
			[=]<typename... T>( T&&... args ) constexpr { return f(g(std::forward<T>(args)...)); }, 
			to_function(g)
		);
	}

	namespace detail
	{
		template<typename F, typename... B, typename... A>
		constexpr auto partial_(F&& f, TypePack<A...>, B... bound) {
			return pick_function<A...>([=](A&&... args) { return f(bound..., args...); });
		}
	}

	template<typename F, typename... Args>
	template<typename... TArgs> requires ( 1 < sizeof...( TArgs ) && sizeof...( TArgs ) < sizeof...( Args ) &&
		std::same_as<
		typename TypePack<TArgs...>::template Morph<std::remove_cvref_t>,
		typename TypePack<Args...>::template Take<sizeof...( TArgs )>::template Morph<std::remove_cvref_t>> )
	constexpr auto function<F, Args...>::operator()(TArgs&&... args) const 
	{
		return detail::partial_(*this, typename TypePack<Args...>::template Drop<sizeof...( TArgs )>{}, args...);
	}
}