/*
Grid.hpp --- ConnectX game board
Copyright (C) 2022  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)

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

#ifndef _GRID_HPP_
#define _GRID_HPP_


#include <cstdint>
#include <vector>
#include <ostream>


/**
 * @brief Grid class
 */
class Grid
{
public:
    /**< Types of player markers */
    enum EPlayerMark {EMPTY = 0, PLAYER1 = 1, PLAYER2 = 2};

    /* Getters */
    uint8_t GetWidth() const noexcept;
    uint8_t GetHeight() const noexcept;
    uint8_t GetCellsToWin() const noexcept;
    const std::vector<std::vector<EPlayerMark> >& GetCells() const noexcept;
    int8_t GetNextCell(uint8_t uyColumn) const noexcept;

    explicit Grid(uint8_t uyWidth = 7, uint8_t uyHeight = 6, uint8_t uyNumberToMatch = 4);   /**< Default constructor */
    

    const std::vector<EPlayerMark>& operator [](uint8_t uyIndex) const noexcept; /**< Bracket operator */

    /**
     * @brief Makes a move in the grid
     * 
     * @param CePlayerMark the mark of the player that makes the move
     * @param uyPlayColumn the chosen column for the move
     */
    void MakeMove(const EPlayerMark& CePlayerMark, uint8_t uyPlayColumn);

    /**
     * @brief Checks if a move would be valid
     * 
     * @param uyPlayColumn the chosen column for the move
     * @return true if the move is valid
     * @return false if the move is invalid
     */
    bool IsValidMove(uint8_t uyPlayColumn) const noexcept;

    /**
     * @brief Checks if the grid is full
     * 
     * @return true if the grid is full
     * @return false if the grid is not full
     */
    bool IsFull() const noexcept;

    /**
     * @brief Checks if the game has been won
     * 
     * @return EPlayerMark the mark of the player that won
     */
    EPlayerMark CheckWinner() const noexcept;

private:
    uint8_t _uyWidth;         /**< Width of the grid */
    uint8_t _uyHeight;        /**< Height of the grid */
    uint8_t _uyCellsToWin;    /**< Number of markers in a row that must be achieved */
    std::vector<std::vector<EPlayerMark> > _a2playerMarkCells;   /**< A matrix of markers representing the board */
    std::vector<int8_t> _ayNextCell;        /**< Indicates the next playable cell in a column */
    uint8_t _uyEmptyCells;                  /**< Indicates if the number of empty cells remaining */
    EPlayerMark _ePlayerMarkWinner;         /**< The marker of the player who won the game */

    /**
     * @brief Checks if a given move has won the game
     * 
     * @param CePlayerMark the mark of the player that made the previous move
     * @param yPlayColumn the chosen column for the move
     * @return true if the move won the game
     * @return false if the move did not win the game
     */
    bool IsWinnerMove(const EPlayerMark& CePlayerMark, int8_t yPlayColumn) noexcept;

};


inline uint8_t Grid::GetWidth() const noexcept { return _uyWidth; }
inline uint8_t Grid::GetHeight() const noexcept { return _uyHeight; }
inline uint8_t Grid::GetCellsToWin() const noexcept { return _uyCellsToWin; }
inline const std::vector<std::vector<Grid::EPlayerMark> >& Grid::GetCells() const noexcept 
{ return _a2playerMarkCells; }
inline int8_t Grid::GetNextCell(uint8_t uyColumn) const noexcept { return _ayNextCell[uyColumn]; }


inline const std::vector<Grid::EPlayerMark>& Grid::operator [](uint8_t uyIndex) const noexcept 
{ return _a2playerMarkCells[uyIndex]; }

inline bool operator ==(const Grid& Cgrid1, const Grid& Cgrid2) noexcept
{ return Cgrid1.GetCells() == Cgrid2.GetCells(); }

inline Grid::EPlayerMark Grid::CheckWinner() const noexcept { return _ePlayerMarkWinner; }

inline bool Grid::IsFull() const noexcept { return (_uyEmptyCells == 0); }


/* Stream insertion operator overloads */
std::ostream& operator <<(std::ostream& ostream, const Grid::EPlayerMark& CePlayerMark) noexcept;
std::ostream& operator <<(std::ostream& ostream, const Grid& Cgrid) noexcept;


#endif