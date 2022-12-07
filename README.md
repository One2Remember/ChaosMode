# ChaosMode
A package that can forcibly introduce non-deterministic behavior to Non-Deterministic C++ STL data structures for debugging flaky tests, loosely inspired by [Mozilla ChaosMode](https://hg.mozilla.org/mozilla-central/file/tip/mfbt/ChaosMode.h) and the [NonDex](https://github.com/TestingResearchIllinois/NonDex) tool.

### Prior Work
This project was made as an attempt to construct a C++ equivalent to the [NonDex](https://github.com/TestingResearchIllinois/NonDex) flaky-test debugging tool for projects using Java JDK8. To understand the importance of this work, please check out the README on the original NonDex repo. 

### Basic Usage
This code gives the user access to chaotic versions of the C++ Standard Template Library [std::unordered_set](https://cplusplus.com/reference/unordered_set/unordered_set/) and [std::unordered_map](https://cplusplus.com/reference/unordered_map/unordered_map/). In the makefile provided, a flag called chaos_mode can be set to either 0 or 1. 

If the flag is set to 1, any instance of an UnorderedMap or UnorderedSet will compile as a chaotic version of the original class that will shuffle the underlying hash table whenever an element is added to the data structure. This effectively forces the 'unordered' property of the structures, so that any tests throughout the project that may accidentally be relying on an assumption of some kind of ordering are practically guaranteed to fail, making these bugs trivial to hunt down. 

If the flag is not set to 1, the UnorderedMap and UnorderedSet classes will simply compile as aliases to the C++ STL base versions, and any template parameters are passed down to the base class with experimentally negligible overhead.

### Integrating Code into Larger Projects
Integrating this code into your project is a simple two-step process.
1) Clone the repository to your desired location, navigate to the project directory, and run 'make'. 'make clean' will clean the directory as expected, leaving only the source code and README. For projects that use gmake, or another build pattern, integrate the makefile however you normally would. If this is being integrated into a larger makefile, chaos_mode should be compiled before the rest of the project files are compiled.
2) Refactor the project to replace any usages of std::unordered_map with UnorderedMap and std::unordered_set with UnorderedSet. The class interfaces are identical, so no other changes are required aside from the #include statements.
3) Swap any #include<unordered_x> with #include "[path_to_chaos_mode]/UnorderedX"

Note that this will not change any unordered_maps/sets being used in other external library dependencies. You must either assume they do not contain flaky behavior, or similarly refactor them as well and recompile them from source. If you do refactor those and build them from source, and discover some flaky behavior, it would be good form to open a PR against the original library letting them know about the flaky behavior (: If you do, shoot me a message, too, so I know this tool was helpful!

### Dependencies
This code relies on a C++ compiler with features supported up to C++11. If methods such as insert_or_assign() (introduced in C++17 standard) are used, they will not shuffle the map on insertion, and if no other C++11-based insertion methods are used, the map will never get shuffled and the non-deterministic behavior will not be discovered.
