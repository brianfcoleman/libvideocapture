Provides a C++ wrapper around DirectShow on windows to allow
capturing of video from webcams.
Also provides a test application which uses SDL to display
the video frames.
A multithreaded media sample processing pipeline is used to
connect the video frames to the SDL surface that renders them
to the screen.