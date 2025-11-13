#include "Common.h"
#include "Option_manager.h"
#include "Bin_tree.h"

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

    FILE *in_stream = nullptr;
    MAIN_CHECK_FUNC(fopen_s, &in_stream, "./Tree_in", "r");
    #undef FINAL_CODE
    #define FINAL_CODE              \
        Config_Dtor(&cur_config);   \
        Bin_tree_Dtor(&cur_tree);   \
        fclose(in_stream);          \
        in_stream = nullptr;

    char *str = nullptr;
    MAIN_CHECK_FUNC(get_all_content, in_stream, nullptr, &str);
    MAIN_CHECK_FUNC(Bin_tree_node_Dtor, cur_tree.root);
    MAIN_CHECK_FUNC(str_read_subtree,  &cur_tree.root, str);
    fclose(in_stream);

    #undef FINAL_CODE
    #define FINAL_CODE              \
        Config_Dtor(&cur_config);   \
        Bin_tree_Dtor(&cur_tree);

    FILE *out_stream = nullptr;
    MAIN_CHECK_FUNC(fopen_s, &out_stream, "./Tree_out", "w");
    #undef FINAL_CODE
    #define FINAL_CODE              \
        Config_Dtor(&cur_config);   \
        Bin_tree_Dtor(&cur_tree);   \
        fclose(out_stream);          \
        out_stream = nullptr;

    MAIN_CHECK_FUNC(write_subtree, cur_tree.root, out_stream);
    fclose(out_stream);

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
        fclose(dump_stream);        \
        dump_stream = nullptr;

    BIN_TREE_VISUAL_DUMP(cur_tree, dump_stream, 0, MAIN_CHECK_FUNC);

    colored_printf(GREEN, BLACK, "\n\n\nCOMMIT GITHUB\n\n");
    CLEAR_RESOURCES();
    return 0;
}
