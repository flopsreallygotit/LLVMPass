#include "llvm/IR/PassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Plugins/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

struct MyPass : public PassInfoMixin<MyPass> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &) {
        errs() << "=== MyPass running on module: " << M.getName() << " ===\n";
        for (Function &F : M) {
            if (F.isDeclaration()) continue;
            errs() << "Function: " << F.getName() << "\n";
            
            for (BasicBlock &BB : F) {
                for (Instruction &I : BB) {
                    errs() << "  " << I << "\n";
                }
            }
        }

        return PreservedAnalyses::all();
    }
};

}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "MyPass", "v0.1",
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "my-pass") {
                        MPM.addPass(MyPass());
                        return true;
                    }

                    return false;
                });
        }};
}

