#ifndef SPEC_BASIC_SPECTRUM_H
#define SPEC_BASIC_SPECTRUM_H
#include "spectrum.h"
#include <map>
#include <set>
#include "common/constants.h"

namespace spec {

    class BasicSpectrum : public ISpectrum
    {
    public: 

        BasicSpectrum()
            : spectre() {}

        BasicSpectrum(const BasicSpectrum &s) = default;

        BasicSpectrum(BasicSpectrum &&s)
            : cached_wavelenghts(std::move(s.cached_wavelenghts)), modified(std::move(s.modified)), spectre(std::move(s.spectre)), last_wavelenght(spectre.begin()) {}

        void set(Float wavelenght, Float value);

        const std::set<Float> &get_wavelenghts() const;

        Float &operator[](Float w);

        Float &get_or_create(Float w);

        Float operator[](Float w) const;

        Float get_or_interpolate(Float w) const override;

        BasicSpectrum &operator=(const BasicSpectrum &other) = default;
        BasicSpectrum &operator=(const BasicSpectrum &&other);

        const std::map<Float, Float> &get_map() const {
            return spectre;
        }

        static const BasicSpectrum none;

    private:
        mutable std::set<Float> cached_wavelenghts{};
        mutable bool modified = true;
        std::map<Float, Float> spectre;
        mutable decltype(spectre)::const_iterator last_wavelenght;
    };

}
#endif