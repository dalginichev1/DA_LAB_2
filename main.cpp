#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>

#ifdef DMALLOC
#include "dmalloc.h"
#endif

template <class T>
class vector {
  private:
    T* data;
    size_t sz;
    size_t cap;

  public:
    vector() : data(nullptr), sz(0), cap(0) {
    }

    class Iterator {
      private:
        T* ptr;

      public:
        Iterator(T* p = nullptr) : ptr(p) {
        }

        T& operator*() const {
            return *ptr;
        }
        T* operator->() const {
            return ptr;
        }

        Iterator& operator++() {
            ptr++;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ptr++;
            return temp;
        }

        Iterator& operator--() {
            ptr--;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            ptr--;
            return temp;
        }

        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }
        bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }

        Iterator operator-(ptrdiff_t n) const {
            return ptr - n;
        }
        Iterator operator+(ptrdiff_t n) const {
            return ptr + n;
        }
        ptrdiff_t operator-(const Iterator& other) const {
            return ptr - other.ptr;
        }

        bool operator<(const Iterator& other) const {
            return ptr < other.ptr;
        }

        bool operator>(const Iterator& other) const {
            return ptr > other.ptr;
        }
    };

    using iterator = Iterator;
    using const_iterator = const T*;

    vector(size_t size_) : sz(size_), cap(size_) {
        data = new T[size_];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = T();
        }
    }

    vector(const vector& other) : sz(other.sz), cap(other.cap) {
        data = new T[cap];
        for (size_t i = 0; i < sz; ++i) {
            data[i] = other.data[i];
        }
    }

    vector& operator=(const vector& other) {
        if (this != &other) {
            delete[] data;
            sz = other.sz;
            cap = other.cap;
            data = new T[cap];
            for (size_t i = 0; i < sz; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    vector(vector&& other) noexcept : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            sz = other.sz;
            cap = other.cap;
            data = other.data;
            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    void reserve(size_t new_cap) {
        if (new_cap > cap) {
            T* data_new = new T[new_cap];
            for (size_t i = 0; i < sz; ++i) {
                data_new[i] = std::move(data[i]);
            }
            delete[] data;
            data = data_new;
            cap = new_cap;
        }
    }

    iterator begin() {
        return data;
    }
    const_iterator begin() const {
        return data;
    }
    iterator end() {
        return data + sz;
    }
    const_iterator end() const {
        return data + sz;
    }

    bool empty() const {
        return sz == 0;
    }

    void clear() {
        sz = 0;
    }

    void push_back(const T& value) {
        if (sz >= cap) {
            size_t new_cap = cap == 0 ? 1 : cap * 2;
            reserve(new_cap);
        }
        data[sz] = value;
        sz++;
    }

    void push_back(T&& value) {
        if (sz >= cap) {
            size_t new_cap = cap == 0 ? 1 : cap * 2;
            reserve(new_cap);
        }
        data[sz] = std::move(value);
        sz++;
    }

    size_t size() const {
        return sz;
    }

    T& operator[](int index) {
        return data[index];
    }
    const T& operator[](int index) const {
        return data[index];
    }

    iterator insert(iterator pos, const T& value) {
        size_t index = pos - begin();

        if (sz >= cap) {
            size_t new_cap = cap == 0 ? 1 : cap * 2;
            reserve(new_cap);
            pos = begin() + index;
        }

        for (size_t i = sz; i > index; --i) {
            data[i] = std::move(data[i - 1]);
        }

        data[index] = value;
        sz++;
        return begin() + index;
    }

    iterator insert(iterator pos, T&& value) {
        size_t index = pos - begin();

        if (sz >= cap) {
            size_t new_cap = cap == 0 ? 1 : cap * 2;
            reserve(new_cap);
            pos = begin() + index;
        }

        for (size_t i = sz; i > index; --i) {
            data[i] = std::move(data[i - 1]);
        }

        data[index] = std::move(value);
        sz++;

        return begin() + index;
    }

    ~vector() {
        delete[] data;
    }
};

class string final {
  private:
    char* str;
    size_t len;
    size_t cap;

  public:
    string() : len(0), cap(1) {
        str = new char[cap];
        str[0] = '\0';
    }

    string(size_t length) : len(length), cap(length + 1) {
        str = new char[cap];
        for (size_t i = 0; i < len; ++i)
            str[i] = '\0';
        str[len] = '\0';
    }

    string(const char* cstr) : len(0) {
        if (cstr) {
            while (cstr[len] != '\0')
                ++len;
            cap = len + 1;
            str = new char[cap];
            for (size_t i = 0; i < len; ++i)
                str[i] = cstr[i];
            str[len] = '\0';
        } else {
            cap = 1;
            str = new char[cap];
            str[0] = '\0';
            len = 0;
        }
    }

    string(const string& other) : len(other.len), cap(other.cap) {
        str = new char[cap];
        memcpy(str, other.str, len + 1);
    }

    string& operator=(const string& other) {
        if (this != &other) {
            delete[] str;
            len = other.len;
            cap = other.cap;
            str = new char[cap];
            memcpy(str, other.str, len + 1);
        }
        return *this;
    }

    string(string&& other) noexcept : str(other.str), len(other.len), cap(other.cap) {
        other.str = nullptr;
        other.len = 0;
        other.cap = 0;
    }

    string& operator=(string&& other) noexcept {
        if (this != &other) {
            delete[] str;
            str = other.str;
            len = other.len;
            cap = other.cap;
            other.str = nullptr;
            other.len = 0;
            other.cap = 0;
        }
        return *this;
    }

    ~string() noexcept {
        delete[] str;
    }

    void reserve(size_t new_cap) {
        if (new_cap > cap) {
            char* new_str = new char[new_cap];
            memcpy(new_str, str, len + 1);
            delete[] str;
            str = new_str;
            cap = new_cap;
        }
    }

    size_t length() const {
        return len;
    }
    bool empty() const {
        return len == 0;
    }
    const char* c_str() const {
        return str;
    }
    char* data() {
        return str;
    }
    const char* data() const {
        return str;
    }

    size_t find(char ch, size_t pos = 0) const {
        if (pos >= len)
            return len;
        for (size_t i = pos; i < len; ++i)
            if (str[i] == ch)
                return i;
        return len;
    }

    string substr(size_t begin, size_t count) const {
        if (begin > len || begin + count > len)
            return string();
        string result(count);
        memcpy(result.str, str + begin, count);
        result.str[count] = '\0';
        return result;
    }

    void clear() {
        delete[] str;
        cap = 1;
        str = new char[cap];
        str[0] = '\0';
        len = 0;
    }

    void push_back(const char& c) {
        if (len >= cap) {
            cap = (cap == 0) ? 16 : cap * 2;
            char* new_str = new char[cap];
            memcpy(new_str, str, len + 1);
            delete[] str;
            str = new_str;
        }
        str[len++] = c;
        str[len] = '\0';
    }

    void push_back(char&& c) {
        if (len >= cap) {
            cap = (cap == 0) ? 16 : cap * 2;
            char* new_str = new char[cap];
            memcpy(new_str, str, len + 1);
            delete[] str;
            str = std::move(new_str);
        }
        str[len++] = c;
        str[len] = '\0';
    }

    char& operator[](size_t index) {
        return str[index];
    }
    const char& operator[](size_t index) const {
        return str[index];
    }

    bool operator==(const string& other) const {
        if (len != other.len)
            return false;
        for (size_t i = 0; i < len; ++i)
            if (str[i] != other.str[i])
                return false;
        return true;
    }
    bool operator!=(const string& other) const {
        return !(*this == other);
    }
    bool operator<(const string& other) const {
        size_t min_len = len < other.len ? len : other.len;
        for (size_t i = 0; i < min_len; ++i) {
            if (str[i] != other.str[i])
                return str[i] < other.str[i];
        }
        return len < other.len;
    }
    bool operator>(const string& other) const {
        return other < *this;
    }
    bool operator<=(const string& other) const {
        return !(*this > other);
    }
    bool operator>=(const string& other) const {
        return !(*this < other);
    }

    friend std::ostream& operator<<(std::ostream& os, const string& s) {
        os << s.c_str();
        return os;
    }
};

void to_lower_inplace(string& s) {
    for (size_t i = 0; i < s.length(); ++i) {
        char& c = s[i];
        if (c >= 'A' && c <= 'Z')
            c += 'a' - 'A';
    }
}

template <typename T>
class Stack final {
  private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n) : data(d), next(n) {
        }
    };
    Node* top_node;
    size_t size_;

  public:
    Stack() : top_node(nullptr), size_(0) {
    }

    Stack(const Stack& other) : top_node(nullptr), size_(0) {
        if (other.empty())
            return;
        top_node = new Node(other.top_node->data, nullptr);
        Node* cur = top_node;
        Node* other_cur = other.top_node->next;
        while (other_cur) {
            cur->next = new Node(other_cur->data, nullptr);
            cur = cur->next;
            other_cur = other_cur->next;
        }
        size_ = other.size_;
    }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            Stack temp(other);
            swap(temp);
        }
        return *this;
    }

    ~Stack() {
        while (!empty())
            pop();
    }

    size_t size() const {
        return size_;
    }
    bool empty() const {
        return size_ == 0;
    }

    void swap(Stack& other) {
        std::swap(top_node, other.top_node);
        std::swap(size_, other.size_);
    }

    void push(const T& value) {
        top_node = new Node(value, top_node);
        ++size_;
    }

    void push(T&& value) {
        top_node = new Node(std::move(value), std::move(top_node));
        ++size_;
    }

    void pop() {
        if (empty())
            throw std::out_of_range("Stack is empty");
        Node* old = top_node;
        top_node = top_node->next;
        delete old;
        --size_;
    }

    T& top() {
        if (empty())
            throw std::out_of_range("Stack is empty");
        return top_node->data;
    }
    const T& top() const {
        if (empty())
            throw std::out_of_range("Stack is empty");
        return top_node->data;
    }
};

