level.h
  1. The "even" log is in debug, I'm not sure if it's on purpose. If it is, we are not going to see "even" when we set the log level to info in main.cpp
  2. The g_level keeps increasing, which will be overflowed if the program runs long enough. I change g_level++ to mod 1000000007 to avoid this situation.
  3. The two threads are changing the same non thread safe variable: g_level. I merge the two threads together.
  4. The two threads are running unrelatively, so it's not going to show logs like "even" -> "odd" -> "even". Not sure if that's the purpose of this class or not. If that's the purpose, we should an even number like 1000000006 not 1000000006. Otherwise, 1000000006 prints "even" and the next one will also print "even".
  5. We should never delete the pointer of a private variable of the class in the deconstrution function. It's going to be destoryed automatically.
  6. We should alwasy use spdlog::info("{}", msg) instead of printing the msg directly. Otherwise, the "{}" in msg will be a problem.

string_uaf.h
  1. It only passes the pointer of the tmp string to the spdlog. But the memory of that address might be freed before spdlog print it. I use the owning type std::string instead of the pointer of the string to print. It may takes some time rather than format the string later, but it's safer.
  2. std::rand() may not thread safe.

shutdown.h
  1. It is dangerious to cache the pointer of the logger, it may change after caching it. I use the owning variable instead of the pointer to make it safer.
  2. It is not appropriate to shutdown the logger in this class. We should just set the stop to true, which could gentally shutdown other thread and make them take care of the shutting down. Not just call the shutdown, it will lose some logs in other threads. 

timebase.h
  1. t1 and t2 were not the same clock, t1 is steady_clock and t2 was system_lock, I change t2 to steady_clock to make them the same.
  2. It may loose accurcy if we convert the steady_clock to count first then do the subtraction. I make them do the subtraction first then converting them to the count.