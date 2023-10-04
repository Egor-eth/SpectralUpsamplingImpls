#include "spectral_image.h"

const Spectre Spectre::none{};

template<>
constexpr bool canSave<BaseImage<Spectre>> = false;
template<>
constexpr bool canLoad<BaseImage<Spectre>> = false;
 
template class BaseImage<Spectre>;