template <typename T>
void writeToStream(std::ostream& os, const T& value) {
    os.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

void writeToStream(std::ostream& os, const string& s) {
    uint64_t len = static_cast<uint64_t>(s.length());
    writeToStream(os, len);
    os.write(s.c_str(), len);
}

template <typename T>
void readFromStream(std::istream& is, T& value) {
    is.read(reinterpret_cast<char*>(&value), sizeof(T));
}

void readFromStream(std::istream& is, string& s) {
    uint64_t len;
    readFromStream(is, len);
    char* buf = new char[len + 1];
    is.read(buf, len);
    buf[len] = '\0';
    s = string(std::move(buf));
    delete[] buf;
}

template <class Key, class T, class Compare = std::less<Key>>
class RBTree final {
  private:
    struct Node final {
      public:
        Key key;
        T value;
        Node* parent;
        Node* left;
        Node* right;
        bool red;
        Node(const Key& k, const T& v)
            : key(k), value(v), parent(nullptr), left(nullptr), right(nullptr), red(true) {
        }
        Node(Key&& k, T&& v)
            : key(std::move(k)), value(std::move(v)), parent(nullptr), left(nullptr),
              right(nullptr), red(true) {
        }
    };

    Node* root;
    Compare comp;
    size_t size_;

    Node* minimum(Node* node) const {
        while (node && node->left)
            node = node->left;
        return node;
    }

    void left_rotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left)
            y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void right_rotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right)
            x->right->parent = y;
        x->parent = y->parent;
        if (!y->parent)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        x->right = y;
        y->parent = x;
    }

    void transplant(Node* u, Node* v) {
        if (!u->parent)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v)
            v->parent = u->parent;
    }

    void insertBalance(Node* z) {
        while (z->parent && z->parent->red) {
            if (!z->parent->parent)
                break;
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y && y->red) {
                    z->parent->red = false;
                    y->red = false;
                    z->parent->parent->red = true;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        left_rotate(z);
                    }
                    z->parent->red = false;
                    z->parent->parent->red = true;
                    right_rotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y && y->red) {
                    z->parent->red = false;
                    y->red = false;
                    z->parent->parent->red = true;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        right_rotate(z);
                    }
                    z->parent->red = false;
                    z->parent->parent->red = true;
                    left_rotate(z->parent->parent);
                }
            }
        }
        root->red = false;
    }

    void removeBalance(Node* x) {
        while (x != root && (!x || !x->red)) {
            Node* p = x ? x->parent : nullptr;
            if (!p)
                break;

            if (x == p->left) {
                Node* w = p->right;
                if (w && w->red) {
                    w->red = false;
                    p->red = true;
                    left_rotate(p);
                    w = p->right;
                }
                if ((!w || !w->left || !w->left->red) && (!w || !w->right || !w->right->red)) {
                    if (w)
                        w->red = true;
                    x = p;
                } else {
                    if (!w || !w->right || !w->right->red) {
                        if (w && w->left)
                            w->left->red = false;
                        if (w)
                            w->red = true;
                        if (w)
                            right_rotate(w);
                        w = p->right;
                    }
                    if (w)
                        w->red = p->red;
                    p->red = false;
                    if (w && w->right)
                        w->right->red = false;
                    left_rotate(p);
                    x = root;
                }
            } else {
                Node* w = p->left;
                if (w && w->red) {
                    w->red = false;
                    p->red = true;
                    right_rotate(p);
                    w = p->left;
                }
                if ((!w || !w->right || !w->right->red) && (!w || !w->left || !w->left->red)) {
                    if (w)
                        w->red = true;
                    x = p;
                } else {
                    if (!w || !w->left || !w->left->red) {
                        if (w && w->right)
                            w->right->red = false;
                        if (w)
                            w->red = true;
                        if (w)
                            left_rotate(w);
                        w = p->left;
                    }
                    if (w)
                        w->red = p->red;
                    p->red = false;
                    if (w && w->left)
                        w->left->red = false;
                    right_rotate(p);
                    x = root;
                }
            }
        }
        if (x)
            x->red = false;
    }

    void destroy() {
        if (!root)
            return;
        Stack<Node*> stack;
        stack.push(root);
        Node* last = nullptr;
        while (!stack.empty()) {
            Node* cur = stack.top();
            if (cur->left && last != cur->left && last != cur->right) {
                stack.push(cur->left);
            } else if (cur->right && last != cur->right) {
                stack.push(cur->right);
            } else {
                stack.pop();
                last = cur;
                delete cur;
            }
        }
        root = nullptr;
        size_ = 0;
    }

  public:
    RBTree() : root(nullptr), size_(0) {
    }

    ~RBTree() {
        destroy();
    }

    RBTree(const RBTree& other) : root(nullptr), comp(other.comp) {
        if (!other.root)
            return;

        Node* cur = other.root;
        Node* prev = nullptr;

        Node* newRoot = new Node(cur->key, cur->value);
        newRoot->red = cur->red;
        root = newRoot;

        Node* newCur = root;

        while (cur) {
            if (prev == cur->parent && cur->left) {
                Node* node = new Node(cur->left->key, cur->left->value);
                node->red = cur->left->red;
                node->parent = newCur;
                newCur->left = node;

                prev = cur;
                cur = cur->left;
                newCur = node;
                continue;
            }

            else if ((prev == cur->left || !cur->left) && cur->right) {
                Node* node = new Node(cur->right->key, cur->right->value);
                node->red = cur->right->red;
                node->parent = newCur;
                newCur->right = node;

                prev = cur;
                cur = cur->right;
                newCur = node;
                continue;
            }

            prev = cur;
            cur = cur->parent;
            if (newCur)
                newCur = newCur->parent;
        }
    }

    RBTree& operator=(const RBTree& other) {
        if (this != &other) {
            RBTree temp(other);
            swap(temp);
        }

        return *this;
    }

    bool find(const Key& key, T& out) const {
        Node* cur = root;
        while (cur) {
            if (comp(key, cur->key))
                cur = cur->left;
            else if (comp(cur->key, key))
                cur = cur->right;
            else {
                out = cur->value;
                return true;
            }
        }
        return false;
    }

    bool insert(const Key& key, const T& value) {
        return insert_general(key, value);
    }

    bool insert(Key&& key, const T& value) {
        return insert_general(std::move(key), value);
    }

    bool insert(const Key& key, T&& value) {
        return insert_general(key, std::move(value));
    }

    bool insert(Key&& key, T&& value) {
        return insert_general(std::move(key), std::move(value));
    }

    template <typename K, typename V>
    bool insert_general(K&& key, V&& value) {
        Node* parent = nullptr;
        Node* cur = root;
        bool isLeft = false;

        while (cur) {
            parent = cur;

            if (comp(key, cur->key)) {
                cur = cur->left;
                isLeft = true;
            } else if (comp(cur->key, key)) {
                cur = cur->right;
                isLeft = false;
            } else {
                return false;
            }
        }

        Node* newNode = new Node(std::forward<K>(key), std::forward<V>(value));
        newNode->parent = parent;

        if (!parent)
            root = newNode;
        else
            (isLeft ? parent->left : parent->right) = newNode;

        insertBalance(newNode);
        size_++;
        return true;
    }

    bool remove(const Key& key) {
        Node* z = root;
        while (z) {
            if (comp(key, z->key))
                z = z->left;
            else if (comp(z->key, key))
                z = z->right;
            else
                break;
        }
        if (!z)
            return false;

        Node* y = z;
        Node* x = nullptr;
        bool yOriginalRed = y->red;

        if (!z->left) {
            x = z->right;
            transplant(z, z->right);
        } else if (!z->right) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalRed = y->red;
            x = y->right;

            if (y->parent == z) {
                if (x)
                    x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                if (y->right)
                    y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            if (y->left)
                y->left->parent = y;
            y->red = z->red;
        }

        delete z;

        if (!yOriginalRed) {
            removeBalance(x);
        }

        size_--;
        return true;
    }

    void save_keys(std::ostream& os) const {
        if (!root)
            return;
        Node* cur = root;
        while (cur->left)
            cur = cur->left;

        while (cur) {
            writeToStream(os, cur->key);
            if (cur->right) {
                cur = cur->right;
                while (cur->left)
                    cur = cur->left;
            } else {
                while (cur->parent && cur == cur->parent->right)
                    cur = cur->parent;
                cur = cur->parent;
            }
        }
    }

    void save_values(std::ostream& os) const {
        if (!root)
            return;
        Node* cur = root;
        while (cur->left)
            cur = cur->left;

        while (cur) {
            writeToStream(os, cur->value);
            if (cur->right) {
                cur = cur->right;
                while (cur->left)
                    cur = cur->left;
            } else {
                while (cur->parent && cur == cur->parent->right)
                    cur = cur->parent;
                cur = cur->parent;
            }
        }
    }

    void save(std::ostream& os) const {
        writeToStream(os, size_);
        if (size_ == 0)
            return;
        save_keys(os);
        save_values(os);
    }

    void load(std::istream& is) {
        destroy();

        uint64_t n;
        readFromStream(is, n);

        if (n == 0) {
            root = nullptr;
            return;
        }

        vector<Key> keys;
        for (uint64_t i = 0; i < n; i++) {
            Key key;
            readFromStream(is, key);
            keys.push_back(std::move(key));
        }

        vector<T> values;
        for (uint64_t i = 0; i < n; i++) {
            T value;
            readFromStream(is, value);
            values.push_back(std::move(value));
        }

        struct BuildFrame {
            int64_t left;
            int64_t right;
            Node* parent;
            bool isLeft;
        };

        Stack<BuildFrame> st;
        st.push({0, static_cast<int64_t>(n) - 1, nullptr, false});

        int h = 0;
        while ((1ULL << h) <= n)
            h++;
        h--;
        uint64_t fullBlack = (1ULL << h) - 1;
        uint64_t extra = n - fullBlack;
        uint64_t redLeafs = 0;

        while (!st.empty()) {
            BuildFrame cur = st.top();
            st.pop();

            if (cur.left > cur.right)
                continue;

            int64_t mid = cur.left + (cur.right - cur.left) / 2;
            Node* node = new Node(std::move(keys[mid]), std::move(values[mid]));
            node->parent = cur.parent;
            node->red = false;

            if (cur.left == cur.right && redLeafs < extra) {
                node->red = true;
                redLeafs++;
            }

            if (!cur.parent) {
                root = node;
                node->red = false;
            } else {
                if (cur.isLeft)
                    cur.parent->left = node;
                else
                    cur.parent->right = node;
            }

            if (mid < cur.right)
                st.push({mid + 1, cur.right, node, false});
            if (mid > cur.left)
                st.push({cur.left, mid - 1, node, true});
        }
    }

    void clear() {
        destroy();
    }

    void swap(RBTree& other) {
        Node* temp_root = root;
        root = other.root;
        other.root = temp_root;

        Compare temp_comp = std::move(comp);
        comp = std::move(other.comp);
        other.comp = std::move(temp_comp);

        size_t temp_size = size_;
        size_ = other.size_;
        other.size_ = temp_size;
    }

    size_t size() const {
        return size_;
    }
};

