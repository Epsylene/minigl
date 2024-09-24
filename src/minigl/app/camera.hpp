#pragma once

#include "core.hpp"
#include "minigl/geometry.hpp"
#include "input/input.hpp"

namespace minigl
{
    /// @brief Generic camera object class
    ///
    /// @details This class constructs a generic perspective
    /// camera, but doesn't handle controls, which are
    /// implemented in child classes.
    class Camera
    {
        public:

            /// @brief Constructs a default perspective camera
            /// (90º FOV, 16/9 aspect ratio, distances to near
            /// and far planes set to 0.1 and 10).
            Camera();

            /// @brief Constructs a perspective 3D camera with
            /// the given parameters
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
            Camera(float fov, float aspect, float near, float far);

            const Vec3& getPosition() const;
            void setPosition(const Vec3& pos);

            /// @brief Get the camera's eye direction vector
            const Vec3& getDirection() const;
            void setDirection(const Vec3& dir);

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

            /// @brief Update the free camera (movement with
            /// WASD, looking around with the mouse).
            /// @param input 
            /// @param dt 
            void update_free_camera(Ref<Input> input, float dt);

            virtual void onUpdate(Ref<Input> input, float dt) = 0;

        public:

            Mat4 proj;
            Mat4 view;
            Mat4 viewProj;

        protected:

            Vec3 pos, direction, up;

            /// @brief 	Calculates the view-projection matrix
            /// of the 3D camera.
            ///
            /// The view matrix is a `lookAt()` with `pos` as
            /// eye, `pos + direction` as center and `up` as
            /// up. The projection matrix is a `perspective()`
            /// that takes the arguments from the constructor.
            virtual void compute_viewProj() = 0;
    };

    /// @brief Free 3D perspective camera. The camera can be
    /// moved with WASD and look around with the mouse.
    class FreeCamera: public Camera
    {
        public:

            /// @brief Free 3D perspective camera created with
            /// the default values of the base Camera class.
            FreeCamera() = default;

            /// @brief Called by the application on each frame
            /// to control the camera.
            void onUpdate(Ref<Input> input, float dt) override;

            /// @brief Set the direction vector of the camera
            /// manually. The yaw and pitch angles are
            /// recomputed automatically.
            void setDirection(const Vec3& dir);

        private:

            float pitch = 0.f, yaw = -90.f, roll = 0.f;
            
            void set_rotation(float pitch, float yaw, float roll);
            void compute_viewProj() override;

            /// @brief Compute the yaw and pitch angles from
            /// the direction vector.
            void compute_yaw_pitch();
    };
}