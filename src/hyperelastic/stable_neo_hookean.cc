#include "stable_neo_hookean.h"

namespace nm::hyperelastic {
    auto psi(const mat3 &F, real lambda, real mu) -> real {
        const real alpha = 1.0 + mu / lambda;
        const real Ic = F.squaredNorm();
        const real J = F.determinant();

        // As specified in HOBAK, we drop (mu/2) * (log(Ic + 1)) as it was mostly
        // included as a formality for a situation that never happens.
        // Equation here is from "Stable Neo-Hookean Flesh Simulation" eq. 14
        return 0.5 * (mu * (Ic - 3) + lambda * (J - alpha) * (J - alpha));
    }

    auto pk1(const mat3 &F, real lambda, real mu) -> mat3 {
        const real Ic = F.squaredNorm();
        const real J = F.determinant();
        const mat3 pJpF = identities::partialJpartialF(F);
        const real alpha = 1.0 + mu / lambda;

        // First Piola Kirchoff Stress Tensor
        // Equation here is from "Stable Neo-Hookean Flesh Simulation" eq. 18
        return mu * (1 - 1 / (Ic + 1)) * F + lambda * (J - alpha) * pJpF;
    }

    auto dpk1(const mat3 &U, const vec3 &sigma, const mat3 &V, real lambda, real mu) -> mat9 {
        vec9 eigenvalues;
        mat9 eigenvectors;
        const real J = sigma(0) * sigma(1) * sigma(2);
        const real front = lambda * (J - 1.0) - mu;
        eigenvalues(0) = front * sigma(0) + mu;
        eigenvalues(1) = front * sigma(1) + mu;
        eigenvalues(2) = front * sigma(2) + mu;
        eigenvalues(3) = -front * sigma(0) + mu;
        eigenvalues(4) = -front * sigma(1) + mu;
        eigenvalues(5) = -front * sigma(2) + mu;

        // Populate the matrix for scaling the eigenvalues.
        mat3 A;
        const real s0s0 = sigma(0) * sigma(0);
        const real s1s1 = sigma(1) * sigma(1);
        const real s2s2 = sigma(2) * sigma(2);
        A(0, 0) = mu + lambda * s1s1 * s2s2;
        A(1, 1) = mu + lambda * s0s0 * s2s2;
        A(2, 2) = mu + lambda * s0s0 * s1s1;

        const real frontOfDiag = lambda * (2.0 * J - 1.0) - mu;
        A(0, 1) = frontOfDiag * sigma(2);
        A(0, 2) = frontOfDiag * sigma(1);
        A(1, 2) = frontOfDiag * sigma(0);
        A(1, 0) = A(0, 1);
        A(2, 0) = A(0, 2);
        A(2, 1) = A(1, 2);

        // Get the scaling eigenvalues
        const Eigen::SelfAdjointEigenSolver<mat3> aegis(A);
        eigenvalues(6) = aegis.eigenvalues()(0);
        eigenvalues(7) = aegis.eigenvalues()(1);
        eigenvalues(8) = aegis.eigenvalues()(2);

        // Compute the eigenvectors
        utils::buildTwistAndFlipEigenvectors(U, V, eigenvectors);
        utils::buildScalingEigenvectors(U, aegis.eigenvectors(), V, eigenvectors);

        // Clamp the eigenvalues
        for (int ii = 0; ii < 9; ++ii) {
            if (eigenvalues(ii) < 0.0) { eigenvalues(ii) = 0.0; }
        }

        return eigenvectors * eigenvalues.asDiagonal() * eigenvectors.transpose();
    }
}// namespace nm::hyperelastic
