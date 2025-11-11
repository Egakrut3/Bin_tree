#include "Bin_tree.h"

#undef FINAL_CODE
#define FINAL_CODE

errno_t Bin_subtree_Dtor(Bin_tree_node *const node_ptr) {
    if (!node_ptr) {
        return 0;
    }

    if (!node_ptr->is_valid) {
        return ALREADY_DELETED;
    }

    node_ptr->is_valid = false;
    CHECK_FUNC(Bin_subtree_Dtor, node_ptr->left);
    CHECK_FUNC(Bin_subtree_Dtor, node_ptr->right);

    node_ptr->is_valid = true;
    CHECK_FUNC(Bin_tree_node_Dtor, node_ptr);

    return 0;
}
