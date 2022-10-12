#ifndef _AI_HPP_
#define _AI_HPP_

/**
	@file		AI.hpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		12/10/2022
    @brief		AI class
    @par		Description
                AI for playing ConnectX
				
*/

#include <cstdint>
#include <limits>
#include <queue>
#include "Player.hpp"
#include "../Grid.hpp"


/**
 * @brief AI class
 */
class AI : public Player
{
public:
    uint8_t GetSearchLimit() const noexcept;

    /**
     * @brief Construct a new AI player
     * 
     * @param CePlayerMark the mark assigned to this player
     * @param uySearchLimit the depth of levels that the AI will explore
     */
    explicit AI(const Grid::EPlayerMark& CePlayerMark = Grid::EPlayerMark::EMPTY, 
        uint8_t uySearchLimit = std::numeric_limits<uint8_t>::max()) noexcept;

    /**
     * @brief Makes the AI choose a play on the board
     * 
     * @param grid the main game board
     */
    void ChooseMove(Grid& grid) const noexcept;

private:
    uint8_t _uySearchLimit; /**< The levels of depth that the AI will explore */


    /**
     * @brief Min function of the AB-Pruning algorithm
     * 
     * @param Cgrid the main game board
     * @param CePlayerMark the mark of the min player
     * @param uyDepth the depth level of exploration
     * @param lAlpha alpha value for the AB-Pruning algorithm
     * @param lBeta beta value for the AB-Pruning algorithm
     * @return int64_t the revised value of beta
     */
    int64_t AB_MinValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, uint8_t uyDepth, 
        int64_t lAlpha, int64_t lBeta) const noexcept;

    /**
     * @brief Max function of the AB-Pruning algorithm
     * 
     * @param Cgrid the main game board
     * @param CePlayerMark the mark of the max player
     * @param uyDepth the depth level of exploration
     * @param lAlpha alpha value for the AB-Pruning algorithm
     * @param lBeta beta value for the AB-Pruning algorithm
     * @return int64_t the revised value of alpha
     */
    int64_t AB_MaxValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, uint8_t uyDepth, 
        int64_t lAlpha, int64_t lBeta) const noexcept;

    /**
     * @brief Evaluation function
     * 
     * @param Cgrid the main game board
     * @return int64_t a numeric evaluation of the board
     */
    int64_t Heuristic(const Grid& Cgrid) const noexcept;

    /**
     * @brief Helper function for the heuristic function. Used to build adn keep track of free sectors, 
     * where there is only one type of player marker and where such player still has the chance to win
     * 
     * @param Cgrid the main game board
     * @param uyRow the row of the next cell to be added to the sector
     * @param uyColumn the column of the next cell to be added to the sector
     * @param quPlayerMarks the queue of cells that form the sector
     * @param ePlayerMarkLast the type of the last  non-empty cell that was found
     * @param uySamePlayerMarkCount the number of player marks of the same type that have been found in the
     *  current sector
     * @param uyEmptyCellCount the number of empty cells that have been found since the last non-empty cell
     * @return int64_t the heuristic evaluation for the current sector
     */
    int64_t EvaluateSector(const Grid& Cgrid, uint8_t uyRow, uint8_t uyColumn, 
        std::queue<Grid::EPlayerMark>& quPlayerMarks, Grid::EPlayerMark& ePlayerMarkLast,
        uint8_t& uySamePlayerMarkCount, uint8_t& uyEmptyCellCount) const noexcept;


    /**
     * @brief Gets the mark of the next player
     * 
     * @param CePlayerMark the mark of the current player
     * @return EPlayerMark the mark of the next player
     */
    Grid::EPlayerMark NextPlayer(const Grid::EPlayerMark& CePlayerMark) const noexcept;


    /**
     * @brief Converts a player mark into a value for the evaluation function
     * 
     * @param CePlayerMark the player mark to convert
     * @return int8_t a numeric representation of the player mark for the evaluation function
     */
    int8_t PlayerMark2Heuristic(const Grid::EPlayerMark& CePlayerMark) const noexcept;
};


inline uint8_t AI::GetSearchLimit() const noexcept { return _uySearchLimit; }


#endif