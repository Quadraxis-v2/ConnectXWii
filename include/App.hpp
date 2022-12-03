/*
App.hpp --- App main class
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

#ifndef _APP_HPP_
#define _APP_HPP_

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>
#include "EventListener.hpp"
#include "Settings.hpp"
#include "video/Surface.hpp"
#include "Grid.hpp"
#include "players/Joystick.hpp"
#include "players/Player.hpp"


/**
 * @brief Main application class
 */
class App : public EventListener    // Receive events in this class
{
public:
    enum EState {STATE_START, STATE_INGAME, STATE_END};    /**< Application states for the state machine */


    friend int32_t SDLCALL RunAI(void* pData);


    static App& GetInstance();

    bool GetStopThreads() const noexcept;


    App(const App& CappOther) = delete;             /**< Copy constructor */
    App(App&& appOther) = default;                  /**< Move constructor */
    App& operator =(const App& CappOther) = delete; /**< Copy assignment operator */
    App& operator =(App&& appOther) = default;      /**< Move assignment operator */


    /**
     * @brief Starts the application and handles general processing
     */
    void OnExecute();

private:
    bool _bRunning;             /**< Marks whether the application should continue running */
    EState _eStateCurrent;      /**< The current state of the application for the state machine */
    Settings _settingsGlobal;   /**< The global settings of the application */
    SDL_Thread* _pSdlThreadAI;  /**< Background AI thread */
    SDL_sem* _pSdlSemaphoreAI;  /**< Semaphore for the AI thread */
    bool _bStopThreads;         /**< Signal threads to stop */

    Surface _surfaceDisplay;    /**< The main display surface */
    Surface _surfaceStart;      /**< Picture for the start screen */
    Surface _surfaceGrid;       /**< Picture of the grid */
    Surface _surfaceMarker1;    /**< Picture of the red marker for the grid */
    Surface _surfaceMarker2;    /**< Picture of the yellow marker for the grid */
    Surface _surfaceWinPlayer1; /**< Picture for the end screen when red wins */
    Surface _surfaceWinPlayer2; /**< Picture for the end screen when yellow wins */
    Surface _surfaceDraw;       /**< Picture for the end screen when there is a draw */
    Surface _surfaceCursor;
    Surface _surfaceCursorShadow;

    Grid _grid;                             /**< Main playing grid */
    std::unordered_map<uint8_t, Joystick*>  _htJoysticks;   /**< The joysticks in use */
    std::vector<Player*> _vectorpPlayers;   /**< The current players in the game */
    uint8_t _uyCurrentPlayer;               /** The index for the current player */
    bool _bSingleController;                /** The main controller can be used for all players */
    int8_t _yPlayColumn;                    /**< The value of the column currently selected by the user */


    App();    /**< Default constructor */

    virtual ~App() noexcept;    /**< Destructor */


    /**
     * @brief Handles all the data updates between frames
     */
    void OnLoop() const noexcept;

    /**
     * @brief Handles all the rendering for each frame
     */
    void OnRender();

    /**
     * @brief Resets the application to the initial values
     */
    void Reset();

    /**
     * @brief Handles events where the mouse enters the application window
     */
    virtual void OnMouseFocus() override;

    /**
     * @brief Handles events where the mouse exits the application window
     */
    virtual void OnMouseBlur() override;

    /**
     * @brief Handles events where the application gains keyboard focus
     */
    virtual void OnInputFocus() override;

    /**
     * @brief Handles events where the application loses keyboard focus
     */
    virtual void OnInputBlur() override;

    /**
     * @brief Handles events where the window is restored to its size
     */
    virtual void OnRestore() override;

    /**
     * @brief Handles events where the window is minimized
     */
    virtual void OnMinimize() override;

    /**
     * @brief Handles keyboard press events
     *
     * @param sdlKeySymbol the key that was pressed
     * @param sdlMod the current state of keyboard modifiers
     * @param urUnicode the Unicode value of the pressed key
     */
    virtual void OnKeyDown(SDLKey sdlKeySymbol, SDLMod sdlMod, uint16_t urUnicode) override;

    /**
     * @brief Handles keyboard release events
     *
     * @param sdlKeySymbol the key that was released
     * @param sdlMod the current state of keyboard modifiers
     * @param urUnicode the Unicode value of the released key
     */
    virtual void OnKeyUp(SDLKey sdlKeySymbol, SDLMod sdlMod, uint16_t urUnicode) override;

