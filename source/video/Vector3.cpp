/*
Vector3.cpp --- 3D vector class
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

#include <limits>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <ios>
#include <iomanip>

#include "../../include/video/Vector3.hpp"


const Vector3 Vector3::SCvectorBack{0, 0, -1}; 
const Vector3 Vector3::SCvectorDown{0, -1, 0};
const Vector3 Vector3::SCvectorForward{0, 0, 1};
const Vector3 Vector3::SCvectorLeft{-1, 0, 0};
const Vector3 Vector3::SCvectorNegativeInfinity{-std::numeric_limits<float>::infinity(),
    -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()};
const Vector3 Vector3::SCvectorOne{1, 1, 1};
const Vector3 Vector3::SCvectorPositiveInfinity{std::numeric_limits<float>::infinity(), 
    std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()};
const Vector3 Vector3::SCvectorRight{1, 0, 0};
const Vector3 Vector3::SCvectorUp{0, 1, 0};
const Vector3 Vector3::SCvectorZero{0, 0, 0};


void Vector3::Set(float fNewX, float fNewY, float fNewZ) noexcept
{
    fX = fNewX;
    fY = fNewY;
    fZ = fNewZ;
}


Vector3::Vector3(float fNewX, float fNewY, float fNewZ) noexcept : fX{fNewX}, fY{fNewY}, fZ{fNewZ}
{}


Vector3 Vector3::operator -() const noexcept
{ return *this * -1; }


Vector3& Vector3::operator +=(const Vector3& CvectorOther) noexcept
{
    fX += CvectorOther.fX;
    fY += CvectorOther.fY;
    fZ += CvectorOther.fZ;

    return *this;
}


Vector3& Vector3::operator -=(const Vector3& CvectorOther) noexcept
{ return *this += -CvectorOther; }


Vector3& Vector3::operator *=(float fScalar) noexcept
{
    fX *= fScalar;
    fY *= fScalar;
    fZ *= fScalar;

    return *this;
}


Vector3& Vector3::operator /=(float fScalar) noexcept
{ return *this *= 1 / fScalar; }


Vector3 Vector3::operator *(float fScalar) const noexcept 
{ 
    Vector3 vectorTemp{*this};
    return vectorTemp *= fScalar; 
}


Vector3 Vector3::operator /(float fScalar) const noexcept
{ 
    Vector3 vectorTemp{*this};
    return vectorTemp /= fScalar; 
}


float Vector3::operator [](uint8_t uyComponent) const
{
    switch (uyComponent)
    {
    case 0:     return fX;  break;
    case 1:     return fY;  break;
    case 2:     return fZ;  break;
    default:    throw std::invalid_argument("Invalid component index"); break;
    }
}


float& Vector3::operator [](uint8_t uyComponent)
{
    switch (uyComponent)
    {
    case 0:     return fX;  break;
    case 1:     return fY;  break;
    case 2:     return fZ;  break;
    default:    throw std::invalid_argument("Invalid component index"); break;
    }
}


Vector3::operator std::string() const noexcept
{
    std::ostringstream ossTemp{};
    ossTemp << *this;
    return ossTemp.str();
}


Vector3 Vector3::Cross(const Vector3& Cvector1, const Vector3& Cvector2) noexcept
{ 
    return Vector3(Cvector1.fY * Cvector2.fZ - Cvector1.fZ * Cvector2.fY, 
        Cvector1.fZ * Cvector2.fX - Cvector1.fX * Cvector2.fZ, 
        Cvector1.fX * Cvector2.fY - Cvector1.fY * Cvector2.fX); 
}


double Vector3::SquaredDistance(const Vector3& Cvector1, const Vector3& Cvector2) noexcept
{ return (Cvector2 - Cvector1).SquaredMagnitude(); }


double Vector3::Distance(const Vector3& Cvector1, const Vector3& Cvector2) noexcept
{ return std::sqrt(Vector3::SquaredDistance(Cvector1, Cvector2)); }


double Vector3::Dot(const Vector3& Cvector1, const Vector3& Cvector2) noexcept
{ return Cvector1.fX * Cvector2.fX + Cvector1.fY * Cvector2.fY + Cvector1.fZ * Cvector2.fZ; }


Vector3 Vector3::Max(const Vector3& Cvector1, const Vector3& Cvector2) noexcept
{
    return Vector3(std::max(Cvector1.fX, Cvector2.fX), std::max(Cvector1.fY, Cvector2.fY), 
        std::max(Cvector1.fZ, Cvector2.fZ));
}


Vector3 Vector3::Min(const Vector3& Cvector1, const Vector3& Cvector2) noexcept
{
    return Vector3(std::min(Cvector1.fX, Cvector2.fX), std::min(Cvector1.fY, Cvector2.fY), 
        std::min(Cvector1.fZ, Cvector2.fZ));
}


Vector3 Vector3::Normalize(const Vector3& Cvector) noexcept
{
    double dMagnitude{Cvector.Magnitude()};
    return Vector3(Cvector.fX / dMagnitude, Cvector.fY / dMagnitude, Cvector.fZ / dMagnitude);
}


double Vector3::SquaredMagnitude() const noexcept 
{ 
    double dX{fX}, dY{fY}, dZ{fZ};
    return dX * dX + dY * dY + dZ * dZ;
}


double Vector3::Magnitude() const noexcept 
{ return std::sqrt(SquaredMagnitude()); }


void Vector3::Normalize() noexcept
{ *this = Vector3::Normalize(*this); }


bool Vector3::Equals(const Vector3& CvectorOther) const noexcept
{ return fX == CvectorOther.fX && fY == CvectorOther.fY && CvectorOther.fZ == CvectorOther.fZ; }


Vector3 operator +(const Vector3& Cvector1, const Vector3& Cvector2) noexcept
{
    Vector3 vectorTemp{Cvector1};
    return vectorTemp += Cvector2;
}


Vector3 operator -(const Vector3& Cvector1, const Vector3& Cvector2) noexcept
{
    Vector3 vectorTemp{Cvector1};
    return vectorTemp -= Cvector2;
}


bool operator ==(const Vector3& Cvector1, const Vector3& Cvector2) noexcept
{ return Vector3::SquaredDistance(Cvector1, Cvector2) < 0.0000000001; }


bool operator !=(const Vector3& Cvector1, const Vector3& Cvector2) noexcept
{ return !(Cvector1 == Cvector2); }


std::ostream& operator <<(std::ostream& ostreamOut, const Vector3& CvectorOut) noexcept
{
    return ostreamOut << "(" << std::fixed << std::setprecision(2) << CvectorOut.fX << ", " << 
        CvectorOut.fY << ", " << CvectorOut.fZ << ")";
}
