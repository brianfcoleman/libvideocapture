SOURCES=*.cpp
HEADERS=..\..\boost
OBJECTS=*.obj
TARGET=Test.exe
CL=cl
CLFLAGS=/c /EHsc /I$(HEADERS)
LINK=link
LINKFLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib strmiids.lib

.cpp.obj:
  @echo Compiling
  $(CL) $(CLFLAGS) $<  

$(OBJECTS):$(SOURCE)

test:$(OBJECTS)
  @echo Linking
  link /out:$(TARGET) $(LINKFLAGS) $**

clean:
  del $(TARGET)
  del $(OBJECTS)
  cls