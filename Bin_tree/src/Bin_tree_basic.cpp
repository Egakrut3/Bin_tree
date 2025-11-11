#include "Bin_tree.h"

#undef FINAL_CODE
#define FINAL_CODE

errno_t Bin_tree_node_Ctor(Bin_tree_node *const node_ptr,
                           Bin_tree_node *const left, Bin_tree_node *const right,
                           const_tree_elem_t const val) {
    assert(node_ptr); assert(!node_ptr->is_valid);

    node_ptr->left  = left;
    node_ptr->right = right;
    TREE_ELEM_COPY(node_ptr->val, val);

    node_ptr->is_valid = true;
    return 0;
}

errno_t Bin_tree_node_Dtor(Bin_tree_node *const node_ptr) {
    assert(node_ptr);

    if (!node_ptr->is_valid) {
        return ALREADY_DELETED;
    }

    TREE_ELEM_DTOR(node_ptr->val);

    node_ptr->is_valid = false;
    return 0;
}

errno_t Bin_tree_node_verify(Bin_tree_node *const node_ptr, errno_t *const err_ptr) {
    assert(node_ptr); assert(err_ptr);

    *err_ptr = 0;

    if (!node_ptr->is_valid) {
        *err_ptr |= TREE_NODE_INVALID;
    }

    return 0;
}

errno_t get_new_Bin_tree_node(Bin_tree_node **const dest,
                              Bin_tree_node *const left, Bin_tree_node *const right,
                              const_tree_elem_t const val) {
    assert(dest);

    CHECK_FUNC(My_calloc, (void **)dest, 1, sizeof(Bin_tree_node));
    CHECK_FUNC(Bin_tree_node_Ctor, *dest, left, right, val);

    return 0;
}

errno_t Bin_tree_Ctor(Bin_tree *const tree_ptr
           ON_DEBUG(, Var_info const var_info)) {
    assert(tree_ptr); assert(!tree_ptr->is_valid);
    ON_DEBUG(
    assert(var_info.position.file_name); assert(var_info.position.function_name);
    assert(var_info.name);
    )

    ON_DEBUG(tree_ptr->var_info = var_info;)

    CHECK_FUNC(get_new_Bin_tree_node, &tree_ptr->root, nullptr, nullptr, INITIAL_VAL);

    tree_ptr->is_valid = true;
    return 0;
}

errno_t Bin_tree_Dtor(Bin_tree *const tree_ptr) {
    assert(tree_ptr);

    if (!tree_ptr->is_valid) {
        return ALREADY_DELETED;
    }

    CHECK_FUNC(Bin_subtree_Dtor, tree_ptr->root);

    tree_ptr->is_valid = false;
    return 0;
}

errno_t Bin_tree_verify(Bin_tree const *const tree_ptr, errno_t *const err_ptr) {
    assert(tree_ptr); assert(err_ptr);

    *err_ptr = 0;

    if (!tree_ptr->is_valid) {
        *err_ptr |= TREE_INVALID;
    }

    if (!tree_ptr->root) {
        *err_ptr |= TREE_NULL_ROOT;
    }

    if (!(*err_ptr & TREE_NULL_ROOT)) { //TODO - make check structure

    }

    return 0;
}

static uint32_t ptr_hash(void const *const ptr) {
    uint32_t const mlt  = 0X01'00'01'93;
    uint32_t       hash = 0X81'1C'9D'C5;

    for (size_t i = 0; i < sizeof(ptr); ++i) {
        hash = (hash ^ (uint32_t)((size_t)ptr >> i * CHAR_BIT & 0XFF)) * mlt;
    }

    return hash;
}

static uint32_t ptr_color(void const *const ptr) {
    uint32_t hash = ptr_hash(ptr);

    if ((hash       & 0XFF) +
        (hash >> 8  & 0XFF) +
        (hash >> 16 & 0XFF) < 0X80 * 3) {
        hash = ~hash;
    }

    return hash & 0XFF'FF'FF;
}

