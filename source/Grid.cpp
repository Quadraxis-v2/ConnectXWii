#include <vector>
#include <stdexcept>
#include <ostream>
#include <cstdint>
#include "../include/Grid.hpp"


Grid::Grid() noexcept : _a2playerMarkCells{std::vector<std::vector<PlayerMark> >(Grid::SCuyHeight, 
    std::vector<PlayerMark>(Grid::SCuyWidth, GRID_TYPE_NONE))},
    _ayNextCell{std::vector<int8_t>(Grid::SCuyWidth, Grid::SCuyHeight - 1)}, 
    _playerMarkWinner{PlayerMark::GRID_TYPE_NONE}
{}


Grid::PlayerMark Grid::nextPlayer(const PlayerMark& CEplayerMark) noexcept
{
    switch(CEplayerMark)
    {
        case PlayerMark::GRID_TYPE_YELLOW:  return PlayerMark::GRID_TYPE_RED;       break;
        case PlayerMark::GRID_TYPE_RED:     return PlayerMark::GRID_TYPE_YELLOW;    break;
        default:                            return PlayerMark::GRID_TYPE_NONE;      break;
    }
}


void Grid::makePlay(const PlayerMark& CEplayerMark, uint8_t uyPlayColumn)
{
    if (!isValidPlay(uyPlayColumn)) throw std::domain_error("Play is not valid");

    _a2playerMarkCells[_ayNextCell[uyPlayColumn]][uyPlayColumn] = CEplayerMark;
    _ayNextCell[uyPlayColumn]--;

    if (isWinnerPlay(CEplayerMark, uyPlayColumn)) _playerMarkWinner = CEplayerMark;
}


bool Grid::isValidPlay(uint8_t uyPlayColumn) const noexcept
{
    return (uyPlayColumn < Grid::SCuyWidth && _ayNextCell[uyPlayColumn] >= 0);
}


bool Grid::isWinnerPlay(const PlayerMark& CEplayerMark, int8_t yPlayColumn) noexcept
{
    int8_t yPlayRow = _ayNextCell[yPlayColumn] + 1;

    // Downwards check
    uint8_t uyCounter = 1;
    if (_ayNextCell[yPlayColumn] + 1 <= Grid::SCuyHeight - Grid::SCuyNumberToMatch)
    {
        for (uint8_t i = 1; i < Grid::SCuyNumberToMatch && yPlayRow + i < Grid::SCuyHeight && 
            _a2playerMarkCells[yPlayRow + i][yPlayColumn] == CEplayerMark; i++) uyCounter++;

        if (uyCounter == Grid::SCuyNumberToMatch) return true;
    }

    // Check the remaining directions except upwards
    int8_t a2yDirections[][2] = {{0, -1}, {-1, -1}, {-1, 1}};

    for (int8_t* ayDirection : a2yDirections)
    {
        int8_t yDirectionX = ayDirection[0];
        int8_t yDirectionY = ayDirection[1];

        uyCounter = 1;
        for (int8_t i = 1; i < Grid::SCuyNumberToMatch && 
            yPlayRow + i * yDirectionX >= 0 && yPlayRow + i * yDirectionX < Grid::SCuyHeight && 
            yPlayColumn + i * yDirectionY >= 0 && yPlayColumn + i * yDirectionY < Grid::SCuyWidth &&
            _a2playerMarkCells[yPlayRow + i * yDirectionX][yPlayColumn + i * yDirectionY] == CEplayerMark;
            i++) uyCounter++;

        if (uyCounter == Grid::SCuyNumberToMatch) return true;

        for (int8_t i = 1; i < Grid::SCuyNumberToMatch && 
            yPlayRow - i * yDirectionX >= 0 && yPlayRow - i * yDirectionX < Grid::SCuyHeight && 
            yPlayColumn - i * yDirectionY >= 0 && yPlayColumn - i * yDirectionY < Grid::SCuyWidth &&
            _a2playerMarkCells[yPlayRow - i * yDirectionX][yPlayColumn - i * yDirectionY] == CEplayerMark; 
            i++) uyCounter++;

        if (uyCounter == Grid::SCuyNumberToMatch) return true;
    }

    return false;
}


std::ostream& operator <<(std::ostream& ostream, const Grid::PlayerMark& CEplayerMark) noexcept
{
    switch(CEplayerMark)
    {
        case Grid::PlayerMark::GRID_TYPE_NONE:      return ostream << ' ';      break;
        case Grid::PlayerMark::GRID_TYPE_YELLOW:    return ostream << 'Y';      break;
        case Grid::PlayerMark::GRID_TYPE_RED:       return ostream << 'R';      break;
        default:                                    return ostream << ' ';      break;
    }
}


std::ostream& operator <<(std::ostream& ostream, const Grid& Cgrid) noexcept
{
    const std::vector<std::vector<Grid::PlayerMark> > a2playerMarkCells = Cgrid.getCells();

    for (uint8_t i = 0; i < Grid::SCuyHeight; i++)
    {
        ostream << "---+---+---+---+---+---+---" << std::endl <<
            " " << a2playerMarkCells[i][0] << " ";

        for (uint8_t j = 1; j < Grid::SCuyWidth; j++)
            ostream << "| " << a2playerMarkCells[i][j] << " ";

        ostream << std::endl;
    }
    ostream << "---+---+---+---+---+---+---" << std::endl;

    return ostream;
}