string read_line() {
    static char buffer[512];
    static size_t pos = 0;
    static size_t size = 0;

    string result;
    result.reserve(512);

    while (true) {
        if (pos >= size) {
            std::cin.read(buffer, sizeof(buffer));
            size = std::cin.gcount();
            pos = 0;

            if (size == 0) {
                return result;
            }
        }

        for (; pos < size; ++pos) {
            if (buffer[pos] == '\n') {
                ++pos;
                return result;
            }
            result.push_back(std::move(buffer[pos]));
        }
    }
}

uint64_t parse_uint64(const string& s) {
    uint64_t res = 0;
    for (size_t i = 0; i < s.length(); ++i) {
        char c = s[i];
        if (c >= '0' && c <= '9')
            res = res * 10 + (c - '0');
        else
            break;
    }
    return res;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    RBTree<string, uint64_t> dict;
    while (true) {
        string line = read_line();
        if (line.empty())
            break;

        try {
            char first = line[0];
            if (first == '+') {
                size_t space_pos = line.find(' ', 2);
                if (space_pos == line.length())
                    throw std::runtime_error("Invalid command format");

                string word = line.substr(2, space_pos - 2);
                to_lower_inplace(word);

                string num_str = line.substr(space_pos + 1, line.length() - space_pos - 1);
                uint64_t num = parse_uint64(num_str);

                if (dict.insert(word, num))
                    std::cout << "OK\n";
                else
                    std::cout << "Exist\n";
            } else if (first == '-') {
                string word = line.substr(2, line.length() - 2);
                to_lower_inplace(word);

                if (dict.remove(word))
                    std::cout << "OK\n";
                else
                    std::cout << "NoSuchWord\n";
            } else if (first == '!') {
                size_t space_pos = line.find(' ', 2);
                if (space_pos == line.length())
                    throw std::runtime_error("Invalid command format");
                string cmd = line.substr(2, space_pos - 2);
                string path = line.substr(space_pos + 1, line.length() - space_pos - 1);
                if (path.empty())
                    throw std::runtime_error("File path is empty");

                if (cmd == "Save") {
                    std::ofstream out(path.c_str(), std::ios::binary);
                    if (!out.is_open())
                        throw std::runtime_error("Cannot open file for writing");
                    dict.save(out);
                    out.close();
                    std::cout << "OK\n";
                } else if (cmd == "Load") {
                    std::ifstream in(path.c_str(), std::ios::binary);
                    if (!in.is_open()) {
                        dict.clear();
                        std::cout << "OK\n";
                    } else {
                        in.peek();
                        if (in.eof()) {
                            dict.clear();
                            std::cout << "OK\n";
                        } else {
                            RBTree<string, uint64_t> newDict;
                            newDict.load(in);
                            dict.swap(newDict);
                            std::cout << "OK\n";
                        }
                    }
                } else {
                    throw std::runtime_error("Unknown command");
                }
            } else {
                to_lower_inplace(line);
                uint64_t val;
                if (dict.find(line, val))
                    std::cout << "OK: " << val << "\n";
                else
                    std::cout << "NoSuchWord\n";
            }
        } catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what() << "\n";
        }
    }

    return 0;
}
