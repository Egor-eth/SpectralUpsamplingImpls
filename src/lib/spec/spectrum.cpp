#include <stdexcept>
#include "spectrum.h"
#include "math/math.h"

void Spectrum::set(Float wavelenght, Float value)
{
    modified = true;
    spectre[wavelenght] = value;
}

const std::set<Float> &Spectrum::get_wavelenghts() const
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

Float &Spectrum::operator[](Float w)
{
    modified = true;
    return spectre.at(w);
}

Float Spectrum::operator[](Float w) const
{
    return spectre.at(w);
}

Float &Spectrum::get_or_create(Float w)
{
    return spectre[w];
}

#include <iostream>

Float Spectrum::get_or_interpolate(Float w) const
{
    auto b_it = spectre.lower_bound(w);
    Float b;
    Float f_b;
    if(b_it == spectre.end()) {
        return 0.0f;
    }
    
    b = b_it->first;
    f_b = b_it->second;
    if(b == w) return f_b;
    

    Float a;
    Float f_a;
    if(b_it == spectre.begin()) {
        return 0.0f;
    }

    auto a_it = b_it;
    --a_it;
    a = a_it->first;
    f_a = a_it->second;
    
    return math::interpolate(w, a, b, f_a, f_b);
}

Spectrum &Spectrum::operator=(const Spectrum &&other)
{
    if(this != &other) { 
        cached_wavelenghts = std::move(other.cached_wavelenghts);
        modified = std::move(other.modified);
        spectre = std::move(other.spectre);
    }
    return *this;
}

const Spectrum Spectrum::none{};