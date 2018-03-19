#ifndef __visualfunc_formulation_interactionstate__
#define __visualfunc_formulation_interactionstate__
#include <iostream>
#include <dlib/geometry.h>
#include "interactionstate.hxx"
#include "textbuffer.hxx"
/*
	Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 . Software distributed under the License is distributed on an "AS IS" BASIS, NO WARRANTIES OR CONDITIONS OF ANY KIND, explicit or implicit. See the License for details on permissions and limitations.
*/

namespace visualfunc {
namespace formulation {
struct InteractionState {
  public:
    InteractionState(const InteractionState&) = default;
    InteractionState() = default;

    bool
    IsWindowOpen,
    IsWindowResized,
    IsMouseDown,
    IsMouseUp,
    IsVisualModelChanged,
    IsDisplayUpdated,
    IsDisplayUpdating,
    IsKeyAvailable,
    IsKeyUp,
    IsKeyDown,
    IsShiftDown,
    IsCapsLockOn
        = false;

    int
    MonitorWidth,
    MonitorHeight,
    WindowWidth,
    WindowHeight,
    MouseButton,
    MouseDirection,
    KeyboardKeyCode;

    unsigned
    KeyModifiers;

    dlib::drectangle
    WindowDimensions;

    dlib::dpoint
    MousePosition;

    dlib::dpoint
    WindowPosition;

    bool operator ==(const InteractionState& rhs);
    bool operator !=(const InteractionState& rhs);
};
}
}
#endif
