level.h
  1. The "even" log is in debug, I'm not sure if it's on purpose. If it is, we are not going to see "even" when we set the log level to info in main.cpp
  2. The g_level keeps increasing, which will be overflowed if the program runs long enough. I change g_level++ to mod 1000000007 to avoid this situation.
  3. The two threads are changing the same non thread safe variable: g_level. I merge the two threads together.
  4. The two threads are running unrelatively, so it's not going to show logs like "even" -> "odd" -> "even". Not sure if that's the purpose of this class or not. If that's the purpose, we should an even number like 1000000006 not 1000000006. Otherwise, 1000000006 prints "even" and the next one will also print "even".
  5. We should never delete the pointer of a private variable of the class in the deconstrution function. It's going to be destoryed automatically.
  6. We should alwasy use spdlog::info("{}", msg) instead of printing the msg directly. Otherwise, the "{}" in msg will be a problem.