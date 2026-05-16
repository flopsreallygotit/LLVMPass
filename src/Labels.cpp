#include "Labels.h"

#include "llvm/IR/Argument.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"

#include <cassert>

namespace mypass {
std::string ShortLabel(const llvm::Value *Value) {
    if (Value == nullptr) {
        return "null";
    }

    if (auto *Instruction = llvm::dyn_cast<llvm::Instruction>(Value)) {
        std::string string = "";
        llvm::raw_string_ostream os(string);

        if (Instruction->hasName()) {
            os << "%" << Instruction->getName();
        } else {
            os << "%?";
        }

        os << " = " << Instruction->getOpcodeName();
        return os.str();
    }

    if (auto *Argument = llvm::dyn_cast<llvm::Argument>(Value)) {
        std::string name = Argument->getName().empty() ? "?" : Argument->getName().str();
        return "arg %" + name;
    }

    if (auto *Constant = llvm::dyn_cast<llvm::ConstantInt>(Value)) {
        return "const " + std::to_string(Constant->getSExtValue());
    }

    return "value";
}

std::string EscapeForDot(const std::string &string) {
    assert(!string.empty());

    std::string out_string = "";
    out_string.reserve(string.size());
    for (char c : string) {
        if (c == '"' || c == '\\') {
            out_string += '\\';
        }

        out_string += c;
    }

    return out_string;
}

} // mypass