#pragma once

#include "core.hpp"
#include "minigl/geometry.hpp"
#include "input/input.hpp"

namespace minigl
{
    /// Generic camera object
    ///
    /// This class represents a camera in a 3D scene. It holds
    /// state for its position, direction and up vector, as
    /// well as the view and projection matrices. Controls and
    /// actual computation of the view-proj matrix are left to
    /// derived classes.
    class Camera
    {
        public:

            /// Construct a camera at a default position,
            /// looking towards the center of the scene, with a
            /// Y-up vector.
            Camera();

            virtual const Vec3& getPosition() const { return pos; };
            virtual void setPosition(const Vec3& pos);

            virtual const Vec3& getDirection() const { return direction; }
            virtual void setDirection(const Vec3& dir);

            virtual void onUpdate(Ref<Input> input, float dt) = 0;

        public:

            Mat4 proj;
            Mat4 view;
            Mat4 viewProj;

        protected:

            Vec3 pos, direction, up;
            virtual void compute_viewProj() = 0;
    };

    /// Generic orthographic camera
    class OrthoCamera: public Camera
    {
        public:

            /// Default orthographic camera with a 10x10
            /// viewing area, near plane at 0.1 and far plane
            /// at 100.
            OrthoCamera();

            /// Orthographic camera with a `(left,bottom)` to
            /// `(right,top)` viewing area and `near` and `far`
            /// planes.
            OrthoCamera(float left, float right, float bottom, float top, float near, float far);

            void onUpdate(Ref<Input> input, float dt) override {}

        protected:

            float left, right, bottom, top, near, far;
            void compute_viewProj() override;
    };

    /// Generic perspective camera class
    class PerspectiveCamera: public Camera
    {
        public:

            /// Construct a default 3D perspective camera (60º
            /// FOV, 16/9 aspect ratio).
            PerspectiveCamera();

            /// Construct a perspective 3D camera
            ///
            /// @param fov Field of view, that is, the angle the
            ///     camera "sees" of the world; a FOV of 90º means
            ///     that the righmost and leftmost objects in the
            ///     camera's visual field are at 45º right and
            ///     left.
            /// @param aspect Aspect ratio of the camera's visual
            ///     field
            /// @param near Near plane distance
            /// @param far Far plane distance
            PerspectiveCamera(float fov, float aspect, float near, float far);

            /// Set the camera rotation in Euler angles
            ///
            /// @param pitch Rotation around the right vector
            /// @param yaw Rotation around the up vector
            /// @param roll Rotation around the direction vector
            void set_rotation(float pitch, float yaw, float roll);

            /// Set the camera projection parameters
            ///
            /// @param fov Field of view, that is, the angle the
            ///     camera "sees" of the world; a FOV of 90º means
            ///     that the righmost and leftmost objects in the
            ///     camera's visual field are at 45º right and
            ///     left.
            /// @param aspect Aspect ratio of the camera2D's visual
            ///     field
            /// @param near Near plane distance
            /// @param far Far plane distance
            void change_projection(float fov, float aspect, float near, float far);

        protected:

            virtual void compute_viewProj() = 0;
    };

    /// Free 3D perspective camera. The camera can be moved
    /// with WASD and look around with the mouse.
    class FreeCamera: public PerspectiveCamera
    {
        public:

            /// Free 3D perspective camera created with the
            /// default values of the base Camera class.
            FreeCamera() = default;

            /// Called by the application on each frame to
            /// control the camera.
            void onUpdate(Ref<Input> input, float dt) override;

            /// Set the direction vector of the camera
            /// manually. The yaw and pitch angles are
            /// recomputed automatically.
            void setDirection(const Vec3& dir) override;

            void setSpeedFactor(float factor);

        private:

            float pitch = 0.f, yaw = -90.f, roll = 0.f;
            float speed_factor = 1.f;
            
            void set_rotation(float pitch, float yaw, float roll);
            
            /// View matrix: `lookAt()` with `pos` as eye,
            /// `pos + direction` as center and `up` as up.
            /// Projection: `perspective()` that takes the
            /// arguments from the constructor.
            void compute_viewProj() override;

            /// Compute the yaw and pitch angles from the
            /// direction vector.
            void compute_yaw_pitch();
    };
}