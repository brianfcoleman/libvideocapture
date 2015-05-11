Build instructions
I compiled this library using Visual Studio 2008

1. Download boost. I used version 1.44.0

2. Download SDL. I used version 1.2.14

3. Extract boost and SDL in the same directory as the libvideocapture repository
> i.e. you should have a folder structure like this
> a\_path\_to\_a\_directory\libvideocapture
> a\_path\_to\_a\_directory\SDL-1.2.14
> a\_path\_to\_a\_directory\boost\_1\_44\_0
4. Copy libvideocapture\VisualC to SDL-1.2.14\VisualC
> This allows building a version of SDL which is linked against the static c
> runtime.
5. To build the test application open a visual studio command prompt
6. cd to a\_path\_to\_a\_directory\libvideocapture
7. run nmake test
> This should build boost and sdl first and then the test application
8. The test application also invokes nmake videocapture\_static
> This leaves libvideocapture\lib\libvideocapture\_static.lib
> This is a lib which you can link against when compiling against the source
> files in libvideocapture\src\videocapture. It is linked against the static
> c runtime.
> If you want a libvideocapture compiled against the dynamic c runtime
> then run nmake videocapture
9. The test application also invokes nmake sdlmanager\_static
> This leaves libvideocapture\lib\libsdlmanager\_static.lib
> This is a lib which you can link against when compiling against the source
> files in libvideocapture\src\sdl. It is linked against the static
> c runtime.
> If you want a libsdlmanager compiled against the dynamic c runtime
> then run nmake sdlmanager
10. The files in libvideocapture\src\videocapture provide an interface to
> the system's video capture devices such as webcams using direct show
11.The files in libvideocapture\src\sdl provide a means to use sdl to
> render the output of video capture devices. The sdl objects must run on
> the main thread of the application.
12.The test application provides a way to create a multithreaded media processing
> pipeline which takes the output of a video device, rotates it if it is
> upside down and renders it to a window.
13.VideoCaptureDeviceManager gives a list of VideoCaptureDevice's. It manages
> the devices and cleans up after them.
> A VideoCaptureDevice has a list of supported RGBVideoFormat's which can be set.
> Call startCapturing to start capturing.
14.If you want to use different names for the boost and SDL directories,
> these can be changed in the Makefile. Just change BOOST\_ROOT and/or SDL\_ROOT