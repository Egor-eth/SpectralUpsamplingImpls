#ifndef IMAGEUTIL_SPECTRE_H
#define IMAGEUTIL_SPECTRE_H
#include <unordered_map>
#include <set>

using SpectreFloat = float;


class Spectre
{
public: 

    Spectre()
        : spectre() {}

    Spectre(const Spectre &s) = default;

    Spectre(Spectre &&s)
        : cached_wavelenghts(std::move(s.cached_wavelenghts)), modified(std::move(s.modified)), spectre(std::move(s.spectre)) {}

    void set(SpectreFloat wavelenght, SpectreFloat value);

    const std::set<SpectreFloat> &get_wavelenghts() const;

    SpectreFloat &operator[](SpectreFloat w);

    SpectreFloat operator[](SpectreFloat w) const;

    Spectre &operator=(const Spectre &other) = default;
    Spectre &operator=(const Spectre &&other);

    const std::unordered_map<SpectreFloat, SpectreFloat> &get_map() const {
        return spectre;
    }

    static const Spectre none;

private:
    mutable std::set<SpectreFloat> cached_wavelenghts{};
    mutable bool modified = true;
    std::unordered_map<SpectreFloat, SpectreFloat> spectre;
};

#endif