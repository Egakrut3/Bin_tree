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

errno_t Split_node(Bin_tree_node *const node_ptr,
                   Bin_tree_node **const left_dest, Bin_tree_node **const right_dest,
                   const_tree_elem_t const left_val, const_tree_elem_t const right_val) {
    assert(node_ptr); assert(left_dest); assert(right_dest);

    CHECK_FUNC(get_new_Bin_tree_node, left_dest,  nullptr, nullptr, left_val);
    CHECK_FUNC(get_new_Bin_tree_node, right_dest, nullptr, nullptr, right_val);

    node_ptr->left  = *left_dest;
    node_ptr->right = *right_dest;

    return 0;
}
