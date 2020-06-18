#pragma once

#include <string>
#include "program.hh"

namespace pogl {
    std::string load(const std::string &filename);

    bool setup_glitch(const Vector3 &eye, const Vector3 &center, const Vector3 &up, const OpenGLObject &rectangle);

    bool setup_anaglyph(const Vector3 &eye, const Vector3 &center, const Vector3 &up,
                        const OpenGLObject &rectangle, shared_prog scene_program);

    bool setup_depth_of_field(const Vector3 &eye, const Vector3 &center, const Vector3 &up,
                              const OpenGLObject &rectangle, shared_prog scene_program);
}