static void tree_dump(FILE *const out_stream, Bin_tree_node const *const cur_node) {
    assert(out_stream);
    char const border_color[]      = "black",
               empty_color[]        = "lightgreen",

               left_arrow_color[]  = "red",
               right_arrow_color[] = "blue";

    if (!cur_node) {
        return;
    }

    fprintf_s(out_stream, "\tnode%p [shape = plaintext color = %s style = \"\" "
                                     "label = <<TABLE BORDER=\"0\" CELLBORDER=\"1\" "
                                                     "BGCOLOR=\"#%06X\">"
                                     "<TR><TD COLSPAN=\"2\" PORT=\"top\">"
                                         "VAL = " TREE_ELEM_FRM "</TD></TR>",
                          cur_node,
                          border_color, ptr_color(cur_node),
                          cur_node->val);

    if (cur_node->left) {
        fprintf_s(out_stream, "<TR><TD PORT=\"left\" BGCOLOR=\"#%06X\">left = %p</TD>",
                              ptr_color(cur_node->left), cur_node->left);
    }
    else {
        fprintf_s(out_stream, "<TR><TD PORT=\"left\" BGCOLOR=\"%s\">left = nothing</TD>",
                              empty_color);
    }
    if (cur_node->right) {
        fprintf_s(out_stream, "<TD PORT=\"right\" BGCOLOR=\"#%06X\">right = %p</TD></TR>",
                              ptr_color(cur_node->right), cur_node->right);
    }
    else {
        fprintf_s(out_stream, "<TD PORT=\"right\" BGCOLOR=\"%s\">right = nothing</TD></TR>",
                              empty_color);
    }

    fprintf_s(out_stream, "</TABLE>>]\n",
                          cur_node->val);

    if (cur_node->left) {
        fprintf_s(out_stream, "\tnode%p:left -> node%p:top"
                              "[color = %s]\n",
                              cur_node, cur_node->left,
                              left_arrow_color);
        tree_dump(out_stream, cur_node->left);
    }

    if(cur_node->right) {
        fprintf_s(out_stream, "\tnode%p:right -> node%p:top"
                              "[color = %s]\n",
                              cur_node, cur_node->right,
                              right_arrow_color);
        tree_dump(out_stream, cur_node->right);
    }
}

errno_t Bin_tree_visual_dump(Bin_tree const *const tree_ptr, FILE *const out_stream,
                            Position_info const from_where) { //TODO - add extra info and tabulation
    assert(tree_ptr); assert(out_stream);
    ON_DEBUG(
    assert(tree_ptr->var_info.position.file_name); assert(tree_ptr->var_info.position.function_name);
    assert(tree_ptr->var_info.name);
    )
    assert(from_where.file_name); assert(from_where.function_name);

    errno_t verify_err = 0;
    CHECK_FUNC(Bin_tree_verify, tree_ptr, &verify_err);

    fprintf_s(out_stream, "<pre>\n");

    fprintf_s(out_stream, "called at file %s, line %zu in \"%s\" function for reasons of:\n",
                          from_where.file_name, from_where.line, from_where.function_name);

    if (!verify_err) {
        fprintf_s(out_stream, "\tNo error\n");
    }

    if (verify_err & TREE_INVALID) {
        fprintf_s(out_stream, "\tTree is invalid\n");
    }

    if (verify_err & TREE_NULL_ROOT) {
        fprintf_s(out_stream, "\tTree has null root\n");
    }

    if (verify_err & TREE_INVALID_STRUCTURE) {
        fprintf_s(out_stream, "\tTree has invalid structure\n");
    }

    fprintf_s(out_stream, "\nBin_tree<" TREE_ELEM_STR ">[%p]"
                 ON_DEBUG(" \"%s\" declared in file %s, line %zu in \"%s\" function")
                          " {\n",
                          tree_ptr
               ON_DEBUG(, tree_ptr->var_info.name,
                          tree_ptr->var_info.position.file_name, tree_ptr->var_info.position.line,
                          tree_ptr->var_info.position.function_name));

    if (verify_err & TREE_INVALID_STRUCTURE) {
        fprintf_s(out_stream, "\tInvalid structure\n");
    }
    else {
        FILE *dot_stream = nullptr;
        #define DOT_FILE_PATH "./Visual_html/DOT_file.txt"
        CHECK_FUNC(fopen_s, &dot_stream, DOT_FILE_PATH, "w");

        char const background_color[] = "white";

        fprintf_s(dot_stream, "digraph {\n");
        fprintf_s(dot_stream, "\tnode [shape = octagon style = filled fillcolor = red "
                              "height = 1.0 width = 1.5]\n");
        fprintf_s(dot_stream, "\tedge [color = red penwidth = 3.0]\n");
        fprintf_s(dot_stream, "\tbgcolor = \"%s\"\n", background_color);

        tree_dump(dot_stream, tree_ptr->root);

        fprintf_s(dot_stream, "}");

        fclose(dot_stream);
        #define DOT_OUTPUT_NAME "DOT_output.svg"
        CHECK_FUNC(system, "dot -Tsvg " DOT_FILE_PATH " > ./Visual_html/" DOT_OUTPUT_NAME); //TODO - how to change filename

        fprintf_s(out_stream, "\t<img src=\" " DOT_OUTPUT_NAME "\" width=1000/>\n");
    }

    fprintf_s(out_stream, "\tis_valid = %d\n", tree_ptr->is_valid);

    fprintf_s(out_stream, "}\n");

    fprintf_s(out_stream, "</pre>\n");

    return 0;
}
