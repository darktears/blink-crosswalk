/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @param {!Object} obj
 * @return {boolean}
 */
Object.isEmpty = function(obj)
{
    for (var i in obj)
        return false;
    return true;
}

/**
 * @param {!Object.<string,!T>} obj
 * @return {!Array.<!T>}
 * @template T
 */
Object.values = function(obj)
{
    var result = Object.keys(obj);
    var length = result.length;
    for (var i = 0; i < length; ++i)
        result[i] = obj[result[i]];
    return result;
}

/**
 * @param {number} m
 * @param {number} n
 * @return {number}
 */
function mod(m, n)
{
    return ((m % n) + n) % n;
}

/**
 * @param {string} string
 * @return {!Array.<number>}
 */
String.prototype.findAll = function(string)
{
    var matches = [];
    var i = this.indexOf(string);
    while (i !== -1) {
        matches.push(i);
        i = this.indexOf(string, i + string.length);
    }
    return matches;
}

/**
 * @return {!Array.<number>}
 */
String.prototype.lineEndings = function()
{
    if (!this._lineEndings) {
        this._lineEndings = this.findAll("\n");
        this._lineEndings.push(this.length);
    }
    return this._lineEndings;
}

/**
 * @return {number}
 */
String.prototype.lineCount = function()
{
    var lineEndings = this.lineEndings();
    return lineEndings.length;
}

/**
 * @return {string}
 */
String.prototype.lineAt = function(lineNumber)
{
    var lineEndings = this.lineEndings();
    var lineStart = lineNumber > 0 ? lineEndings[lineNumber - 1] + 1 : 0;
    var lineEnd = lineEndings[lineNumber];
    var lineContent = this.substring(lineStart, lineEnd);
    if (lineContent.length > 0 && lineContent.charAt(lineContent.length - 1) === "\r")
        lineContent = lineContent.substring(0, lineContent.length - 1);
    return lineContent;
}

/**
 * @param {string} chars
 * @return {string}
 */
String.prototype.escapeCharacters = function(chars)
{
    var foundChar = false;
    for (var i = 0; i < chars.length; ++i) {
        if (this.indexOf(chars.charAt(i)) !== -1) {
            foundChar = true;
            break;
        }
    }

    if (!foundChar)
        return String(this);

    var result = "";
    for (var i = 0; i < this.length; ++i) {
        if (chars.indexOf(this.charAt(i)) !== -1)
            result += "\\";
        result += this.charAt(i);
    }

    return result;
}

/**
 * @return {string}
 */
String.regexSpecialCharacters = function()
{
    return "^[]{}()\\.^$*+?|-,";
}

/**
 * @return {string}
 */
String.prototype.escapeForRegExp = function()
{
    return this.escapeCharacters(String.regexSpecialCharacters());
}

/**
 * @return {string}
 */
String.prototype.escapeHTML = function()
{
    return this.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/"/g, "&quot;"); //" doublequotes just for editor
}

/**
 * @return {string}
 */
String.prototype.collapseWhitespace = function()
{
    return this.replace(/[\s\xA0]+/g, " ");
}

/**
 * @param {number} maxLength
 * @return {string}
 */
String.prototype.trimMiddle = function(maxLength)
{
    if (this.length <= maxLength)
        return String(this);
    var leftHalf = maxLength >> 1;
    var rightHalf = maxLength - leftHalf - 1;
    return this.substr(0, leftHalf) + "\u2026" + this.substr(this.length - rightHalf, rightHalf);
}

/**
 * @param {number} maxLength
 * @return {string}
 */
String.prototype.trimEnd = function(maxLength)
{
    if (this.length <= maxLength)
        return String(this);
    return this.substr(0, maxLength - 1) + "\u2026";
}

/**
 * @param {?string=} baseURLDomain
 * @return {string}
 */
