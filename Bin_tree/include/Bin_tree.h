#ifndef BIN_TREE_H
#define BIN_TREE_H

#include "Common.h"

typedef int tree_elem_t;
#define TREE_ELEM_STR "int"
#define TREE_ELEM_FRM "%d"

struct Bin_tree_node {
    Bin_tree_node *left,
                  *right;
    tree_elem_t   val;
};

errno_t get_Bin_tree_node(Bin_tree_node **dest);

//uint64_t const    TREE_BIT_CANARY = 0XFACE'FACE'FACE'FACE;
tree_elem_t const TREE_CANARY     = 666; //TODO -

struct Bin_tree {
    ON_DEBUG(Var_info var_info;)
    Bin_tree_node     *root;

    bool              is_valid;
};

void Bin_tree_Dtor(Bin_tree *list_ptr);

#define TREE_INVALID_STRUCTURE 0B10'000'000'000
errno_t Bin_tree_verify(Bin_tree const *tree_ptr, errno_t *err_ptr);

errno_t Bin_tree_visual_dump(Bin_tree const *tree_ptr, FILE *out_stream,
                            Position_info from_where);
#define BIN_TREE_VISUAL_DUMP(name, out_stream, handler)                                         \
handler(Bin_tree_visual_dump, &name, out_stream, Position_info{__FILE__, __func__, __LINE__})

#endif
