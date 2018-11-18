#pragma once

#include <memory>

template <typename T> using SP = std::shared_ptr<T>;
template <typename T, typename... Args> SP<T> MS(Args&&... args) {return std::make_shared<T>(std::forward<Args&&>(args)...);}

template <typename T> using UP = std::unique_ptr<T>;
template <typename T, typename... Args> UP<T> MU(Args&&... args) {return std::make_unique<T>(std::forward<Args&&>(args)...);}

#define BGA Beast::Game
#define BGFX Beast::Graphics
#define BGRT Beast::Graphics::Raytracing
#define BAU Beast::Audio
#define BUT Beast::Util
#define BMI Beast::Math::Interpolation
#define BMG Beast::Math::General

#if __has_include("glm/glm.hpp")
#define BMC Beast::Math::Conversion
#endif
