//
// Created by rimuru on 19/03/2020.
//

#include "matrix4.hh"
#include "vector3.hh"
#include <cstring>

namespace pogl {


    matrix4 &matrix4::operator*=(const matrix4 &rhs) {
        float *line = new float(4);
        for (int i = 0; i < 4; ++i) {
            std::memcpy(line, mat[i], sizeof(float) * 4);
            for (int j = 0; j < 4; ++j) {
                mat[i][j] = 0;
                for (int k = 0; k < 4; ++k)
                    mat[i][j] += line[k] * rhs.mat[k][j];
            }
        }
        delete[] line;
        return *this;
    }

    matrix4 matrix4::identity() {
        matrix4 mat;
        mat.mat[0][0] = 1;
        mat.mat[1][1] = 1;
        mat.mat[2][2] = 1;
        mat.mat[3][3] = 1;
        return mat;
    }

    matrix4 &matrix4::translated(float x, float y, float z) {
        matrix4 tmat;
        tmat.mat[0][0] = 1;
        tmat.mat[0][3] = x;
        tmat.mat[1][1] = 1;
        tmat.mat[1][3] = y;
        tmat.mat[2][2] = 1;
        tmat.mat[2][3] = z;
        tmat.mat[3][3] = 1;
        *this *= tmat;
        return *this;
    }

    matrix4 &matrix4::translated(const Vector3 &v) {
        return translated(v.x(), v.y(), v.z());
    }

    matrix4 &matrix4::transpose() {
        float v;
        for (unsigned i = 0; i < 4; ++i)
            for (unsigned j = 0; j < i; ++j) {
                v = mat[i][j];
                mat[i][j] = mat[j][i];
                mat[j][i] = v;
            }

        return *this;
    }

    std::ostream &operator<<(std::ostream &out, const matrix4 &m) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                out << m.mat[i][j] << " \n"[j == 3];
        return out;
    }


    void frustum(matrix4 &mat,
                 const float &left, const float &right,
                 const float &bottom, const float &top,
                 const float &z_near, const float &z_far) {
        matrix4 mat2;
        mat2.mat[0][0] = 2 * z_near / (right - left);

        mat2.mat[1][1] = 2 * z_near / (top - bottom);

        mat2.mat[2][0] = (right + left) / (right - left);//A
        mat2.mat[2][1] = (top + bottom) / (top - bottom);//B
        mat2.mat[2][2] = -(z_far + z_near) / (z_far - z_near);
        mat2.mat[2][3] = -1;

        mat2.mat[3][2] = -2 * z_far * z_near / (z_far - z_near);


        mat *= mat2;

    }

    void look_at(matrix4 &mat, const Vector3 &eye, const Vector3 &center, Vector3 up) {
        Vector3 forward = center - eye;
        Vector3 right = forward.cross(up);

        right.normalize();
        forward.normalize();
        up = right.cross(forward);

        matrix4 mul = matrix4::identity();
        mul.mat[0][0] = right.x();
        mul.mat[0][1] = right.y();
        mul.mat[0][2] = right.z();
        mul.mat[1][0] = up.x();
        mul.mat[1][1] = up.y();
        mul.mat[1][2] = up.z();
        mul.mat[2][0] = -forward.x();
        mul.mat[2][1] = -forward.y();
        mul.mat[2][2] = -forward.z();

        mul.mat[0][3] = eye.dot(right) * -1;
        mul.mat[1][3] = eye.dot(up) * -1;
        mul.mat[2][3] = eye.dot(forward);
        mat *= mul;

        mat.transpose();

    }
}