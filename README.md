Chibitronics Test Jig
=====================

This software runs in the factory that produces Chibitronics Circuit Stickers.

It talks to the stickers via a custom jig.  Basically, it bit-bangs TWI over a
kind of SPI system, with an extra pin that's responsible for controling TWI direction.

See also https://github.com/xobs/avrdude-chibitronics

It is extra fancy, and uses QML for displaying data, signals for driving
states, and it runs as a non-privileged user.


Presentation / View
-------------------

Presentation (or the View layer) is handled by QML.  The primary QML file is
located in qml/chibitronics-jig/main.qml.  There is a "mode" property that
can be set to either "sensor" or "effects", which will choose which of the two
stickers it will try to program/test.


Test Sequence
-------------

The actual test sequence is driven by chibisequence.cpp.  The constructor
populates two lists with a series of steps.  Depending on the mode, one of
these steps will be executed.

If a step fails, the test will continue.  This is part of a drive to test
all aspects of the system fully.


Tests
-----

All tests derive from the ChibiTest class.  Tests are run in their own thread,
and should not call any graphics functions on the main thread.  Tests can
block, and indeed ChibiTest contains an msleep() call that tests can use.

When your test is invoked, the runTest() method is called.  As this is in
its own thread, you do not need to worry about being particularly responsive
in returning control to the main thread.

Tests pass messages to the main thread by emitting the testMessage() signal.
Convenience functions have been created in ChibiTest for testInfo(),
testDebug(), and testError(), each of which accepts a single QString.  To give
progress on your test, for example, you might call:

    testInfo("Reticulating splines...");

This would pass a test message to the main test infrastructure, which would
annotate the test with more information.  You should call testInfo() when
your test is long-running, to let the tester know the board has not locked up.

Info messages produced by testInfo() are logged.  Error messages produced by
testError() are logged as well, and will appear in big red letters.  Debug
messages produced by testDebug() are not logged, and are simply printed out
to the console, which normally is not visible.

If a test fails, call testError() and pass it a description of the error.
If you return from runTest() without calling testError(), it is assume that
the test has succeeded.


Keyboard Interface
------------------

If you plug in a keyboard, you can manipulate some functions for test.

| Key | Function  |
| --- | --------- |
|  c  | Bring up the "camera calibration" screen.  Press 'c' again to dismiss it. |
|  s  | Start a test.  Equivalent to clicking the mouse, or shorting the "start" pins. |
|  l  | Switch to "LtC" mode, ignoring the ltc/dataviewer strapping pin. |
|  d  | Switch to "Data viewer" mode, ignoring the ltc/dataviewer strapping pin. |
| ESC | Quit the program. If running on real hardware, the watchdog will start it again. |
