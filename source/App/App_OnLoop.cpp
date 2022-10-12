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
 * @brief 
 * 
 * @param pData 
 * @return int32_t 
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
