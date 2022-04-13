#pragma once

#include "../geometry.h"
#include "../nm_math.h"

namespace nm::fem {
    template<typename Integrand, typename Return>
    void quadratureSinglePoint(const Integrand &integrand, const vecXr &q, const vec4i &element, real volume,
                               Return &ret) {
        mat43r deformedTetrahedron;
        for (int ii = 0; ii < 4; ++ii) { deformedTetrahedron.row(ii) = q.segment<3>(3 * element(ii)).transpose(); }

        const vec3r centroid = computeTetrahedralCircumcenter(deformedTetrahedron);
        ret = volume * integrand(q, element, centroid);
    }
}// namespace nm::fem