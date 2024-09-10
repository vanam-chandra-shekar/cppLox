INCLUDEDIR = Lox Scanner generated_ast Parser AstPrinter Interpreter Environment
SOURCEDIR = . Lox Scanner generated_ast Parser Interpreter
LIBDIR = 
BINDIR = bin
LANGUAGE = cpp
PREPROCESSER = WINDOWS 
LIBRARIES = 

TARGET = main

CXX = g++

INCLUDEDIR := $(foreach D,$(INCLUDEDIR),$(patsubst %,-I%,$(D)))
LIBDIR := $(foreach D,$(LIBDIR),$(patsubst %,-L%,$(D)))
PREPROCESSER := $(foreach D,$(PREPROCESSER),$(patsubst %,-D%,$(D)))
LIBRARIES := $(foreach D,$(LIBRARIES),$(patsubst %,-l%,$(D)))
SOURCEFILES := $(foreach D,$(SOURCEDIR),$(wildcard $(D)/*.$(LANGUAGE)))
OBJFILES := $(patsubst %.$(LANGUAGE),%.o,$(foreach D,$(SOURCEFILES),$(patsubst $(dir $(D))%,$(BINDIR)/%,$(D))))
DEPFILES := $(patsubst %.o,%.d,$(OBJFILES))

ifndef (CONFIG)
	CONFIG = DEBUG
endif

ifndef (VERBOS)
	SILENT = @
endif

ifeq ($(CONFIG),DEBUG)
	DEPFLAGS = -MP -MD
	CXXFLAGS = $(INCLUDEDIR) $(DEPFLAGS) -g $(PREPROCESSER)
endif

ifeq ($(CONFIG),NDEBUG)
	DEPFLAGS = -MP -MD
	OPT = O2
	CXXFLAGS = $(INCLUDEDIR) $(DEPFLAGS) $(OPT) $(PREPROCESSER)
endif

LINKFLG = $(LIBDIR) $(LIBRARIES)

VPATH = $(SOURCEDIR)

.PHONY: all clean run runScript genAst

all:$(TARGET)

$(TARGET):$(OBJFILES)
	$(SILENT)echo ===== Linking =====
	$(SILENT)$(CXX) $^ -o $@ $(CXXFLAGS)


$(BINDIR)/%.o:%.$(LANGUAGE) | $(BINDIR)
	$(SILENT) echo [Compiling $<]
	$(SILENT)$(CXX)  -c $< -o $@ $(CXXFLAGS)

$(BINDIR):
	$(SILENT)echo Creating $@ Dir
	$(SILENT)mkdir $@

run: all
	$(SILENT).\$(TARGET)

runScript: all script.txt
	$(SILENT).\$(TARGET) script.txt

genAst:
	@ python.exe .\Tools\GenerateAst.py

astPrint:
	@ $(CXX) $(CXXFLAGS) AstPrinter/AstPrinter.cpp -o ast

clean:
	$(echo)echo ===== CLEANING =====
	$(SILENT)del /Q $(TARGET).exe
	$(SILENT)del /Q $(BINDIR)


-include $(DEPFILES)