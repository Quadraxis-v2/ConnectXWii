/*
App_OnLoop.cpp --- App processing
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

#include <cstdint>
#include <SDL_thread.h>
#include "../../include/App.hpp"
#include "../../include/players/AI.hpp"
#include "../../include/Grid.hpp"


/**
 * @brief Handles all the data updates between frames
 */
void App::OnLoop() noexcept
{
    switch (_eStateCurrent)
    {
    case STATE_INGAME:
    {
        // AIs' turn
        if (typeid(*(_vectorpPlayers[_uyCurrentPlayer])) == typeid(AI) && !_bIsAIRunning)
        {
            _bIsAIRunning = true;
            SDL_CreateThread(RunAI, this);
        }
        break;
    }
    default: break;
    }
}


/**
 * @brief Callback for running the AI algorithm in a separate thread
 * 
 * @param pData pointer to the globaL App object
 * @return int32_t error code of the thread
 */
int32_t SDLCALL RunAI(void* pData)
{
    App* pApp = static_cast<App*>(pData);

    dynamic_cast<AI*>(pApp->_vectorpPlayers[pApp->_uyCurrentPlayer])->ChooseMove(pApp->_grid);

    ++(pApp->_uyCurrentPlayer) %= pApp->_vectorpPlayers.size();

    // If the game is won or there is a draw go to the corresponding state
    if (pApp->_grid.CheckWinner() != Grid::EPlayerMark::EMPTY || pApp->_grid.IsFull())
        pApp->_eStateCurrent = App::EState::STATE_END;

    pApp->_bIsAIRunning = false;

    return 0;
}
