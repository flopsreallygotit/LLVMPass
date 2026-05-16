#ifndef INSTRUMENTER_H_
#define INSTRUMENTER_H_

#include "ValueIds.h"
#include "llvm/IR/Module.h"

namespace mypass {
class Instrumenter {
public:
    explicit Instrumenter(llvm::Module &Module);
    int Instrument(ValueIds &ids);

private:
    void DeclareLogFunctions(void);
    bool ShouldInstrument(const llvm::Instruction &Instruction) const;

    llvm::Module &module_;
    llvm::FunctionCallee log_i32_;
    llvm::FunctionCallee log_i64_;
};

} // mypass

#endif // INSTRUMENTER_H_