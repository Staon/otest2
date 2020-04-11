---
title: Control Functions
permalink: /reference/controls/
toc: true
---

## Switching of Test States

```c++
void switchState(void (*state_)(), int delay_);
```

Schedule next `state_` run in the current test case. The `delay_` is time
in milliseconds for what the process stays in the main loop.

Usage of test states is explained in the 
[custom main loop]({{ "/examples/main-loop/" | relative_url }}) example. 
