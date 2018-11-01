#include "Camera.h"
#include <cmath>
#include "Utility.h"

namespace
{
    const double pi = 3.141592653589793238462643383279;
}

Camera::Camera()
    : m_mode(CameraMode::Static),
      m_isBoundless(true),
      m_minDimensions(128, 72),
      m_maxDimensions(2560, 1440),
      m_zoom(1),
      m_zoomLerp(0.25),
      m_pFollowedEntity(nullptr),
      m_followLerp(0.3),
      m_ticksRemaining(0),
      m_ticksTotal(0)
{
    SetDimensions(m_minDimensions);
}

// Resize the given dimensions to be at least as big as the given minimum dimensions while preserving their aspect ratio
void Camera::ResizeToFitMinDimensions(sf::Vector2f& rDimensions, const sf::Vector2f& minDimensions)
{
    // If there is no need to resize
    if (rDimensions.x > minDimensions.x && rDimensions.y > minDimensions.y)
    {
        return;
    }

    rDimensions *= Utility::GetScaleToFill(rDimensions, minDimensions);
}

// Resize the given dimensions to fit the given maximum dimensions while preserving their aspect ratio
void Camera::ResizeToFitMaxDimensions(sf::Vector2f& rDimensions, const sf::Vector2f& maxDimensions)
{
    // If there is no need to resize
    if (rDimensions.x < maxDimensions.x && rDimensions.y < maxDimensions.y)
    {
        return;
    }

    rDimensions *= Utility::GetScaleToFit(rDimensions, maxDimensions);
}

// Modify the given position to fix any possible out-of-bounds error
void Camera::BoundsCollision(sf::Vector2f& rPosition, const sf::Vector2f& dimensions)
{
    if (m_isBoundless == true || m_bounds == sf::Vector2f(0, 0))
    {
        return;
    }

    // Horizontal Map edge stopping
    if (rPosition.x - dimensions.x / 2 < 0)
    {
        rPosition.x = dimensions.x / 2;
    }
    else if (rPosition.x + dimensions.x / 2 > m_bounds.x)
    {
        rPosition.x = m_bounds.x - dimensions.x / 2;
    }
    // Vertical Map edge stopping
    if (rPosition.y - dimensions.y / 2 < 0)
    {
        rPosition.y = dimensions.y / 2;
    }
    else if (rPosition.y + dimensions.y / 2 > m_bounds.y)
    {
        rPosition.y = m_bounds.y - dimensions.y / 2;
    }
}

