cmake_minimum_required(VERSION 3.14)
project(Forth-LLVM CXX)

include(CMakePrintHelpers)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(forth_runtime)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# LLVM
set(LLVM_INCLUDE_DIRS f:/LLVM/llvm-project/llvm/include)
set(llvm_libs
       LLVMDemangle.lib
    LLVMSupport.lib
    LLVMTableGen.lib
    LLVMCore.lib
    LLVMFuzzerCLI.lib
    LLVMFuzzMutate.lib
    LLVMFileCheck.lib
    LLVMInterfaceStub.lib
    LLVMIRPrinter.lib
    LLVMIRReader.lib
    LLVMCodeGenTypes.lib
    LLVMCodeGen.lib
    LLVMSelectionDAG.lib
    LLVMAsmPrinter.lib
    LLVMMIRParser.lib
    LLVMGlobalISel.lib
    LLVMBinaryFormat.lib
    LLVMBitReader.lib
    LLVMBitWriter.lib
    LLVMBitstreamReader.lib
    LLVMDWARFLinker.lib
    LLVMDWARFLinkerParallel.lib
    LLVMExtensions.lib
    LLVMFrontendHLSL.lib
    LLVMFrontendOpenACC.lib
    LLVMFrontendOpenMP.lib
    LLVMTransformUtils.lib
    LLVMInstrumentation.lib
    LLVMAggressiveInstCombine.lib
    LLVMInstCombine.lib
    LLVMScalarOpts.lib
    LLVMipo.lib
    LLVMVectorize.lib
    LLVMObjCARCOpts.lib
    LLVMCoroutines.lib
    LLVMCFGuard.lib
    LLVMLinker.lib
    LLVMAnalysis.lib
    LLVMLTO.lib
    LLVMMC.lib
    LLVMMCParser.lib
    LLVMMCDisassembler.lib
    LLVMMCA.lib
    LLVMObjCopy.lib
    LLVMObject.lib
    LLVMObjectYAML.lib
    LLVMOption.lib
    LLVMRemarks.lib
    LLVMDebugInfoDWARF.lib
    LLVMDebugInfoGSYM.lib
    LLVMDebugInfoLogicalView.lib
    LLVMDebugInfoMSF.lib
    LLVMDebugInfoCodeView.lib
    LLVMDebugInfoPDB.lib
    LLVMSymbolize.lib
    LLVMDebugInfoBTF.lib
    LLVMDWP.lib
    LLVMExecutionEngine.lib
    LLVMInterpreter.lib
    LLVMJITLink.lib
    LLVMMCJIT.lib
    LLVMOrcJIT.lib
    LLVMOrcShared.lib
    LLVMOrcTargetProcess.lib
    LLVMRuntimeDyld.lib
    LLVMTarget.lib
    LLVMAArch64CodeGen.lib
    LLVMAArch64AsmParser.lib
    LLVMAArch64Disassembler.lib
    LLVMAArch64Desc.lib
    LLVMAArch64Info.lib
    LLVMAArch64Utils.lib
    LLVMAMDGPUCodeGen.lib
    LLVMAMDGPUAsmParser.lib
    LLVMAMDGPUDisassembler.lib
    LLVMAMDGPUTargetMCA.lib
    LLVMAMDGPUDesc.lib
    LLVMAMDGPUInfo.lib
    LLVMAMDGPUUtils.lib
    LLVMARMCodeGen.lib
    LLVMARMAsmParser.lib
    LLVMARMDisassembler.lib
    LLVMARMDesc.lib
    LLVMARMInfo.lib
    LLVMARMUtils.lib
    LLVMAVRCodeGen.lib
    LLVMAVRAsmParser.lib
    LLVMAVRDisassembler.lib
    LLVMAVRDesc.lib
    LLVMAVRInfo.lib
    LLVMBPFCodeGen.lib
    LLVMBPFAsmParser.lib
    LLVMBPFDisassembler.lib
    LLVMBPFDesc.lib
    LLVMBPFInfo.lib
    LLVMHexagonCodeGen.lib
    LLVMHexagonAsmParser.lib
    LLVMHexagonDisassembler.lib
    LLVMHexagonDesc.lib
    LLVMHexagonInfo.lib
    LLVMLanaiCodeGen.lib
    LLVMLanaiAsmParser.lib
    LLVMLanaiDisassembler.lib
    LLVMLanaiDesc.lib
    LLVMLanaiInfo.lib
    LLVMLoongArchCodeGen.lib
    LLVMLoongArchAsmParser.lib
    LLVMLoongArchDisassembler.lib
    LLVMLoongArchDesc.lib
    LLVMLoongArchInfo.lib
    LLVMMipsCodeGen.lib
    LLVMMipsAsmParser.lib
    LLVMMipsDisassembler.lib
    LLVMMipsDesc.lib
    LLVMMipsInfo.lib
    LLVMMSP430CodeGen.lib
    LLVMMSP430Desc.lib
    LLVMMSP430Info.lib
    LLVMMSP430AsmParser.lib
    LLVMMSP430Disassembler.lib
    LLVMNVPTXCodeGen.lib
    LLVMNVPTXDesc.lib
    LLVMNVPTXInfo.lib
    LLVMPowerPCCodeGen.lib
    LLVMPowerPCAsmParser.lib
    LLVMPowerPCDisassembler.lib
    LLVMPowerPCDesc.lib
    LLVMPowerPCInfo.lib
    LLVMRISCVCodeGen.lib
    LLVMRISCVAsmParser.lib
    LLVMRISCVDisassembler.lib
    LLVMRISCVDesc.lib
    LLVMRISCVTargetMCA.lib
    LLVMRISCVInfo.lib
    LLVMSparcCodeGen.lib
    LLVMSparcAsmParser.lib
    LLVMSparcDisassembler.lib
    LLVMSparcDesc.lib
    LLVMSparcInfo.lib
    LLVMSystemZCodeGen.lib
    LLVMSystemZAsmParser.lib
    LLVMSystemZDisassembler.lib
    LLVMSystemZDesc.lib
    LLVMSystemZInfo.lib
    LLVMVECodeGen.lib
    LLVMVEAsmParser.lib
    LLVMVEDisassembler.lib
    LLVMVEInfo.lib
    LLVMVEDesc.lib
    LLVMWebAssemblyCodeGen.lib
    LLVMWebAssemblyAsmParser.lib
    LLVMWebAssemblyDisassembler.lib
    LLVMWebAssemblyDesc.lib
    LLVMWebAssemblyInfo.lib
    LLVMWebAssemblyUtils.lib
    LLVMX86CodeGen.lib
    LLVMX86AsmParser.lib
    LLVMX86Disassembler.lib
    LLVMX86TargetMCA.lib
    LLVMX86Desc.lib
    LLVMX86Info.lib
    LLVMXCoreCodeGen.lib
    LLVMXCoreDisassembler.lib
    LLVMXCoreDesc.lib
    LLVMXCoreInfo.lib
    LLVMAsmParser.lib
    LLVMLineEditor.lib
    LLVMProfileData.lib
    LLVMCoverage.lib
    LLVMPasses.lib
    LLVMTargetParser.lib
    LLVMTextAPI.lib
    LLVMDlltoolDriver.lib
    LLVMLibDriver.lib
    LLVMXRay.lib
    LLVMWindowsDriver.lib
    LLVMWindowsManifest.lib
)

list(TRANSFORM llvm_libs PREPEND "f:/LLVM/llvm-project/llvm/bin/Debug/lib/")


set(AST_SOURCES

)

set(SOURCES
    main.cpp
    tokenizer.cpp
    parser.cpp
    tokenizer.h
    parser.h
    token.h
    token.cpp
    ast_entity.cpp
    ast_entity.h
    ast_builder.cpp
    ast_builder.h
    ast_codegen.cpp
    ast_codegen.h
    enviroment.h
)

list(TRANSFORM SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)
list(TRANSFORM AST_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(${PROJECT_NAME} ${SOURCES} ${AST_SOURCES})

source_group("AST" FILES ${AST_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${LLVM_SOURCE_DIR}/include
    ${clang_SOURCE_DIR}/include
    ${llvm_BINARY_DIR}/tools/clang/include
    ${llvm_BINARY_DIR}/include
    ${LLVM_INCLUDE_DIRS}
)


target_link_libraries(${PROJECT_NAME} PRIVATE ${llvm_libs} Forth_Runtime)