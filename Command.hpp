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

#include "GalactiQuestBase.hpp"

namespace gquest {

    enum class CommandType : ui64 {
        None,
        Wait,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        MoveLeftUp,
        MoveLeftDown,
        MoveRightUp,
        MoveRightDown,
        LivelySplatter_Spawn,
        LivelySplatter_Move,
        DEBUG_BecomeSmiley,
        DEBUG_BecomePlayer,
    };

    class Command {
    private:
        uint_ _when;
        CommandType _command;
        list<any> _args;
    public:
        Command() = delete;

        Command(uint_ when, CommandType command) : _when(when), _command(command), _args{ } { }

        template <class ...AnyTypes>
        Command(uint_ when, CommandType command, AnyTypes... args) : _when(when), _command(command) {
            _args = {
                args...
            };
            //addArgs(args...);
        }

        Command(Command const& command) : _when(command._when), _command(command._command), _args(command._args) { }
        Command(Command && command) : _when(command._when), _command(command._command), _args(command._args) { }

        inline Command & operator =(Command const& command) {
            _when = command._when;
            _command = command._command;
            _args = command._args;
            return *this;
        }

        inline Command & operator =(Command && command) {
            _when = command._when;
            _command = command._command;
            _args = command._args;
            return *this;
        }

        friend bool operator <(Command const& lhs, Command const& rhs);
        friend bool operator >(Command const& lhs, Command const& rhs);
        friend bool operator <=(Command const& lhs, Command const& rhs);
        friend bool operator >=(Command const& lhs, Command const& rhs);

        inline uint_ GetWhen() const { return _when; }
        inline CommandType GetCommandType() const { return _command; }
        inline list<any> const& GetArguments() const { return _args; }
    };

    inline bool operator <(Command const& lhs, Command const& rhs) {
        return lhs._when < rhs._when;
    }
    inline bool operator >(Command const& lhs, Command const& rhs) {
        return lhs._when > rhs._when;
    }
    inline bool operator <=(Command const& lhs, Command const& rhs) {
        return lhs._when <= rhs._when;
    }
    inline bool operator >=(Command const& lhs, Command const& rhs) {
        return lhs._when >= rhs._when;
    }

}