    /**
     * @brief Handles mouse/IR movement events
     *
     * @param urMouseX the X coordinate of the mouse
     * @param urMouseY the Y coordinate of the mouse
     * @param rRelX the relative motion in the X direction
     * @param rRelY the relative motion in the Y direction
     * @param bLeft the current state of the left mouse button
     * @param bRight the current state of the right mouse button
     * @param bMiddle the current state of the middle mouse button
     */
    virtual void OnMouseMove(uint16_t urMouseX, uint16_t urMouseY, int16_t rRelX, int16_t rRelY,
        bool bLeft, bool bRight, bool bMiddle) noexcept override;

    /**
     * @brief Handles mouse wheel movement events
     *
     * @param bUp true if the mouse wheel is moving upwards
     * @param bDown true if the mouse wheel is moving downwards
     */
    virtual void OnMouseWheel(bool bUp, bool bDown) override;

    /**
     * @brief Handles mouse left button press events
     *
     * @param urMouseX the X coordinate of the mouse
     * @param urMouseY the Y coordinate of the mouse
     */
    virtual void OnLButtonDown(uint16_t urMouseX, uint16_t urMouseY) override;

    /**
     * @brief Handles mouse right button press events
     *
     * @param urMouseX the X coordinate of the mouse
     * @param urMouseY the Y coordinate of the mouse
     */
    virtual void OnRButtonDown(uint16_t urMouseX, uint16_t urMouseY) override;

    /**
     * @brief Handles mouse middle button press events
     *
     * @param urMouseX the X coordinate of the mouse
     * @param urMouseY the Y coordinate of the mouse
     */
    virtual void OnMButtonDown(uint16_t urMouseX, uint16_t urMouseY) override;

    /**
     * @brief Handles mouse left button release events
     *
     * @param urMouseX the X coordinate of the mouse
     * @param urMouseY the Y coordinate of the mouse
     */
    virtual void OnLButtonUp(uint16_t urMouseX, uint16_t urMouseY) override;

    /**
     * @brief Handles mouse right button release events
     *
     * @param urMouseX the X coordinate of the mouse
     * @param urMouseY the Y coordinate of the mouse
     */
    virtual void OnRButtonUp(uint16_t urMouseX, uint16_t urMouseY) override;

    /**
     * @brief Handles mouse middle button release events
     *
     * @param urMouseX the X coordinate of the mouse
     * @param urMouseY the Y coordinate of the mouse
     */
    virtual void OnMButtonUp(uint16_t urMouseX, uint16_t urMouseY) override;

    /**
     * @brief Handles joystick axis events
     *
     * @param uyWhich the joystick device index
     * @param uyAxis the joystick axis index
     * @param rValue the axis value
     */
    virtual void OnJoyAxis(uint8_t uyWhich, uint8_t uyAxis, int16_t rValue) noexcept override;

    /**
     * @brief Handles joystick trackball motion events
     *
     * @param uyWhich the joystick device index
     * @param uyBall the joystick ball index
     * @param rRelativeX the relative motion in the X direction
     * @param rRelativeY the relative motion in the Y direction
     */
    virtual void OnJoyBall(uint8_t uyWhich, uint8_t uyBall, int16_t rRelativeX, int16_t rRelativeY) override;

    /**
     * @brief Handles joystick button press events
     *
     * @param uyWhich the joystick device index
     * @param uyButton the joystick button index
     */
    virtual void OnJoyButtonDown(uint8_t uyWhich, uint8_t uyButton) noexcept override;

    /**
     * @brief Handles joystick button release events
     *
     * @param uyWhich the joystick device index
     * @param uyButton the joystick button index
     */
    virtual void OnJoyButtonUp(uint8_t uyWhich, uint8_t uyButton) noexcept override;

    /**
     * @brief Handles joystick hat position events
     *
     * @param uyWhich the joystick device index
     * @param uyHat the joystick hat index
     * @param uyValue the hat value
     */
    virtual void OnJoyHat(uint8_t uyWhich, uint8_t uyHat, uint8_t uyValue) noexcept override;

    /**
     * @brief Handles user-requested quits
     */
    virtual void OnExit() noexcept override;

    /**
     * @brief Handles window resize events
     *
     * @param iWidth the new width of the window
     * @param iHeight the new height of the window
     */
    virtual void OnResize(int32_t iWidth, int32_t iHeight) override;

    /**
     * @brief Handles window redraw events
     */
    virtual void OnExpose() override;

    /**
     * @brief Handles user-defined events
     *
     * @param uyType a user-defined event type
     * @param iCode a user-defined event code
     * @param pData1 a user-defined data pointer
     * @param pData2 a user-defined data pointer
     */
    virtual void OnUser(uint8_t uyType, int32_t iCode, void* pData1, void* pData2) noexcept override;

};


inline bool App::GetStopThreads() const noexcept { return _bStopThreads; }


#endif
