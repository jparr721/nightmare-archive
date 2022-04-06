#include "external_forces.h"

namespace nm::fem {
    void applyForce(const vecXr &externalForce, vecXr &forces) {
        forces += externalForce;
    }
}
