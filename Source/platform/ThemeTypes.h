/*
 * Copyright (C) 2008, 2009, 2010 Apple Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ThemeTypes_h
#define ThemeTypes_h

namespace blink {

enum ControlState {
    HoverControlState = 1,
    PressedControlState = 1 << 1,
    FocusControlState = 1 << 2,
    EnabledControlState = 1 << 3,
    CheckedControlState = 1 << 4,
    ReadOnlyControlState = 1 << 5,
    WindowInactiveControlState = 1 << 7,
    IndeterminateControlState = 1 << 8,
    SpinUpControlState = 1 << 9, // Sub-state for HoverControlState and PressedControlState.
    AllControlStates = 0xffffffff
};

typedef unsigned ControlStates;

// Must follow CSSValueKeywords.in order
enum ControlPart {
    NoControlPart, CheckboxPart, RadioPart, PushButtonPart, SquareButtonPart, ButtonPart,
    ButtonBevelPart, InnerSpinButtonPart, ListboxPart, ListItemPart,
    MediaEnterFullscreenButtonPart, MediaExitFullscreenButtonPart, MediaFullScreenVolumeSliderPart, MediaFullScreenVolumeSliderThumbPart, MediaMuteButtonPart, MediaPlayButtonPart,
    MediaOverlayPlayButtonPart, MediaToggleClosedCaptionsButtonPart,
    MediaSliderPart, MediaSliderThumbPart, MediaVolumeSliderContainerPart, MediaVolumeSliderPart, MediaVolumeSliderThumbPart,
    MediaControlsBackgroundPart, MediaControlsFullscreenBackgroundPart, MediaCurrentTimePart, MediaTimeRemainingPart,
    MenulistPart, MenulistButtonPart, MenulistTextPart, MenulistTextFieldPart, MeterPart, ProgressBarPart, ProgressBarValuePart,
    SliderHorizontalPart, SliderVerticalPart, SliderThumbHorizontalPart,
    SliderThumbVerticalPart, CaretPart, SearchFieldPart, SearchFieldDecorationPart,
    SearchFieldResultsDecorationPart,
    SearchFieldCancelButtonPart, TextFieldPart,
    RelevancyLevelIndicatorPart, ContinuousCapacityLevelIndicatorPart, DiscreteCapacityLevelIndicatorPart, RatingLevelIndicatorPart,
    TextAreaPart, CapsLockIndicatorPart
};

enum SelectionPart {
    SelectionBackground, SelectionForeground
};

enum ThemeFont {
    CaptionFont, IconFont, MenuFont, MessageBoxFont, SmallCaptionFont, StatusBarFont, MiniControlFont, SmallControlFont, ControlFont
};

enum ThemeColor {
    ActiveBorderColor, ActiveCaptionColor, AppWorkspaceColor, BackgroundColor, ButtonFaceColor, ButtonHighlightColor, ButtonShadowColor,
    ButtonTextColor, CaptionTextColor, GrayTextColor, HighlightColor, HighlightTextColor, InactiveBorderColor, InactiveCaptionColor,
    InactiveCaptionTextColor, InfoBackgroundColor, InfoTextColor, MatchColor, MenuTextColor, ScrollbarColor, ThreeDDarkDhasowColor,
    ThreeDFaceColor, ThreeDHighlightColor, ThreeDLightShadowColor, ThreeDShadowCLor, WindowColor, WindowFrameColor, WindowTextColor,
    FocusRingColor, ActiveListBoxSelection, ActiveListBoxSelectionText, InactiveListBoxSelection, InactiveListBoxSelectionText
};

}
#endif
