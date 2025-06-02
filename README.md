### 🍪 Crumble 🍪
Aggressively gcc+*nix C macros, modules, and tools. 

### 🛠️ Features 🛠️
1. optimized-away runtime asserts.
2. overloaded macros for variadic behavior.

Dedicated Modules:
1. `common.h`, common and miscellaneous macros of non-template libraries.
2. `test.c|h`, ASAN&LSAN testing for other modules.
3. `log.c|h`, rich logging with colors on supported terminals.
4. `prandom.c|h`, pRNG via xoshiro256++ 1.0 optionally seeded with splitmix64.

### 🏗️ Project Structure 🏗️
```ruby
root
├ build # Ignore if not building for local testing. 
│ ├ ...  # Arcane personal testing-executable build scripts.
│ └ test # Local testing executable (only `test.h::test_crumble`).
└ crumble # The actual library.
  ├ common.h
  ├ test.c|h
  ├ log.c|h
  └ prandom.c|h
```
