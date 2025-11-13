#include "Bin_tree.h"

#undef FINAL_CODE
#define FINAL_CODE

errno_t Bin_tree_Ctor(Bin_tree *const tree_ptr
           ON_DEBUG(, Var_info const var_info)) {
    assert(tree_ptr); assert(!tree_ptr->is_valid);
    ON_DEBUG(
    assert(var_info.position.file_name); assert(var_info.position.function_name);
    assert(var_info.name);
    )

    ON_DEBUG(tree_ptr->var_info = var_info;) //TODO - I copy pointers, so I mustn't clear it in Dtor

    CHECK_FUNC(get_new_Bin_tree_node, &tree_ptr->root, nullptr, nullptr, INITIAL_VAL);

    tree_ptr->is_valid = true;
    return 0;
}

errno_t Bin_tree_Dtor(Bin_tree *const tree_ptr) {
    assert(tree_ptr); assert(tree_ptr->is_valid);

    CHECK_FUNC(Bin_subtree_Dtor, tree_ptr->root);

    tree_ptr->is_valid = false;
    return 0;
}

errno_t Bin_tree_verify(Bin_tree const *const tree_ptr, errno_t *const err_ptr) {
    assert(tree_ptr); assert(err_ptr);

    if (!tree_ptr->is_valid) {
        *err_ptr |= TREE_INVALID;
    }

    if (!tree_ptr->root) {
        *err_ptr |= TREE_NULL_ROOT;
    }

    CHECK_FUNC(Bin_subtree_verify, tree_ptr->root, err_ptr);

    return 0;
}

errno_t Bin_tree_visual_dump(Bin_tree const *const tree_ptr, FILE *const out_stream, size_t const id,
                             Position_info const from_where) { //TODO - add extra info and tabulation
    assert(tree_ptr); assert(out_stream);
    ON_DEBUG(
    assert(tree_ptr->var_info.position.file_name); assert(tree_ptr->var_info.position.function_name);
    assert(tree_ptr->var_info.name);
    )
    assert(from_where.file_name); assert(from_where.function_name);

    errno_t verify_err = 0;
    CHECK_FUNC(Bin_tree_verify, tree_ptr, &verify_err);

    fprintf_s(out_stream, "<font size=\"7\">\n\tDump №%zu\n</font>\n\n", id);

    fprintf_s(out_stream, "<pre>\n");

    fprintf_s(out_stream, "called at file %s, line %zu in \"%s\" function for reasons of:\n",
                          from_where.file_name, from_where.line, from_where.function_name);

    if (!verify_err) {
        fprintf_s(out_stream, "\tNo error\n");
    }

    if (verify_err & TREE_NODE_INVALID) {
        fprintf_s(out_stream, "\tOne of tree's nodes is invalid\n");
    }

    if (verify_err & TREE_NODE_VERIFY_USED) {
        fprintf_s(out_stream, "\tOne of tree's nodes has \"verify_used\" field set to true\n");
    }

    if (verify_err & TREE_INVALID_STRUCTURE) {
        fprintf_s(out_stream, "\tTree has invalid structure\n");
    }

    if (verify_err & TREE_INVALID) {
        fprintf_s(out_stream, "\tTree is invalid\n");
    }

    if (verify_err & TREE_NULL_ROOT) {
        fprintf_s(out_stream, "\tTree has null root\n");
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
        CHECK_FUNC(Bin_subtree_dot_dump, dot_stream, tree_ptr->root);
        fclose(dot_stream);

        size_t const SYS_STR_MAX_SIZE = 100;
        char sys_str[SYS_STR_MAX_SIZE] = {};
        sprintf_s(sys_str, SYS_STR_MAX_SIZE, "dot -Tsvg " DOT_FILE_PATH
                                             " > ./Visual_html/DOT_output%zu.svg", id);
        CHECK_FUNC(system, sys_str);
        fprintf_s(out_stream, "\t<img src=\"DOT_output%zu.svg\" width=1000/>\n", id);
    }

    fprintf_s(out_stream, "\tis_valid = %d\n", tree_ptr->is_valid);

    fprintf_s(out_stream, "}\n");

    fprintf_s(out_stream, "</pre>\n");

    return 0;
}
