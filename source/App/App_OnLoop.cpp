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
        if (_bAITurn)
        {
            dynamic_cast<AI*>(_vectorPlayers.at(0))->ChooseMove(_grid);
            _ePlayerMarkCurrent = Grid::NextPlayer(_ePlayerMarkCurrent);

            // If the game is won or there is a draw go to the corresponding state
            if (_grid.CheckWinner() != Grid::EPlayerMark::GRID_TYPE_NONE || _grid.IsFull())
                _eStateCurrent = EState::STATE_END;
            else _bAITurn = false;
        }
        break;
    }
    default: break;
    }
}
