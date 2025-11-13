#include "Bin_tree_node.h"

#undef FINAL_CODE
#define FINAL_CODE

errno_t split_node(Bin_tree_node *const node_ptr,
                   tree_elem_t const left_val, tree_elem_t const right_val) {
    assert(node_ptr);
    ON_DEBUG(
    errno_t verify_err = 0;
    CHECK_FUNC(Bin_tree_node_verify, node_ptr, &verify_err);
    if (verify_err) { return verify_err; }
    )

    CHECK_FUNC(get_new_Bin_tree_node, &node_ptr->left,  nullptr, nullptr, left_val);
    CHECK_FUNC(get_new_Bin_tree_node, &node_ptr->right, nullptr, nullptr, right_val);

    return 0;
}
