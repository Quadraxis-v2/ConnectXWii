#ifndef _APP_HPP_
#define _APP_HPP_

#include <cstdint>
#include <unordered_map>
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>
#include "EventListener.hpp"
#include "Surface.hpp"
#include "Grid.hpp"
#include "players/Joystick.hpp"
#include "players/Player.hpp"


/**
 * @brief Main application class
 * Uses the singleton pattern
 */
class App : public EventListener    // Receive events in this class
{
    public:
        enum EState {STATE_START, STATE_INGAME, STATE_END};    /**< Application states for the state machine */

        static const uint16_t SCurWindowWidth = 640;    /**< Fixed width of the application */
        static const uint16_t SCurWindowHeight = 480;   /**< Fixed height of the application */


        static App& GetInstance();

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

        Surface _surfaceDisplay;    /**< The main display surface */
        Surface _surfaceStart;      /**< Picture for the start screen */
        Surface _surfaceGrid;       /**< Picture of the grid */
        Surface _surfaceRed;        /**< Picture of the red marker for the grid */
        Surface _surfaceYellow;     /**< Picture of the yellow marker for the grid */
        Surface _surfaceWinRed;     /**< Picture for the end screen when red wins */
        Surface _surfaceWinYellow;  /**< Picture for the end screen when yellow wins */
        Surface _surfaceDraw;       /**< Picture for the end screen when there is a draw */

        Grid _grid;                             /**< Main playing grid */
        Grid::EPlayerMark _ePlayerMarkCurrent;  /**< The current player that has to make a play */
        std::unordered_map<uint8_t, Joystick*>  _htJoysticks;         /**< The joysticks in use */
        std::unordered_map<Grid::EPlayerMark, Player*> _htPlayers;    /**< The current players in the game */
        int8_t _yPlayColumn;                    /**< The value of the column currently selected by the user */


        App() noexcept;    /**< Default constructor */

        /**
         * @brief Handles the initial loading of data
         */
        void OnInit();

        /**
         * @brief Handles all the data updates between frames
         */
        void OnLoop() const noexcept;

        /**
         * @brief Handles all the rendering for each frame
         */
        void OnRender();

        /**
         * @brief Cleans up any resources loaded before the end of the execution
         */
        void OnCleanup() noexcept;

        /**
         * @brief Resets the application to the initial values
         */
        void Reset() noexcept;

        /**
         * @brief Handles events where the mouse enters the application window
         */
        virtual void OnMouseFocus();
 
        /**
         * @brief Handles events where the mouse exits the application window
         */
        virtual void OnMouseBlur();

        /**
         * @brief Handles events where the application gains keyboard focus
         */
        virtual void OnInputFocus();

        /**
         * @brief Handles events where the application loses keyboard focus
         */
        virtual void OnInputBlur();

        /**
         * @brief Handles events where the window is restored to its size
         */
        virtual void OnRestore();

        /**
         * @brief Handles events where the window is minimized
         */
        virtual void OnMinimize();
 
        /**
         * @brief Handles keyboard press events
         * 
         * @param sdlKeySymbol the key that was pressed
         * @param sdlMod the current state of keyboard modifiers
         * @param urUnicode the Unicode value of the pressed key
         */
        virtual void OnKeyDown(SDLKey sdlKeySymbol, SDLMod sdlMod, uint16_t urUnicode);
 
