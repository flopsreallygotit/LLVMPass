#include "ValueIds.h"

namespace mypass {
int ValueIds::GetOrAssign(const llvm::Value *Value) {

    auto it = ids_.find(Value);
    if (it != ids_.end()) {
        return it->second;
    }

    int id = next_id_;
    next_id_++;
    ids_[Value] = id;
    
    return id;
}

} // mypass