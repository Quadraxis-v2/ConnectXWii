/*
WiiController.hpp --- Wii controller wrapper
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

#ifndef _WIICONTROLLER_HPP_
#define _WIICONTROLLER_HPP_

#include <cstdint>
#include <SDL_joystick.h>
#include <SDL_mouse.h>

#include "SDL_Wii_Joystick.hpp"


/**
 * @brief WiiController class
 */
class WiiController : public SDL_Wii_Joystick
{
public:
    static const uint8_t SCuyMaxWiiControllers{4}; /**< Maximum number of Wii controller supported by SDL-wii */


    int16_t GetRightTriggerClassic() const noexcept;
    int16_t GetLeftTriggerClassic() const noexcept;
    float GetPitch() const noexcept;
    float GetRoll() const noexcept;
    float GetYaw() const noexcept;
    bool GetButton1() const noexcept;
    bool GetButton2() const noexcept;
    bool GetButtonMinus() const noexcept;
    bool GetButtonPlus() const noexcept;
    bool GetButtonHOME() const noexcept;
    bool GetButtonZ() const noexcept;
    bool GetButtonC() const noexcept;
    bool GetButtonAClassic() const noexcept;
    bool GetButtonBClassic() const noexcept;
    bool GetButtonXClassic() const noexcept;
    bool GetButtonYClassic() const noexcept;
    bool GetButtonLClassic() const noexcept;
    bool GetButtonRClassic() const noexcept;
    bool GetButtonZLClassic() const noexcept;
    bool GetButtonZRClassic() const noexcept;
    bool GetButtonMinusClassic() const noexcept;
    bool GetButtonPlusClassic() const noexcept;
    bool GetButtonHOMEClassic() const noexcept;


    /**
     * @brief Construct a new Wii Controller
     * 
     * @param uyIndex the index fot the new controller
     */
    explicit WiiController(uint8_t uyIndex);
    WiiController(const WiiController& CwiiControllerOther) = delete;   /**< Copy constructor */
    WiiController(WiiController&& wiiControllerOther) = default;        /**< Move constructor */


    WiiController& operator =(const WiiController& CwiiControllerOther) = delete;   /**< Copy assignment operator */
    WiiController& operator =(WiiController&& wiiControllerOther) = default;        /**< Move assignment operator */


    float GetMotionAngle(uint32_t uiAxis) const noexcept;

};


inline int16_t WiiController::GetRightTriggerClassic() const noexcept { return GetAxis(4); }
inline int16_t WiiController::GetLeftTriggerClassic() const noexcept { return GetAxis(5); }
inline float WiiController::GetPitch() const noexcept { return GetMotionAngle(6); }
inline float WiiController::GetRoll() const noexcept { return GetMotionAngle(7); }
inline float WiiController::GetYaw() const noexcept { return GetMotionAngle(8); }
inline bool WiiController::GetButton1() const noexcept { return GetButton(2); }
inline bool WiiController::GetButton2() const noexcept { return GetButton(3); }
inline bool WiiController::GetButtonMinus() const noexcept { return GetButton(4); }
inline bool WiiController::GetButtonPlus() const noexcept { return GetButton(5); }
inline bool WiiController::GetButtonHOME() const noexcept { return GetButton(6); }
inline bool WiiController::GetButtonZ() const noexcept { return GetButton(7); }
inline bool WiiController::GetButtonC() const noexcept { return GetButton(8); }
inline bool WiiController::GetButtonAClassic() const noexcept { return GetButton(9); }
inline bool WiiController::GetButtonBClassic() const noexcept { return GetButton(10); }
inline bool WiiController::GetButtonXClassic() const noexcept { return GetButton(11); }
inline bool WiiController::GetButtonYClassic() const noexcept { return GetButton(12); }
inline bool WiiController::GetButtonLClassic() const noexcept { return GetButton(13); }
inline bool WiiController::GetButtonRClassic() const noexcept { return GetButton(14); }
inline bool WiiController::GetButtonZLClassic() const noexcept { return GetButton(15); }
inline bool WiiController::GetButtonZRClassic() const noexcept { return GetButton(16); }
inline bool WiiController::GetButtonMinusClassic() const noexcept { return GetButton(17); }
inline bool WiiController::GetButtonPlusClassic() const noexcept { return GetButton(18); }
inline bool WiiController::GetButtonHOMEClassic() const noexcept { return GetButton(19); }


#endif