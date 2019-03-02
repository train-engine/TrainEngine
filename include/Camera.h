#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>
#include "Entity.h"

enum class CameraMode
{
    Static,
    Moving,
    Follow,
    Translate,
    SmoothTranslate
};

class Camera final
{
private:
    sf::View m_view;

    CameraMode m_mode;
    bool m_isBoundless; // If the Camera should be contained within the bounds

    sf::Vector2f m_bounds; // Area the Camera is allowed to travel in (generally Map bounds)

    sf::Vector2f m_position;
    sf::Vector2f m_previousPosition; // Previous position for interpolation
    sf::Vector2f m_moveOffset; // Position offset for free movement

    sf::Vector2f m_dimensions;
    sf::Vector2f m_previousDimensions; // Previous dimensions for interpolation
    sf::Vector2f m_targetDimensions; // Target dimensions for zoom lerp (lerp if not identical to m_dimensions)

    sf::Vector2f m_minDimensions;
    sf::Vector2f m_maxDimensions;

    float m_zoom;
    float m_zoomLerp;

    // For CameraMode::Follow
    const Entity* m_pFollowedEntity;
    float m_followLerp;

    // For CameraMode::Translate
    sf::Vector2f m_startTranslationPosition;
    sf::Vector2f m_finalTranslationPosition;
    unsigned int m_ticksRemaining;
    unsigned int m_ticksTotal;

    // Functions
    void ResizeToFitMinDimensions(sf::Vector2f& rDimensions, const sf::Vector2f& minDimensions);
    void ResizeToFitMaxDimensions(sf::Vector2f& rDimensions, const sf::Vector2f& maxDimensions);
    void BoundsCollision(sf::Vector2f& rPosition, const sf::Vector2f& dimensions);

public:
    // Constructor
    Camera();

    // Functions
    void Update();
    void Interpolate(float lag);
    void SetPosition(const sf::Vector2f& position);
    void Move(const sf::Vector2f& offset);
    void SetFollow(const Entity& followedEntity, bool snapOnSet = false);
    void SetTranslate(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition, unsigned int tickDuration, bool isSlowDownSmooth = false);

    // Setters
    void SetBoundless(bool isBoundless) {m_isBoundless = isBoundless;}
    void SetBounds(const sf::Vector2f& bounds);
    void SetDimensions(const sf::Vector2f& dimensions);
    void SetMinDimensions(const sf::Vector2f& minDimensions);
    void SetMaxDimensions(const sf::Vector2f& maxDimensions);
    void Zoom(float zoom);
    void SetZoom(float absoluteZoom);
    void SetZoomLerp(float zoomLerp) {m_zoomLerp = zoomLerp;}
    void SetFollowLerp(float followLerp) {m_followLerp = followLerp;}
    void SetRotation(float angle) {m_view.setRotation(angle);}

    // Getters
    const sf::View& GetView() const {return m_view;}
    CameraMode GetMode() const {return m_mode;}
    bool IsBoundless() const {return m_isBoundless;}
    const sf::Vector2f& GetPosition() const {return m_view.getCenter();}
    const sf::Vector2f& GetDimensions() const {return m_view.getSize();}
    float GetZoom() const {return m_zoom;}
};

#endif // CAMERA_H
