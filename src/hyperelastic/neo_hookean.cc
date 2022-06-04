#include "neo_hookean.h"

namespace nm::hyperelastic {
    auto psi(const mat3 &F, real lambda, real mu) -> real {
        const real J = F.determinant();
        const real logJ = std::log(J);

        // Invariant 2
        // http://www.tkim.graphics/DYNAMIC_DEFORMABLES/DynamicDeformables.pdf
        // Appendix B
        const real I2 = doubleContraction(F, F);
        return mu * 0.5 * (I2 - 3.0) - mu * logJ + lambda * 0.5 * logJ * logJ;
    }

    auto pk1(const mat3 &F, real lambda, real mu) -> mat3 {
        const real J = F.determinant();
        const real logJ = std::log(J);
        const mat3 pJpF = identities::partialJpartialF(F);
        return mu * (F - (1.0 / J) * pJpF) + lambda * (logJ / J) * pJpF;
    }

    auto dpk1(const mat3 &F, real lambda, real mu) -> mat9 {
        const real J = F.determinant();
        const real logJ = std::log(J);
        const mat9 I99 = mat9::Identity();
        const mat3 f0hat = utils::crossMatrix(F, 0);
        const mat3 f1hat = utils::crossMatrix(F, 1);
        const mat3 f2hat = utils::crossMatrix(F, 2);

        // Build the fractal cross product by combining several sub matrix
        // values and spreading their indices over the matrix.
        // http://www.tkim.graphics/DYNAMIC_DEFORMABLES/DynamicDeformables.pdf
        // Section 4.2
        const mat9 HJ = utils::fractalCrossProduct(f0hat, f1hat, f2hat);
        const vec9 gradJFlat = utils::vectorize(identities::partialJpartialF(F));

        return mu * I99 + ((mu + lambda * (1.0 - logJ)) / (J * J)) * gradJFlat * gradJFlat.transpose() *
                                  ((lambda * logJ - mu) / J) * HJ;
    }
}// namespace nm::hyperelastic
