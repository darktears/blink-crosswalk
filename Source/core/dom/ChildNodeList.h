/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2014 Samsung Electronics. All rights reserved.
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

#ifndef ChildNodeList_h
#define ChildNodeList_h

#include "core/dom/ContainerNode.h"
#include "core/dom/NodeList.h"
#include "core/html/CollectionIndexCache.h"
#include "wtf/PassRefPtr.h"

namespace blink {

class ChildNodeList FINAL : public NodeList {
public:
    static PassRefPtrWillBeRawPtr<ChildNodeList> create(ContainerNode& rootNode)
    {
        return adoptRefWillBeNoop(new ChildNodeList(rootNode));
    }

    virtual ~ChildNodeList();

    // DOM API.
    virtual unsigned length() const OVERRIDE { return m_collectionIndexCache.nodeCount(*this); }
    virtual Node* item(unsigned index) const OVERRIDE { return m_collectionIndexCache.nodeAt(*this, index); }

    // Non-DOM API.
    void invalidateCache() { m_collectionIndexCache.invalidate(); }
    ContainerNode& ownerNode() const { return *m_parent; }

    ContainerNode& rootNode() const { return ownerNode(); }

    // CollectionIndexCache API.
    bool canTraverseBackward() const { return true; }
    Node* traverseToFirstElement() const { return rootNode().firstChild(); }
    Node* traverseToLastElement() const { return rootNode().lastChild(); }
    Node* traverseForwardToOffset(unsigned offset, Node& currentNode, unsigned& currentOffset) const;
    Node* traverseBackwardToOffset(unsigned offset, Node& currentNode, unsigned& currentOffset) const;

    virtual void trace(Visitor*) OVERRIDE;

private:
    explicit ChildNodeList(ContainerNode& rootNode);

    virtual bool isChildNodeList() const OVERRIDE { return true; }
    virtual Node* virtualOwnerNode() const OVERRIDE;

    RefPtrWillBeMember<ContainerNode> m_parent;
    mutable CollectionIndexCache<ChildNodeList, Node> m_collectionIndexCache;
};

DEFINE_TYPE_CASTS(ChildNodeList, NodeList, nodeList, nodeList->isChildNodeList(), nodeList.isChildNodeList());

} // namespace blink

#endif // ChildNodeList_h
