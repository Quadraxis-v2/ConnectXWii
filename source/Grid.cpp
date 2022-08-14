#include <vector>
#include <stdexcept>
#include <ostream>
#include <cstdint>
#include "../include/Grid.hpp"


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
}


bool Grid::isValidPlay(uint8_t uyPlayColumn) const noexcept
{
    return (uyPlayColumn >= 0 && uyPlayColumn < Grid::SCuyWidth &&
        _ayNextCell[uyPlayColumn] >= 0);
}


Grid::PlayerMark Grid::checkWinner() const noexcept
{
    for (uint8_t i = 0; i < Grid::SCuyWidth; i++)
    {
        for (uint8_t j = Grid::SCuyHeight - 1; j > _ayNextCell[i]; j--)
        {
            // Vertical up check
            if (j >= Grid::SCuyNumberToMatch - 1 && (j == Grid::SCuyHeight - 1 ||
                _a2playerMarkCells[j + 1][i] != _a2playerMarkCells[j][i]))
            {
                uint8_t uyOffset = 1, yCounter = 1;
                while (yCounter < Grid::SCuyNumberToMatch &&
                    _a2playerMarkCells[j][i] == _a2playerMarkCells[j - uyOffset][i])
                    yCounter++, uyOffset++;

                if (yCounter == Grid::SCuyNumberToMatch) return _a2playerMarkCells[j][i];
            }
            // Horizontal right check
            if (i <= Grid::SCuyWidth - Grid::SCuyNumberToMatch &&
                (i == 0 || _a2playerMarkCells[j][i - 1] != _a2playerMarkCells[j][i]))
            {
                uint8_t uyOffset = 1, yCounter = 1;
                while (yCounter < Grid::SCuyNumberToMatch &&
                    _a2playerMarkCells[j][i] == _a2playerMarkCells[j][i + uyOffset])
                    yCounter++, uyOffset++;

                if (yCounter == Grid::SCuyNumberToMatch) return _a2playerMarkCells[j][i];
            }
            // Diagonal up right check
            if (j >= Grid::SCuyNumberToMatch - 1 && i <= Grid::SCuyWidth - Grid::SCuyNumberToMatch &&
                (j == Grid::SCuyHeight - 1 || i == 0 ||
                _a2playerMarkCells[j + 1][i - 1] != _a2playerMarkCells[j][i]))
            {
                uint8_t uyOffset = 1, yCounter = 1;
                while (yCounter < Grid::SCuyNumberToMatch &&
                    _a2playerMarkCells[j][i] == _a2playerMarkCells[j - uyOffset][i + uyOffset])
                    yCounter++, uyOffset++;

                if (yCounter == Grid::SCuyNumberToMatch) return _a2playerMarkCells[j][i];
            }
            // Diagonal down right check
            if (j <= Grid::SCuyHeight - Grid::SCuyNumberToMatch &&
                i <= Grid::SCuyWidth - Grid::SCuyNumberToMatch &&
                (j == 0 || i == 0 || _a2playerMarkCells[j - 1][i - 1] != _a2playerMarkCells[j][i]))
            {
                uint8_t uyOffset = 1, yCounter = 1;
                while (yCounter < Grid::SCuyNumberToMatch &&
                    _a2playerMarkCells[j][i] == _a2playerMarkCells[j + uyOffset][i + uyOffset])
                    yCounter++, uyOffset++;

                if (yCounter == Grid::SCuyNumberToMatch) return _a2playerMarkCells[j][i];
            }
        }
    }
    return PlayerMark::GRID_TYPE_NONE;
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
