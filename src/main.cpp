#include "Common.h"
#include "Option_manager.h"
#include "Bin_tree.h"
//TODO - make Ctor clear all resources in case of fail

int main(int const argc, char const *const *const argv) {
    assert(argc > 0); assert(argv);

    #undef FINAL_CODE
    #define FINAL_CODE

    Config cur_config = {};
    MAIN_CHECK_FUNC(Config_Ctor, &cur_config, argc, argv);
    #undef FINAL_CODE
    #define FINAL_CODE              \
        Config_Dtor(&cur_config);

    BIN_TREE_CTOR(MAIN_CHECK_FUNC, cur_tree);
    #undef FINAL_CODE
    #define FINAL_CODE              \
        Config_Dtor(&cur_config);   \
        Bin_tree_Dtor(&cur_tree);

    FILE *dump_stream = nullptr;
    MAIN_CHECK_FUNC(fopen_s, &dump_stream, "./Visual_html/Tree_log.html", "w");
    #undef FINAL_CODE
    #define FINAL_CODE              \
        Config_Dtor(&cur_config);   \
        Bin_tree_Dtor(&cur_tree);   \
        fclose(dump_stream);

    Bin_tree_node *v1 = nullptr,
                  *v2 = nullptr,
                  *v3 = nullptr,
                  *v4 = nullptr,
                  *v5 = nullptr,
                  *v6 = nullptr;

    MAIN_CHECK_FUNC(get_new_Bin_tree_node, &v1, nullptr, nullptr, "v1");
    MAIN_CHECK_FUNC(get_new_Bin_tree_node, &v2, nullptr, nullptr, "v2");
    MAIN_CHECK_FUNC(get_new_Bin_tree_node, &v3, nullptr, nullptr, "v3");
    MAIN_CHECK_FUNC(get_new_Bin_tree_node, &v4, nullptr, nullptr, "v4");
    MAIN_CHECK_FUNC(get_new_Bin_tree_node, &v5, nullptr, nullptr, "v5");
    MAIN_CHECK_FUNC(get_new_Bin_tree_node, &v6, nullptr, nullptr, "v6");

    cur_tree.root->left  = v1;
    cur_tree.root->right = v2;

    v1->left  = v3;
    v1->right = v4;

    v2->left  = nullptr;
    v2->right = nullptr;

    v3->left  = nullptr;
    v3->right = nullptr;

    v4->left  = v5;
    v4->right = v6;

    v5->left  = nullptr;
    v5->right = nullptr;

    v6->left  = nullptr;
    v6->right = nullptr;

    #undef FINAL_CODE
    #define FINAL_CODE              \
        Config_Dtor(&cur_config);   \
        Bin_tree_Dtor(&cur_tree);   \
        fclose(dump_stream);

    BIN_TREE_VISUAL_DUMP(cur_tree, dump_stream, MAIN_CHECK_FUNC);

    colored_printf(GREEN, BLACK, "\n\n\nCOMMIT GITHUB\n\n");
    CLEAR_RESOURCES();
    return 0;
}
