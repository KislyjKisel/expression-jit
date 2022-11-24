#pragma once
#include <memory>
#include <vector>
#include <EvoNDZ/app/scene.h>
#include <NoisePollution/scientific_pitch_notation.h>
#include <NoisePollution/envelope/envelope_dahdsr.h>
#include "../expression_compiler.h"

namespace portaudio
{
	class AutoSystem;
}
namespace np
{
	class Synthesizer;
}

namespace ed
{
	class SynthesizerScene : public evo::Scene {
	public:
		void initialize()  override;
		void update()  override;
		void render()  override { }
		void gui() override;
		void terminate() override;

		SynthesizerScene();
		~SynthesizerScene();

	private:
		std::unique_ptr<portaudio::AutoSystem> audioSystem;
		std::unique_ptr<np::Synthesizer> synthesizer;
		ExpressionCompiler compiler;

		struct KeyConfig {
			char func[128];
			float frequency;
			float delay;
			float attack;
			float hold;
			float decay;
			float sustain;
			float release;
			bool enabled;
		};
		size_t selectedKey = 0;
		std::vector<KeyConfig> keys;

		void imguiSynthKey(size_t);
		void addKey(np::ScientificPitchName note);
	};	
}