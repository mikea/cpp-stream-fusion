cpp-stream-fusion
=================

Stream-fusion based vector implementation in c++.
Inspired by: https://research.microsoft.com/en-us/um/people/simonpj/papers/ndp/haskell-beats-C.pdf

This is just an exercise in C++ and attempt to implement fusion-based vectors in
effective way. Main idea is to make ToStream(FromStream()) automatically to
compile into no-op => no temp allocation.

todo
=================
- use lambdas for operations
- restructure code for easier reading
- implement more vector operations and some algorithms.
- benchmark code to insure that no performance degradation occurs
