#include "btree.h"
#include <cassert>
#include <iterator>

// Helper: build tree from file
BTree build_tree(std::string fname)
{
	return BTree(fname);
}

// Helper: capture tree print into a string
std::string tree_str(BTree &tree)
{
    std::ostringstream out;
    std::streambuf* oldBuf = std::cout.rdbuf(out.rdbuf());
    tree.print();
    std::cout.rdbuf(oldBuf);
    return out.str();
}

// Helper: capture file into a string
std::string get_result(std::string fname)
{
	std::ifstream in(fname);
    if (!in)
    {
        std::cerr << "Error opening file\n";
        return "error";
    }

    std::string result_str((std::istreambuf_iterator<char>(in)),
                           std::istreambuf_iterator<char>());
    return result_str;
}

// Helper: vaidate result
void check_result(std::string result, std::string res_file, std::string msg, int &correct_count)
{
	BTree tree(res_file);
    std::string expected = tree_str(tree);
    if (result == expected)
    {
        correct_count += 1;
    }
    else
    {
        std::cout << msg << std::endl;
        std::cout << "\texpected tree:" << std::endl
                  << expected << std::endl;
        std::cout << "\treceived tree:" << std::endl
                  << result << std::endl;
    }
}

void test_case1(int &correct, int &total)
{
    int correct_count = 0;
    // key not in tree
    // key in leaf
    BTree tree = build_tree("tests/test_1.txt");
    tree.remove(6);
    std::string result = tree_str(tree);
    check_result(result, "results/test_11.txt", "incorrect result removing key from leaf node", correct_count);

    tree.remove(18);
    result = tree_str(tree);
    check_result(result, "results/test_12.txt", "incorrect result attempting to remove a key not found in the tree", correct_count);

    tree.remove(10);
    tree.remove(5);
    tree.remove(15);
    tree.remove(20);
    result = tree_str(tree);
    check_result(result, "results/test_13.txt", "incorrect result attempting to remove last key from the tree", correct_count);

    tree.remove(1);
    check_result(result, "results/test_14.txt", "incorrect result attempting to remove a key from an empty tree", correct_count);

    std::cout << "Passed " << correct_count << "/4 tests in test_case1" << std::endl;

    correct += correct_count;
    total += 4;
}

void test_case2a(int &correct, int &total)
{
    int correct_count = 0;
    // key in internal node, x.ci has at least t keys
    BTree tree = build_tree("tests/test_2a.txt");
    tree.remove(10);
    std::string result = tree_str(tree);
    check_result(result, "results/test_2a.txt", "incorrect result in case 2a", correct_count);

    std::cout << "Passed " << correct_count << "/1 tests in test_case2a" << std::endl;

    correct += correct_count;
    total += 1;
}

void test_case2b(int &correct, int &total)
{
    int correct_count = 0;
    // key in internal node, x.ci has t-1 keys and x.ci+1 has at least t keys
    BTree tree = build_tree("tests/test_2b.txt");
    tree.remove(20);
    std::string result = tree_str(tree);
    check_result(result, "results/test_2b.txt", "incorrect result in case 2b", correct_count);

    std::cout << "Passed " << correct_count << "/1 tests in test_case2b" << std::endl;

    correct += correct_count;
    total += 1;
}

void test_2c(int &correct, int &total)
{
    int correct_count = 0;
    // key in internal node, both x.ci and x.ci+1 have t-1 keys
    BTree tree = build_tree("tests/test_2c.txt");
    tree.remove(15);
    std::string result = tree_str(tree);
    check_result(result, "results/test_2c1.txt", "incorrect result in case 2c", correct_count);

    tree.remove(8);
    tree.remove(25);
    result = tree_str(tree);
    check_result(result, "results/test_2c2.txt", "incorrect result in case 2c when tree decreases in height", correct_count);

    std::cout << "Passed " << correct_count << "/2 tests in test_case2c" << std::endl;

    correct += correct_count;
    total += 2;
}

void test_3a(int &correct, int &total)
{
    int correct_count = 0;
    // key in internal node, both x.ci and x.ci+1 have t-1 keys
    BTree tree = build_tree("tests/test_3a.txt");
    tree.remove(9);
    std::string result = tree_str(tree);
    check_result(result, "results/test_3a1.txt", "incorrect result in case 3a if immediate right sibling has t keys", correct_count);

    tree.remove(26);
    result = tree_str(tree);
    check_result(result, "results/test_3a2.txt", "incorrect result in case 3a if immediate left sibling has t keys", correct_count);

    tree.remove(18);
    result = tree_str(tree);
    check_result(result, "results/test_3a3.txt", "incorrect result in case 3 if xci has t keys", correct_count);

    std::cout << "Passed " << correct_count << "/3 tests in test_case3a" << std::endl;

    correct += correct_count;
    total += 3;
}

void test_3b(int &correct, int &total)
{
    int correct_count = 0;
    // key in internal node, both x.ci and x.ci+1 have t-1 keys
    BTree tree = build_tree("tests/test_3b.txt");
    tree.remove(5);
    std::string result = tree_str(tree);
    check_result(result, "results/test_3b1.txt", "incorrect result in case 3b merging x.ci with right sibling", correct_count);

    tree.remove(16);
    result = tree_str(tree);
    check_result(result, "results/test_3b2.txt", "incorrect result in case 3b merging x.ci with left sibling", correct_count);

    std::cout << "Passed " << correct_count << "/2 tests in test_case3b" << std::endl;

    correct += correct_count;
    total += 2;
}

int main()
{
    int all_passed = 0;
    int all_total = 0;

    test_case1(all_passed, all_total);
    test_case2a(all_passed, all_total);
    test_case2b(all_passed, all_total);
    test_2c(all_passed, all_total);
    test_3a(all_passed, all_total);
    test_3b(all_passed, all_total);

    std::cout << "\nPassed a total of " << all_passed << "/" << all_total << " tests." << std::endl;

    return 0;
}