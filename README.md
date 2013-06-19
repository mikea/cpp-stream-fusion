cpp-stream-fusion
=================

Stream-fusion based vector implementation in c++.
Inspired by: https://research.microsoft.com/en-us/um/people/simonpj/papers/ndp/haskell-beats-C.pdf

This is just an exercise in C++ and attempt to implement fusion-based vectors in
effective way. Main idea is to make ToStream(FromStream()) automatically to
compile into no-op => no temp allocation.

todo
=================
- implement memory management. Right now the test fails with g++ but passes (strangely) with clang on mac.
- implement more vector operations and some algorithms.
- add tests which will ensure that no temp allocation happens. some sort of expectation checker.
- benchmark code to insure that no performance degradation occurs
