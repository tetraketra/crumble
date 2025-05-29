### 🍪 Crumble 🍪
Aggressively gcc+*nix C macros, modules, and tools. 

### 🏗️ Project Structure 🏗️
```ruby
root
├ build # Ignore if not building for local testing. 
│ ├ ...  # Arcane personal testing-executable build scripts.
│ └ test # Local testing executable (only `test.h::test_crumble`).
└ crumble # The actual library.
  ├ common.h    # Common Crumble libraries and macros.
  ├ log.c|h     # Rich logging with console-aware colors.
  ├ prandom.c|h # pRNG via xoshiro256++ 1.0, optionally seeded by splitmix64.
  └ test.c|h    # Testing of other Crumble components. Requires ASAN+LSAN.
```
