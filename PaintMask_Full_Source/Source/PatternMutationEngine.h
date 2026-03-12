#pragma once
#include <JuceHeader.h>
#include "PaintCanvas.h"
class PatternMutationEngine
{
public:
    PaintDocument mirrorX(const PaintDocument& source) const;
    PaintDocument mirrorY(const PaintDocument& source) const;
    PaintDocument rotateQuarterTurn(const PaintDocument& source) const;
    PaintDocument densityShift(const PaintDocument& source, float amount01) const;
};
