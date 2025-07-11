/*
Vector3.hpp --- 3D vector class
Copyright (C) 2022  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)
juan.dlcruzcg@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef _VECTOR3_HPP_
#define _VECTOR3_HPP_


#include <cstdint>
#include <string>
#include <ostream>


class Vector3
{
public:
    static const Vector3 SCvectorBack;
    static const Vector3 SCvectorDown;
    static const Vector3 SCvectorForward;
    static const Vector3 SCvectorLeft; 
    static const Vector3 SCvectorNegativeInfinity;
    static const Vector3 SCvectorOne;
    static const Vector3 SCvectorPositiveInfinity;
    static const Vector3 SCvectorRight;
    static const Vector3 SCvectorUp;
    static const Vector3 SCvectorZero; 

    float fX, fY, fZ;


    void Set(float fNewX, float fNewY, float fNewZ) noexcept;


    Vector3(float fNewX, float fNewY, float fNewZ = 0) noexcept;


    Vector3 operator -() const noexcept;
    Vector3& operator +=(const Vector3& CvectorOther) noexcept;
    Vector3& operator -=(const Vector3& CvectorOther) noexcept;
    Vector3& operator *=(float fScalar) noexcept;
    Vector3& operator /=(float fScalar) noexcept;
    Vector3 operator *(float fScalar) const noexcept;
    Vector3 operator /(float fScalar) const noexcept;
    float operator [](uint8_t uyComponent) const;
    float& operator [](uint8_t uyComponent);
    operator std::string() const noexcept;

    static Vector3 Cross(const Vector3& Cvector1, const Vector3& Cvector2) noexcept;
    static double SquaredDistance(const Vector3& Cvector1, const Vector3& Cvector2) noexcept;
    static double Distance(const Vector3& Cvector1, const Vector3& Cvector2) noexcept;
    static double Dot(const Vector3& Cvector1, const Vector3& Cvector2) noexcept;
    static Vector3 Max(const Vector3& Cvector1, const Vector3& Cvector2) noexcept;
    static Vector3 Min(const Vector3& Cvector1, const Vector3& Cvector2) noexcept;
    static Vector3 Normalize(const Vector3& Cvector) noexcept;

    double SquaredMagnitude() const noexcept;
    double Magnitude() const noexcept;
    void Normalize() noexcept;
    bool Equals(const Vector3& CvectorOther) const noexcept;

private:

};

Vector3 operator +(const Vector3& Cvector1, const Vector3& Cvector2) noexcept;
Vector3 operator -(const Vector3& Cvector1, const Vector3& Cvector2) noexcept;
bool operator ==(const Vector3& Cvector1, const Vector3& Cvector2) noexcept;
bool operator !=(const Vector3& Cvector1, const Vector3& Cvector2) noexcept;

std::ostream& operator <<(std::ostream& ostreamOut, const Vector3& CvectorOut) noexcept;

#endif
