#ifndef _AI_HPP_
#define _AI_HPP_

#include <cstdint>
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
        explicit AI(const Grid::EPlayerMark& CePlayerMark = Grid::EPlayerMark::GRID_TYPE_NONE, 
            uint8_t uySearchLimit = CHAR_MAX) noexcept;

        /**
         * @brief Makes the AI choose a play on the board
         * 
         * @param grid the main game board
         */
        void ChooseMove(Grid& grid) const noexcept;

    private:
        uint8_t _uySearchLimit; /**< The depth of levels that the AI will explore */

        /**
         * @brief Min function of the AB-Pruning algorithm
         * 
         * @param Cgrid the main game board
         * @param CePlayerMark the mark of the min player
         * @param iDepth the depth level of exploration
         * @param iAlpha alpha value for the AB-Pruning algorithm
         * @param iBeta beta value for the AB-Pruning algorithm
         * @return int32_t the revised value of beta
         */
        int32_t AB_MinValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, int32_t iDepth, 
            int32_t iAlpha, int32_t iBeta) const noexcept;

        /**
         * @brief Max function of the AB-Pruning algorithm
         * 
         * @param Cgrid the main game board
         * @param CePlayerMark the mark of the max player
         * @param iDepth the depth level of exploration
         * @param iAlpha alpha value for the AB-Pruning algorithm
         * @param iBeta beta value for the AB-Pruning algorithm
         * @return int32_t the revised value of alpha
         */
        int32_t AB_MaxValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, int32_t iDepth, 
            int32_t iAlpha, int32_t iBeta) const noexcept;

        /**
         * @brief Evaluation function
         * 
         * @param Cgrid the main game board
         * @return int32_t a numeric evaluation of the board
         */
        int32_t Heuristic(const Grid& Cgrid) const noexcept;

        /**
         * @brief Converts a cell into a value for the evaluation function
         * 
         * @param Cgrid the main game board
         * @param uyRow the row of the cell to convert
         * @param uyColumn the column of the cell to convert
         * @return int8_t a numeric representation of the cell for the evaluation function
         */
        int8_t PlayerMark2Heuristic(const Grid& Cgrid, uint8_t uyRow, uint8_t uyColumn) const;
};


inline uint8_t AI::GetSearchLimit() const noexcept { return _uySearchLimit; }


#endif