        /**
         * @brief Handles keyboard release events
         * 
         * @param sdlKeySymbol the key that was released
         * @param sdlMod the current state of keyboard modifiers
         * @param urUnicode the Unicode value of the released key
         */
        virtual void OnKeyUp(SDLKey sdlKeySymbol, SDLMod sdlMod, uint16_t urUnicode);

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
            bool bLeft, bool bRight, bool bMiddle) noexcept;

        /**
         * @brief Handles mouse wheel movement events
         * 
         * @param bUp true if the mouse wheel is moving upwards
         * @param bDown true if the mouse wheel is moving downwards
         */
        virtual void OnMouseWheel(bool bUp, bool bDown);
 
        /**
         * @brief Handles mouse left button press events
         * 
         * @param urMouseX the X coordinate of the mouse
         * @param urMouseY the Y coordinate of the mouse
         */
        virtual void OnLButtonDown(uint16_t urMouseX, uint16_t urMouseY);
 
        /**
         * @brief Handles mouse right button press events
         * 
         * @param urMouseX the X coordinate of the mouse
         * @param urMouseY the Y coordinate of the mouse
         */
        virtual void OnRButtonDown(uint16_t urMouseX, uint16_t urMouseY);

        /**
         * @brief Handles mouse middle button press events
         * 
         * @param urMouseX the X coordinate of the mouse
         * @param urMouseY the Y coordinate of the mouse
         */
        virtual void OnMButtonDown(uint16_t urMouseX, uint16_t urMouseY);

        /**
         * @brief Handles mouse left button release events
         * 
         * @param urMouseX the X coordinate of the mouse
         * @param urMouseY the Y coordinate of the mouse
         */
        virtual void OnLButtonUp(uint16_t urMouseX, uint16_t urMouseY);

        /**
         * @brief Handles mouse right button release events
         * 
         * @param urMouseX the X coordinate of the mouse
         * @param urMouseY the Y coordinate of the mouse
         */
        virtual void OnRButtonUp(uint16_t urMouseX, uint16_t urMouseY);
 
        /**
         * @brief Handles mouse middle button release events
         * 
         * @param urMouseX the X coordinate of the mouse
         * @param urMouseY the Y coordinate of the mouse
         */
        virtual void OnMButtonUp(uint16_t urMouseX, uint16_t urMouseY);

        /**
         * @brief Handles joystick axis events
         * 
         * @param uyWhich the joystick device index
         * @param uyAxis the joystick axis index
         * @param rValue the axis value
         */
        virtual void OnJoyAxis(uint8_t uyWhich, uint8_t uyAxis, int16_t rValue) noexcept;

        /**
         * @brief Handles joystick trackball motion events
         * 
         * @param uyWhich the joystick device index
         * @param uyBall the joystick ball index
         * @param rRelativeX the relative motion in the X direction
         * @param rRelativeY the relative motion in the Y direction
         */
        virtual void OnJoyBall(uint8_t uyWhich, uint8_t uyBall, int16_t rRelativeX, int16_t rRelativeY);

        /**
         * @brief Handles joystick button press events
         * 
         * @param uyWhich the joystick device index
         * @param uyButton the joystick button index
         */
        virtual void OnJoyButtonDown(uint8_t uyWhich, uint8_t uyButton) noexcept;

        /**
         * @brief Handles joystick button release events
         * 
         * @param uyWhich the joystick device index
         * @param uyButton the joystick button index
         */
        virtual void OnJoyButtonUp(uint8_t uyWhich, uint8_t uyButton) noexcept;

        /**
         * @brief Handles joystick hat position events
         * 
         * @param uyWhich the joystick device index
         * @param uyHat the joystick hat index
         * @param uyValue the hat value
         */
        virtual void OnJoyHat(uint8_t uyWhich, uint8_t uyHat, uint8_t uyValue) noexcept;

        /**
         * @brief Handles user-requested quits
         */
        virtual void OnExit() noexcept;

        /**
         * @brief Handles window resize events
         * 
         * @param iWidth the new width of the window
         * @param iHeight the new height of the window
         */
        virtual void OnResize(int32_t iWidth, int32_t iHeight);
 
        /**
         * @brief Handles window redraw events
         */
        virtual void OnExpose();

        /**
         * @brief Handles user-defined events
         * 
         * @param uyType a user-defined event type
         * @param iCode a user-defined event code
         * @param pData1 a user-defined data pointer
         * @param pData2 a user-defined data pointer
         */
        virtual void OnUser(uint8_t uyType, int32_t iCode, void* pData1, void* pData2) noexcept;

};


#endif
