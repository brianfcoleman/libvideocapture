HEADER_DIR=..\boost
LIBRARIES=kernel32.lib \
  user32.lib \
  gdi32.lib \
  winspool.lib \
  comdlg32.lib \
  advapi32.lib \
  shell32.lib \
  ole32.lib \
  oleaut32.lib \
  uuid.lib \
  odbc32.lib \
  odbccp32.lib \
  strmiids.lib
CPP_EXT=cpp
OBJECT_EXT=obj
EXECUTABLE_EXT=exe
SOURCE_DIR=src
OBJECT_DIR=build
TARGET_DIR=bin
TARGET=$(TARGET_DIR)\Test.$(EXECUTABLE_EXT)
COMPILER=cl
COMPILER_OPTIONS=/nologo /c /EHsc
LINKER=link
LINKER_OPTIONS=/nologo
DELETE=del
LIST_OBJECT_FILES=ListObjectFiles.bat
OBJECT_FILE_LIST=ObjectFilesList.txt

!IF [$(LIST_OBJECT_FILES) $(OBJECT_FILE_LIST) $(SOURCE_DIR) $(OBJECT_DIR) $(CPP_EXT) $(OBJECT_EXT)]
!INCLUDE $(OBJECT_FILE_LIST)
!ENDIF

{$(SOURCE_DIR)}.$(CPP_EXT){$(OBJECT_DIR)}.$(OBJECT_EXT):
  $(COMPILER) $(COMPILER_OPTIONS) /I$(HEADER_DIR) /Fo$(OBJECT_DIR)\ $<

$(TARGET):$(OBJECT_FILES)
  $(LINKER) $(LINKER_OPTIONS) /out:$@ $(LIBRARIES) $**

test:$(TARGET)

clean:
  $(DELETE) $(OBJECT_DIR)\*.$(OBJECT_EXT)
  $(DELETE) $(TARGET_DIR)\*.$(EXECUTABLE_EXT)