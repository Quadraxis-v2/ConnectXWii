/*
Button.hpp --- Clickable button class
Copyright (C) 2025  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)
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

#ifndef _BUTTON_HPP_
#define _BUTTON_HPP_

#include <cstdint>

#include "Vector3.hpp"
#include "Surface.hpp"


class Button
{
public:
    const Vector3& GetTopLeft() const noexcept;
    void SetTopLeft(const Vector3& CvectorTopLeft) noexcept;
    const Vector3& GetBottomRight() const noexcept;
    void SetBottomRight(const Vector3& CvectorBottomRight) noexcept;


    Button(const Vector3& CvectorTopLeft, const Vector3& CvectorBottomRight) noexcept;


    bool IsInside(const Vector3& CvectorLocation) const noexcept;

private:
    Vector3 _vectorTopLeft;
    Vector3 _vectorBottomRight;

};

inline const Vector3& Button::GetTopLeft() const noexcept { return _vectorTopLeft; }
inline void Button::SetTopLeft(const Vector3& CvectorTopLeft) noexcept { _vectorTopLeft = CvectorTopLeft; }
inline const Vector3& Button::GetBottomRight() const noexcept { return _vectorBottomRight; }
inline void Button::SetBottomRight(const Vector3& CvectorBottomRight) noexcept 
{ _vectorBottomRight = CvectorBottomRight; }

#endif