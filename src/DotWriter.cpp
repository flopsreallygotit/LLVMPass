#include "DotWriter.h"

#include "Labels.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Use.h"

namespace mypass {
DotWriter::DotWriter(const std::string &dot_path, const std::string &mapping_path) {
    dot_.open(dot_path);
    mapping_.open(mapping_path);
    ready_ = dot_.is_open() && mapping_.is_open();
}

void DotWriter::Write(llvm::Module &Module, ValueIds &ids) {
    if (!ready_) return;

    WriteHeader();
    for (llvm::Function &Function : Module) {
        if (Function.isDeclaration()) continue;
        WriteFunctionNodes(Function, ids);
    }

    WriteEdges(Module, ids);
    WriteFooter();
}

void DotWriter::WriteHeader(void) {
    dot_ << "digraph DefUse {\n";
    dot_ << "\trankdir=TB;\n";
    dot_ << "\tnode [shape=box, fontname=\"Courier\"];\n";
}

void DotWriter::WriteFunctionNodes(llvm::Function &Function, ValueIds &ids) {
    std::string function_name = Function.getName().str();

    dot_ << "\tsubgraph cluster_" << function_name << " {\n";
    dot_ << "\t\tlabel=\"" << EscapeForDot(function_name) << "\";\n";
    dot_ << "\t\tstyle=dashed;\n";

    for (llvm::Argument &Argument : Function.args()) {
        int id = ids.GetOrAssign(&Argument);
        dot_ << "\t\tn" << id << " [label=\""
             << "id=" << id << "\\n" << EscapeForDot(ShortLabel(&Argument))
             << "\", style=filled, fillcolor=lightyellow];\n";

        mapping_ << id << "\targ\t" << function_name << "\t"
                 << Argument.getName().str() << "\n";
    }

    for (llvm::BasicBlock &BasicBlock : Function) {
        for (llvm::Instruction &Instruction : BasicBlock) {
            int id = ids.GetOrAssign(&Instruction);
            dot_ << "\t\tn" << id << " [label=\""
                 << "id=" << id << "\\n" << EscapeForDot(ShortLabel(&Instruction))
                 << "\"];\n";

            mapping_ << id << "\tinst\t" << function_name << "\t"
                     << Instruction.getOpcodeName() << "\n";
        }
    }

    dot_ << "\t}\n";
}

void DotWriter::WriteEdges(llvm::Module &Module, ValueIds &ids) {
    for (llvm::Function &Function : Module) {
        if (Function.isDeclaration()) continue;

        for (llvm::BasicBlock &BasicBlock : Function) {
            for (llvm::Instruction &Instruction : BasicBlock) {
                int dst_id = ids.GetOrAssign(&Instruction);

                for (llvm::Use &Use : Instruction.operands()) {
                    llvm::Value *Value = Use.get();
                    if (Value == nullptr) continue;

                    if (llvm::isa<llvm::Instruction>(Value) ||
                            llvm::isa<llvm::Argument>(Value)) {
                        int src_id = ids.GetOrAssign(Value);
                        dot_ << "\tn" << src_id << " -> n" << dst_id << ";\n";
                    }
                }
            }
        }
    }
}

void DotWriter::WriteFooter(void) {
    dot_ << "}\n";
    dot_.close();
    mapping_.close();
}

} // mypass