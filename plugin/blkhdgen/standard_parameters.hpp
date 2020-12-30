#pragma once

#include <sstream>
#include "envelope_spec.hpp"
#include "slider_spec.hpp"

namespace blkhdgen {
namespace std_params {

inline std::string amp_display(float v)
{
	std::stringstream ss;

	ss << v << " dB";

	return ss.str();
}

inline std::string pan_display(float v)
{
	std::stringstream ss;

	if (v < 0.0f)
	{
		ss << v << " L";
	}
	else if (v > 0.0f)
	{
		ss << v << " R";
	}
	else
	{
		ss << "Center";
	}

	return ss.str();
}

inline std::string speed_display(float v)
{
	std::stringstream ss;

	if (v == 0.0f)
	{
		ss << "Freeze";
	}
	else if (v == 0.25f)
	{
		ss << v << "Quarter";
	}
	else if (v == 0.5f)
	{
		ss << v << "Half";
	}
	else if (v == 1.0f)
	{
		ss << v << "Normal";
	}
	else if (v == 2.0f)
	{
		ss << v << "Double";
	}
	else
	{
		ss << v;
	}

	return ss.str();
}

namespace envelopes {

inline EnvelopeSpec amp()
{
	EnvelopeSpec out;

	out.uuid = "273e7c30-404b-4db6-ba97-20f33d49fe51";
	out.name = "Amp";

	out.transform = [](float v)
	{
		return math::db_to_af(v);
	};

	out.inverse_transform = [](float v)
	{
		return math::af_to_db(v);
	};

	out.display_value = amp_display;

	out.range.min_range.range.min = -60.0f;
	out.range.min_range.range.max = -60.0f;
	out.range.min_range.value = -60.0f;
	out.range.min_range.step_size = 0.0f;

	out.range.max_range.range.min = -60.0f;
	out.range.max_range.range.max = 12.0f;
	out.range.max_range.value = 0.0f;
	out.range.max_range.step_size = 0.0;

	out.step_size.range.min = 0.0f;
	out.step_size.range.max = 0.0f;
	out.step_size.value = 0.0f;
	out.step_size.step_size = 0.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 0.0f;

	return out;
}

inline EnvelopeSpec pan()
{
	EnvelopeSpec out;

	out.uuid = "9c312a2c-a1b4-4a8d-ab68-07ea157c4574";
	out.name = "Pan";

	out.transform = [](float v)
	{
		return v;
	};

	out.inverse_transform = [](float v)
	{
		return v;
	};

	out.display_value = pan_display;

	out.range.min_range.range.min = -1.0f;
	out.range.min_range.range.max = -1.0f;
	out.range.min_range.value = -1.0f;
	out.range.min_range.step_size = 0.0f;

	out.range.max_range.range.min = -1.0f;
	out.range.max_range.range.max = 1.0f;
	out.range.max_range.value = 1.0f;
	out.range.max_range.step_size = 0.0;

	out.step_size.range.min = 1.0f;
	out.step_size.range.max = 1.0f;
	out.step_size.value = 1.0f;
	out.step_size.step_size = 0.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 0.0f;

	return out;
}

inline EnvelopeSpec pitch()
{
	EnvelopeSpec out;

	out.uuid = "ca2529db-e7bd-4019-9a07-22aee24526d1";
	out.name = "Pitch";

	out.transform = [](float v)
	{
		return v;
	};

	out.inverse_transform = [](float v)
	{
		return v;
	};

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.range.min_range.range.min = -60.0f;
	out.range.min_range.range.max = 0.0f;
	out.range.min_range.value = -24.0f;
	out.range.min_range.step_size = 1.0f;

	out.range.max_range.range.min = -60.0f;
	out.range.max_range.range.max = 60.0f;
	out.range.max_range.value = 24.0f;
	out.range.max_range.step_size = 1.0;

	out.step_size.range.min = 0.0f;
	out.step_size.range.max = 60.0f;
	out.step_size.value = 1.0f;
	out.step_size.step_size = 1.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 1.0f;

	return out;
}

inline EnvelopeSpec speed()
{
	EnvelopeSpec out;

	out.uuid = "02f68738-f54a-4f35-947b-c30e73896aa4";
	out.name = "Speed";

	out.transform = [](float v)
	{
		return std::pow(v, 2.0f);
	};

	out.inverse_transform = [](float v)
	{
		return std::sqrt(v);
	};

	out.display_value = speed_display;

	out.range.min_range.range.min = 0.0f;
	out.range.min_range.range.max = 1.0f;
	out.range.min_range.value = 0.0f;
	out.range.min_range.step_size = 0.0f;

	out.range.max_range.range.min = 0.5f;
	out.range.max_range.range.max = 32.0f;
	out.range.max_range.value = 2.0f;
	out.range.max_range.step_size = 0.0f;

	out.step_size.range.min = 0.0f;
	out.step_size.range.max = 32.0f;
	out.step_size.value = 1.0f;
	out.step_size.step_size = 0.25f;

	out.default_value = 1.0f;
	out.default_snap_amount = 0.0f;

	return out;
}

inline EnvelopeSpec formant()
{
	EnvelopeSpec out;

	out.uuid = "7b72dbef-e36d-4dce-958b-b0fa498ae41e";
	out.name = "Formant";

	out.transform = [](float v)
	{
		return v;
	};

	out.inverse_transform = [](float v)
	{
		return v;
	};

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.range.min_range.range.min = -60.0f;
	out.range.min_range.range.max = 0.0f;
	out.range.min_range.value = -24.0f;
	out.range.min_range.step_size = 1.0f;

	out.range.max_range.range.min = -60.0f;
	out.range.max_range.range.max = 60.0f;
	out.range.max_range.value = 24.0f;
	out.range.max_range.step_size = 1.0;

	out.step_size.range.min = 0.0f;
	out.step_size.range.max = 60.0f;
	out.step_size.value = 1.0f;
	out.step_size.step_size = 1.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 1.0f;

	return out;
}

inline EnvelopeSpec noise_amount()
{
	EnvelopeSpec out;

	out.uuid = "29d5ecb5-cb5d-4f19-afd3-835dd805682a";
	out.name = "Noise Amount";

	out.transform = [](float v)
	{
		return v;
	};

	out.inverse_transform = [](float v)
	{
		return v;
	};

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.range.min_range.range.min = 0.0f;
	out.range.min_range.range.max = 0.0f;
	out.range.min_range.value = 0.0f;
	out.range.min_range.step_size = 0.0f;

	out.range.max_range.range.min = 1.0f;
	out.range.max_range.range.max = 1.0f;
	out.range.max_range.value = 1.0f;
	out.range.max_range.step_size = 0.0;

	out.step_size.range.min = 0.0f;
	out.step_size.range.max = 0.0f;
	out.step_size.value = 0.0f;
	out.step_size.step_size = 0.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 0.0f;

	return out;
}

inline EnvelopeSpec noise_color()
{
	EnvelopeSpec out;

	out.uuid = "30100123-7343-4386-9ed2-f913b9e1e571";
	out.name = "Noise Color";

	out.transform = [](float v)
	{
		return v;
	};

	out.inverse_transform = [](float v)
	{
		return v;
	};

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.range.min_range.range.min = -1.0f;
	out.range.min_range.range.max = -1.0f;
	out.range.min_range.value = -1.0f;
	out.range.min_range.step_size = 0.0f;

	out.range.max_range.range.min = 1.0f;
	out.range.max_range.range.max = 1.0f;
	out.range.max_range.value = 1.0f;
	out.range.max_range.step_size = 0.0;

	out.step_size.range.min = 1.0f;
	out.step_size.range.max = 1.0f;
	out.step_size.value = 1.0f;
	out.step_size.step_size = 0.0f;

	out.default_value = 0.0f;
	out.default_snap_amount = 0.0f;

	return out;
}

} // envelopes

namespace sliders
{

inline SliderSpec amp()
{
	SliderSpec out;

	out.uuid = "a6ae4ad0-2965-448c-ab04-ee378e0c4ab5";
	out.name = "Amp";

	out.transform = [](float v)
	{
		return math::db_to_af(v);
	};

	out.inverse_transform = [](float v)
	{
		return math::af_to_db(v);
	};

	out.display_value = amp_display;

	out.range.range.min = -60.0f;
	out.range.range.max = 12.0f;
	out.range.value = -60.0f;
	out.range.step_size = 0.0f;

	return out;
}

inline SliderSpec pan()
{
	SliderSpec out;

	out.uuid = "b5bf03f3-17e2-4546-8cc2-e29790ea02a2";
	out.name = "Pan";

	out.transform = [](float v)
	{
		return v;
	};

	out.inverse_transform = [](float v)
	{
		return v;
	};

	out.display_value = pan_display;

	out.range.range.min = -1.0f;
	out.range.range.max = 1.0f;
	out.range.value = 0.0f;
	out.range.step_size = 0.0f;

	return out;
}

inline SliderSpec pitch()
{
	SliderSpec out;

	out.uuid = "00859eeb-ce9e-43cd-9994-bff881a9d32d";
	out.name = "Pitch";

	out.transform = [](float v)
	{
		return v;
	};

	out.inverse_transform = [](float v)
	{
		return v;
	};

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.range.range.min = -60.0f;
	out.range.range.max = 60.0f;
	out.range.value = 0.0f;
	out.range.step_size = 1.0f;

	return out;
}

inline SliderSpec speed()
{
	SliderSpec out;

	out.uuid = "04293c38-3a64-42b2-80f0-43a4f8190ba7";
	out.name = "Speed";

	out.transform = [](float v)
	{
		return std::pow(v, 2.0f);
	};

	out.inverse_transform = [](float v)
	{
		return std::sqrt(v);
	};

	out.display_value = [](float v)
	{
		std::stringstream ss;

		ss << v;

		return ss.str();
	};

	out.range.range.min = 0.0f;
	out.range.range.max = 32.0f;
	out.range.value = 1.0f;
	out.range.step_size = 0.0f;

	return out;
}

} // sliders

namespace toggles {

inline ToggleSpec loop()
{
	ToggleSpec out;

	out.uuid = "dfa36d24-3c41-4a13-9b57-dc0116ef19f7";
	out.name = "Loop";
	out.flags = blkhdgen_ToggleFlags_ShowInContextMenu;
	out.default_value = false;

	return out;
}

inline ToggleSpec reverse()
{
	ToggleSpec out;

	out.uuid = "e7cacaf8-4afc-4e81-83de-50620fed4b13";
	out.name = "Reverse";
	out.flags = blkhdgen_ToggleFlags_ShowInContextMenu;
	out.default_value = false;

	return out;
}

} // toggles

}}