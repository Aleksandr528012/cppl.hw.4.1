#include <catch2/catch_test_macros.hpp>
#include <iostream>

struct ListNode {
public:
    ListNode(int value, ListNode* prev = nullptr, ListNode* next = nullptr)
        : value(value), prev(prev), next(next) {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }

public:
    int value;
    ListNode* prev;
    ListNode* next;
};

class List {
public:
    List()
        : m_head(new ListNode(static_cast<int>(0))), m_size(0),
        m_tail(new ListNode(0, m_head)) {
    }

    virtual ~List() {
        Clear();
        delete m_head;
        delete m_tail;
    }

    bool Empty() { return m_size == 0; }
    unsigned long Size() { return m_size; }

    void PushFront(int value) {
        new ListNode(value, m_head, m_head->next);
        ++m_size;
    }

    void PushBack(int value) {
        new ListNode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int PopFront() {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int PopBack() {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void Clear() {
        auto current = m_head->next;
        while (current != m_tail) {
            current = current->next;
            delete extractPrev(current);
        }
    }

private:
    ListNode* extractPrev(ListNode* node) {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    ListNode* m_head;
    ListNode* m_tail;
    unsigned long m_size;
};

TEST_CASE("Empty function tests", "[empty]") {
    List list;

    SECTION("New list is empty") {
        REQUIRE(list.Empty() == true);
    }

    SECTION("List with elements is not empty") {
        list.PushBack(1);
        REQUIRE(list.Empty() == false);
    }

    SECTION("List becomes empty after clearing") {
        list.PushFront(10);
        list.Clear();
        REQUIRE(list.Empty() == true);
    }
}

TEST_CASE("Size function tests", "[size]") {
    List list;

    SECTION("New list has size 0") {
        REQUIRE(list.Size() == 0);
    }

    SECTION("Size increases when adding elements") {
        list.PushBack(1);
        REQUIRE(list.Size() == 1);

        list.PushFront(2);
        REQUIRE(list.Size() == 2);
    }

    SECTION("Size decreases when removing elements") {
        list.PushBack(1);
        list.PushBack(2);
        list.PopFront();
        REQUIRE(list.Size() == 1);
    }

    SECTION("Size is 0 after clearing") {
        list.PushBack(1);
        list.PushBack(2);
        list.PushBack(3);
        list.Clear();
        REQUIRE(list.Size() == 0);
    }
}

TEST_CASE("Clear function tests", "[clear]") {
    List list;

    SECTION("Clear empty list") {
        REQUIRE_NOTHROW(list.Clear());
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }

    SECTION("Clear list with elements") {
        list.PushBack(1);
        list.PushBack(2);
        list.PushBack(3);

        list.Clear();

        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }

    SECTION("Can add elements after clearing") {
        list.PushFront(10);
        list.PushFront(20);
        list.Clear();

        list.PushBack(30);
        REQUIRE(list.Size() == 1);
        REQUIRE(list.Empty() == false);
    }
}

TEST_CASE("Combined operations tests", "[combined]") {
    List list;

    SECTION("Multiple operations sequence") {
        REQUIRE(list.Empty());
        REQUIRE(list.Size() == 0);

        list.PushBack(10);
        REQUIRE_FALSE(list.Empty());
        REQUIRE(list.Size() == 1);

        list.PushFront(20);
        REQUIRE(list.Size() == 2);

        list.Clear();
        REQUIRE(list.Empty());
        REQUIRE(list.Size() == 0);

        list.PushBack(30);
        REQUIRE(list.Size() == 1);
    }
}