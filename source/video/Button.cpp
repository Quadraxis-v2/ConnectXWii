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

#include <stdexcept>

#include "../../include/video/Button.hpp"


Button::Button(const Vector3& CvectorTopLeft, const Vector3& CvectorBottomRight) noexcept : 
    _vectorTopLeft{CvectorTopLeft}, _vectorBottomRight{CvectorBottomRight}
{}


bool Button::IsInside(const Vector3& CvectorLocation) const noexcept
{
    return (CvectorLocation.fX >= _vectorTopLeft.fX && CvectorLocation.fX <= _vectorBottomRight.fX &&
        CvectorLocation.fY >= _vectorTopLeft.fY && CvectorLocation.fY <= _vectorBottomRight.fY);
}
