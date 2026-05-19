#ifndef DOT_WRITER_H_
#define DOT_WRITER_H_

#include "ValueIds.h"
#include "llvm/IR/Module.h"

#include <fstream>
#include <string>

namespace mypass {
class DotWriter {
public:
    DotWriter(const std::string &dot_path, const std::string &mapping_path); //FIXME[flops]: Use string_view there instead

    bool IsReady(void) const {
        return ready_;
    }

    void Write(llvm::Module &Module, ValueIds &ids);

private:
    void WriteHeader(void);
    void WriteFunctionNodes(llvm::Function &Function, ValueIds &ids);
    void WriteEdges(llvm::Module &Module, ValueIds &ids);
    void WriteFooter(void);

    std::ofstream dot_;
    std::ofstream mapping_;
    bool ready_ = false;
};

} // mypass

#endif // DOT_WRITER_H_