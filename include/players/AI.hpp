#ifndef _AI_HPP_
#define _AI_HPP_

#include <cstdint>
#include "Player.hpp"
#include "../Grid.hpp"


class AI : public Player
{
    public:
        uint8_t getSearchLimit() const noexcept;

        explicit AI(const Grid::PlayerMark& CEplayerMark = Grid::PlayerMark::GRID_TYPE_NONE, 
            uint8_t uySearchLimit = CHAR_MAX) noexcept;

        void ab_pruning(Grid& grid) const noexcept;

    private:
        uint8_t _uySearchLimit;

        int32_t ab_minValue(const Grid& Cgrid, const Grid::PlayerMark& CEplayerMark, int32_t iDepth, 
            int32_t iAlpha, int32_t iBeta) const noexcept;
        int32_t ab_maxValue(const Grid& Cgrid, const Grid::PlayerMark& CEplayerMark, int32_t iDepth, 
            int32_t iAlpha, int32_t iBeta) const noexcept;
        int32_t heuristic(const Grid& Cgrid) const noexcept;
        int8_t playerMark2Heuristic(const Grid& Cgrid, uint8_t uyRow, uint8_t uyColumn) const;
};


inline AI::AI(const Grid::PlayerMark& CEplayerMark, uint8_t uySearchLimit) noexcept : Player{CEplayerMark},
    _uySearchLimit{uySearchLimit} {}
inline uint8_t AI::getSearchLimit() const noexcept { return _uySearchLimit; }


#endif