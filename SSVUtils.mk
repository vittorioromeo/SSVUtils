##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=SSVUtils
ConfigurationName      :=Release
WorkspacePath          := "D:\Vee\Software\GitHub\OHWorkspace"
ProjectPath            := "D:\Vee\Software\GitHub\OHWorkspace\SSVUtils"
IntermediateDirectory  :=./_INTERMEDIATE
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=vittorio.romeo
Date                   :=18/03/2013
CodeLitePath           :="C:\Program Files (x86)\CodeLite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=./_RELEASE/$(ProjectName).dll
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="SSVUtils.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=windres
LinkOptions            :=  -O2
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -W -s -pedantic -O3 -Wextra -std=c++11 -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
UNIT_TEST_PP_SRC_DIR:=C:\UnitTest++-1.3
WXWIN:=C:\wxWidgets-2.9.4
WXCFG:=gcc_dll\mswu
Objects0=$(IntermediateDirectory)/FileSystem_FileSystem$(ObjectSuffix) $(IntermediateDirectory)/Log_Log$(ObjectSuffix) $(IntermediateDirectory)/Utils_Utils$(ObjectSuffix) $(IntermediateDirectory)/Utils_UtilsString$(ObjectSuffix) $(IntermediateDirectory)/Utils_UtilsCollections$(ObjectSuffix) $(IntermediateDirectory)/Utils_UtilsMath$(ObjectSuffix) $(IntermediateDirectory)/Timeline_Command$(ObjectSuffix) $(IntermediateDirectory)/Timeline_Do$(ObjectSuffix) $(IntermediateDirectory)/Timeline_Go$(ObjectSuffix) $(IntermediateDirectory)/Timeline_Timeline$(ObjectSuffix) \
	$(IntermediateDirectory)/Timeline_Wait$(ObjectSuffix) 

Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "D:\Vee\Software\GitHub\OHWorkspace/.build-release"
	@echo rebuilt > "D:\Vee\Software\GitHub\OHWorkspace/.build-release/SSVUtils"

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./_INTERMEDIATE"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/FileSystem_FileSystem$(ObjectSuffix): FileSystem/FileSystem.cpp $(IntermediateDirectory)/FileSystem_FileSystem$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Vee/Software/GitHub/OHWorkspace/SSVUtils/FileSystem/FileSystem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileSystem_FileSystem$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileSystem_FileSystem$(DependSuffix): FileSystem/FileSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileSystem_FileSystem$(ObjectSuffix) -MF$(IntermediateDirectory)/FileSystem_FileSystem$(DependSuffix) -MM "FileSystem/FileSystem.cpp"

$(IntermediateDirectory)/FileSystem_FileSystem$(PreprocessSuffix): FileSystem/FileSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileSystem_FileSystem$(PreprocessSuffix) "FileSystem/FileSystem.cpp"

$(IntermediateDirectory)/Log_Log$(ObjectSuffix): Log/Log.cpp $(IntermediateDirectory)/Log_Log$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Vee/Software/GitHub/OHWorkspace/SSVUtils/Log/Log.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Log_Log$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Log_Log$(DependSuffix): Log/Log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Log_Log$(ObjectSuffix) -MF$(IntermediateDirectory)/Log_Log$(DependSuffix) -MM "Log/Log.cpp"

$(IntermediateDirectory)/Log_Log$(PreprocessSuffix): Log/Log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Log_Log$(PreprocessSuffix) "Log/Log.cpp"

$(IntermediateDirectory)/Utils_Utils$(ObjectSuffix): Utils/Utils.cpp $(IntermediateDirectory)/Utils_Utils$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Vee/Software/GitHub/OHWorkspace/SSVUtils/Utils/Utils.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Utils_Utils$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Utils_Utils$(DependSuffix): Utils/Utils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Utils_Utils$(ObjectSuffix) -MF$(IntermediateDirectory)/Utils_Utils$(DependSuffix) -MM "Utils/Utils.cpp"

$(IntermediateDirectory)/Utils_Utils$(PreprocessSuffix): Utils/Utils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Utils_Utils$(PreprocessSuffix) "Utils/Utils.cpp"

$(IntermediateDirectory)/Utils_UtilsString$(ObjectSuffix): Utils/UtilsString.cpp $(IntermediateDirectory)/Utils_UtilsString$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Vee/Software/GitHub/OHWorkspace/SSVUtils/Utils/UtilsString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Utils_UtilsString$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Utils_UtilsString$(DependSuffix): Utils/UtilsString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Utils_UtilsString$(ObjectSuffix) -MF$(IntermediateDirectory)/Utils_UtilsString$(DependSuffix) -MM "Utils/UtilsString.cpp"

$(IntermediateDirectory)/Utils_UtilsString$(PreprocessSuffix): Utils/UtilsString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Utils_UtilsString$(PreprocessSuffix) "Utils/UtilsString.cpp"

$(IntermediateDirectory)/Utils_UtilsCollections$(ObjectSuffix): Utils/UtilsCollections.cpp $(IntermediateDirectory)/Utils_UtilsCollections$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Vee/Software/GitHub/OHWorkspace/SSVUtils/Utils/UtilsCollections.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Utils_UtilsCollections$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Utils_UtilsCollections$(DependSuffix): Utils/UtilsCollections.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Utils_UtilsCollections$(ObjectSuffix) -MF$(IntermediateDirectory)/Utils_UtilsCollections$(DependSuffix) -MM "Utils/UtilsCollections.cpp"

