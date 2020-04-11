//
// Created by rimuru on 19/03/2020.
//

#ifndef matrix4_HH
#define matrix4_HH

#include <iostream>
#include "vector3.hh"

namespace pogl {
    class matrix4 {
    public:
        matrix4() {};

        matrix4 &operator*=(const matrix4 &rhs);

        matrix4 &translated(float x, float y, float z);
        matrix4 &translated(const Vector3& v);
        matrix4 &transpose();
        static matrix4 identity();

        float mat[4][4] = {0};
    };

    std::ostream &operator<<(std::ostream &out, const matrix4 &m);

    void frustum(matrix4 &mat,
                 const float &left, const float &right,
                 const float &bottom, const float &top,
                 const float &z_near, const float &z_far);

    void look_at(matrix4 &mat, const Vector3 &eye, const Vector3& center, Vector3 up);

}
#endif //matrix4_HH
