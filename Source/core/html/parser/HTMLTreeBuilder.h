/*
 * Copyright (C) 2010 Google, Inc. All Rights Reserved.
 * Copyright (C) 2011 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY GOOGLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL GOOGLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HTMLTreeBuilder_h
#define HTMLTreeBuilder_h

#include "core/html/parser/HTMLConstructionSite.h"
#include "core/html/parser/HTMLElementStack.h"
#include "core/html/parser/HTMLParserOptions.h"
#include "platform/heap/Handle.h"
#include "wtf/Noncopyable.h"
#include "wtf/PassOwnPtr.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefPtr.h"
#include "wtf/Vector.h"
#include "wtf/text/StringBuilder.h"
#include "wtf/text/TextPosition.h"

namespace blink {

class AtomicHTMLToken;
class Document;
class DocumentFragment;
class Element;
class LocalFrame;
class HTMLToken;
class HTMLDocument;
class Node;
class HTMLDocumentParser;

class HTMLTreeBuilder FINAL : public NoBaseWillBeGarbageCollectedFinalized<HTMLTreeBuilder> {
    WTF_MAKE_NONCOPYABLE(HTMLTreeBuilder); WTF_MAKE_FAST_ALLOCATED_WILL_BE_REMOVED;
public:
    static PassOwnPtrWillBeRawPtr<HTMLTreeBuilder> create(HTMLDocumentParser* parser, HTMLDocument* document, ParserContentPolicy parserContentPolicy, bool reportErrors, const HTMLParserOptions& options)
    {
        return adoptPtrWillBeNoop(new HTMLTreeBuilder(parser, document, parserContentPolicy, reportErrors, options));
    }
    static PassOwnPtrWillBeRawPtr<HTMLTreeBuilder> create(HTMLDocumentParser* parser, DocumentFragment* fragment, Element* contextElement, ParserContentPolicy parserContentPolicy, const HTMLParserOptions& options)
    {
        return adoptPtrWillBeNoop(new HTMLTreeBuilder(parser, fragment, contextElement, parserContentPolicy, options));
    }
    ~HTMLTreeBuilder();
    void trace(Visitor*);

    const HTMLElementStack* openElements() const { return m_tree.openElements(); }

    bool isParsingFragment() const { return !!m_fragmentContext.fragment(); }
    bool isParsingTemplateContents() const { return m_tree.openElements()->hasTemplateInHTMLScope(); }
    bool isParsingFragmentOrTemplateContents() const { return isParsingFragment() || isParsingTemplateContents(); }

    void detach();

    void constructTree(AtomicHTMLToken*);

    bool hasParserBlockingScript() const { return !!m_scriptToProcess; }
    // Must be called to take the parser-blocking script before calling the parser again.
    PassRefPtrWillBeRawPtr<Element> takeScriptToProcess(TextPosition& scriptStartPosition);

    // Done, close any open tags, etc.
    void finished();

    // Synchronously empty any queues, possibly creating more DOM nodes.
    void flush() { m_tree.flush(); }

    void setShouldSkipLeadingNewline(bool shouldSkip) { m_shouldSkipLeadingNewline = shouldSkip; }

private:
    class CharacterTokenBuffer;
    // Represents HTML5 "insertion mode"
    // http://www.whatwg.org/specs/web-apps/current-work/multipage/parsing.html#insertion-mode
    enum InsertionMode {
        InitialMode,
        BeforeHTMLMode,
        BeforeHeadMode,
        InHeadMode,
        InHeadNoscriptMode,
        AfterHeadMode,
        TemplateContentsMode,
        InBodyMode,
        TextMode,
        InTableMode,
        InTableTextMode,
        InCaptionMode,
        InColumnGroupMode,
        InTableBodyMode,
        InRowMode,
        InCellMode,
        InSelectMode,
        InSelectInTableMode,
        AfterBodyMode,
        InFramesetMode,
        AfterFramesetMode,
        AfterAfterBodyMode,
        AfterAfterFramesetMode,
    };

    HTMLTreeBuilder(HTMLDocumentParser*, HTMLDocument*, ParserContentPolicy, bool reportErrors, const HTMLParserOptions&);
    HTMLTreeBuilder(HTMLDocumentParser*, DocumentFragment*, Element* contextElement, ParserContentPolicy, const HTMLParserOptions&);

    void processToken(AtomicHTMLToken*);

    void processDoctypeToken(AtomicHTMLToken*);
    void processStartTag(AtomicHTMLToken*);
    void processEndTag(AtomicHTMLToken*);
    void processComment(AtomicHTMLToken*);
    void processCharacter(AtomicHTMLToken*);
    void processEndOfFile(AtomicHTMLToken*);

    bool processStartTagForInHead(AtomicHTMLToken*);
    void processStartTagForInBody(AtomicHTMLToken*);
    void processStartTagForInTable(AtomicHTMLToken*);
    void processEndTagForInBody(AtomicHTMLToken*);
    void processEndTagForInTable(AtomicHTMLToken*);
    void processEndTagForInTableBody(AtomicHTMLToken*);
    void processEndTagForInRow(AtomicHTMLToken*);
    void processEndTagForInCell(AtomicHTMLToken*);

    void processIsindexStartTagForInBody(AtomicHTMLToken*);
    void processHtmlStartTagForInBody(AtomicHTMLToken*);
    bool processBodyEndTagForInBody(AtomicHTMLToken*);
    bool processTableEndTagForInTable();
    bool processCaptionEndTagForInCaption();
    bool processColgroupEndTagForInColumnGroup();
    bool processTrEndTagForInRow();
    // FIXME: This function should be inlined into its one call site or it
    // needs to assert which tokens it can be called with.
    void processAnyOtherEndTagForInBody(AtomicHTMLToken*);

    void processCharacterBuffer(CharacterTokenBuffer&);
    inline void processCharacterBufferForInBody(CharacterTokenBuffer&);

    void processFakeStartTag(const QualifiedName&, const Vector<Attribute>& attributes = Vector<Attribute>());
    void processFakeEndTag(const QualifiedName&);
    void processFakeEndTag(const AtomicString&);
    void processFakePEndTagIfPInButtonScope();

    void processGenericRCDATAStartTag(AtomicHTMLToken*);
    void processGenericRawTextStartTag(AtomicHTMLToken*);
    void processScriptStartTag(AtomicHTMLToken*);

    // Default processing for the different insertion modes.
    void defaultForInitial();
    void defaultForBeforeHTML();
    void defaultForBeforeHead();
    void defaultForInHead();
    void defaultForInHeadNoscript();
    void defaultForAfterHead();
    void defaultForInTableText();

    inline HTMLStackItem* adjustedCurrentStackItem() const;
    inline bool shouldProcessTokenInForeignContent(AtomicHTMLToken*);
    void processTokenInForeignContent(AtomicHTMLToken*);

    Vector<Attribute> attributesForIsindexInput(AtomicHTMLToken*);

    void callTheAdoptionAgency(AtomicHTMLToken*);

    void closeTheCell();

    template <bool shouldClose(const HTMLStackItem*)>
    void processCloseWhenNestedTag(AtomicHTMLToken*);

    void parseError(AtomicHTMLToken*);

    InsertionMode insertionMode() const { return m_insertionMode; }
    void setInsertionMode(InsertionMode mode) { m_insertionMode = mode; }

    void resetInsertionModeAppropriately();

    void processTemplateStartTag(AtomicHTMLToken*);
    bool processTemplateEndTag(AtomicHTMLToken*);
    bool processEndOfFileForInTemplateContents(AtomicHTMLToken*);

    class FragmentParsingContext {
        WTF_MAKE_NONCOPYABLE(FragmentParsingContext);
        DISALLOW_ALLOCATION();
    public:
        FragmentParsingContext();
        FragmentParsingContext(DocumentFragment*, Element* contextElement);
        ~FragmentParsingContext();

        DocumentFragment* fragment() const { return m_fragment; }
        Element* contextElement() const { ASSERT(m_fragment); return m_contextElementStackItem->element(); }
        HTMLStackItem* contextElementStackItem() const { ASSERT(m_fragment); return m_contextElementStackItem.get(); }

        void trace(Visitor*);

    private:
        RawPtrWillBeMember<DocumentFragment> m_fragment;
        RefPtrWillBeMember<HTMLStackItem> m_contextElementStackItem;
    };

    bool m_framesetOk;
#if ENABLE(ASSERT)
    bool m_isAttached;
#endif
    FragmentParsingContext m_fragmentContext;
    HTMLConstructionSite m_tree;

    // http://www.whatwg.org/specs/web-apps/current-work/multipage/parsing.html#insertion-mode
    InsertionMode m_insertionMode;

    // http://www.whatwg.org/specs/web-apps/current-work/multipage/parsing.html#original-insertion-mode
    InsertionMode m_originalInsertionMode;

    Vector<InsertionMode> m_templateInsertionModes;

    // http://www.whatwg.org/specs/web-apps/current-work/multipage/tokenization.html#pending-table-character-tokens
    StringBuilder m_pendingTableCharacters;

    bool m_shouldSkipLeadingNewline;

    // We access parser because HTML5 spec requires that we be able to change the state of the tokenizer
    // from within parser actions. We also need it to track the current position.
    RawPtrWillBeMember<HTMLDocumentParser> m_parser;

    RefPtrWillBeMember<Element> m_scriptToProcess; // <script> tag which needs processing before resuming the parser.
    TextPosition m_scriptToProcessStartPosition; // Starting line number of the script tag needing processing.

    HTMLParserOptions m_options;
};

}

#endif