$(IntermediateDirectory)/Utils_UtilsCollections$(PreprocessSuffix): Utils/UtilsCollections.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Utils_UtilsCollections$(PreprocessSuffix) "Utils/UtilsCollections.cpp"

$(IntermediateDirectory)/Utils_UtilsMath$(ObjectSuffix): Utils/UtilsMath.cpp $(IntermediateDirectory)/Utils_UtilsMath$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Vee/Software/GitHub/OHWorkspace/SSVUtils/Utils/UtilsMath.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Utils_UtilsMath$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Utils_UtilsMath$(DependSuffix): Utils/UtilsMath.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Utils_UtilsMath$(ObjectSuffix) -MF$(IntermediateDirectory)/Utils_UtilsMath$(DependSuffix) -MM "Utils/UtilsMath.cpp"

$(IntermediateDirectory)/Utils_UtilsMath$(PreprocessSuffix): Utils/UtilsMath.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Utils_UtilsMath$(PreprocessSuffix) "Utils/UtilsMath.cpp"

$(IntermediateDirectory)/Timeline_Command$(ObjectSuffix): Timeline/Command.cpp $(IntermediateDirectory)/Timeline_Command$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Vee/Software/GitHub/OHWorkspace/SSVUtils/Timeline/Command.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Timeline_Command$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Timeline_Command$(DependSuffix): Timeline/Command.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Timeline_Command$(ObjectSuffix) -MF$(IntermediateDirectory)/Timeline_Command$(DependSuffix) -MM "Timeline/Command.cpp"

$(IntermediateDirectory)/Timeline_Command$(PreprocessSuffix): Timeline/Command.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Timeline_Command$(PreprocessSuffix) "Timeline/Command.cpp"

$(IntermediateDirectory)/Timeline_Do$(ObjectSuffix): Timeline/Do.cpp $(IntermediateDirectory)/Timeline_Do$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Vee/Software/GitHub/OHWorkspace/SSVUtils/Timeline/Do.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Timeline_Do$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Timeline_Do$(DependSuffix): Timeline/Do.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Timeline_Do$(ObjectSuffix) -MF$(IntermediateDirectory)/Timeline_Do$(DependSuffix) -MM "Timeline/Do.cpp"

$(IntermediateDirectory)/Timeline_Do$(PreprocessSuffix): Timeline/Do.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Timeline_Do$(PreprocessSuffix) "Timeline/Do.cpp"

$(IntermediateDirectory)/Timeline_Go$(ObjectSuffix): Timeline/Go.cpp $(IntermediateDirectory)/Timeline_Go$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Vee/Software/GitHub/OHWorkspace/SSVUtils/Timeline/Go.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Timeline_Go$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Timeline_Go$(DependSuffix): Timeline/Go.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Timeline_Go$(ObjectSuffix) -MF$(IntermediateDirectory)/Timeline_Go$(DependSuffix) -MM "Timeline/Go.cpp"

$(IntermediateDirectory)/Timeline_Go$(PreprocessSuffix): Timeline/Go.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Timeline_Go$(PreprocessSuffix) "Timeline/Go.cpp"

$(IntermediateDirectory)/Timeline_Timeline$(ObjectSuffix): Timeline/Timeline.cpp $(IntermediateDirectory)/Timeline_Timeline$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Vee/Software/GitHub/OHWorkspace/SSVUtils/Timeline/Timeline.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Timeline_Timeline$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Timeline_Timeline$(DependSuffix): Timeline/Timeline.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Timeline_Timeline$(ObjectSuffix) -MF$(IntermediateDirectory)/Timeline_Timeline$(DependSuffix) -MM "Timeline/Timeline.cpp"

$(IntermediateDirectory)/Timeline_Timeline$(PreprocessSuffix): Timeline/Timeline.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Timeline_Timeline$(PreprocessSuffix) "Timeline/Timeline.cpp"

$(IntermediateDirectory)/Timeline_Wait$(ObjectSuffix): Timeline/Wait.cpp $(IntermediateDirectory)/Timeline_Wait$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Vee/Software/GitHub/OHWorkspace/SSVUtils/Timeline/Wait.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Timeline_Wait$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Timeline_Wait$(DependSuffix): Timeline/Wait.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Timeline_Wait$(ObjectSuffix) -MF$(IntermediateDirectory)/Timeline_Wait$(DependSuffix) -MM "Timeline/Wait.cpp"

$(IntermediateDirectory)/Timeline_Wait$(PreprocessSuffix): Timeline/Wait.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Timeline_Wait$(PreprocessSuffix) "Timeline/Wait.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/FileSystem_FileSystem$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/FileSystem_FileSystem$(DependSuffix)
	$(RM) $(IntermediateDirectory)/FileSystem_FileSystem$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Log_Log$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Log_Log$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Log_Log$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Utils_Utils$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Utils_Utils$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Utils_Utils$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Utils_UtilsString$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Utils_UtilsString$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Utils_UtilsString$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Utils_UtilsCollections$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Utils_UtilsCollections$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Utils_UtilsCollections$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Utils_UtilsMath$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Utils_UtilsMath$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Utils_UtilsMath$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Command$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Command$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Command$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Do$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Do$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Do$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Go$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Go$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Go$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Timeline$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Timeline$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Timeline$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Wait$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Wait$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Timeline_Wait$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile)
	$(RM) "../.build-release/SSVUtils"


