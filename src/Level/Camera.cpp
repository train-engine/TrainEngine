#include "Level/Camera.h"
#include <cmath>
#include "Misc/Utility.h"

namespace
{
    const double pi = 3.141592653589793238462643383279;
} // namespace

Camera::Camera()
    : m_mode(CameraMode::Static)
    , m_isBoundless(true)
    , m_minDimensions(128, 72)
    , m_maxDimensions(2560, 1440)
    , m_zoom(1)
    , m_zoomLerp(0.25)
    , m_followedEntity(nullptr)
    , m_followLerp(0.3)
    , m_ticksRemaining(0)
    , m_ticksTotal(0)
{
    setDimensions(m_minDimensions);
}

// Resize the given dimensions to be at least as big as the given minimum dimensions while preserving their aspect ratio
void Camera::resizeToFitMinDimensions(sf::Vector2f& dimensions, const sf::Vector2f& minDimensions)
{
    // If there is no need to resize
    if (dimensions.x > minDimensions.x && dimensions.y > minDimensions.y)
    {
        return;
    }

    dimensions *= Utility::getScaleToFill(dimensions, minDimensions);
}

// Resize the given dimensions to fit the given maximum dimensions while preserving their aspect ratio
void Camera::resizeToFitMaxDimensions(sf::Vector2f& dimensions, const sf::Vector2f& maxDimensions)
{
    // If there is no need to resize
    if (dimensions.x < maxDimensions.x && dimensions.y < maxDimensions.y)
    {
        return;
    }

    dimensions *= Utility::getScaleToFit(dimensions, maxDimensions);
}

// Modify the given position to fix any possible out-of-bounds error
void Camera::boundsCollision(sf::Vector2f& position, const sf::Vector2f& dimensions)
{
    if (m_isBoundless == true || m_bounds == sf::Vector2f(0, 0))
    {
        return;
    }

    // Horizontal Map edge stopping
    if (position.x - dimensions.x / 2 < 0)
    {
        position.x = dimensions.x / 2;
    }
    else if (position.x + dimensions.x / 2 > m_bounds.x)
    {
        position.x = m_bounds.x - dimensions.x / 2;
    }
    // Vertical Map edge stopping
    if (position.y - dimensions.y / 2 < 0)
    {
        position.y = dimensions.y / 2;
    }
    else if (position.y + dimensions.y / 2 > m_bounds.y)
    {
        position.y = m_bounds.y - dimensions.y / 2;
    }
}

void Camera::update()
{
    m_previousDimensions = m_dimensions;
    m_previousPosition = m_position;

    // Zoom lerp
    static const float epsilon = 1e-6;
    if (std::fabs((m_dimensions.x - m_targetDimensions.x) / m_targetDimensions.x) > epsilon ||
        std::fabs((m_dimensions.y - m_targetDimensions.y) / m_targetDimensions.y) > epsilon)
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
            boundsCollision(m_position, m_dimensions);
        }

        if (m_dimensions != m_targetDimensions)
        {
            boundsCollision(m_position, m_dimensions);
            boundsCollision(m_previousPosition, m_previousDimensions);
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
            // Disable horizontal lerp to snap to left or right of bounds when zooming
            mustSnapHorizontally =
                ((m_previousPosition.x - m_previousDimensions.x / 2 <= 0 && // If Camera is colliding with the left bounds
                  m_previousPosition.x >= m_followedEntity->getPosition().x) || // And if Camera is to the right of followed Entity
                 (m_previousPosition.x + m_previousDimensions.x / 2 >= m_bounds.x && // Or, if Camera is colliding with the right bounds
                  m_previousPosition.x <= m_followedEntity->getPosition().x)); // And if Camera is to the left of followed Entity

            // Disable vertical lerp to snap to bottom or top of bounds when zooming
            mustSnapVertically =
                ((m_previousPosition.y - m_previousDimensions.y / 2 <= 0 && // If Camera is colliding with the top bounds
                  m_previousPosition.y >= m_followedEntity->getPosition().y) || // And if Camera is below followed Entity
                 (m_previousPosition.y + m_previousDimensions.y / 2 >= m_bounds.y && // Or, if Camera is colliding with the bottom bounds
                  m_previousPosition.y <= m_followedEntity->getPosition().y)); // And if Camera above of followed Entity
        }
        else
        {
            mustSnapHorizontally = false;
            mustSnapVertically = false;
        }

        if (mustSnapHorizontally == true) // Zooming with horizontal lerp and interpolation disabled
        {
            m_position.x = m_followedEntity->getPosition().x;
            m_previousPosition.x = m_position.x;
        }
        else
        {
            m_position.x += (m_followedEntity->getPosition().x - m_position.x) * m_followLerp;
        }

        if (mustSnapVertically == true) // Zooming with vertical lerp and interpolation disabled
        {
            m_position.y = m_followedEntity->getPosition().y;
            m_previousPosition.y = m_position.y;
        }
        else
        {
            m_position.y += (m_followedEntity->getPosition().y - m_position.y) * m_followLerp;
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
            setPosition(m_position);
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
            setPosition(m_position);
        }
    }
    boundsCollision(m_position, m_dimensions);
    boundsCollision(m_previousPosition, m_previousDimensions);
}

void Camera::interpolate(float lag)
{
    m_view.setCenter(m_previousPosition + (m_position - m_previousPosition) * lag);
    m_view.setSize(m_previousDimensions + (m_dimensions - m_previousDimensions) * lag);
}

