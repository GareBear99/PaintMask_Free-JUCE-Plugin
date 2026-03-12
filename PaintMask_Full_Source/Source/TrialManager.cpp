#include "TrialManager.h"

static juce::File getPaintMaskStateFile()
{
    return juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
        .getChildFile("PaintMask")
        .getChildFile("license_state.json");
}

TrialManager::TrialManager() : stateFile(getPaintMaskStateFile())
{
    load();
}

void TrialManager::load()
{
    stateFile.getParentDirectory().createDirectory();

    if (stateFile.existsAsFile())
    {
        const auto json = stateFile.loadFileAsString();
        const auto parsed = juce::JSON::parse(json);
        if (! parsed.isVoid())
        {
            state = parsed;
            return;
        }
    }

    auto* obj = new juce::DynamicObject();
    const auto nowMs = juce::Time::getCurrentTime().toMilliseconds();
    obj->setProperty("firstRunMs", nowMs);
    obj->setProperty("licenseKey", juce::String());
    obj->setProperty("licenseState", "trial");
    obj->setProperty("graceDays", 7);
    obj->setProperty("machineTag", juce::SystemStats::getDeviceDescription());
    state = juce::var(obj);
    save();
}

void TrialManager::save() const
{
    if (auto json = juce::JSON::toString(state, true); json.isNotEmpty())
        stateFile.replaceWithText(json);
}

TrialManager::Mode TrialManager::getMode() const
{
    const auto firstRunMs = (int64) state.getProperty("firstRunMs", 0);
    const auto nowMs = juce::Time::getCurrentTime().toMilliseconds();
    const auto days = int((nowMs - firstRunMs) / (1000 * 60 * 60 * 24));
    const auto licenseKey = state.getProperty("licenseKey", {}).toString().trim();
    const auto graceDays = juce::jmax(0, int(state.getProperty("graceDays", 7)));

    if (licenseKey.isNotEmpty())
        return Mode::studio;

    if (days < 30)
        return Mode::trial;

    if (days < 30 + graceDays)
        return Mode::grace;

    return Mode::lite;
}

bool TrialManager::isStudioActive() const
{
    const auto mode = getMode();
    return mode == Mode::studio || mode == Mode::trial || mode == Mode::grace;
}

bool TrialManager::isTrialActive() const
{
    return getMode() == Mode::trial;
}

bool TrialManager::isGraceActive() const
{
    return getMode() == Mode::grace;
}

bool TrialManager::isLiteActive() const
{
    return getMode() == Mode::lite;
}

int TrialManager::getDaysRemaining() const
{
    const auto mode = getMode();
    const auto firstRunMs = (int64) state.getProperty("firstRunMs", 0);
    const auto nowMs = juce::Time::getCurrentTime().toMilliseconds();
    const auto days = int((nowMs - firstRunMs) / (1000 * 60 * 60 * 24));

    if (mode == Mode::studio)
        return 9999;

    if (mode == Mode::trial)
        return juce::jmax(0, 30 - days);

    if (mode == Mode::grace)
        return juce::jmax(0, 30 + int(state.getProperty("graceDays", 7)) - days);

    return 0;
}

juce::String TrialManager::getModeName() const
{
    switch (getMode())
    {
        case Mode::trial: return "Trial";
        case Mode::studio: return "Studio";
        case Mode::grace: return "Grace";
        case Mode::lite: return "Lite";
    }
    return "Lite";
}

void TrialManager::activateStudioLocally(const juce::String& key)
{
    if (auto* obj = state.getDynamicObject())
    {
        obj->setProperty("licenseKey", key.trim());
        obj->setProperty("licenseState", "studio");
        save();
    }
}
