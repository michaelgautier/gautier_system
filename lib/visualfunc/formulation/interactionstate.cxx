/*
Copyright 2018 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
POCO C++ Libraries released under the Boost Software License; Copyright 2018, Applied Informatics Software Engineering GmbH and Contributors;
C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#include "interactionstate.hxx"

bool visualfunc::formulation::InteractionState::operator ==(const InteractionState& rhs) {
    auto lhs = *this;
    bool BoolsEqual = (lhs.IsWindowOpen == rhs.IsWindowOpen && lhs.IsWindowResized == rhs.IsWindowResized && lhs.IsMouseDown == rhs.IsMouseDown && lhs.IsMouseUp == rhs.IsMouseUp && lhs.IsVisualModelChanged == rhs.IsVisualModelChanged);
    bool IntsEqual = (lhs.MonitorWidth == rhs.MonitorWidth && lhs.MonitorHeight == rhs.MonitorHeight && lhs.WindowWidth == rhs.WindowWidth && lhs.WindowHeight == rhs.WindowHeight && lhs.MouseButton == rhs.MouseButton && lhs.MouseDirection == rhs.MouseDirection);
    bool WindowDimensionsEqual = (lhs.WindowDimensions.left() == rhs.WindowDimensions.left() && lhs.WindowDimensions.top() == rhs.WindowDimensions.top() && lhs.WindowDimensions.right() == rhs.WindowDimensions.right() && lhs.WindowDimensions.bottom() == rhs.WindowDimensions.bottom());
    bool MousePositionEqual = (lhs.MousePosition.x() == rhs.MousePosition.x() && lhs.MousePosition.y() == rhs.MousePosition.y());
    bool WindowPositionEqual = (lhs.WindowPosition.x() == rhs.WindowPosition.x() && lhs.WindowPosition.y() == rhs.WindowPosition.y());

    return (BoolsEqual && IntsEqual && WindowDimensionsEqual && MousePositionEqual && WindowPositionEqual);
}

bool visualfunc::formulation::InteractionState::operator !=(const InteractionState& rhs) {
    return !(*this == rhs);
}
