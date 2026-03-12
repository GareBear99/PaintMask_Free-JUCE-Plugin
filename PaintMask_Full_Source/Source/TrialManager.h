#pragma once
#include <JuceHeader.h>

class TrialManager
{
public:
    enum class Mode
    {
        trial,
        studio,
        grace,
        lite
    };

    TrialManager();

    bool isStudioActive() const;
    bool isTrialActive() const;
    bool isGraceActive() const;
    bool isLiteActive() const;
    int getDaysRemaining() const;
    juce::String getModeName() const;
    void activateStudioLocally(const juce::String& key);

private:
    juce::File stateFile;
    juce::var state;

    void load();
    void save() const;
    Mode getMode() const;
};
