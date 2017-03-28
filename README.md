# humanize

humanize header-only lib for C++

## usage

### Compact integers
```cpp
humanize::compactInteger(5000); // "5k"
humanize::compactInteger(5500); // "5k"
humanize::compactInteger(5999); // "5k"
humanize::compactInteger(5999, 1); // "5.9k"
```
