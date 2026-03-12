#include "PerformanceMode.h"
void PerformanceMode::setEnabled(bool shouldBeEnabled) noexcept { enabled = shouldBeEnabled; }
bool PerformanceMode::isEnabled() const noexcept { return enabled; }
void PerformanceMode::storeSceneA(const PerformanceScene& scene) { a = scene; }
void PerformanceMode::storeSceneB(const PerformanceScene& scene) { b = scene; }
void PerformanceMode::storeSceneC(const PerformanceScene& scene) { c = scene; }
PerformanceScene PerformanceMode::getSceneA() const { return a; }
PerformanceScene PerformanceMode::getSceneB() const { return b; }
PerformanceScene PerformanceMode::getSceneC() const { return c; }
