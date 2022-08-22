#ifndef _CAPP_HPP_
#define _CAPP_HPP_

#include <cstdint>
#include <vector>
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>
#include "CEvent.hpp"
#include "Grid.hpp"
#include "players/Player.hpp"


/**
 * @brief Main application class
 * Uses the singleton pattern
 */
class CApp : public CEvent
{
    public:
        enum State_t {STATE_START, STATE_INGAME, STATE_WIN};    /**< Application states for the state machine */

        static const uint16_t SCurWindowWidth = 640;    /**< Fixed width of the application */
        static const uint16_t SCurWindowHeight = 480;   /**< Fixed height of the application */

        /**
         * @brief Creates a singleton instance if it does not exist and gets it
         * @return CApp* A pointer to the Singleton instance of the application
         */
        static CApp* getInstance();

        /* Constructors and assignment operators are deleted so no more than one instance of the class 
            is present */
        CApp(const CApp& CCAppOther) = delete;
        CApp(CApp&& CAppOther) = delete;
        CApp& operator =(const CApp& CCAppOther) = delete;
        CApp&& operator=(CApp&& CAppOther) = delete;

        /**
         * @brief Starts the application and handles general processing
         */
        void OnExecute();

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
         * @brief Handles all input events
         * @param pSdlEvent a pointer to an event that was triggered
         */
        virtual void OnEvent(SDL_Event* pSdlEvent) noexcept;

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
         * @brief Handles joystick axis events
         * 
         * @param uyWhich the joystick device index
         * @param uyAxis the joystick axis index
         * @param rValue the axis value
         */
        virtual void OnJoyAxis(uint8_t uyWhich, uint8_t uyAxis, int16_t rValue) noexcept;

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

        /**
         * @brief Handles user-defined events
         * 
         * @param uyType a user-defined event type
         * @param iCode a user-defined event code
         * @param pData1 a user-defined data pointer
         * @param pData2 a user-defined data pointer
         */
        virtual void OnUser(uint8_t uyType, int32_t iCode, void* pData1, void* pData2) noexcept;

        virtual void OnLButtonDown(uint16_t urMouseX, uint16_t urMouseY);
        virtual void OnKeyDown(SDLKey sdlKeySymbol, SDLMod sdlMod, uint16_t urUnicode);

    private:
        static CApp* _SpCAppInstance;   /**< The singleton instance of the application */

        bool _bRunning;         /**< Marks whether the application should continue running */
        State_t _EcurrentState; /**< The current state of the application for the state machine */

        SDL_Surface* _pSdlSurfaceDisplay;   /**< The main display surface */
        SDL_Surface* _pSdlSurfaceStart;     /**< Picture for the start screen */
        SDL_Surface* _pSdlSurfaceGrid;      /**< Picture of the grid */
        SDL_Surface* _pSdlSurfaceRed;       /**< Picture of the red marker for the grid */
        SDL_Surface* _pSdlSurfaceYellow;    /**< Picture of the yellow marker for the grid */
        SDL_Surface* _pSdlSurfaceWinRed;    /**< Picture for the winning screen when red wins */
        SDL_Surface* _pSdlSurfaceWinYellow; /**< Picture for the winning screen when yellow wins */

        Grid _grid;                             /**< Main playing grid */
        Grid::PlayerMark _EplayerMarkCurrent;   /**< The current player that has to make a play */
        std::vector<Player*> _apPlayer;         /**< The current players in the game */
        int8_t _yPlayColumn;

        /**
         * @brief Default constructor
         */
        CApp() noexcept;

};


#endif
