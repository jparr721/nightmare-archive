#include "dpsi_neo_hookean_dF.h"

namespace nm::fem {

    auto dpsiNeoHookeanDf(const mat3r &F, real mu, real lambda) -> vec9r {
        vec9r dpsi;

        real f11 = F(0, 0);
        real f12 = F(0, 1);
        real f13 = F(0, 2);
        real f21 = F(1, 0);
        real f22 = F(1, 1);
        real f23 = F(1, 2);
        real f31 = F(2, 0);
        real f32 = F(2, 1);
        real f33 = F(2, 2);

        real C = mu;
        real D = lambda;

        dpsi(0) = C * (2 * f11 /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        2 / 3) +
                       (-2 * f22 * f33 / 3 + 2 * f23 * f32 / 3) *
                               (f11 * f11 + f12 * f12 + f13 * f13 + f21 * f21 + f22 * f22 + f23 * f23 + f31 * f31 +
                                f32 * f32 + f33 * f33) /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        5 / 3)) +
                  D * (2 * f22 * f33 - 2 * f23 * f32) *
                          (f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 + f13 * f21 * f32 -
                           f13 * f22 * f31 - 1);
        dpsi(1) = C * (2 * f21 /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        2 / 3) +
                       (2 * f12 * f33 / 3 - 2 * f13 * f32 / 3) *
                               (f11 * f11 + f12 * f12 + f13 * f13 + f21 * f21 + f22 * f22 + f23 * f23 + f31 * f31 +
                                f32 * f32 + f33 * f33) /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        5 / 3)) +
                  D * (-2 * f12 * f33 + 2 * f13 * f32) *
                          (f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 + f13 * f21 * f32 -
                           f13 * f22 * f31 - 1);
        dpsi(2) = C * (2 * f31 /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        2 / 3) +
                       (-2 * f12 * f23 / 3 + 2 * f13 * f22 / 3) *
                               (f11 * f11 + f12 * f12 + f13 * f13 + f21 * f21 + f22 * f22 + f23 * f23 + f31 * f31 +
                                f32 * f32 + f33 * f33) /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        5 / 3)) +
                  D * (2 * f12 * f23 - 2 * f13 * f22) *
                          (f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 + f13 * f21 * f32 -
                           f13 * f22 * f31 - 1);
        dpsi(3) = C * (2 * f12 /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        2 / 3) +
                       (2 * f21 * f33 / 3 - 2 * f23 * f31 / 3) *
                               (f11 * f11 + f12 * f12 + f13 * f13 + f21 * f21 + f22 * f22 + f23 * f23 + f31 * f31 +
                                f32 * f32 + f33 * f33) /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        5 / 3)) +
                  D * (-2 * f21 * f33 + 2 * f23 * f31) *
                          (f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 + f13 * f21 * f32 -
                           f13 * f22 * f31 - 1);
        dpsi(4) = C * (2 * f22 /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        2 / 3) +
                       (-2 * f11 * f33 / 3 + 2 * f13 * f31 / 3) *
                               (f11 * f11 + f12 * f12 + f13 * f13 + f21 * f21 + f22 * f22 + f23 * f23 + f31 * f31 +
                                f32 * f32 + f33 * f33) /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        5 / 3)) +
                  D * (2 * f11 * f33 - 2 * f13 * f31) *
                          (f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 + f13 * f21 * f32 -
                           f13 * f22 * f31 - 1);
        dpsi(5) = C * (2 * f32 /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        2 / 3) +
                       (2 * f11 * f23 / 3 - 2 * f13 * f21 / 3) *
                               (f11 * f11 + f12 * f12 + f13 * f13 + f21 * f21 + f22 * f22 + f23 * f23 + f31 * f31 +
                                f32 * f32 + f33 * f33) /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        5 / 3)) +
                  D * (-2 * f11 * f23 + 2 * f13 * f21) *
                          (f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 + f13 * f21 * f32 -
                           f13 * f22 * f31 - 1);
        dpsi(6) = C * (2 * f13 /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        2 / 3) +
                       (-2 * f21 * f32 / 3 + 2 * f22 * f31 / 3) *
                               (f11 * f11 + f12 * f12 + f13 * f13 + f21 * f21 + f22 * f22 + f23 * f23 + f31 * f31 +
                                f32 * f32 + f33 * f33) /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        5 / 3)) +
                  D * (2 * f21 * f32 - 2 * f22 * f31) *
                          (f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 + f13 * f21 * f32 -
                           f13 * f22 * f31 - 1);
        dpsi(7) = C * (2 * f23 /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        2 / 3) +
                       (2 * f11 * f32 / 3 - 2 * f12 * f31 / 3) *
                               (f11 * f11 + f12 * f12 + f13 * f13 + f21 * f21 + f22 * f22 + f23 * f23 + f31 * f31 +
                                f32 * f32 + f33 * f33) /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        5 / 3)) +
                  D * (-2 * f11 * f32 + 2 * f12 * f31) *
                          (f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 + f13 * f21 * f32 -
                           f13 * f22 * f31 - 1);
        dpsi(8) = C * (2 * f33 /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        2 / 3) +
                       (-2 * f11 * f22 / 3 + 2 * f12 * f21 / 3) *
                               (f11 * f11 + f12 * f12 + f13 * f13 + f21 * f21 + f22 * f22 + f23 * f23 + f31 * f31 +
                                f32 * f32 + f33 * f33) /
                               std::pow(f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 +
                                                f13 * f21 * f32 - f13 * f22 * f31,
                                        5 / 3)) +
                  D * (2 * f11 * f22 - 2 * f12 * f21) *
                          (f11 * f22 * f33 - f11 * f23 * f32 - f12 * f21 * f33 + f12 * f23 * f31 + f13 * f21 * f32 -
                           f13 * f22 * f31 - 1);

        return dpsi;
    }
}// namespace nm::fem
