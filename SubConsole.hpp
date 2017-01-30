// The MIT License (MIT)
//
// Copyright (c) 2017 Drew Wibbenmeyer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

#include "IConsole.hpp"

namespace conlib {

    class SubConsole : public IConsole {
    private:
        Attr _curAttr;
        short _width;
        short _height;
        std::vector<CChar> _buffer;
    public:
        SubConsole() = delete;
        SubConsole(short width, short height, Attr attr = Attr::FgWhite);
        // Inherited via IConsole
        virtual ~SubConsole() override;
        virtual CChar GetChar(short x, short y) const override;
        virtual short Width() const override;
        virtual short Height() const override;
        virtual void SetWidth(short width) override;
        virtual void SetHeight(short height) override;
        virtual void Size(short & width, short & height) const override;
        virtual void Resize(short width, short height) override;
        virtual Attr CurrentAttr() const override;
        virtual void SetCurrentAttr(Attr attr) override;
        virtual void SetChar(short x, short y, CChar ch) override;
        virtual void SetChar(short x, short y, wchar_t ch) override;
        virtual void SetChar(short x, short y, wchar_t ch, Attr attr) override;
        virtual void PutString(short x, short y, std::wstring const & str, Attr attr, int max_length = -1) override;
        virtual void PutString(short x, short y, std::wstring const & str, int max_length = -1) override;
        template <class RandomAccessContainerCChar> void PutString(short x, short y, RandomAccessContainerCChar const& str, int max_length = -1);
        virtual void Clear() override;
        virtual void Clear(wchar_t fill_ch) override;
        virtual void Clear(CChar fill_ch) override;
        virtual void Clear(wchar_t fill_ch, Attr attr) override;
        virtual void Box(short x, short y, short width, short height, BoxType box_type = BoxType::Box) override;
        virtual void Box(short x, short y, short width, short height, Attr attr, BoxType box_type = BoxType::Box) override;
        virtual void Box(short x, short y, short width, short height, wchar_t tl, wchar_t t, wchar_t tr, wchar_t l, wchar_t r, wchar_t bl, wchar_t b, wchar_t br) override;
        virtual void Box(short x, short y, short width, short height, Attr attr, wchar_t tl, wchar_t t, wchar_t tr, wchar_t l, wchar_t r, wchar_t bl, wchar_t b, wchar_t br) override;
        virtual void Fill(short x, short y, short width, short height, wchar_t ch, Attr attr) override;
        virtual void Fill(short x, short y, short width, short height, CChar ch) override;
    };

    template <class RandomAccessContainerCChar> void SubConsole::PutString(short x,
        short y, RandomAccessContainerCChar const& str, int max_length) {

        if(max_length < 0) { max_length = (int)std::min((std::size_t)INT32_MAX, str.size()); }
        int start_idx = x + y * _width;
        CChar tmpch;
        for(int i = 0; (i < str.size()) && (i < max_length) && (i + start_idx < _buffer.size()); ++i) {
            tmpch = str[i];
            if(!Equal(_buffer[i + start_idx], tmpch)) {
                _buffer[i + start_idx] = tmpch;
            }
        }
    }

}
