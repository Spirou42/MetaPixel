
#ifndef __EFFECT_WHITE_H__
#define __EFFECT_WHITE_H__
#include "Effect.h"
#include "metaPixel.h"

int white(unsigned long now,void* userdata);

class EffectWhite : public Effect
{
public:
	newParameter_t *colorIndexParam;

	EffectWhite():Effect("White"),colorIndexParam(NULL){};

	EffectWhite(newParameter_t* colIdxPar):Effect("White"),colorIndexParam(colIdxPar){};
	virtual void initializeEffect(){setMaxValueFor(colorIndexParam,255);};
	virtual void runEffect(unsigned long now);

};


#endif