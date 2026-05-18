#include "DotWriter.h"
#include "Instrumenter.h"
#include "ValueIds.h"

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"

#if __has_include("llvm/Plugins/PassPlugin.h")
    #include "llvm/Plugins/PassPlugin.h"
#else
    #include "llvm/Passes/PassPlugin.h"
#endif

#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct MyPass : public PassInfoMixin<MyPass> {
    PreservedAnalyses run(Module &Module, ModuleAnalysisManager &) {
        mypass::ValueIds ids{};

        mypass::DotWriter writer("dots/graph.dot", "mapping.txt");
        if (!writer.IsReady()) {
            errs() << "Error: cannot open output files.\n";
            return PreservedAnalyses::all();
        }

        writer.Write(Module, ids);
        errs() << "Written graph.dot and mapping.txt\n";

        mypass::Instrumenter instrumenter(Module);
        int count = instrumenter.Instrument(ids);
        errs() << "Instrumented " << count << " instructions\n";

        return PreservedAnalyses::none();
    }
};

} // namespace

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo
llvmGetPassPluginInfo(void) {
    return {LLVM_PLUGIN_API_VERSION, "MyPass", "v0.1",
        [](PassBuilder &PassBuilder) {
            PassBuilder.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &ModulePassManager, ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "my-pass") {
                        ModulePassManager.addPass(MyPass());
                        return true;
                    }

                    return false;
                });
        }};
}