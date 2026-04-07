#define main _main_disabled
#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "main.cpp"

using ::string;  

TEST(StringTest, DefaultConstructor) {
    string s;
    EXPECT_EQ(s.length(), 0);
    EXPECT_TRUE(s.empty());
}

TEST(StringTest, CStringConstructor) {
    string s("hello");
    EXPECT_EQ(s.length(), 5);
    EXPECT_STREQ(s.c_str(), "hello");
}

TEST(StringTest, PushBack) {
    string s;
    s.push_back('a');
    s.push_back('b');
    EXPECT_EQ(s.length(), 2);
    EXPECT_STREQ(s.c_str(), "ab");
}

TEST(StringTest, FindChar) {
    string s("hello world");
    EXPECT_EQ(s.find('o'), 4);
    EXPECT_EQ(s.find('z'), s.length());
}

TEST(StringTest, ToLower) {
    string s("HELLO");
    to_lower_inplace(s);
    EXPECT_STREQ(s.c_str(), "hello");
}

// Простые тесты для Stack
TEST(StackTest, PushAndPop) {
    Stack<int> s;
    s.push(10);
    EXPECT_EQ(s.top(), 10);
    EXPECT_EQ(s.size(), 1);

    s.pop();
    EXPECT_TRUE(s.empty());
}

TEST(StackTest, MultiplePushes) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    EXPECT_EQ(s.top(), 3);
    EXPECT_EQ(s.size(), 3);

    s.pop();
    EXPECT_EQ(s.top(), 2);
}

TEST(StackTest, CopyConstructor) {
    Stack<int> s1;
    s1.push(1);
    s1.push(2);

    Stack<int> s2(s1);
    EXPECT_EQ(s2.size(), 2);
    EXPECT_EQ(s2.top(), 2);
}

// Простые тесты для RBTree
TEST(RBTreeTest, InsertAndFind) {
    RBTree<string, uint64_t> tree;

    EXPECT_TRUE(tree.insert("apple", 100));
    EXPECT_FALSE(tree.insert("apple", 200)); // дубликат

    uint64_t val;
    EXPECT_TRUE(tree.find("apple", val));
    EXPECT_EQ(val, 100);
}

TEST(RBTreeTest, FindNonExistent) {
    RBTree<string, uint64_t> tree;
    uint64_t val;
    EXPECT_FALSE(tree.find("test", val));
}

TEST(RBTreeTest, Remove) {
    RBTree<string, uint64_t> tree;
    tree.insert("apple", 100);
    tree.insert("banana", 200);

    EXPECT_TRUE(tree.remove("apple"));
    EXPECT_FALSE(tree.remove("apple")); // уже удалено

    uint64_t val;
    EXPECT_FALSE(tree.find("apple", val));
    EXPECT_TRUE(tree.find("banana", val));
}

TEST(RBTreeTest, MultipleInserts) {
    RBTree<string, uint64_t> tree;
    tree.insert("c", 3);
    tree.insert("a", 1);
    tree.insert("b", 2);

    uint64_t val;
    EXPECT_TRUE(tree.find("a", val));
    EXPECT_EQ(val, 1);
    EXPECT_TRUE(tree.find("b", val));
    EXPECT_EQ(val, 2);
    EXPECT_TRUE(tree.find("c", val));
    EXPECT_EQ(val, 3);
}

TEST(RBTreeTest, Clear) {
    RBTree<string, uint64_t> tree;
    tree.insert("test", 100);
    tree.clear();

    uint64_t val;
    EXPECT_FALSE(tree.find("test", val));
}

// Тесты сохранения и загрузки
TEST(RBTreeTest, SaveAndLoad) {
    RBTree<string, uint64_t> tree1;
    tree1.insert("key1", 111);
    tree1.insert("key2", 222);

    // Сохраняем в строковый поток
    std::stringstream ss;
    tree1.save(ss);

    // Загружаем в новое дерево
    RBTree<string, uint64_t> tree2;
    tree2.load(ss);

    // Проверяем
    uint64_t val;
    EXPECT_TRUE(tree2.find("key1", val));
    EXPECT_EQ(val, 111);
    EXPECT_TRUE(tree2.find("key2", val));
    EXPECT_EQ(val, 222);
}

TEST(RBTreeTest, SaveEmptyTree) {
    RBTree<string, uint64_t> tree1;
    std::stringstream ss;
    tree1.save(ss);

    RBTree<string, uint64_t> tree2;
    tree2.load(ss);

    uint64_t val;
    EXPECT_FALSE(tree2.find("anything", val));
}

TEST(RBTreeTest, CopyTree) {
    RBTree<string, uint64_t> tree1;
    tree1.insert("test", 123);

    RBTree<string, uint64_t> tree2(tree1);

    uint64_t val;
    EXPECT_TRUE(tree2.find("test", val));
    EXPECT_EQ(val, 123);
}

// Тесты для parse_uint64
TEST(ParseTest, ParseUint64) {
    EXPECT_EQ(parse_uint64(string("123")), 123);
    EXPECT_EQ(parse_uint64(string("0")), 0);
    EXPECT_EQ(parse_uint64(string("456")), 456);
    EXPECT_EQ(parse_uint64(string("123abc")), 123);
    EXPECT_EQ(parse_uint64(string("")), 0);
}

// Тесты для read_line (простой)
TEST(ReadLineTest, ReadSimpleLine) {
    // Сохраняем оригинальный cin
    std::streambuf* old_cin = std::cin.rdbuf();

    // Создаем строковый поток с тестовыми данными
    std::stringstream ss;
    ss << "hello world\n";
    std::cin.rdbuf(ss.rdbuf());

    string line = read_line();
    EXPECT_STREQ(line.c_str(), "hello world");

    // Восстанавливаем cin
    std::cin.rdbuf(old_cin);
}

// Комплексный тест с эмуляцией команд
TEST(IntegrationTest, BasicCommands) {
    RBTree<string, uint64_t> dict;

    // Тест добавления
    EXPECT_TRUE(dict.insert("apple", 100));

    uint64_t val;
    EXPECT_TRUE(dict.find("apple", val));
    EXPECT_EQ(val, 100);

    // Тест удаления
    EXPECT_TRUE(dict.remove("apple"));
    EXPECT_FALSE(dict.find("apple", val));
}

// Тест регистронезависимости
TEST(IntegrationTest, CaseInsensitive) {
    RBTree<string, uint64_t> dict;

    string word1 = "Apple";
    string word2 = "APPLE";
    to_lower_inplace(word1);
    to_lower_inplace(word2);

    EXPECT_TRUE(dict.insert(word1, 100));

    uint64_t val;
    EXPECT_TRUE(dict.find(word2, val));
    EXPECT_EQ(val, 100);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        if (!root) return true;

        stack<TreeNode*> st;
        TreeNode* curr = root;
        long long int prev = LLONG_MIN; 

        while (curr || !st.empty()) {
            while (curr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top();
            st.pop();

            if (curr->val <= prev) return false;
            prev = curr->val;

            curr = curr->right;
        }
        return true;
    }
};
*/