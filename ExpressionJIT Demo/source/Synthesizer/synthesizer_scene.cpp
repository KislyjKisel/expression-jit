#include "synthesizer_scene.h"
#include <portaudiocpp/AutoSystem.hxx>
#include <imgui/imgui.h>
#include <NoisePollution/synthesizer.h>
#include "expression_oscillator.h"

#define NOMINMAX
#include <Windows.h>

namespace ed
{
	const char* InputFloatFormat = "%5.3f";
	constexpr float stdDelay = 0.01;
	constexpr float stdAttack = 0.15;
	constexpr float stdHold = 0.03;
	constexpr float stdDecay = 0.07;
	constexpr float stdSustain = 0.6;
	constexpr float stdRelease = 0.3;

	np::EnvelopeParametersDAHDSR stdEnvelope(stdDelay, stdAttack, stdHold, stdDecay, stdSustain, stdRelease);

	void SynthesizerScene::addKey(np::ScientificPitchName note) {
		float freq = np::Pitch(note);

		synthesizer->addKey(np::SynthesizerKey(std::make_unique<np::EnvelopeGeneratorDAHDSR>(stdEnvelope), nullptr));

		keys.push_back({ .func = "sin x" });
		keys[keys.size() - 1].frequency = freq;
		keys[keys.size() - 1].delay = stdDelay;
		keys[keys.size() - 1].attack = stdAttack;
		keys[keys.size() - 1].hold = stdHold;
		keys[keys.size() - 1].decay = stdDecay;
		keys[keys.size() - 1].sustain = stdSustain;
		keys[keys.size() - 1].release = stdRelease;
		keys[keys.size() - 1].enabled = false;

	}

	void SynthesizerScene::initialize() {
		compiler.arg('x', 0, exprjit::DataType::Float);

		audioSystem = std::make_unique<portaudio::AutoSystem>();
		synthesizer = std::make_unique<np::Synthesizer>(41000, true);

		for (size_t ki = 0; ki < 9; ++ki) {
			addKey(np::ScientificPitchNameFromNoteOctave('A', ki));
		}

		synthesizer->start();
	}

	void SynthesizerScene::update() {
		for (size_t i = 0; i < synthesizer->keyCount(); ++i) {
			synthesizer->setKeyState(i, GetAsyncKeyState('1' + i) & 0x8000);
		}
	}

	void SynthesizerScene::imguiSynthKey(size_t i) {
		ImGui::PushID(i);

		ImGui::InputText("f(x)", keys[i].func, sizeof(keys[i].func));
		if (ImGui::InputFloat("Frequency", &keys[i].frequency, 0, 0, InputFloatFormat)) {
			if(keys[i].enabled) synthesizer->setKeyFrequency(i, keys[i].frequency);
		}
		ImGui::Separator();
		ImGui::Text("Envelope DAHDSR");
		ImGui::InputFloat("Delay", &keys[i].delay, 0, 0, InputFloatFormat);
		ImGui::InputFloat("Attack", &keys[i].attack, 0, 0, InputFloatFormat);
		ImGui::InputFloat("Hold", &keys[i].hold, 0, 0, InputFloatFormat);
		ImGui::InputFloat("Decay", &keys[i].decay, 0, 0, InputFloatFormat);
		ImGui::InputFloat("Sustain", &keys[i].sustain, 0, 0, InputFloatFormat);
		ImGui::InputFloat("Release", &keys[i].release, 0, 0, InputFloatFormat);
		ImGui::Separator();

		static std::string perrtext;
		if (ImGui::Button("Apply")) {
			keys[i].enabled = false;
			synthesizer->setKeyEnabled(i, false);
			synthesizer->setKeyEnvelope(i, std::make_unique<np::EnvelopeGeneratorDAHDSR>(np::EnvelopeParametersDAHDSR(
				keys[i].delay, keys[i].attack, keys[i].hold, keys[i].decay, keys[i].sustain, keys[i].release
			)));
			exprjit::Function<double(double)>* f = nullptr;
			try {
				f = compiler.compile<double, double>(keys[i].func);
			}
			catch (exprjit::ParserException pe) {
				perrtext = pe.what();
				ImGui::OpenPopup("Parser Error");
			}
			if (perrtext.empty()) {
				synthesizer->setKeyOscillator(i, 
					std::make_unique<ExpressionOscillator>(
						std::unique_ptr< exprjit::Function<double(double)>>(f), keys[i].frequency
					)
				);
				synthesizer->setKeyEnabled(i, true);
				keys[i].enabled = true;
			}
		}
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::BeginPopup("Parser Error")) {
			ImGui::Text(perrtext.c_str());
			if (ImGui::Button("OK"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		ImGui::PopID();
	}

	void SynthesizerScene::gui() {
		ImGui::Begin("Synthesizer", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Key %i", (int)(selectedKey + 1));
		imguiSynthKey(selectedKey);
		if (ImGui::Button("<-")) {
			if (selectedKey == 0)
				selectedKey = 8;
			else
				--selectedKey;
		}
		ImGui::SameLine();
		if (ImGui::Button("->")) {
			if (selectedKey == 8)
				selectedKey = 0;
			else
				++selectedKey;
		}

		ImGui::End();
	}

	void SynthesizerScene::terminate() {
		synthesizer->stop();
	}

	SynthesizerScene::SynthesizerScene() = default;
	SynthesizerScene::~SynthesizerScene() = default;
}