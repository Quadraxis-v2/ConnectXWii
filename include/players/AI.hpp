#ifndef _AI_HPP_
#define _AI_HPP_

#include <cstdint>
#include "Player.hpp"
#include "../Grid.hpp"


class AI : public Player
{
    public:
        uint8_t GetSearchLimit() const noexcept;

        explicit AI(const Grid::EPlayerMark& CePlayerMark = Grid::EPlayerMark::GRID_TYPE_NONE, 
            uint8_t uySearchLimit = CHAR_MAX) noexcept;

        void AB_Pruning(Grid& grid) const noexcept;

    private:
        uint8_t _uySearchLimit;

        int32_t AB_MinValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, int32_t iDepth, 
            int32_t iAlpha, int32_t iBeta) const noexcept;
        int32_t AB_MaxValue(const Grid& Cgrid, const Grid::EPlayerMark& CePlayerMark, int32_t iDepth, 
            int32_t iAlpha, int32_t iBeta) const noexcept;
        int32_t Heuristic(const Grid& Cgrid) const noexcept;
        int8_t PlayerMark2Heuristic(const Grid& Cgrid, uint8_t uyRow, uint8_t uyColumn) const;
};


inline uint8_t AI::GetSearchLimit() const noexcept { return _uySearchLimit; }


#endif