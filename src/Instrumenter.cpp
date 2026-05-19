#include "Instrumenter.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"

#include <vector>
// TODO type for bare 32 and 64

static constexpr const char *kRuntimePrefix = "__mypass_";

namespace mypass {
Instrumenter::Instrumenter(llvm::Module &Module) : module_(Module), log_i32_(), log_i64_() {
    DeclareLogFunctions();
}

void Instrumenter::DeclareLogFunctions(void) {
    llvm::LLVMContext &context = module_.getContext();

    llvm::Type *void_ty = llvm::Type::getVoidTy(context);
    llvm::Type *i32_ty = llvm::Type::getInt32Ty(context);
    llvm::Type *i64_ty = llvm::Type::getInt64Ty(context);

    log_i32_ = module_.getOrInsertFunction("__mypass_log_i32",
        llvm::FunctionType::get(void_ty, {i32_ty, i32_ty}, false));

    log_i64_ = module_.getOrInsertFunction("__mypass_log_i64",
        llvm::FunctionType::get(void_ty, {i32_ty, i64_ty}, false));
}

bool Instrumenter::ShouldInstrument(const llvm::Instruction &Instruction) const {
    if (Instruction.isTerminator()) return false;
    if (llvm::isa<llvm::PHINode>(&Instruction)) return false;
    if (Instruction.getType()->isVoidTy()) return false;
    if (Instruction.getType()->isIntegerTy(32)) return true;
    if (Instruction.getType()->isIntegerTy(64)) return true;
    return false;
}

int Instrumenter::Instrument(ValueIds &ids) {
    std::vector<llvm::Instruction *> targets;

    for (llvm::Function &Function : module_) {
        if (Function.isDeclaration()) continue;
        if (Function.getName().starts_with(kRuntimePrefix)) continue; // FIXME[flops]: This check is extra. You can't reach your own function by design + target module doesn't contain your runtime funcs bodies (Runtime.c contains it), so declaration check is enough

        for (llvm::BasicBlock &BasicBlock : Function) {
            for (llvm::Instruction &Instruction : BasicBlock) {
                if (ShouldInstrument(Instruction)) {
                    targets.push_back(&Instruction); // ------| (1)
                }                                    //       | FIXME[flops]: You don't need to do 2 for cycles and store target in array
            }                                        //       | You can already instrument func in (1) cycle                
        }                                            //       |
    }                                                //       | Instead of returning `targets.size()` at the end, you can count instrumented funcs 
                                                     //       | and just return the counter
    for (llvm::Instruction *Instruction : targets) { // <-----/ (2)
        int id = ids.GetOrAssign(Instruction);
        llvm::Instruction *next = Instruction->getNextNode();
        if (next == nullptr) continue;

        llvm::IRBuilder<> builder(next);
        llvm::Value *id_const = builder.getInt32(id);

        if (Instruction->getType()->isIntegerTy(32)) {
            builder.CreateCall(log_i32_, {id_const, Instruction});
        } else if (Instruction->getType()->isIntegerTy(64)) {
            builder.CreateCall(log_i64_, {id_const, Instruction});
        }
    }

    return static_cast<int>(targets.size());
}

} // mypass