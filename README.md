# humanize

humanize header-only lib for C++

## usage

### Numbers

#### Compact integers
```cpp
humanize::compactInteger(5000); // "5k"
humanize::compactInteger(5500); // "5k"
humanize::compactInteger(5999); // "5k"
humanize::compactInteger(5999, 1); // "5.9k"
```

### Time

#### Relative time
```cpp
auto res = humanize::relativeTime(std::chrono::hours(12));
// res = { str: "12 hours", resultType: Future }

auto res = humanize::relativeTime(std::chrono::system_time::now() + std::chrono::hours(12));
// res = { str: "12 hours", resultType: Future }

auto res = humanize::relativeTime(std::chrono::system_time::now() - std::chrono::hours(12));
// res = { str: "12 hours", resultType: Past }

auto res = humanize::relativeTime(std::chrono::hours(12), humanize::SuffixType::Short);
// res = { str: "12h", resultType: Future }
```

#### Diff time
```cpp
auto res = humanize::diffTime(std::chrono::hours(12));
// res = "12 hours"

auto res = humanize::diffTime(std::chrono::system_time::now() + std::chrono::hours(12));
// res = "12 hours"

auto res = humanize::diffTime(std::chrono::system_time::now() - std::chrono::hours(12));
// res = "12 hours"

auto res = humanize::diffTime(std::chrono::hours(12), humanize::SuffixType::Short);
// res = "12h"
```
