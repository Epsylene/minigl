#pragma once

#include "core.hpp"
#include "geometry.hpp"

namespace minigl
{
    class Input;

    /// @brief Generic camera object class
    ///
    /// @details This class is not intended to be instanciated,
    /// but to be derived from and create actual camera objects
    /// with a defined view-projection matrix.
    class Camera
    {
        public:

            /// @brief Constructs a default perspective
            /// camera (90º FOV, 16/9 aspect ratio, distances to
            /// near and far planes set to 0.1 and 10).
            Camera();

            /// @brief Constructs a perspective 3D camera with
            /// the given parameters
            ///
            /// @param fov Field of view, that is, the angle the
            ///     camera2D "sees" of the world; a FOV of 90º means
            ///     that the righmost and leftmost objects in the
            ///     camera2D's visual field are at 45º right and
            ///     left.
            /// @param aspect Aspect ratio of the camera2D's visual
            ///     field
            /// @param near Near plane distance
            /// @param far Far plane distance
            Camera(float fov, float aspect, float near, float far);

            const Vec3& getPosition() const;
            void setPosition(const Vec3& pos);

            /// @brief Get the camera's eye direction vector
            const Vec3& getDirection() const;

            /// @brief Set the camera's rotation in Euler angles
            ///
            /// @param pitch Rotation around the right vector
            /// @param yaw Rotation around the up vector
            /// @param roll Rotation around the direction vector
            void set_rotation(float pitch, float yaw, float roll);

            /// @brief Set the camera's projection matrix arguments
            ///
            /// @param fov Field of view, that is, the angle the
            ///     camera2D "sees" of the world; a FOV of 90º means
            ///     that the righmost and leftmost objects in the
            ///     camera2D's visual field are at 45º right and
            ///     left.
            /// @param aspect Aspect ratio of the camera2D's visual
            ///     field
            /// @param near Near plane distance
            /// @param far Far plane distance
            void change_projection(float fov, float aspect, float near, float far);

            void onUpdate(Ref<Input> input, float dt);

        public:

            Mat4 projMatrix;
            Mat4 viewMatrix;
            Mat4 viewProjMatrix;

        private:

            Vec3 pos, direction, up;
            float pitch = 0.f, yaw = -90.f, roll = 0.f;

            /// @brief 	Calculates the view-projection matrix
            /// of the 3D camera.
            ///
            /// The view matrix is a `lookAt()` with `pos` as
            /// eye, `pos + direction` as center and `up` as
            /// up. The projection matrix is a `perspective()`
            /// that takes the arguments from the constructor.
            ///
            /// @see Camera3D constructor, `minigl::lookAt()`, `minigl::perspective()`
            void recalculateViewProj();
    };
}