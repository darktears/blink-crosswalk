/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2009 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef Comment_h
#define Comment_h

#include "core/dom/CharacterData.h"

namespace blink {

class Comment FINAL : public CharacterData {
public:
    static PassRefPtrWillBeRawPtr<Comment> create(Document&, const String&);

private:
    Comment(Document&, const String&);

    virtual String nodeName() const OVERRIDE;
    virtual NodeType nodeType() const OVERRIDE;
    virtual PassRefPtrWillBeRawPtr<Node> cloneNode(bool deep = true) OVERRIDE;
};

DEFINE_NODE_TYPE_CASTS(Comment, nodeType() == Node::COMMENT_NODE);

} // namespace blink

#endif // Comment_h
