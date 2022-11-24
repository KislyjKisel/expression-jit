#pragma once
#include <memory>
#include <NoisePollution/oscillator.h>
#include <exprjit/function.h>
#include "../expression_compiler.h"

namespace ed
{
	class ExpressionOscillator : public np::Oscillator {
	public:
		ExpressionOscillator(std::unique_ptr<exprjit::Function<double(double)>>&& f, float frequency) 
			: np::Oscillator(frequency), f(std::move(f)) { }

		void setFunction(std::unique_ptr<exprjit::Function<double(double)>>&& f) noexcept {
			this->f = std::move(f);
		}

	private:
		float wave(float x) const override {
			return (float)(*f)(x);
		}

		std::unique_ptr<exprjit::Function<double(double)>> f;
	};
}