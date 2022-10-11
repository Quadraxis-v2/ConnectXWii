#include <cstdint>
#include "SDL_thread.h"
#include "../../include/App.hpp"
#include "../../include/players/AI.hpp"
#include "../../include/Grid.hpp"

static Grid* Spgrid;

int32_t SDLCALL test(void* data)
{
    AI* pAI = (AI*)data;
    pAI->ChooseMove(*Spgrid);

    return 0;
}


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
        if (_bAITurn)
        {
            _bAITurn = false;
            Spgrid = &_grid;
            AI* pAI = dynamic_cast<AI*>(_vectorPlayers.at(1));

            SDL_CreateThread(test, pAI);
            

            _ePlayerMarkCurrent = Grid::NextPlayer(_ePlayerMarkCurrent);

            // If the game is won or there is a draw go to the corresponding state
            if (_grid.CheckWinner() != Grid::EPlayerMark::GRID_TYPE_NONE || _grid.IsFull())
                _eStateCurrent = EState::STATE_END;
        }
        break;
    }
    default: break;
    }
}


