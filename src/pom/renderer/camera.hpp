#ifndef SRC_POM_RENDERER_CAMERA_HPP
#define SRC_POM_RENDERER_CAMERA_HPP

#include <cmath>

#include "glm/gtc/matrix_transform.hpp"

class Camera {
 public:
  enum class Direction { Up, Down, Left, Right, Forward, Backward };

  [[nodiscard]] auto GetViewMatrix() const noexcept -> glm::mat4 {
    return view_matrix_;
  }

  auto Turn(Direction direction) -> void {
    switch (direction) {
      case Direction::Up:
        GetPitch() = std::max(-kMaxPitch, GetPitch() - kTurnStep);
        break;
      case Direction::Down:
        GetPitch() = std::min(kMaxPitch, GetPitch() + kTurnStep);
        break;
      case Direction::Left:
        GetYaw() += kTurnStep;
        break;
      case Direction::Right:
        GetYaw() -= kTurnStep;
        break;
      case Direction::Forward:
        [[fallthrough]];
      case Direction::Backward:
        // nothing to do
        return;
    }
    UpdateViewMatrix();
  }

  auto Move(Direction direction) -> void {
    switch (direction) {
      case Direction::Up:
        position_[2] += kMoveStep;
        break;
      case Direction::Down:
        position_[2] -= kMoveStep;
        break;
      case Direction::Left:
        // cross() returns a vector that is orthogonal to both input vectors
        position_ += kMoveStep * glm::normalize(glm::cross(GetDirectionVector(),
                                                           GetUpVector()));
        break;
      case Direction::Right:
        // cross() returns a vector that is orthogonal to both input vectors
        position_ -= kMoveStep * glm::normalize(glm::cross(GetDirectionVector(),
                                                           GetUpVector()));
        break;
      case Direction::Forward:
        position_ -= kMoveStep * GetDirectionVector();
        break;
      case Direction::Backward:
        position_ += kMoveStep * GetDirectionVector();
        break;
    }
    UpdateViewMatrix();
  }

 private:
  static auto constexpr kXAxis = glm::vec3{1.0F, 0.0F, 0.0F};
  static auto constexpr kYAxis = glm::vec3{0.0F, 1.0F, 0.0F};
  static auto constexpr kZAxis = glm::vec3{0.0F, 0.0F, 1.0F};

  static auto constexpr kNorthDirection = glm::vec4{kYAxis, 0.0F};
  static auto constexpr kMoveStep = 0.1F;
  static auto constexpr kTurnStep = glm::radians(1.0F);
  static auto constexpr kMaxPitch = glm::radians(80.0F);

  glm::mat4 view_matrix_{};
  glm::vec3 position_{0.0F, 0.0F, 1.0F};
  glm::vec3 angle_{0.0F, 0.0F, 0.0F};

  [[nodiscard]] auto GetYaw() const& noexcept -> float { return angle_[0]; }
  [[nodiscard]] auto GetYaw() & noexcept -> float& { return angle_[0]; }

  [[nodiscard]] auto GetPitch() const& noexcept -> float { return angle_[1]; }
  [[nodiscard]] auto GetPitch() & noexcept -> float& { return angle_[1]; }

  [[nodiscard]] auto GetRoll() const& noexcept -> float { return angle_[2]; }
  [[nodiscard]] auto GetRoll() & noexcept -> float& { return angle_[2]; }

  [[nodiscard]] static constexpr auto GetUpVector() noexcept -> glm::vec3 {
    // z is always up
    return kZAxis;
  }

  [[nodiscard]] auto GetDirectionVector() const noexcept -> glm::vec3 {
    // starting with north, first pitch, then yaw... order is important!
    glm::vec4 direction{kNorthDirection};
    // rotate around x axis
    direction = glm::rotate(glm::mat4{1.0F}, GetPitch(), kXAxis) * direction;
    // rotate around z axis
    direction = glm::rotate(glm::mat4{1.0F}, GetYaw(), kZAxis) * direction;
    // normalize to ensure its length is 1.0
    return glm::normalize(direction);
  }

  auto UpdateViewMatrix() noexcept -> void {
    view_matrix_ =
        glm::lookAt(position_, position_ - GetDirectionVector(), GetUpVector());
  }
};

#endif  // SRC_POM_RENDERER_CAMERA_HPP