@echo off
set BOOST_ROOT=%1
set BOOST_BUILD=%BOOST_ROOT%\build
set BOOST_STAGE=.
cd %BOOST_ROOT%
if not exist %BOOST_BUILD% mkdir %BOOST_BUILD%
bjam --build-dir=%BOOST_BUILD% --stagedir=%BOOST_STAGE% toolset=msvc --build-type=complete --with-date_time --with-thread stage