void Camera::Update()
{
    m_previousDimensions = m_dimensions;
    m_previousPosition = m_position;

    // Zoom lerp
    static const float epsilon = 1e-6;
    if (std::fabs((m_dimensions.x - m_targetDimensions.x) / m_targetDimensions.x) > epsilon || std::fabs((m_dimensions.y - m_targetDimensions.y) / m_targetDimensions.y) > epsilon)
    {
        sf::Vector2f newDimensions = m_dimensions + (m_targetDimensions - m_dimensions) * m_zoomLerp;
        m_zoom *= newDimensions.x / m_dimensions.x;
        m_dimensions = newDimensions;
    }
    else
    {
        m_dimensions = m_targetDimensions;
    }


    // Independent Camera movement
    if (m_mode == CameraMode::Static || m_mode == CameraMode::Moving)
    {
        if (m_mode == CameraMode::Moving)
        {
            m_mode = CameraMode::Static;
            m_position += m_moveOffset;
            m_moveOffset = sf::Vector2f(0, 0);
            BoundsCollision(m_position, m_dimensions);
        }

        if (m_dimensions != m_targetDimensions)
        {
            BoundsCollision(m_previousPosition, m_previousDimensions);
            BoundsCollision(m_position, m_dimensions);
        }

        return;
    }

    if (m_mode == CameraMode::Follow)
    {
        // Disable lerp and interpolation to snap to the followed Entity if colliding with Map edge when zooming
        bool mustSnapHorizontally;
        bool mustSnapVertically;

        if (m_dimensions != m_targetDimensions)
        {
            mustSnapHorizontally = ((m_previousPosition.x - m_previousDimensions.x / 2 <= 0 && m_previousPosition.x >= m_pFollowedEntity->GetPosition().x) ||
                                  (m_previousPosition.x + m_previousDimensions.x / 2 >= m_bounds.x && m_previousPosition.x <= m_pFollowedEntity->GetPosition().x));
            mustSnapVertically = ((m_previousPosition.y - m_previousDimensions.y / 2 <= 0 && m_previousPosition.y >= m_pFollowedEntity->GetPosition().y) ||
                                (m_previousPosition.y + m_previousDimensions.y / 2 >= m_bounds.y && m_previousPosition.y <= m_pFollowedEntity->GetPosition().y));
        }
        else
        {
            mustSnapHorizontally = false;
            mustSnapVertically = false;
        }

        if (mustSnapHorizontally == true) // Zooming with horizontal lerp and interpolation disabled
        {
            m_position.x = m_pFollowedEntity->GetPosition().x;
            m_previousPosition.x = m_position.x;
        }
        else
        {
            m_position.x += (m_pFollowedEntity->GetPosition().x - m_position.x) * m_followLerp;
        }

        if (mustSnapVertically == true) // Zooming with vertical lerp and interpolation disabled
        {
            m_position.y = m_pFollowedEntity->GetPosition().y;
            m_previousPosition.y = m_position.y;
        }
        else
        {
            m_position.y += (m_pFollowedEntity->GetPosition().y - m_position.y) * m_followLerp;
        }
    }
    else if (m_mode == CameraMode::Translate)
    {
        if (m_ticksRemaining > 0)
        {
            m_position += (m_finalTranslationPosition - m_position) / static_cast<float>(m_ticksRemaining);
            m_ticksRemaining--;
        }
        else
        {
            m_startTranslationPosition = sf::Vector2f(0, 0);
            m_finalTranslationPosition = sf::Vector2f(0, 0);
            SetPosition(m_position);

        }
    }
    else if (m_mode == CameraMode::SmoothTranslate)
    {
        if (m_ticksRemaining > 0)
        {
            // Sinerp function
            m_position += ((m_finalTranslationPosition - m_startTranslationPosition) / static_cast<float>(m_ticksTotal)) *
                static_cast<float>(pi / 2.0f * std::cos(pi / 2.0f * (m_ticksTotal - m_ticksRemaining) / m_ticksTotal));
            m_ticksRemaining--;
        }
        else
        {
            m_startTranslationPosition = sf::Vector2f(0, 0);
            m_finalTranslationPosition = sf::Vector2f(0, 0);
            m_ticksTotal = 0;
            SetPosition(m_position);
        }
    }
    BoundsCollision(m_position, m_dimensions);
    BoundsCollision(m_previousPosition, m_previousDimensions);
}

void Camera::Interpolate(float lag)
{
    m_view.setCenter(m_previousPosition + (m_position - m_previousPosition) * lag);
    m_view.setSize(m_previousDimensions + (m_dimensions - m_previousDimensions) * lag);
}

// Set to static position
void Camera::SetPosition(const sf::Vector2f& position)
{
    if (m_mode != CameraMode::Static && m_mode != CameraMode::Moving)
    {
        // Reset unused variables
        m_pFollowedEntity = nullptr;
        m_startTranslationPosition = sf::Vector2f(0, 0);
        m_finalTranslationPosition = sf::Vector2f(0, 0);
        m_ticksRemaining = 0;
        m_ticksTotal = 0;
    }

    m_mode = CameraMode::Static;
    m_position = position;
    BoundsCollision(m_position, m_dimensions);
    m_previousPosition = m_position;
}

// Move position by offset with interpolation
void Camera::Move(const sf::Vector2f& offset)
{
    if (m_mode != CameraMode::Static && m_mode != CameraMode::Moving)
    {
        // Reset unused variables
        m_pFollowedEntity = nullptr;
        m_startTranslationPosition = sf::Vector2f(0, 0);
        m_finalTranslationPosition = sf::Vector2f(0, 0);
        m_ticksRemaining = 0;
        m_ticksTotal = 0;
    }

    m_mode = CameraMode::Moving;
    m_moveOffset += offset;
}

