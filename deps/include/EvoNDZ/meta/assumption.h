#pragma once

namespace evo
{
	template<unsigned Index = 0>
	struct some { };

	using ﹖a = some<0>;
	using ﹖b = some<1>;
	using ﹖c = some<2>;
	using ﹖d = some<3>;
	using ﹖e = some<4>;
	using ﹖f = some<5>;
	using ﹖g = some<6>;
	using ﹖h = some<7>;

	template<typename Result, typename... Args>
	struct some_function {
		Result operator()(Args...);
	};
}