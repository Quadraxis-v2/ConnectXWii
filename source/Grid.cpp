#include <vector>
#include <stdexcept>
#include <ostream>
#include <cstdint>
#include "../include/Grid.hpp"


/**
 * @brief Construct a new empty Grid
 */
Grid::Grid() noexcept : _a2playerMarkCells{std::vector<std::vector<EPlayerMark> >(Grid::SCuyHeight,
    std::vector<EPlayerMark>(Grid::SCuyWidth, GRID_TYPE_NONE))},
    _ayNextCell{std::vector<int8_t>(Grid::SCuyWidth, Grid::SCuyHeight - 1)},
    _ePlayerMarkWinner{EPlayerMark::GRID_TYPE_NONE}
{}


/**
 * @brief Gets the mark of the next player
 *
 * @param CePlayerMark the mark of the current player
 * @return EPlayerMark the mark of the next player
 */
Grid::EPlayerMark Grid::NextPlayer(const EPlayerMark& CePlayerMark) noexcept
{
    switch(CePlayerMark)
    {
        case EPlayerMark::GRID_TYPE_YELLOW: return EPlayerMark::GRID_TYPE_RED;       break;
        case EPlayerMark::GRID_TYPE_RED:    return EPlayerMark::GRID_TYPE_YELLOW;    break;
        default:                            return EPlayerMark::GRID_TYPE_NONE;      break;
    }
}


/**
 * @brief Makes a play in the grid
 *
 * @param CePlayerMark the mark of the player that makes the play
 * @param uyPlayColumn the chosen column for the play
 */
void Grid::MakePlay(const EPlayerMark& CePlayerMark, uint8_t uyPlayColumn)
{
    if (!IsValidPlay(uyPlayColumn)) throw std::domain_error("Play is not valid");

    _a2playerMarkCells[_ayNextCell[uyPlayColumn]][uyPlayColumn] = CePlayerMark;
    _ayNextCell[uyPlayColumn]--;

    if (IsWinnerPlay(CePlayerMark, uyPlayColumn)) _ePlayerMarkWinner = CePlayerMark;
}


/**
 * @brief Checks if a play would be valid
 *
 * @param uyPlayColumn the chosen column for the play
 * @return true if the play is valid
 * @return false if the play is invalid
 */
bool Grid::IsValidPlay(uint8_t uyPlayColumn) const noexcept
{
    return (uyPlayColumn < Grid::SCuyWidth && _ayNextCell[uyPlayColumn] >= 0 &&
        _ePlayerMarkWinner == EPlayerMark::GRID_TYPE_NONE);
}


/**
 * @brief Checks if a given play has won the game
 *
 * @param CePlayerMark the mark of the player that made the previous play
 * @param yPlayColumn the chosen column for the play
 * @return true if the play won the game
 * @return false if the play did not win the game
 */
bool Grid::IsWinnerPlay(const EPlayerMark& CePlayerMark, int8_t yPlayColumn) noexcept
{
    int8_t yPlayRow = _ayNextCell[yPlayColumn] + 1; // The previous row is where the previous play was made

    // Downwards check
    uint8_t uyCounter = 1;
    if (yPlayRow <= Grid::SCuyHeight - Grid::SCuyNumberToMatch)
    {
        for (uint8_t i = 1; i < Grid::SCuyNumberToMatch && yPlayRow + i < Grid::SCuyHeight &&
            _a2playerMarkCells[yPlayRow + i][yPlayColumn] == CePlayerMark; i++) uyCounter++;

        if (uyCounter == Grid::SCuyNumberToMatch) return true;
    }

    // Check the remaining directions except upwards
    int8_t a2yDirections[][2] = {{0, -1}, {-1, -1}, {-1, 1}};

    for (int8_t* ayDirection : a2yDirections)
    {
        int8_t yDirectionX = ayDirection[0];
        int8_t yDirectionY = ayDirection[1];

        // Check one way
        uyCounter = 1;
        for (int8_t i = 1; i < Grid::SCuyNumberToMatch &&
            yPlayRow + i * yDirectionX >= 0 && yPlayRow + i * yDirectionX < Grid::SCuyHeight &&
            yPlayColumn + i * yDirectionY >= 0 && yPlayColumn + i * yDirectionY < Grid::SCuyWidth &&
            _a2playerMarkCells[yPlayRow + i * yDirectionX][yPlayColumn + i * yDirectionY] == CePlayerMark;
            i++) uyCounter++;

        if (uyCounter == Grid::SCuyNumberToMatch) return true;

        // Check the opposite way
        for (int8_t i = 1; i < Grid::SCuyNumberToMatch &&
            yPlayRow - i * yDirectionX >= 0 && yPlayRow - i * yDirectionX < Grid::SCuyHeight &&
            yPlayColumn - i * yDirectionY >= 0 && yPlayColumn - i * yDirectionY < Grid::SCuyWidth &&
            _a2playerMarkCells[yPlayRow - i * yDirectionX][yPlayColumn - i * yDirectionY] == CePlayerMark;
            i++) uyCounter++;

        if (uyCounter == Grid::SCuyNumberToMatch) return true;
    }

    return false;
}


/**
 * @brief Stream insertion operator overload for player marks
 */
std::ostream& operator <<(std::ostream& ostream, const Grid::EPlayerMark& CePlayerMark) noexcept
{
    switch(CePlayerMark)
    {
        case Grid::EPlayerMark::GRID_TYPE_NONE:     return ostream << ' ';      break;
        case Grid::EPlayerMark::GRID_TYPE_YELLOW:   return ostream << 'Y';      break;
        case Grid::EPlayerMark::GRID_TYPE_RED:      return ostream << 'R';      break;
        default:                                    return ostream << ' ';      break;
    }
}


/**
 * @brief Stream insertion operator overload for the grid
 */
std::ostream& operator <<(std::ostream& ostream, const Grid& Cgrid) noexcept
{
    const std::vector<std::vector<Grid::EPlayerMark> > a2playerMarkCells = Cgrid.GetCells();

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
