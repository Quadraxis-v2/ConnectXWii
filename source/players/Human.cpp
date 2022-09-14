#include <cstdint>
#include <utility>
#include "../../include/players/Player.hpp"
#include "../../include/players/Human.hpp"
#include "../../include/players/Joystick.hpp"
#include "../../include/Grid.hpp"


Human::Human(Joystick& joystick, const Grid::EPlayerMark& CePlayerMark) noexcept : Player{CePlayerMark}, 
    _htJoysticks{} 
{ _htJoysticks.insert(std::make_pair(joystick.GetIndex(), &joystick)); }


void Human::AssociateJoystick(Joystick& joystick) noexcept
{ _htJoysticks.insert(std::make_pair(joystick.GetIndex(), &joystick)); }


void Human::DisassociateJoystick(Joystick& joystick) noexcept
{ _htJoysticks.erase(joystick.GetIndex()); }
