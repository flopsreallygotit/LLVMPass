#ifndef VALUE_IDS_H_
#define VALUE_IDS_H_

#include "llvm/IR/Value.h"

#include <unordered_map>

namespace mypass {
class ValueIds {
public:
    ValueIds() = default;
    int GetOrAssign(const llvm::Value *V);
    int Size(void) const {
        return next_id_;
    }

private:
    std::unordered_map<const llvm::Value *, int> ids_;
    int next_id_ = 0;
};

} // mypass

#endif // VALUE_IDS_H_