#include "Bin_tree.h"

#undef FINAL_CODE
#define FINAL_CODE

errno_t get_Bin_tree_node(Bin_tree_node **const dest) {
    assert(dest);

    CHECK_FUNC(My_calloc, (void **)dest, 1, sizeof(Bin_tree_node));

    return 0;
}



static void Bin_subtree_Dtor(Bin_tree_node *const node_ptr) {
    if (!node_ptr) {
        return;
    }

    Bin_subtree_Dtor(node_ptr->left);
    Bin_subtree_Dtor(node_ptr->right);
    free(node_ptr);
}

void Bin_tree_Dtor(Bin_tree *const tree_ptr) {
    assert(tree_ptr);

    Bin_subtree_Dtor(tree_ptr->root);

    tree_ptr->is_valid = false;
}

errno_t Bin_tree_verify(Bin_tree const *const tree_ptr, errno_t *const err_ptr) {
    assert(tree_ptr); assert(err_ptr);

    *err_ptr = 0;

    return 0;
}

static void tree_declare(FILE *const out_stream, Bin_tree_node const *const cur_node) { //TODO - add tabulation
    char const border_color[]     = "black",

               node_color[]       = "lightgreen";

    if (!cur_node) {
        return;
    }

    fprintf_s(out_stream, "\tnode%p [shape = plaintext color = %s style = \"\" "
                                     "label = <<TABLE BORDER=\"0\" CELLBORDER=\"1\" "
                                                     "BGCOLOR=\"%s\">"
                                     "<TR><TD COLSPAN=\"2\" PORT=\"top\">"
                                         "VAL = " TREE_ELEM_FRM "</TD></TR>",
                          cur_node,
                          border_color, node_color,
                          cur_node->val);

    if (cur_node->left) {
        fprintf_s(out_stream, "<TR><TD PORT=\"left\">left = %p</TD>",
                              cur_node->left);
    }
    else {
        fprintf_s(out_stream, "<TR><TD PORT=\"left\">left = nothing</TD>");
    }
    if (cur_node->right) {
        fprintf_s(out_stream, "<TD PORT=\"right\">right = %p</TD></TR>",
                              cur_node->right);
    }
    else {
        fprintf_s(out_stream, "<TD PORT=\"right\">right = nothing</TD></TR>");
    }

    fprintf_s(out_stream, "</TABLE>>]\n",
                          cur_node->val);

    tree_declare(out_stream, cur_node->left);
    tree_declare(out_stream, cur_node->right);
}

static void tree_dump(FILE *const out_stream, Bin_tree_node const *const cur_node) { //TODO - add tabulation
    char const left_arrow_color[]  = "blue",
               right_arrow_color[] = "red";

    if (!cur_node) {
        return;
    }

    if (cur_node->left) {
        fprintf_s(out_stream, "\tnode%p:left -> node%p:top"
                              "[color = %s]\n",
                              cur_node, cur_node->left,
                              left_arrow_color);
    }

    if(cur_node->right) {
        fprintf_s(out_stream, "\tnode%p:right -> node%p:top"
                              "[color = %s]\n",
                              cur_node, cur_node->right,
                              right_arrow_color);
    }

    tree_dump(out_stream, cur_node->left);
    tree_dump(out_stream, cur_node->right);
}

errno_t Bin_tree_visual_dump(Bin_tree const *const tree_ptr, FILE *const out_stream,
                            Position_info const from_where) {
    assert(tree_ptr); assert(out_stream);

    errno_t verify_err = 0;
    CHECK_FUNC(Bin_tree_verify, tree_ptr, &verify_err);

    fprintf_s(out_stream, "<pre>\n");

    fprintf_s(out_stream, "called at file %s, line %zu in \"%s\" function for reasons of:\n",
                          from_where.file_name, from_where.line, from_where.function_name);

    if (!verify_err) {
        fprintf_s(out_stream, "\tNo error\n");
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
                              "height = 1.0 width = 1.5]\n"); //TODO - make bolder
        fprintf_s(dot_stream, "\tedge [color = red penwidth = 3.0]\n");
        fprintf_s(dot_stream, "\tbgcolor = \"%s\"\n", background_color);

        tree_declare(dot_stream, tree_ptr->root);
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
