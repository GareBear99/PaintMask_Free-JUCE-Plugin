#pragma once
#include <JuceHeader.h>
struct PerformanceScene
{
    juce::String name { "Scene" };
    float scanSpeedMultiplier = 1.0f;
    float densityBoost = 0.0f;
};
class PerformanceMode
{
public:
    void setEnabled(bool shouldBeEnabled) noexcept;
    bool isEnabled() const noexcept;
    void storeSceneA(const PerformanceScene& scene);
    void storeSceneB(const PerformanceScene& scene);
    void storeSceneC(const PerformanceScene& scene);
    PerformanceScene getSceneA() const;
    PerformanceScene getSceneB() const;
    PerformanceScene getSceneC() const;
private:
    bool enabled = false;
    PerformanceScene a, b, c;
};
