#include "mathcommon.h"
#include "spectre.h"

Spectre::Spectre(const Spectre &s)
{
    std::copy(s.spectre, s.spectre + SPECTRE_LENGTH, spectre);
}

SpectreFloat &Spectre::operator()(int w)
{
    return spectre[(w - WAVELENGTH_MIN) / WAVELENGTH_STEP];
}

SpectreFloat Spectre::operator()(int w) const
{
    return spectre[(w - WAVELENGTH_MIN) / WAVELENGTH_STEP];
}

SpectreFloat &Spectre::operator[](int p)
{
    return spectre[p];
}

SpectreFloat Spectre::operator[](int p) const
{
    return spectre[p];
}

Spectre &Spectre::operator=(const Spectre &other)
{
    if(&other != this) {
        
    }
    return *this;
}

const Spectre Spectre::none{};