# TODO

Refactor Timer to use async pattern
Add move arguments to packetstream for inline packet args
Describe how to build WebRTC for use with LibSourcey in README
Improve av module documentation
Use std::async for Runnables classes and forward arguments as per constructor in http://en.cppreference.com/w/cpp/thread/thread/thread

template <typename Callable, typename... Args>
Thread(Callable&& f, Args&&... args)
    : exceptionPtr(nullptr)
    , thread([] (typename std::decay<Callable>::type&& f
               , typename std::decay<Args>::type&&... args)
            {
                // (...)
            }
            , std::forward<Callable>(f), std::forward<Args>(args)...)
{
    // (...)
}


# Tests

Test fixtures should be installed relative to compiled test binaries
Integrate TURN tests with `make check`
Video encoder test changing input resolution


# Maybe

Replace zlib with libarchive for better archive support
