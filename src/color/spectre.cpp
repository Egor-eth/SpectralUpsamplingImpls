#include <stdexcept>
#include "spectre.h"

void Spectre::set(SpectreFloat wavelenght, SpectreFloat value)
{
    modified = true;
    spectre[wavelenght] = value;
}

const std::set<SpectreFloat> &Spectre::get_wavelenghts() const
{
    if(modified) {
        cached_wavelenghts.clear();
        for(const auto &p : spectre) {
            cached_wavelenghts.insert(p.first);
        }
        modified = false;
    }
    return cached_wavelenghts;
}

SpectreFloat &Spectre::operator[](SpectreFloat w)
{
    modified = true;
    return spectre[w];
}

SpectreFloat Spectre::operator[](SpectreFloat w) const
{
    return spectre.at(w);
}

Spectre &Spectre::operator=(const Spectre &&other)
{
    if(this != &other) { 
        cached_wavelenghts = std::move(other.cached_wavelenghts);
        modified = std::move(other.modified);
        spectre = std::move(other.spectre);
    }
    return *this;
}

const Spectre Spectre::none{};