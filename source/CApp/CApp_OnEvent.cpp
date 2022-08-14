#include <cstdint>
#include <vector>
#include <SDL_events.h>
#include <SDL_mouse.h>
#include "../../include/CApp.hpp"
#include "../../include/players/AI.hpp"
#include "../../include/players/Human.hpp"


void CApp::OnEvent(SDL_Event* pSdlEvent) noexcept { CEvent::OnEvent(pSdlEvent); }


void CApp::OnLButtonDown(int32_t iMouseX, int32_t iMouseY)
{
    switch (_EcurrentState)
    {
        case State_t::STATE_START:
        {
            _EcurrentState = State_t::STATE_INGAME;

            break;
        }
        case State_t::STATE_INGAME:
        {
            int32_t iColumn = iMouseX / (_pSdlSurfaceDisplay->w / Grid::SCuyWidth);

            if (_grid.isValidPlay(iColumn))
            {
                _grid.makePlay(_EplayerMarkCurrent, iColumn);
                if (_grid.checkWinner() == Grid::PlayerMark::GRID_TYPE_NONE)
                    _EplayerMarkCurrent = Grid::nextPlayer(_EplayerMarkCurrent);
                else _EcurrentState = State_t::STATE_WIN;
            }

            break;
        }
        case State_t::STATE_WIN:
        {
            Reset();

            break;
        }
    }
}


void CApp::OnMouseMove(int32_t iMouseX, int32_t iMouseY, int32_t iRelX, int32_t iRelY, bool bLeft,
    bool bRight, bool bMiddle) noexcept {}


void CApp::OnJoyAxis(uint8_t uyWhich, uint8_t uyAxis, int16_t rValue) noexcept {}


void CApp::OnJoyButtonDown(uint8_t uyWhich, uint8_t uyButton) noexcept
{
    switch (_EcurrentState)
    {
        case State_t::STATE_START:
        {
            switch (uyButton)
            {
                case 0:
                {
                    int32_t iMouseX = 0, iMouseY = 0;
                    SDL_GetMouseState(&iMouseX, &iMouseY);

                    if (iMouseX >= 0 && iMouseX < (CApp::SCurWindowWidth >> 1) && iMouseY >= 0 &&
                        iMouseY < CApp::SCurWindowHeight)
                    {
                        _EcurrentState = State_t::STATE_INGAME;
                        _EplayerMarkCurrent = Grid::PlayerMark::GRID_TYPE_RED;
                        _apPlayer.at(0)->setPlayerMark(Grid::PlayerMark::GRID_TYPE_RED);
                        _apPlayer.push_back(new AI(Grid::PlayerMark::GRID_TYPE_YELLOW));
                    }
                    else if (iMouseX >= (CApp::SCurWindowWidth >> 1) && iMouseX < CApp::SCurWindowWidth &&
                        iMouseY >= 0 && iMouseY < CApp::SCurWindowHeight)
                    {
                        _EcurrentState = State_t::STATE_INGAME;
                        _EplayerMarkCurrent = Grid::PlayerMark::GRID_TYPE_RED;
                        _apPlayer.at(0)->setPlayerMark(Grid::PlayerMark::GRID_TYPE_RED);
                        _apPlayer.push_back(new Human(Grid::PlayerMark::GRID_TYPE_YELLOW));
                    }

                    break;
                }
                case 6: _bRunning = false; break;
            }

            break;
        }
        case State_t::STATE_INGAME:
        {
            //if (_apPlayer.at(uyWhich)->getPlayerMark() == _EplayerMarkCurrent)
            {
                switch (uyButton)
                {
                    case 0:
                    {
                        int32_t iMouseX = 0, iMouseY = 0;
                        SDL_GetMouseState(&iMouseX, &iMouseY);
                        int32_t iColumn = iMouseX / (_pSdlSurfaceDisplay->w / Grid::SCuyWidth);

                        if (_grid.isValidPlay(iColumn))
                        {
                            _grid.makePlay(_EplayerMarkCurrent, iColumn);
                            if (_grid.checkWinner() == Grid::PlayerMark::GRID_TYPE_NONE)
                            {
                                _EplayerMarkCurrent = Grid::nextPlayer(_EplayerMarkCurrent);
                                if (AI* pAI = dynamic_cast<AI*>(_apPlayer.at(1)))
                                {
                                    pAI->ab_pruning(_grid);
                                    _EplayerMarkCurrent = Grid::nextPlayer(_EplayerMarkCurrent);
                                }
                            }
                            else _EcurrentState = State_t::STATE_WIN;
                        }

                        break;
                    }
                    case 6: _bRunning = false; break;
                }
            }
            break;
        }
        case State_t::STATE_WIN:
        {
            switch (uyButton)
            {
                case 0:
                {
                    int32_t iMouseX = 0, iMouseY = 0;
                    SDL_GetMouseState(&iMouseX, &iMouseY);

                    if (iMouseX >= 0 && iMouseX < CApp::SCurWindowWidth && iMouseY >= 0 &&
                        iMouseY < CApp::SCurWindowHeight) Reset();

                    break;
                }
                case 6: _bRunning = false; break;
            }

            break;
        }
    }
}


void CApp::OnJoyButtonUp(uint8_t uyWhich, uint8_t uyButton) noexcept {}


void CApp::OnJoyHat(uint8_t uyWhich, uint8_t uyHat, uint8_t uyValue) noexcept {}


void CApp::OnExpose() noexcept {}


void CApp::OnUser(uint8_t uyType, int32_t iCode, void* pData1, void* pData2) noexcept {}


void CApp::OnExit() noexcept { _bRunning = false; }
