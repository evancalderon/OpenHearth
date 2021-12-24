#pragma once

#include "Character.h"

namespace OpenHearth
{
	struct World
	{
		Character* local_character;
	};

	void InitWorld();
	void DrawWorld();
}
