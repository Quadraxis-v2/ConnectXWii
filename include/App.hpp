#ifndef _App_HPP_
#define _App_HPP_

#include <cstdint>
#include <vector>
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>
#include "EventListener.hpp"
#include "EventManager.hpp"
#include "Surface.hpp"
#include "Grid.hpp"
#include "players/Player.hpp"


/**
 * @brief Main application class
 * Uses the singleton pattern
 */
class App : public EventListener
{
    public:
        enum EState {STATE_START, STATE_INGAME, STATE_WIN};    /**< Application states for the state machine */

        static const uint16_t SCurWindowWidth = 640;    /**< Fixed width of the application */
        static const uint16_t SCurWindowHeight = 480;   /**< Fixed height of the application */

        /**
         * @brief Creates a singleton instance if it does not exist and gets it
         * @return App* A pointer to the Singleton instance of the application
         */
        static App* getInstance();

        /* Constructors and assignment operators are deleted so no more than one instance of the class 
            is present */
        App(const App& CAppOther) = delete;
        App(App&& AppOther) = delete;
        App& operator =(const App& CAppOther) = delete;
        App&& operator=(App&& AppOther) = delete;

        /**
         * @brief Starts the application and handles general processing
         */
        void OnExecute();

    private:
        static App* _SpAppInstance;   /**< The singleton instance of the application */

        bool _bRunning;         /**< Marks whether the application should continue running */
        EventManager _eventManager;
        EState _eStateCurrent; /**< The current state of the application for the state machine */

        Surface _surfaceDisplay;   /**< The main display surface */
        Surface _surfaceStart;     /**< Picture for the start screen */
        Surface _surfaceGrid;      /**< Picture of the grid */
        Surface _surfaceRed;       /**< Picture of the red marker for the grid */
        Surface _surfaceYellow;    /**< Picture of the yellow marker for the grid */
        Surface _surfaceWinRed;    /**< Picture for the winning screen when red wins */
        Surface _surfaceWinYellow; /**< Picture for the winning screen when yellow wins */

        Grid _grid;                             /**< Main playing grid */
        Grid::EPlayerMark _ePlayerMarkCurrent;  /**< The current player that has to make a play */
        std::vector<Player*> _apPlayer;         /**< The current players in the game */
        int8_t _yPlayColumn;


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

        virtual void OnMouseFocus();
 
        virtual void OnMouseBlur();

        virtual void OnInputFocus();
 
        virtual void OnInputBlur();

        virtual void OnRestore();

        virtual void OnMinimize();
 
        virtual void OnKeyDown(SDLKey sdlKeySymbol, SDLMod sdlMod, uint16_t urUnicode);
 
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

        virtual void OnMouseWheel(bool Up, bool Down);
 
        virtual void OnLButtonDown(uint16_t urMouseX, uint16_t urMouseY);
 
        virtual void OnRButtonDown(uint16_t urMouseX, uint16_t urMouseY);
 
        virtual void OnMButtonDown(uint16_t urMouseX, uint16_t urMouseY);

        virtual void OnLButtonUp(uint16_t urMouseX, uint16_t urMouseY);

        virtual void OnRButtonUp(uint16_t urMouseX, uint16_t urMouseY);
 
        virtual void OnMButtonUp(uint16_t urMouseX, uint16_t urMouseY);

        /**
         * @brief Handles joystick axis events
         * 
         * @param uyWhich the joystick device index
         * @param uyAxis the joystick axis index
         * @param rValue the axis value
         */
        virtual void OnJoyAxis(uint8_t uyWhich, uint8_t uyAxis, int16_t rValue) noexcept;

        virtual void OnJoyBall(uint8_t uyWhich, uint8_t uyBall, int16_t rRelativeX, int16_t rRelativeY);

        /**
         * @brief Handles joystick button presses events
         * 
         * @param uyWhich the joystick device index
         * @param uyButton the joystick button index
         */
        virtual void OnJoyButtonDown(uint8_t uyWhich, uint8_t uyButton) noexcept;

        /**
         * @brief Handles joystick button releases events
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

        virtual void OnResize(int32_t iWidth, int32_t iHeight);
 
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
