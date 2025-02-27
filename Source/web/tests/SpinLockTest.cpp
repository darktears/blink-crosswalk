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

#include "config.h"
#include "wtf/SpinLock.h"

#include "platform/Task.h"
#include "public/platform/Platform.h"
#include "public/platform/WebThread.h"
#include "wtf/OwnPtr.h"
#include "wtf/PassOwnPtr.h"

#include <gtest/gtest.h>

using blink::Task;

namespace {

static const size_t bufferSize = 16;

static int lock = 0;

static void fillBuffer(volatile char* buffer, char fillPattern)
{
    for (size_t i = 0; i < bufferSize; ++i)
        buffer[i] = fillPattern;
}

static void changeAndCheckBuffer(volatile char* buffer)
{
    fillBuffer(buffer, '\0');
    int total = 0;
    for (size_t i = 0; i < bufferSize; ++i)
        total += buffer[i];

    EXPECT_EQ(0, total);

    // This will mess with the other thread's calculation if we accidentally get
    // concurrency.
    fillBuffer(buffer, '!');
}

static void threadMain(volatile char* buffer)
{
    for (int i = 0; i < 500000; ++i) {
        spinLockLock(&lock);
        changeAndCheckBuffer(buffer);
        spinLockUnlock(&lock);
    }
}

TEST(WTF_SpinLock, Torture)
{
    char sharedBuffer[bufferSize];

    OwnPtr<blink::WebThread> thread1 = adoptPtr(blink::Platform::current()->createThread("thread1"));
    OwnPtr<blink::WebThread> thread2 = adoptPtr(blink::Platform::current()->createThread("thread2"));

    thread1->postTask(new Task(WTF::bind(&threadMain, static_cast<char*>(sharedBuffer))));
    thread2->postTask(new Task(WTF::bind(&threadMain, static_cast<char*>(sharedBuffer))));

    thread1.clear();
    thread2.clear();
}

} // namespace
