#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "PlaneVector.h"
#include "CollisionBox.h"
#include "Camera.h"

#include <SDL.h>

// renderable entity
class Entity
{
public:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void OnRender( const Camera& cam ) = 0;

    void MakeInvalid();

    const PlaneVector& GetPosition() const { return position; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    PlaneVector GetTexCenter() const;

    bool CheckValid() { return is_valid; }

    void SetPosition( const PlaneVector& pos ) { position = pos; }
    void SetLogicCenter( const PlaneVector& l_center );

    CollisionBox* const GetCollisionBox() const { return collision_box; } // TODO: optimize structure

    /* TODO
protected:
    void InitTexture( const std::string& id, float scale_ratio );

    virtual void InitCollisionBox() = 0;
    */

protected:
    virtual void UpdateCollisionBox() = 0;

protected:
    SDL_Texture* texture = nullptr; // texture destruction lies on ResourceManager

    PlaneVector position = { 0.0, 0.0 };
    int width = 0;
    int height = 0;

    bool is_valid = true;
    CollisionBox* collision_box;
};

#endif