String.prototype.trimURL = function(baseURLDomain)
{
    var result = this.replace(/^(https|http|file):\/\//i, "");
    if (baseURLDomain)
        result = result.replace(new RegExp("^" + baseURLDomain.escapeForRegExp(), "i"), "");
    return result;
}

/**
 * @return {string}
 */
String.prototype.toTitleCase = function()
{
    return this.substring(0, 1).toUpperCase() + this.substring(1);
}

/**
 * @param {string} other
 * @return {number}
 */
String.prototype.compareTo = function(other)
{
    if (this > other)
        return 1;
    if (this < other)
        return -1;
    return 0;
}

/**
 * @param {string} href
 * @return {?string}
 */
function sanitizeHref(href)
{
    return href && href.trim().toLowerCase().startsWith("javascript:") ? null : href;
}

/**
 * @return {string}
 */
String.prototype.removeURLFragment = function()
{
    var fragmentIndex = this.indexOf("#");
    if (fragmentIndex == -1)
        fragmentIndex = this.length;
    return this.substring(0, fragmentIndex);
}

/**
 * @return {boolean}
 */
String.prototype.startsWith = function(substring)
{
    return !this.lastIndexOf(substring, 0);
}

/**
 * @return {boolean}
 */
String.prototype.endsWith = function(substring)
{
    return this.indexOf(substring, this.length - substring.length) !== -1;
}

/**
 * @return {number}
 */
String.prototype.hashCode = function()
{
    var result = 0;
    for (var i = 0; i < this.length; ++i)
        result = (result * 3 + this.charCodeAt(i)) | 0;
    return result;
}

/**
 * @param {number} index
 * @return {boolean}
 */
String.prototype.isDigitAt = function(index)
{
    var c = this.charCodeAt(index);
    return 48 <= c && c <= 57;
}

/**
 * @param {string} a
 * @param {string} b
 * @return {number}
 */
String.naturalOrderComparator = function(a, b)
{
    var chunk = /^\d+|^\D+/;
    var chunka, chunkb, anum, bnum;
    while (1) {
        if (a) {
            if (!b)
                return 1;
        } else {
            if (b)
                return -1;
            else
                return 0;
        }
        chunka = a.match(chunk)[0];
        chunkb = b.match(chunk)[0];
        anum = !isNaN(chunka);
        bnum = !isNaN(chunkb);
        if (anum && !bnum)
            return -1;
        if (bnum && !anum)
            return 1;
        if (anum && bnum) {
            var diff = chunka - chunkb;
            if (diff)
                return diff;
            if (chunka.length !== chunkb.length) {
                if (!+chunka && !+chunkb) // chunks are strings of all 0s (special case)
                    return chunka.length - chunkb.length;
                else
                    return chunkb.length - chunka.length;
            }
        } else if (chunka !== chunkb)
            return (chunka < chunkb) ? -1 : 1;
        a = a.substring(chunka.length);
        b = b.substring(chunkb.length);
    }
}

/**
 * @param {number} num
 * @param {number} min
 * @param {number} max
 * @return {number}
 */
Number.constrain = function(num, min, max)
{
    if (num < min)
        num = min;
    else if (num > max)
        num = max;
    return num;
}

/**
 * @param {number} a
 * @param {number} b
 * @return {number}
 */
Number.gcd = function(a, b)
{
    if (b === 0)
        return a;
    else
        return Number.gcd(b, a % b);
}

/**
 * @param {string} value
 * @return {string}
 */
Number.toFixedIfFloating = function(value)
{
    if (!value || isNaN(value))
        return value;
    var number = Number(value);
    return number % 1 ? number.toFixed(3) : String(number);
}

/**
 * @return {string}
 */
Date.prototype.toISO8601Compact = function()
{
    /**
     * @param {number} x
     * @return {string}
     */
    function leadZero(x)
    {
        return (x > 9 ? "" : "0") + x;
    }
    return this.getFullYear() +
           leadZero(this.getMonth() + 1) +
           leadZero(this.getDate()) + "T" +
           leadZero(this.getHours()) +
           leadZero(this.getMinutes()) +
           leadZero(this.getSeconds());
}

/**
 * @return {string}
 */
 Date.prototype.toConsoleTime = function()
{
    /**
     * @param {number} x
     * @return {string}
     */
    function leadZero2(x)
    {
        return (x > 9 ? "" : "0") + x;
    }

    /**
     * @param {number} x
     * @return {string}
     */
    function leadZero3(x)
    {
        return (Array(4 - x.toString().length)).join('0') + x;
    }

    return this.getFullYear() + "-" +
           leadZero2(this.getMonth() + 1) + "-" +
           leadZero2(this.getDate()) + " " +
           leadZero2(this.getHours()) + ":" +
           leadZero2(this.getMinutes()) + ":" +
           leadZero2(this.getSeconds()) + "." +
           leadZero3(this.getMilliseconds());
}

Object.defineProperty(Array.prototype, "remove",
{
    /**
     * @param {!T} value
     * @param {boolean=} firstOnly
     * @this {Array.<!T>}
     * @template T
     */
    value: function(value, firstOnly)
    {
        var index = this.indexOf(value);
        if (index === -1)
            return;
        if (firstOnly) {
            this.splice(index, 1);
            return;
        }
        for (var i = index + 1, n = this.length; i < n; ++i) {
            if (this[i] !== value)
                this[index++] = this[i];
        }
        this.length = index;
    }
});

Object.defineProperty(Array.prototype, "keySet",
{
    /**
     * @return {!Object.<string, boolean>}
     * @this {Array.<*>}
     */
    value: function()
    {
        var keys = {};
        for (var i = 0; i < this.length; ++i)
            keys[this[i]] = true;
        return keys;
    }
});

Object.defineProperty(Array.prototype, "pushAll",
{
    /**
     * @param {!Array.<!T>} array
     * @this {Array.<!T>}
     * @template T
     */
    value: function(array)
    {
        Array.prototype.push.apply(this, array);
    }
});

Object.defineProperty(Array.prototype, "rotate",
{
    /**
     * @param {number} index
     * @return {!Array.<!T>}
     * @this {Array.<!T>}
     * @template T
     */
    value: function(index)
    {
        var result = [];
        for (var i = index; i < index + this.length; ++i)
            result.push(this[i % this.length]);
        return result;
    }
});

Object.defineProperty(Array.prototype, "sortNumbers",
{
    /**
     * @this {Array.<number>}
     */
    value: function()
    {
        /**
         * @param {number} a
         * @param {number} b
         * @return {number}
         */
        function numericComparator(a, b)
        {
            return a - b;
        }

        this.sort(numericComparator);
    }
});

Object.defineProperty(Uint32Array.prototype, "sort", {
    value: Array.prototype.sort
});

(function() {
var partition = {
    /**
     * @this {Array.<number>}
     * @param {function(number, number): number} comparator
     * @param {number} left
     * @param {number} right
     * @param {number} pivotIndex
     */
    value: function(comparator, left, right, pivotIndex)
    {
        function swap(array, i1, i2)
        {
            var temp = array[i1];
            array[i1] = array[i2];
            array[i2] = temp;
        }

        var pivotValue = this[pivotIndex];
        swap(this, right, pivotIndex);
        var storeIndex = left;
        for (var i = left; i < right; ++i) {
            if (comparator(this[i], pivotValue) < 0) {
                swap(this, storeIndex, i);
                ++storeIndex;
            }
        }
        swap(this, right, storeIndex);
        return storeIndex;
    }
};
Object.defineProperty(Array.prototype, "partition", partition);
Object.defineProperty(Uint32Array.prototype, "partition", partition);

var sortRange = {
    /**
     * @param {function(number, number): number} comparator
     * @param {number} leftBound
     * @param {number} rightBound
     * @param {number} sortWindowLeft
     * @param {number} sortWindowRight
     * @return {!Array.<number>}
     * @this {Array.<number>}
     */
    value: function(comparator, leftBound, rightBound, sortWindowLeft, sortWindowRight)
    {
        function quickSortRange(array, comparator, left, right, sortWindowLeft, sortWindowRight)
        {
            if (right <= left)
                return;
            var pivotIndex = Math.floor(Math.random() * (right - left)) + left;
            var pivotNewIndex = array.partition(comparator, left, right, pivotIndex);
            if (sortWindowLeft < pivotNewIndex)
                quickSortRange(array, comparator, left, pivotNewIndex - 1, sortWindowLeft, sortWindowRight);
            if (pivotNewIndex < sortWindowRight)
                quickSortRange(array, comparator, pivotNewIndex + 1, right, sortWindowLeft, sortWindowRight);
        }
        if (leftBound === 0 && rightBound === (this.length - 1) && sortWindowLeft === 0 && sortWindowRight >= rightBound)
            this.sort(comparator);
        else
            quickSortRange(this, comparator, leftBound, rightBound, sortWindowLeft, sortWindowRight);
        return this;
    }
}
Object.defineProperty(Array.prototype, "sortRange", sortRange);
Object.defineProperty(Uint32Array.prototype, "sortRange", sortRange);
})();

Object.defineProperty(Array.prototype, "stableSort",
{
    /**
     * @param {function(?T, ?T): number=} comparator
     * @return {!Array.<?T>}
     * @this {Array.<?T>}
     * @template T
     */
    value: function(comparator)
    {
        function defaultComparator(a, b)
        {
            return a < b ? -1 : (a > b ? 1 : 0);
        }
        comparator = comparator || defaultComparator;

        var indices = new Array(this.length);
        for (var i = 0; i < this.length; ++i)
            indices[i] = i;
        var self = this;
        /**
         * @param {number} a
         * @param {number} b
         * @return {number}
         */
        function indexComparator(a, b)
        {
            var result = comparator(self[a], self[b]);
            return result ? result : a - b;
        }
        indices.sort(indexComparator);

        for (var i = 0; i < this.length; ++i) {
            if (indices[i] < 0 || i === indices[i])
                continue;
            var cyclical = i;
            var saved = this[i];
            while (true) {
                var next = indices[cyclical];
                indices[cyclical] = -1;
                if (next === i) {
                    this[cyclical] = saved;
                    break;
                } else {
                    this[cyclical] = this[next];
                    cyclical = next;
                }
            }
        }
        return this;
    }
});

Object.defineProperty(Array.prototype, "qselect",
{
    /**
     * @param {number} k
     * @param {function(number, number): number=} comparator
     * @return {number|undefined}
     * @this {Array.<number>}
     */
    value: function(k, comparator)
    {
        if (k < 0 || k >= this.length)
            return;
        if (!comparator)
            comparator = function(a, b) { return a - b; }

        var low = 0;
        var high = this.length - 1;
        for (;;) {
            var pivotPosition = this.partition(comparator, low, high, Math.floor((high + low) / 2));
            if (pivotPosition === k)
                return this[k];
            else if (pivotPosition > k)
                high = pivotPosition - 1;
            else
                low = pivotPosition + 1;
        }
    }
});

Object.defineProperty(Array.prototype, "lowerBound",
{
    /**
     * Return index of the leftmost element that is equal or greater
     * than the specimen object. If there's no such element (i.e. all
     * elements are smaller than the specimen) returns right bound.
     * The function works for sorted array.
     * When specified, |left| (inclusive) and |right| (exclusive) indices
     * define the search window.
     *
     * @param {!T} object
     * @param {function(!T,!S):number=} comparator
     * @param {number=} left
     * @param {number=} right
     * @return {number}
     * @this {Array.<!S>}
     * @template T,S
     */
    value: function(object, comparator, left, right)
    {
        function defaultComparator(a, b)
        {
            return a < b ? -1 : (a > b ? 1 : 0);
        }
        comparator = comparator || defaultComparator;
        var l = left || 0;
        var r = right !== undefined ? right : this.length;
        while (l < r) {
            var m = (l + r) >> 1;
            if (comparator(object, this[m]) > 0)
                l = m + 1;
            else
                r = m;
        }
        return r;
    }
});

Object.defineProperty(Array.prototype, "upperBound",
{
    /**
     * Return index of the leftmost element that is greater
     * than the specimen object. If there's no such element (i.e. all
     * elements are smaller or equal to the specimen) returns right bound.
     * The function works for sorted array.
     * When specified, |left| (inclusive) and |right| (exclusive) indices
     * define the search window.
     *
     * @param {!T} object
     * @param {function(!T,!S):number=} comparator
     * @param {number=} left
     * @param {number=} right
     * @return {number}
     * @this {Array.<!S>}
     * @template T,S
     */
    value: function(object, comparator, left, right)
    {
        function defaultComparator(a, b)
        {
            return a < b ? -1 : (a > b ? 1 : 0);
        }
        comparator = comparator || defaultComparator;
        var l = left || 0;
        var r = right !== undefined ? right : this.length;
        while (l < r) {
            var m = (l + r) >> 1;
            if (comparator(object, this[m]) >= 0)
                l = m + 1;
            else
                r = m;
        }
        return r;
    }
});

Object.defineProperty(Uint32Array.prototype, "lowerBound", {
    value: Array.prototype.lowerBound
});

Object.defineProperty(Uint32Array.prototype, "upperBound", {
    value: Array.prototype.upperBound
});

Object.defineProperty(Float64Array.prototype, "lowerBound", {
    value: Array.prototype.lowerBound
});

Object.defineProperty(Array.prototype, "binaryIndexOf",
{
    /**
     * @param {!T} value
     * @param {function(!T,!S):number} comparator
     * @return {number}
     * @this {Array.<!S>}
     * @template T,S
     */
    value: function(value, comparator)
    {
        var index = this.lowerBound(value, comparator);
        return index < this.length && comparator(value, this[index]) === 0 ? index : -1;
    }
});

Object.defineProperty(Array.prototype, "select",
{
    /**
     * @param {string} field
     * @return {!Array.<!T>}
     * @this {Array.<!Object.<string,!T>>}
     * @template T
     */
    value: function(field)
    {
        var result = new Array(this.length);
        for (var i = 0; i < this.length; ++i)
            result[i] = this[i][field];
        return result;
    }
});

Object.defineProperty(Array.prototype, "peekLast",
{
    /**
     * @return {!T|undefined}
     * @this {Array.<!T>}
     * @template T
     */
    value: function()
    {
        return this[this.length - 1];
    }
});

(function(){

/**
 * @param {!Array.<T>} array1
 * @param {!Array.<T>} array2
 * @param {function(T,T):number} comparator
 * @param {boolean} mergeNotIntersect
 * @return {!Array.<T>}
 * @template T
 */
function mergeOrIntersect(array1, array2, comparator, mergeNotIntersect)
{
    var result = [];
    var i = 0;
    var j = 0;
    while (i < array1.length && j < array2.length) {
        var compareValue = comparator(array1[i], array2[j]);
        if (mergeNotIntersect || !compareValue)
            result.push(compareValue <= 0 ? array1[i] : array2[j]);
        if (compareValue <= 0)
            i++;
        if (compareValue >= 0)
            j++;
    }
    if (mergeNotIntersect) {
        while (i < array1.length)
            result.push(array1[i++]);
        while (j < array2.length)
            result.push(array2[j++]);
    }
    return result;
}

Object.defineProperty(Array.prototype, "intersectOrdered",
{
    /**
     * @param {!Array.<T>} array
     * @param {function(T,T):number} comparator
     * @return {!Array.<T>}
     * @this {!Array.<T>}
     * @template T
     */
    value: function(array, comparator)
    {
        return mergeOrIntersect(this, array, comparator, false);
    }
});

Object.defineProperty(Array.prototype, "mergeOrdered",
{
    /**
     * @param {!Array.<T>} array
     * @param {function(T,T):number} comparator
     * @return {!Array.<T>}
     * @this {!Array.<T>}
     * @template T
     */
    value: function(array, comparator)
    {
        return mergeOrIntersect(this, array, comparator, true);
    }
});

}());


/**
 * @param {!T} object
 * @param {!Array.<!S>} list
 * @param {function(!T,!S):number=} comparator
 * @param {boolean=} insertionIndexAfter
 * @return {number}
 * @template T,S
 */
function insertionIndexForObjectInListSortedByFunction(object, list, comparator, insertionIndexAfter)
{
    if (insertionIndexAfter)
        return list.upperBound(object, comparator);
    else
        return list.lowerBound(object, comparator);
}

/**
 * @param {string} format
 * @param {...*} var_arg
 * @return {string}
 */
String.sprintf = function(format, var_arg)
{
    return String.vsprintf(format, Array.prototype.slice.call(arguments, 1));
}

/**
 * @param {string} format
 * @param {!Object.<string, function(string, ...):*>} formatters
 * @return {!Array.<!Object>}
 */
String.tokenizeFormatString = function(format, formatters)
{
    var tokens = [];
    var substitutionIndex = 0;

    function addStringToken(str)
    {
        tokens.push({ type: "string", value: str });
    }

    function addSpecifierToken(specifier, precision, substitutionIndex)
    {
        tokens.push({ type: "specifier", specifier: specifier, precision: precision, substitutionIndex: substitutionIndex });
    }

    var index = 0;
    for (var precentIndex = format.indexOf("%", index); precentIndex !== -1; precentIndex = format.indexOf("%", index)) {
        addStringToken(format.substring(index, precentIndex));
        index = precentIndex + 1;

        if (format[index] === "%") {
            // %% escape sequence.
            addStringToken("%");
            ++index;
            continue;
        }

        if (format.isDigitAt(index)) {
            // The first character is a number, it might be a substitution index.
            var number = parseInt(format.substring(index), 10);
            while (format.isDigitAt(index))
                ++index;

            // If the number is greater than zero and ends with a "$",
            // then this is a substitution index.
            if (number > 0 && format[index] === "$") {
                substitutionIndex = (number - 1);
                ++index;
            }
        }

        var precision = -1;
        if (format[index] === ".") {
            // This is a precision specifier. If no digit follows the ".",
            // then the precision should be zero.
            ++index;
            precision = parseInt(format.substring(index), 10);
            if (isNaN(precision))
                precision = 0;

            while (format.isDigitAt(index))
                ++index;
        }

        if (!(format[index] in formatters)) {
            addStringToken(format.substring(precentIndex, index + 1));
            ++index;
            continue;
        }

        addSpecifierToken(format[index], precision, substitutionIndex);

        ++substitutionIndex;
        ++index;
    }

    addStringToken(format.substring(index));

    return tokens;
}

String.standardFormatters = {
    /**
     * @return {number}
     */
    d: function(substitution)
    {
        return !isNaN(substitution) ? substitution : 0;
    },

    /**
     * @return {number}
     */
    f: function(substitution, token)
    {
        if (substitution && token.precision > -1)
            substitution = substitution.toFixed(token.precision);
        return !isNaN(substitution) ? substitution : (token.precision > -1 ? Number(0).toFixed(token.precision) : 0);
    },

    /**
     * @return {string}
     */
    s: function(substitution)
    {
        return substitution;
    }
}

/**
 * @param {string} format
 * @param {!Array.<*>} substitutions
 * @return {string}
 */
String.vsprintf = function(format, substitutions)
{
    return String.format(format, substitutions, String.standardFormatters, "", function(a, b) { return a + b; }).formattedResult;
}

/**
 * @param {string} format
 * @param {?Array.<string>} substitutions
 * @param {!Object.<string, function(string, ...):string>} formatters
 * @param {!T} initialValue
 * @param {function(T, string): T|undefined} append
 * @return {!{formattedResult: T, unusedSubstitutions: ?Array.<string>}};
 * @template T
 */
String.format = function(format, substitutions, formatters, initialValue, append)
{
    if (!format || !substitutions || !substitutions.length)
        return { formattedResult: append(initialValue, format), unusedSubstitutions: substitutions };

    function prettyFunctionName()
    {
        return "String.format(\"" + format + "\", \"" + substitutions.join("\", \"") + "\")";
    }

    function warn(msg)
    {
        console.warn(prettyFunctionName() + ": " + msg);
    }

    function error(msg)
    {
        console.error(prettyFunctionName() + ": " + msg);
    }

    var result = initialValue;
    var tokens = String.tokenizeFormatString(format, formatters);
    var usedSubstitutionIndexes = {};

    for (var i = 0; i < tokens.length; ++i) {
        var token = tokens[i];

        if (token.type === "string") {
            result = append(result, token.value);
            continue;
        }

        if (token.type !== "specifier") {
            error("Unknown token type \"" + token.type + "\" found.");
            continue;
        }

        if (token.substitutionIndex >= substitutions.length) {
            // If there are not enough substitutions for the current substitutionIndex
            // just output the format specifier literally and move on.
            error("not enough substitution arguments. Had " + substitutions.length + " but needed " + (token.substitutionIndex + 1) + ", so substitution was skipped.");
            result = append(result, "%" + (token.precision > -1 ? token.precision : "") + token.specifier);
            continue;
        }

        usedSubstitutionIndexes[token.substitutionIndex] = true;

        if (!(token.specifier in formatters)) {
            // Encountered an unsupported format character, treat as a string.
            warn("unsupported format character \u201C" + token.specifier + "\u201D. Treating as a string.");
            result = append(result, substitutions[token.substitutionIndex]);
            continue;
        }

        result = append(result, formatters[token.specifier](substitutions[token.substitutionIndex], token));
    }

    var unusedSubstitutions = [];
    for (var i = 0; i < substitutions.length; ++i) {
        if (i in usedSubstitutionIndexes)
            continue;
        unusedSubstitutions.push(substitutions[i]);
    }

    return { formattedResult: result, unusedSubstitutions: unusedSubstitutions };
}

/**
 * @param {string} query
 * @param {boolean} caseSensitive
 * @param {boolean} isRegex
 * @return {!RegExp}
 */
function createSearchRegex(query, caseSensitive, isRegex)
{
    var regexFlags = caseSensitive ? "g" : "gi";
    var regexObject;

    if (isRegex) {
        try {
            regexObject = new RegExp(query, regexFlags);
        } catch (e) {
            // Silent catch.
        }
    }

    if (!regexObject)
        regexObject = createPlainTextSearchRegex(query, regexFlags);

    return regexObject;
}

/**
 * @param {string} query
 * @param {string=} flags
 * @return {!RegExp}
 */
function createPlainTextSearchRegex(query, flags)
{
    // This should be kept the same as the one in ContentSearchUtils.cpp.
    var regexSpecialCharacters = String.regexSpecialCharacters();
    var regex = "";
    for (var i = 0; i < query.length; ++i) {
        var c = query.charAt(i);
        if (regexSpecialCharacters.indexOf(c) != -1)
            regex += "\\";
        regex += c;
    }
    return new RegExp(regex, flags || "");
}

/**
 * @param {!RegExp} regex
 * @param {string} content
 * @return {number}
 */
function countRegexMatches(regex, content)
{
    var text = content;
    var result = 0;
    var match;
    while (text && (match = regex.exec(text))) {
        if (match[0].length > 0)
            ++result;
        text = text.substring(match.index + 1);
    }
    return result;
}

/**
 * @param {number} value
 * @param {number} symbolsCount
 * @return {string}
 */
function numberToStringWithSpacesPadding(value, symbolsCount)
{
    var numberString = value.toString();
    var paddingLength = Math.max(0, symbolsCount - numberString.length);
    var paddingString = Array(paddingLength + 1).join("\u00a0");
    return paddingString + numberString;
}

/**
 * @return {string}
 */
var createObjectIdentifier = function()
{
    // It has to be string for better performance.
    return "_" + ++createObjectIdentifier._last;
}

createObjectIdentifier._last = 0;

/**
 * @constructor
 * @template T
 */
var Set = function()
{
    /** @type {!Object.<string, !T>} */
    this._set = {};
    this._size = 0;
}

/**
 * @param {!Array.<!T>} array
 * @return {!Set.<T>}
 * @template T
 */
Set.fromArray = function(array)
{
    var result = new Set();
    array.forEach(function(item) { result.add(item); });
    return result;
}

Set.prototype = {
    /**
     * @param {!T} item
     */
    add: function(item)
    {
        var objectIdentifier = item.__identifier;
        if (!objectIdentifier) {
            objectIdentifier = createObjectIdentifier();
            item.__identifier = objectIdentifier;
        }
        if (!this._set[objectIdentifier])
            ++this._size;
        this._set[objectIdentifier] = item;
    },

    /**
     * @param {!T} item
     * @return {boolean}
     */
    remove: function(item)
    {
        if (this._set[item.__identifier]) {
            --this._size;
            delete this._set[item.__identifier];
            return true;
        }
        return false;
    },

    /**
     * @return {!Array.<!T>}
     */
    values: function()
    {
        var result = new Array(this._size);
        var i = 0;
        for (var objectIdentifier in this._set)
            result[i++] = this._set[objectIdentifier];
        return result;
    },

    /**
     * @param {!T} item
     * @return {boolean}
     */
    contains: function(item)
    {
        return !!this._set[item.__identifier];
    },

    /**
     * @return {number}
     */
    size: function()
    {
        return this._size;
    },

    clear: function()
    {
        this._set = {};
        this._size = 0;
    }
}

/**
 * @constructor
 * @template K,V
 */
var Map = function()
{
    /** @type {!Object.<string, !Array.<K|V>>} */
    this._map = {};
    this._size = 0;
}

Map.prototype = {
    /**
     * @param {K} key
     * @param {V} value
     */
    put: function(key, value)
    {
        var objectIdentifier = key.__identifier;
        if (!objectIdentifier) {
            objectIdentifier = createObjectIdentifier();
            key.__identifier = objectIdentifier;
        }
        if (!this._map[objectIdentifier])
            ++this._size;
        this._map[objectIdentifier] = [key, value];
    },

    /**
     * @param {K} key
     * @return {V}
     */
    remove: function(key)
    {
        var result = this._map[key.__identifier];
        if (!result)
            return undefined;
        --this._size;
        delete this._map[key.__identifier];
        return result[1];
    },

    /**
     * @return {!Array.<K>}
     */
    keys: function()
    {
        return this._list(0);
    },

    /**
     * @return {!Array.<V>}
     */
    values: function()
    {
        return this._list(1);
    },

    /**
     * @param {number} index
     * @return {!Array.<K|V>}
     */
    _list: function(index)
    {
        var result = new Array(this._size);
        var i = 0;
        for (var objectIdentifier in this._map)
            result[i++] = this._map[objectIdentifier][index];
        return result;
    },

    /**
     * @param {K} key
     * @return {V|undefined}
     */
    get: function(key)
    {
        var entry = this._map[key.__identifier];
        return entry ? entry[1] : undefined;
    },

    /**
     * @param {K} key
     * @return {boolean}
     */
    contains: function(key)
    {
        var entry = this._map[key.__identifier];
        return !!entry;
    },

    /**
     * @return {number}
     */
    size: function()
    {
        return this._size;
    },

    clear: function()
    {
        this._map = {};
        this._size = 0;
    }
}

/**
 * @constructor
 * @template T
 */
var StringMap = function()
{
    /** @type {!Object.<string, T>} */
    this._map = {};
    this._size = 0;
}

StringMap.prototype = {
    /**
     * @param {string} key
     * @param {T} value
     */
    put: function(key, value)
    {
        if (key === "__proto__") {
            if (!this._hasProtoKey) {
                ++this._size;
                this._hasProtoKey = true;
            }
            /** @type {T} */
            this._protoValue = value;
            return;
        }
        if (!Object.prototype.hasOwnProperty.call(this._map, key))
            ++this._size;
        this._map[key] = value;
    },

    /**
     * @param {string} key
     * @return {T|undefined}
     */
    remove: function(key)
    {
        var result;
        if (key === "__proto__") {
            if (!this._hasProtoKey)
                return undefined;
            --this._size;
            delete this._hasProtoKey;
            result = this._protoValue;
            delete this._protoValue;
            return result;
        }
        if (!Object.prototype.hasOwnProperty.call(this._map, key))
            return undefined;
        --this._size;
        result = this._map[key];
        delete this._map[key];
        return result;
    },

    /**
     * @return {!Array.<string>}
     */
    keys: function()
    {
        var result = Object.keys(this._map) || [];
        if (this._hasProtoKey)
            result.push("__proto__");
        return result;
    },

    /**
     * @return {!Array.<T>}
     */
    values: function()
    {
        var result = Object.values(this._map);
        if (this._hasProtoKey)
            result.push(this._protoValue);
        return result;
    },

    /**
     * @param {string} key
     * @return {T|undefined}
     */
    get: function(key)
    {
        if (key === "__proto__")
            return this._protoValue;
        if (!Object.prototype.hasOwnProperty.call(this._map, key))
            return undefined;
        return this._map[key];
    },

    /**
     * @param {string} key
     * @return {boolean}
     */
    contains: function(key)
    {
        var result;
        if (key === "__proto__")
            return this._hasProtoKey;
        return Object.prototype.hasOwnProperty.call(this._map, key);
    },

    /**
     * @return {number}
     */
    size: function()
    {
        return this._size;
    },

    clear: function()
    {
        this._map = {};
        this._size = 0;
        delete this._hasProtoKey;
        delete this._protoValue;
    }
}

/**
 * @constructor
 * @extends {StringMap.<Set.<!T>>}
 * @template T
 */
var StringMultimap = function()
{
    StringMap.call(this);
}

StringMultimap.prototype = {
    /**
     * @param {string} key
     * @param {T} value
     */
    put: function(key, value)
    {
        if (key === "__proto__") {
            if (!this._hasProtoKey) {
                ++this._size;
                this._hasProtoKey = true;
                /** @type {!Set.<T>} */
                this._protoValue = new Set();
            }
            this._protoValue.add(value);
            return;
        }
        if (!Object.prototype.hasOwnProperty.call(this._map, key)) {
            ++this._size;
            this._map[key] = new Set();
        }
        this._map[key].add(value);
    },

    /**
     * @param {string} key
     * @return {!Set.<!T>}
     */
    get: function(key)
    {
        var result = StringMap.prototype.get.call(this, key);
        if (!result)
            result = new Set();
        return result;
    },

    /**
     * @param {string} key
     * @param {T} value
     */
    remove: function(key, value)
    {
        var values = this.get(key);
        values.remove(value);
        if (!values.size())
            StringMap.prototype.remove.call(this, key)
    },

    /**
     * @param {string} key
     */
    removeAll: function(key)
    {
        StringMap.prototype.remove.call(this, key);
    },

    /**
     * @return {!Array.<!T>}
     */
    values: function()
    {
        var result = [];
        var keys = this.keys();
        for (var i = 0; i < keys.length; ++i)
            result.pushAll(this.get(keys[i]).values());
        return result;
    },

    __proto__: StringMap.prototype
}

/**
 * @constructor
 */
var StringSet = function()
{
    /** @type {!StringMap.<boolean>} */
    this._map = new StringMap();
}

/**
 * @param {!Array.<string>} array
 * @return {!StringSet}
 */
StringSet.fromArray = function(array)
{
    var result = new StringSet();
    array.forEach(function(item) { result.add(item); });
    return result;
}

StringSet.prototype = {
    /**
     * @param {string} value
     */
    add: function(value)
    {
        this._map.put(value, true);
    },

    /**
     * @param {string} value
     * @return {boolean}
     */
    remove: function(value)
    {
        return !!this._map.remove(value);
    },

    /**
     * @return {!Array.<string>}
     */
    values: function()
    {
        return this._map.keys();
    },

    /**
     * @param {string} value
     * @return {boolean}
     */
    contains: function(value)
    {
        return this._map.contains(value);
    },

    /**
     * @return {number}
     */
    size: function()
    {
        return this._map.size();
    },

    clear: function()
    {
        this._map.clear();
    }
}

/**
 * @param {string} url
 * @param {boolean=} async
 * @param {function(?string)=} callback
 * @return {?string}
 */
function loadXHR(url, async, callback)
{
    function onReadyStateChanged()
    {
        if (xhr.readyState !== XMLHttpRequest.DONE)
            return;

        if (xhr.status === 200) {
            callback(xhr.responseText);
            return;
        }

        callback(null);
   }

    var xhr = new XMLHttpRequest();
    xhr.open("GET", url, async);
    if (async)
        xhr.onreadystatechange = onReadyStateChanged;
    xhr.send(null);

    if (!async) {
        if (xhr.status === 200)
            return xhr.responseText;
        return null;
    }
    return null;
}

/**
 * @constructor
 */
function CallbackBarrier()
{
    this._pendingIncomingCallbacksCount = 0;
}

CallbackBarrier.prototype = {
    /**
     * @param {function(...)=} userCallback
     * @return {function(...)}
     */
    createCallback: function(userCallback)
    {
        console.assert(!this._outgoingCallback, "CallbackBarrier.createCallback() is called after CallbackBarrier.callWhenDone()");
        ++this._pendingIncomingCallbacksCount;
        return this._incomingCallback.bind(this, userCallback);
    },

    /**
     * @param {function()} callback
     */
    callWhenDone: function(callback)
    {
        console.assert(!this._outgoingCallback, "CallbackBarrier.callWhenDone() is called multiple times");
        this._outgoingCallback = callback;
        if (!this._pendingIncomingCallbacksCount)
            this._outgoingCallback();
    },

    /**
     * @param {function(...)=} userCallback
     */
    _incomingCallback: function(userCallback)
    {
        console.assert(this._pendingIncomingCallbacksCount > 0);
        if (userCallback) {
            var args = Array.prototype.slice.call(arguments, 1);
            userCallback.apply(null, args);
        }
        if (!--this._pendingIncomingCallbacksCount && this._outgoingCallback)
            this._outgoingCallback();
    }
}

/**
 * @param {*} value
 */
function suppressUnused(value)
{
}

/**
 * @constructor
 * @param {!T} targetObject
 * @template T
 */
function WeakReference(targetObject)
{
    this._targetObject = targetObject;
}

WeakReference.prototype = {
    /**
     * @return {?T}
     */
    get: function()
    {
        return this._targetObject;
    },

    clear: function()
    {
        this._targetObject = null;
    }
};

/**
 * @param {function()} callback
 */
self.setImmediate = (function() {
    var callbacks = [];
    function run() {
        var cbList = callbacks.slice();
        callbacks.length = 0;
        cbList.forEach(function(callback) { callback(); });
    };
    return function setImmediate(callback) {
        if (!callbacks.length)
            new Promise(function(resolve,reject){ resolve(null);}).then(run);
        callbacks.push(callback);
    };
})();
