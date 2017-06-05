///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Pipe_H
#define SCY_Pipe_H


#include "scy/base.h"
#include "scy/stream.h"


namespace scy {


/// Pipe implementation for process stdio
class Base_API Pipe : public Stream<uv_pipe_t>
{
public:
    Pipe(uv::Loop* loop = uv::defaultLoop());
    virtual ~Pipe();

    virtual void init(bool ipc = false);

    virtual bool readStart() override;
    //virtual bool readStop() override;
};


} // namespace scy


#endif // SCY_Stream_H


/// @\}
