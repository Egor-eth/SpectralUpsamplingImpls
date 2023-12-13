#ifndef COLOR_SPECTRE_H
#define COLOR_SPECTRE_H
#include <map>
#include <set>
#include "common/constants.h"

class Spectrum
{
public: 

    Spectrum()
        : spectre() {}

    Spectrum(const Spectrum &s) = default;

    Spectrum(Spectrum &&s)
        : cached_wavelenghts(std::move(s.cached_wavelenghts)), modified(std::move(s.modified)), spectre(std::move(s.spectre)), last_wavelenght(spectre.begin()) {}

    void set(Float wavelenght, Float value);

    const std::set<Float> &get_wavelenghts() const;

    Float &operator[](Float w);

    Float operator[](Float w) const;

    Float &get_or_create(Float w);

    Float get_or_interpolate(Float w) const;

    Spectrum &operator=(const Spectrum &other) = default;
    Spectrum &operator=(const Spectrum &&other);

    const std::map<Float, Float> &get_map() const {
        return spectre;
    }

    static const Spectrum none;

private:
    mutable std::set<Float> cached_wavelenghts{};
    mutable bool modified = true;
    std::map<Float, Float> spectre;
    mutable decltype(spectre)::const_iterator last_wavelenght;
};

#endif