#include "llvm/IR/PassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/Constants.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Plugins/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cassert>

using namespace llvm;

namespace {
static std::string shortLabel(const Value *V) {
    assert(V);

    if (auto *I = dyn_cast<Instruction>(V)) {
        std::string s = "";
        raw_string_ostream os(s);

        if (I->hasName()) {
            os << "%" << I->getName();
        } else {
            os << "%?";
        }

        os << " = " << I->getOpcodeName();

        return os.str();
    }

    if (auto *A = dyn_cast<Argument>(V)) {
        return "arg %" + std::string(A->getName().empty() ? "?" : A->getName().str());
    }

    if (auto *C = dyn_cast<ConstantInt>(V)) {
        return "const " + std::to_string(C->getSExtValue());
    }

    return "value";
}

static std::string Escape(const std::string &s) {
    assert(!s.empty());

    std::string out = "";
    for (char c : s) {
        if (c == '"' || c == '\\') {
            out += '\\';
        }

        out += c;
    }

    return out;
}

struct MyPass : public PassInfoMixin<MyPass> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &) {
        std::ofstream dot = std::ofstream();
        std::ofstream mapping = std::ofstream();
        
        dot.open("graph.dot");
        mapping.open("mapping.txt");
        
        if (!dot.is_open() || !mapping.is_open()) {
            errs() << "Error: failed to open output files.\n";
            return PreservedAnalyses::all();
        }

        dot << "digraph DefUse {\n";
        dot << "\trankdir=TB;\n";
        dot << "\tnode [shape=box, fontname=\"Courier\"];\n";

        std::unordered_map<const Value *, int> ids = std::unordered_map<const Value *, int>();
        int nextId = 0;

        auto GetId = [&](const Value *V) -> int {
            auto it = ids.find(V);
            if (it != ids.end()) {
                return it->second;
            }

            int id = nextId++;
            ids[V] = id;

            return id;
        };

        for (Function &F : M) {
            if (F.isDeclaration()) continue;

            dot << "\tsubgraph cluster_" << F.getName().str() << " {\n";
            dot << "\t\tlabel=\"" << Escape(F.getName().str()) << "\";\n";
            dot << "\t\tstyle=dashed;\n";

            for (Argument &A : F.args()) {
                int id = GetId(&A);
                dot << "\t\tn" << id << " [label=\""
                    << "id=" << id << "\\n" << Escape(shortLabel(&A))
                    << "\", style=filled, fillcolor=lightyellow];\n";

                mapping << id << "\targ\t" << F.getName().str() << "\t"
                        << A.getName().str() << "\n";
            }

            for (BasicBlock &BB : F) {
                for (Instruction &I : BB) {
                    int id = GetId(&I);
                    dot << "\t\tn" << id << " [label=\""
                        << "id=" << id << "\\n" << Escape(shortLabel(&I))
                        << "\"];\n";

                    mapping << id << "\tinst\t" << F.getName().str() << "\t"
                            << I.getOpcodeName() << "\n";
                }
            }

            dot << "  }\n";
        }

        for (Function &F : M) {
            if (F.isDeclaration()) continue;

            for (BasicBlock &BB : F) {
                for (Instruction &I : BB) {
                    int dstId = GetId(&I);
                    for (Use &U : I.operands()) {
                        Value *V = U.get();
                        if (isa<Instruction>(V) || isa<Argument>(V)) {
                            int srcId = GetId(V);
                            dot << "\tn" << srcId << " -> n" << dstId << ";\n";
                        }
                    }
                }
            }
        }

        dot << "}\n";
        dot.close();
        mapping.close();

        errs() << "Written graph.dot and mapping.txt\n";

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