// Set to static position
void Camera::setPosition(const sf::Vector2f& position)
{
    if (m_mode != CameraMode::Static && m_mode != CameraMode::Moving)
    {
        // Reset unused variables
        m_followedEntity = nullptr;
        m_startTranslationPosition = sf::Vector2f(0, 0);
        m_finalTranslationPosition = sf::Vector2f(0, 0);
        m_ticksRemaining = 0;
        m_ticksTotal = 0;
    }

    m_mode = CameraMode::Static;
    m_position = position;
    boundsCollision(m_position, m_dimensions);
    m_previousPosition = m_position;
}

// Move position by offset with interpolation
void Camera::move(const sf::Vector2f& offset)
{
    if (m_mode != CameraMode::Static && m_mode != CameraMode::Moving)
    {
        // Reset unused variables
        m_followedEntity = nullptr;
        m_startTranslationPosition = sf::Vector2f(0, 0);
        m_finalTranslationPosition = sf::Vector2f(0, 0);
        m_ticksRemaining = 0;
        m_ticksTotal = 0;
    }

    m_mode = CameraMode::Moving;
    m_moveOffset += offset;
}

// Follow an Entity
void Camera::setFollow(const Entity& followedEntity, bool snapOnSet)
{
    if (m_mode != CameraMode::Follow)
    {
        // Reset unused variables
        m_startTranslationPosition = sf::Vector2f(0, 0);
        m_finalTranslationPosition = sf::Vector2f(0, 0);
        m_ticksRemaining = 0;
        m_ticksTotal = 0;
    }

    if (m_mode != CameraMode::Follow || m_followedEntity != &followedEntity)
    {
        m_mode = CameraMode::Follow;
        m_followedEntity = &followedEntity;
        if (snapOnSet == true)
        {
            m_position = m_followedEntity->getPosition();
            boundsCollision(m_position, m_dimensions);
            m_previousPosition = m_position;
        }
    }
}

// Set to translate from a start position to an end position
void Camera::setTranslate(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition, unsigned int tickDuration,
                          bool isSlowDownSmooth)
{
    if (m_mode != CameraMode::Translate && m_mode != CameraMode::SmoothTranslate)
    {
        // Reset unused variables
        m_followedEntity = nullptr;
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
    boundsCollision(m_startTranslationPosition, m_dimensions);
    boundsCollision(m_finalTranslationPosition, m_dimensions);
    m_ticksRemaining = tickDuration;

    m_position = m_startTranslationPosition;
    m_previousPosition = m_position;
}

// Set the max bounds the Camera is allowed to travel in
void Camera::setBounds(const sf::Vector2f& bounds)
{
    m_bounds = bounds;

    sf::Vector2f newDimensions = m_dimensions;
    if (m_isBoundless == false && m_bounds != sf::Vector2f(0, 0))
    {
        resizeToFitMaxDimensions(newDimensions, m_bounds);
    }
    m_zoom *= newDimensions.x / m_dimensions.x; // Conserve zoom
    m_dimensions = newDimensions;

    boundsCollision(m_position, m_dimensions);
    boundsCollision(m_previousPosition, m_dimensions);

    m_targetDimensions = m_dimensions;
    m_previousDimensions = m_dimensions;
}

// Set the Camera's view's dimensions
void Camera::setDimensions(const sf::Vector2f& dimensions)
{
    m_dimensions = dimensions;
    resizeToFitMinDimensions(m_dimensions, m_minDimensions);
    resizeToFitMaxDimensions(m_dimensions, m_maxDimensions);
    if (m_isBoundless == false && m_bounds != sf::Vector2f(0, 0))
    {
        resizeToFitMaxDimensions(m_dimensions, m_bounds);
    }

    m_zoom = 1; // Reset zoom

    boundsCollision(m_position, m_dimensions);
    boundsCollision(m_previousPosition, m_dimensions);

    m_targetDimensions = m_dimensions;
    m_previousDimensions = m_dimensions;
}

void Camera::setMinDimensions(const sf::Vector2f& minDimensions)
{
    m_minDimensions = minDimensions;
    resizeToFitMinDimensions(m_dimensions, m_minDimensions);
    resizeToFitMinDimensions(m_previousDimensions, m_minDimensions);
    resizeToFitMinDimensions(m_targetDimensions, m_minDimensions);
}

void Camera::setMaxDimensions(const sf::Vector2f& maxDimensions)
{
    m_maxDimensions = maxDimensions;
    resizeToFitMaxDimensions(m_dimensions, m_maxDimensions);
    resizeToFitMaxDimensions(m_previousDimensions, m_maxDimensions);
    resizeToFitMaxDimensions(m_targetDimensions, m_maxDimensions);
}

void Camera::zoom(float zoom)
{
    setZoom(m_zoom * zoom);
}

void Camera::setZoom(float absoluteZoom)
{
    // Progressively (lerp) resize Camera dimensions towards target dimensions in update()
    m_targetDimensions = m_dimensions * absoluteZoom / m_zoom;

    resizeToFitMinDimensions(m_targetDimensions, m_minDimensions);
    resizeToFitMaxDimensions(m_targetDimensions, m_maxDimensions);
    if (m_isBoundless == false && m_bounds != sf::Vector2f(0, 0))
    {
        resizeToFitMaxDimensions(m_targetDimensions, m_bounds);
    }
}
