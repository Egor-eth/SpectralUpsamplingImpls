
using SpectreFloat = float;

class Spectre
{
public: 
    Spectre()
        : spectre() {}

    Spectre(const Spectre &s);

    SpectreFloat &operator()(int w);

    SpectreFloat operator()(int w) const;

    SpectreFloat &operator[](int p);

    SpectreFloat operator[](int p) const;

    Spectre &operator=(const Spectre &other);

    static const Spectre none;

private:
    SpectreFloat spectre[SPECTRE_LENGTH];
};