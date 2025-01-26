#ifndef _PLANE_VECTOR_H_
#define _PLANE_VECTOR_H_

#include <cmath>
#include <utility>

class PlaneVector
{
public:
    PlaneVector() = default;
    PlaneVector( double x, double y ) : x( x ), y( y ) { }
    PlaneVector( const PlaneVector& rhs ) { x = rhs.x, y = rhs.y; }
    ~PlaneVector() = default;

    PlaneVector operator+( const PlaneVector& rhs ) const
    {
        return PlaneVector( x + rhs.x, y + rhs.y );
    }
    PlaneVector& operator+=( const PlaneVector& rhs )
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    PlaneVector operator-( const PlaneVector& rhs ) const
    {
        return PlaneVector( x - rhs.x, y - rhs.y );
    }
    PlaneVector& operator-=( const PlaneVector& rhs )
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    double operator*( const PlaneVector& rhs ) const
    {
        return x * rhs.x + y * rhs.y;
    }
    PlaneVector operator*( double coef ) const
    {
        return PlaneVector( coef * x, coef * y );
    }
    PlaneVector& operator*=( double coef )
    {
        x *= coef;
        y *= coef;
        return *this;
    }
    PlaneVector& operator=( PlaneVector&& rhs ) noexcept
    {
        if ( this != &rhs )
        {
            x = std::move( rhs.x );
            y = std::move( rhs.y );
        }
        return *this;
    }
    PlaneVector& operator=( const PlaneVector& rhs )
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    double GetLength() const
    {
        return std::sqrt( x * x + y * y );
    }

    PlaneVector ToNormalized() const
    {
        double length = GetLength();
        if ( length == 0 ) return PlaneVector();
        else return PlaneVector( x / length, y / length );
    }

public:
    double x = 0;
    double y = 0;
};

#endif // !_PLANE_VECTOR_H_