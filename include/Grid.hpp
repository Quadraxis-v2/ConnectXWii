#ifndef GRID_HPP_
#define GRID_HPP_


#include <vector>
#include <ostream>
#include <cstdint>
#include <climits>


class Grid
{
    public:
        enum PlayerMark {GRID_TYPE_NONE = 0, GRID_TYPE_RED = INT_MIN, GRID_TYPE_YELLOW = INT_MAX};

        static const uint8_t SCuyHeight = 6;
        static const uint8_t SCuyWidth = 7;
        static const uint8_t SCuyNumberToMatch = 4;

        const std::vector<std::vector<PlayerMark> >& getCells() const noexcept;
        int8_t getNextCell(uint8_t uyColumn) const noexcept;

        explicit Grid() noexcept;

        static PlayerMark nextPlayer(const PlayerMark& CEplayerMark) noexcept;

        const std::vector<PlayerMark>& operator [](uint8_t uyIndex) const noexcept;

        void makePlay(const PlayerMark& CEplayerMark, uint8_t uyPlayColumn);
        bool isValidPlay(uint8_t uyPlayColumn) const noexcept;
        PlayerMark checkWinner() const noexcept;

    private:
        std::vector<std::vector<PlayerMark> > _a2playerMarkCells;
        std::vector<int8_t> _ayNextCell;

};


inline const std::vector<std::vector<Grid::PlayerMark> >& Grid::getCells() const noexcept 
{ return _a2playerMarkCells; }
inline int8_t Grid::getNextCell(uint8_t uyColumn) const noexcept { return _ayNextCell[uyColumn]; }


inline Grid::Grid() noexcept : _a2playerMarkCells{std::vector<std::vector<PlayerMark> >(Grid::SCuyHeight, 
    std::vector<PlayerMark>(Grid::SCuyWidth, GRID_TYPE_NONE))},
    _ayNextCell{std::vector<int8_t>(Grid::SCuyWidth, Grid::SCuyHeight - 1)}
{}


inline const std::vector<Grid::PlayerMark>& Grid::operator [](uint8_t uyIndex) const noexcept 
{ return _a2playerMarkCells[uyIndex]; }

inline bool operator ==(const Grid& Cgrid1, const Grid& Cgrid2) noexcept
{ return Cgrid1.getCells() == Cgrid2.getCells(); }

std::ostream& operator <<(std::ostream& ostream, const Grid::PlayerMark& CEplayerMark) noexcept;
std::ostream& operator <<(std::ostream& ostream, const Grid& Cgrid) noexcept;


#endif