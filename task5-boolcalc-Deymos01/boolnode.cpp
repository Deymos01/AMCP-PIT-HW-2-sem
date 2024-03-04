#include "boolnode.h"

namespace global {
    std::set<BoolNode *> deletedPointers;
}

bool operator==(const ParamNode& left, const ParamNode& right) {
    return left.getId() == right.getId();
}
bool operator!=(const ParamNode& left, const ParamNode& right) {
    return !(left == right);
}
bool operator<(const ParamNode& left, const ParamNode& right) {
    return left.getId() < right.getId();
}
bool operator<=(const ParamNode& left, const ParamNode& right) {
    return left < right || left == right;
}
bool operator>(const ParamNode& left, const ParamNode& right) {
    return right < left;
}
bool operator>=(const ParamNode& left, const ParamNode& right) {
    return left > right || left == right;
}