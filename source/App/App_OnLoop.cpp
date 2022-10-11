#include <cstdint>
#include <SDL_thread.h>
#include "../../include/App.hpp"
#include "../../include/players/AI.hpp"
#include "../../include/Grid.hpp"

static Grid* Spgrid;

int32_t SDLCALL RunAI(void* pData);


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
        if (!_bAIRunning && typeid(*(_vectorpPlayers[_uyCurrentPlayer])) == typeid(AI))
        {
            _bAIRunning = true;
            Spgrid = &_grid;
            SDL_CreateThread(RunAI, dynamic_cast<AI*>(_vectorpPlayers[_uyCurrentPlayer]));

            //++_uyCurrentPlayer %= _vectorpPlayers.size();

            // If the game is won or there is a draw go to the corresponding state
            if (_grid.CheckWinner() != Grid::EPlayerMark::GRID_TYPE_NONE || _grid.IsFull())
                _eStateCurrent = EState::STATE_END;
        }
        break;
    }
    default: break;
    }
}


int32_t SDLCALL RunAI(void* pData)
{
    static_cast<AI*>(pData)->ChooseMove(*Spgrid);
    return 0;
}

