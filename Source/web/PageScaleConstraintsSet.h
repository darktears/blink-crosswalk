/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PageScaleConstraintsSet_h
#define PageScaleConstraintsSet_h

#include "core/dom/ViewportDescription.h"
#include "core/page/PageScaleConstraints.h"
#include "platform/Length.h"
#include "platform/geometry/IntSize.h"

namespace blink {

// This class harmonizes the viewport (particularly page scale) constraints from
// the meta viewport tag and other sources.
class PageScaleConstraintsSet {
public:
    PageScaleConstraintsSet();

    blink::PageScaleConstraints defaultConstraints() const;

    // Settings defined in the website's viewport tag, if viewport tag support
    // is enabled.
    const blink::PageScaleConstraints& pageDefinedConstraints() const { return m_pageDefinedConstraints; }
    void updatePageDefinedConstraints(const blink::ViewportDescription&, blink::Length legacyFallbackWidth);
    void adjustForAndroidWebViewQuirks(const blink::ViewportDescription&, int layoutFallbackWidth, float deviceScaleFactor, bool supportTargetDensityDPI, bool wideViewportQuirkEnabled, bool useWideViewport, bool loadWithOverviewMode, bool nonUserScalableQuirkEnabled);

    // Constraints may also be set from Chromium -- this overrides any
    // page-defined values.
    const blink::PageScaleConstraints& userAgentConstraints() const { return m_userAgentConstraints; }
    void setUserAgentConstraints(const blink::PageScaleConstraints&);

    // Actual computed values, taking into account the above plus the current
    // viewport size and document width.
    const blink::PageScaleConstraints& finalConstraints() const { return m_finalConstraints; }
    void computeFinalConstraints();
    void adjustFinalConstraintsToContentsSize(blink::IntSize contentsSize, int nonOverlayScrollbarWidth);

    void didChangeContentsSize(blink::IntSize contentsSize, float pageScaleFactor);

    // This should be set to true on each page load to note that the page scale
    // factor needs to be reset to its initial value.
    void setNeedsReset(bool);
    bool needsReset() const { return m_needsReset; }

    // This is set when one of the inputs to finalConstraints changes.
    bool constraintsDirty() const { return m_constraintsDirty; }

    void didChangeViewSize(const blink::IntSize&);

    blink::IntSize mainFrameSize(const blink::IntSize& contentsSize) const;

private:
    blink::PageScaleConstraints computeConstraintsStack() const;

    blink::PageScaleConstraints m_pageDefinedConstraints;
    blink::PageScaleConstraints m_userAgentConstraints;
    blink::PageScaleConstraints m_finalConstraints;

    int m_lastContentsWidth;
    blink::IntSize m_viewSize;

    bool m_needsReset;
    bool m_constraintsDirty;
};

} // namespace blink

#endif // PageScaleConstraintsSet_h
