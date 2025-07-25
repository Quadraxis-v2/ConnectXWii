/*
App_Loop.cpp --- App processing
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


#include "../../include/App.hpp"
#include "../../include/video/Time.hpp"
#include "../../include/players/AI.hpp"


/**
 * @brief Handles all the data updates between frames
 */
void App::OnLoop() noexcept
{
    Time::GetInstance().OnLoop();

    switch (_eStateCurrent)
    {
    case EState::STATE_INGAME:
    case EState::STATE_PROMPT:
    {
        if (typeid(*(_vectorpPlayers[_uyCurrentPlayer])) == typeid(AI))
            _htAnimations.at("Loading")->OnAnimate();
        break;
    }
    case EState::STATE_END: _htAnimations.at("Win")->OnAnimate(); break;
    default: break;
    }
}