// Follow an Entity
void Camera::SetFollow(const Entity& followedEntity, bool snapOnSet)
{
    if (m_mode != CameraMode::Follow)
    {
        // Reset unused variables
        m_startTranslationPosition = sf::Vector2f(0, 0);
        m_finalTranslationPosition = sf::Vector2f(0, 0);
        m_ticksRemaining = 0;
        m_ticksTotal = 0;
    }

    if (m_mode != CameraMode::Follow || m_pFollowedEntity != &followedEntity)
    {
        m_mode = CameraMode::Follow;
        m_pFollowedEntity = &followedEntity;
        if (snapOnSet == true)
        {
            m_position = m_pFollowedEntity->GetPosition();
            BoundsCollision(m_position, m_dimensions);
            m_previousPosition = m_position;
        }
    }
}

// Set to translate from a start position to an end position
void Camera::SetTranslate(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition, unsigned int tickDuration, bool isSlowDownSmooth)
{
    if (m_mode != CameraMode::Translate && m_mode != CameraMode::SmoothTranslate)
    {
        // Reset unused variables
        m_pFollowedEntity = nullptr;
    }

    if (isSlowDownSmooth == false)
    {
        m_mode = CameraMode::Translate;
        m_ticksTotal = 0;
    }
    else
    {
        m_mode = CameraMode::SmoothTranslate;
        m_ticksTotal = tickDuration;
    }

    m_startTranslationPosition = startPosition;
    m_finalTranslationPosition = endPosition;
    BoundsCollision(m_startTranslationPosition, m_dimensions);
    BoundsCollision(m_finalTranslationPosition, m_dimensions);
    m_ticksRemaining = tickDuration;

    m_position = m_startTranslationPosition;
    m_previousPosition = m_position;
}

// Set the max bounds the Camera is allowed to travel in
void Camera::SetBounds(const sf::Vector2f& bounds)
{
    m_bounds = bounds;

    sf::Vector2f newDimensions = m_dimensions;
    if (m_isBoundless == false && m_bounds != sf::Vector2f(0, 0))
    {
        ResizeToFitMaxDimensions(newDimensions, m_bounds);
    }
    m_zoom *= newDimensions.x / m_dimensions.x; // Conserve zoom
    m_dimensions = newDimensions;

    BoundsCollision(m_position, m_dimensions);
    BoundsCollision(m_previousPosition, m_dimensions);

    m_targetDimensions = m_dimensions;
    m_previousDimensions = m_dimensions;
}

// Set the Camera's view's dimensions
void Camera::SetDimensions(const sf::Vector2f& dimensions)
{
    m_dimensions = dimensions;
    ResizeToFitMinDimensions(m_dimensions, m_minDimensions);
    ResizeToFitMaxDimensions(m_dimensions, m_maxDimensions);
    if (m_isBoundless == false && m_bounds != sf::Vector2f(0, 0))
    {
        ResizeToFitMaxDimensions(m_dimensions, m_bounds);
    }

    m_zoom = 1; // Reset zoom

    BoundsCollision(m_position, m_dimensions);
    BoundsCollision(m_previousPosition, m_dimensions);

    m_targetDimensions = m_dimensions;
    m_previousDimensions = m_dimensions;
}

void Camera::SetMinDimensions(const sf::Vector2f& minDimensions)
{
    m_minDimensions = minDimensions;
    ResizeToFitMinDimensions(m_dimensions, m_minDimensions);
    ResizeToFitMinDimensions(m_previousDimensions, m_minDimensions);
    ResizeToFitMinDimensions(m_targetDimensions, m_minDimensions);
}

void Camera::SetMaxDimensions(const sf::Vector2f& maxDimensions)
{
    m_maxDimensions = maxDimensions;
    ResizeToFitMaxDimensions(m_dimensions, m_maxDimensions);
    ResizeToFitMaxDimensions(m_previousDimensions, m_maxDimensions);
    ResizeToFitMaxDimensions(m_targetDimensions, m_maxDimensions);
}

void Camera::Zoom(float zoom)
{
    SetZoom(m_zoom * zoom);
}

void Camera::SetZoom(float absoluteZoom)
{
    // Progressively (lerp) resize Camera dimensions towards target dimensions in Update()
    m_targetDimensions = m_dimensions * absoluteZoom / m_zoom;

    ResizeToFitMinDimensions(m_targetDimensions, m_minDimensions);
    ResizeToFitMaxDimensions(m_targetDimensions, m_maxDimensions);
    if (m_isBoundless == false && m_bounds != sf::Vector2f(0, 0))
    {
        ResizeToFitMaxDimensions(m_targetDimensions, m_bounds);
    }
}
