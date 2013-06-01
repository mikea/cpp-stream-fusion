cpp-stream-fusion
=================

Stream-fusion based vector implementation in c++.
Inspired by: https://research.microsoft.com/en-us/um/people/simonpj/papers/ndp/haskell-beats-C.pdf

This is just an exercise in C++ and attempt to implement fusion-based vectors in
effective way. Main idea is to make ToStream(FromStream()) automatically to
compile into no-op => no temp allocation.

todo
=================
- implement memory management. Right now I do not delete data at all.
- implement more vector operations and some algorithms
- add tests which will ensure that no temp allocation happens
