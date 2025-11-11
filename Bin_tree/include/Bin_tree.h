#ifndef BIN_TREE_H
#define BIN_TREE_H

#include "Common.h"

typedef char *       tree_elem_t;
typedef char const * const_tree_elem_t; //TODO -
#define TREE_ELEM_STR "string"
#define TREE_ELEM_FRM "%s"

#define TREE_ELEM_COPY(dest, src)       \
do {                                    \
    CHECK_FUNC(My_strdup, &dest, src);  \
} while (false)

#define TREE_ELEM_DTOR(src) \
do {                        \
    free(src);              \
    src = nullptr;          \
} while (false)

#define TREE_ELEM_EQUAL(a, b)   \
(!strcmp(a, b))

struct Bin_tree_node {
    Bin_tree_node *left,
                  *right;
    tree_elem_t   val;

    bool          is_valid;
};

errno_t Bin_tree_node_Ctor(Bin_tree_node *node_ptr,
                           Bin_tree_node *left, Bin_tree_node *right,
                           const_tree_elem_t val);

errno_t Bin_tree_node_Dtor(Bin_tree_node *node_ptr);

#define TREE_NODE_INVALID 0B10'000'000'000
errno_t Bin_tree_node_verify(Bin_tree_node *node_ptr, errno_t *err_ptr);

errno_t get_new_Bin_tree_node(Bin_tree_node **dest,
                              Bin_tree_node *left, Bin_tree_node *right,
                              const_tree_elem_t val);

errno_t Bin_subtree_Dtor(Bin_tree_node *node_ptr);

errno_t Split_node(Bin_tree_node *node_ptr,
                   Bin_tree_node **left_dest, Bin_tree_node **right_dest,
                   const_tree_elem_t left_val, const_tree_elem_t right_val);



struct Bin_tree {
    ON_DEBUG(Var_info var_info;)
    Bin_tree_node     *root;

    bool              is_valid;
};

#define INITIAL_VAL "I have no idea who could it be"
errno_t Bin_tree_Ctor(Bin_tree *const tree_ptr
           ON_DEBUG(, Var_info var_info));
#ifdef _DEBUG
#define BIN_TREE_CTOR(handler, name)                                    \
Bin_tree name = {};                                                     \
handler(Bin_tree_Ctor, &name,                                           \
        Var_info{Position_info{__FILE__, __func__, __LINE__}, #name})
#else
#define BIN_TREE_CTOR(handler, name)                                    \
Bin_tree name = {};                                                     \
handler(Bin_tree_Ctor, &name)
#endif

errno_t Bin_tree_Dtor(Bin_tree *tree_ptr);

#define TREE_INVALID           0B10'000'000'000
#define TREE_NULL_ROOT         0B100'000'000'000
#define TREE_INVALID_STRUCTURE 0B1'000'000'000'000
errno_t Bin_tree_verify(Bin_tree const *tree_ptr, errno_t *err_ptr);

errno_t Bin_tree_visual_dump(Bin_tree const *tree_ptr, FILE *out_stream,
                             Position_info from_where);
#define BIN_TREE_VISUAL_DUMP(name, out_stream, handler)                                         \
handler(Bin_tree_visual_dump, &name, out_stream, Position_info{__FILE__, __func__, __LINE__})

#endif
