#include "Effect.h"
#include "EffectWhite.h"
#include <Arduino.h>


void EffectWhite::startEffect()
{
	setMaxValueFor(colorIndexParam,255);
	colorIndexParam->value.initTo(128);
}
void EffectWhite::frame(unsigned long now)
{
	CRGB color = ColorFromPalette(colorPalettes[Palette.currentValue()],colorIndexParam->value.currentValue());
	display.fill(color);
	display.flush();
}
