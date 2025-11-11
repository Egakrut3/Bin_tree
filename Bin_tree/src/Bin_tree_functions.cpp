#include "Bin_tree.h"

#undef FINAL_CODE
#define FINAL_CODE

errno_t Split_node(Bin_tree_node *const node_ptr,
                   const_tree_elem_t const left_val, const_tree_elem_t const right_val) {
    assert(node_ptr);
    ON_DEBUG(
    errno_t verify_err = 0;
    CHECK_FUNC(Bin_tree_node_verify, node_ptr, &verify_err);
    assert(!verify_err);
    )

    CHECK_FUNC(get_new_Bin_tree_node, &node_ptr->left,  nullptr, nullptr, left_val);
    CHECK_FUNC(get_new_Bin_tree_node, &node_ptr->right, nullptr, nullptr, right_val);

    return 0;
}
