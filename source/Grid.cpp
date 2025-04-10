/*
Grid.cpp --- ConnectX game board
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

#include <vector>
#include <stdexcept>
#include <ostream>
#include <cstdint>
#include <sstream>
#include "../include/Grid.hpp"


/**
 * @brief Construct a new Grid
 * 
 * @param uyWidth the width of the grid
 * @param uyHeight the height of the grid
 * @param uyCellsToWin the number of cells in a row required to win
 */
Grid::Grid(uint8_t uyWidth, uint8_t uyHeight, uint8_t uyCellsToWin) : _uyWidth{uyWidth},
    _uyHeight{uyHeight}, _uyCellsToWin{uyCellsToWin}, 
    _vector2playerMarkCells{std::vector<std::vector<EPlayerMark> >(_uyHeight, 
    std::vector<EPlayerMark>(_uyWidth, EMPTY))}, 
    _ayNextCell{std::vector<int8_t>(_uyWidth, _uyHeight - 1)}, 
    _uyEmptyCells{static_cast<uint8_t>(_uyWidth * _uyHeight)}, 
    _ePlayerMarkWinner{EPlayerMark::EMPTY}
{ 
    if (_uyCellsToWin > _uyWidth && _uyCellsToWin > _uyHeight) 
        throw std::length_error("Number of cells to win is too big"); 
}


/**
 * @brief Makes a play in the grid
 *
 * @param CePlayerMark the mark of the player that makes the play
 * @param uyPlayColumn the chosen column for the play
 */
void Grid::MakeMove(const EPlayerMark& CePlayerMark, uint8_t uyPlayColumn)
{
    if (!IsValidMove(uyPlayColumn)) throw std::domain_error("Play is not valid");

    _vector2playerMarkCells[_ayNextCell[uyPlayColumn]][uyPlayColumn] = CePlayerMark;
    --_ayNextCell[uyPlayColumn];
    --_uyEmptyCells;

    if (IsWinnerMove(CePlayerMark, uyPlayColumn)) _ePlayerMarkWinner = CePlayerMark;
}


/**
 * @brief Checks if a play would be valid
 *
 * @param uyPlayColumn the chosen column for the play
 * @return true if the play is valid
 * @return false if the play is invalid
 */
bool Grid::IsValidMove(uint8_t uyPlayColumn) const noexcept
{
    return (uyPlayColumn < _uyWidth && _ayNextCell[uyPlayColumn] >= 0 &&
        _ePlayerMarkWinner == EPlayerMark::EMPTY);
}


/**
 * @brief Checks if a given play has won the game
 *
 * @param CePlayerMark the mark of the player that made the previous play
 * @param yPlayColumn the chosen column for the play
 * @return true if the play won the game
 * @return false if the play did not win the game
 */
bool Grid::IsWinnerMove(const EPlayerMark& CePlayerMark, int8_t yPlayColumn) noexcept
{
    int8_t yPlayRow = _ayNextCell[yPlayColumn] + 1; // The previous row is where the previous play was made

    // Downwards check
    uint8_t uyCounter = 1;
    if (yPlayRow <= _uyHeight - _uyCellsToWin)
    {
        for (uint8_t i = 1; i < _uyCellsToWin && yPlayRow + i < _uyHeight &&
            _vector2playerMarkCells[yPlayRow + i][yPlayColumn] == CePlayerMark; ++i) ++uyCounter;

        if (uyCounter == _uyCellsToWin) return true;
    }

    // Check the remaining directions except upwards
    const int8_t Ca2yDirections[][2] = {{0, -1}, {-1, -1}, {-1, 1}};

    for (const int8_t* CayDirection : Ca2yDirections)
    {
        int8_t yDirectionX = CayDirection[0];
        int8_t yDirectionY = CayDirection[1];

        // Check one way
        uyCounter = 1;
        for (int8_t i = 1; i < _uyCellsToWin &&
            yPlayRow + i * yDirectionX >= 0 && yPlayRow + i * yDirectionX < _uyHeight &&
            yPlayColumn + i * yDirectionY >= 0 && yPlayColumn + i * yDirectionY < _uyWidth &&
            _vector2playerMarkCells[yPlayRow + i * yDirectionX][yPlayColumn + i * yDirectionY] == 
            CePlayerMark; ++i) ++uyCounter;

        if (uyCounter == _uyCellsToWin) return true;

        // Check the opposite way
        for (int8_t i = 1; i < _uyCellsToWin &&
            yPlayRow - i * yDirectionX >= 0 && yPlayRow - i * yDirectionX < _uyHeight &&
            yPlayColumn - i * yDirectionY >= 0 && yPlayColumn - i * yDirectionY < _uyWidth &&
            _vector2playerMarkCells[yPlayRow - i * yDirectionX][yPlayColumn - i * yDirectionY] == 
            CePlayerMark; ++i) ++uyCounter;

        if (uyCounter == _uyCellsToWin) return true;
    }

    return false;
}


/**
 * @brief Stream insertion operator overload for player marks
 */
std::ostream& operator <<(std::ostream& ostreamOut, const Grid::EPlayerMark& CePlayerMark) noexcept
{
    switch(CePlayerMark)
    {
        case Grid::EPlayerMark::EMPTY:     return ostreamOut << ' ';      break;
        case Grid::EPlayerMark::PLAYER2:   return ostreamOut << 'Y';      break;
        case Grid::EPlayerMark::PLAYER1:   return ostreamOut << 'R';      break;
        default:                           return ostreamOut << ' ';      break;
    }
}


/**
 * @brief Stream insertion operator overload for the grid
 */
std::ostream& operator <<(std::ostream& ostreamOut, const Grid& Cgrid) noexcept
{
    const std::vector<std::vector<Grid::EPlayerMark> > a2playerMarkCells = Cgrid.GetCells();

    std::ostringstream ossSeparator{"---"};
    for (uint8_t i = 0; i < Cgrid.GetWidth() - 1; ++i) ossSeparator << "+---";

    for (uint8_t i = 0; i < Cgrid.GetHeight(); ++i)
    {
        ostreamOut << ossSeparator.str() << std::endl <<
            " " << a2playerMarkCells[i][0] << " ";

        for (uint8_t j = 1; j < Cgrid.GetWidth(); ++j)
            ostreamOut << "| " << a2playerMarkCells[i][j] << " ";

        ostreamOut << std::endl;
    }
    ostreamOut << ossSeparator.str() << std::endl;

    return ostreamOut;
}
