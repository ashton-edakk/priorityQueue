#include "prqueue.h"

#include "gtest/gtest.h"


using namespace std;

TEST(BlankTest, Test) {
}

TEST(prqueue, default_constructor) {
    prqueue<int> queue;
    EXPECT_EQ(queue.size(), 0);
}

TEST(prqueue, size_func) {
    prqueue<int> queue;
    queue.enqueue(10, 1);
    queue.enqueue(30, 3);
    queue.enqueue(20, 2);
    queue.enqueue(40, 4);
    queue.enqueue(50, 5);
    EXPECT_EQ(queue.size(), 5);
}

TEST(prqueue, peek_func) {
    prqueue<int> queue;
    EXPECT_EQ(queue.peek(), 0);

    queue.enqueue(10, 3);
    EXPECT_EQ(queue.peek(), 10);

    queue.enqueue(30, 1);
    queue.enqueue(20, 2);
    queue.enqueue(40, 4);
    queue.enqueue(50, 5);

    EXPECT_EQ(queue.peek(), 30);

    queue.clear();
}


TEST(prqueue, dequeue_func_empty) {
    prqueue<int> test;
    EXPECT_EQ(test.dequeue(), 0);
    EXPECT_EQ(test.size(), 0);
}
TEST(prqueue, dequeue_func_single) {
    prqueue<int> test;
    test.enqueue(10, 1);

    EXPECT_EQ(test.dequeue(), 10);
    EXPECT_EQ(test.size(), 0);
}
TEST(prqueue, dequeue_func_multi) {
    prqueue<int> test;
    test.enqueue(50, 5);
    test.enqueue(20, 2);
    test.enqueue(80, 8);
    test.enqueue(100, 10);
    test.enqueue(70, 7);
    test.enqueue(10, 1);
    test.enqueue(40, 4);

    EXPECT_EQ(test.dequeue(), 10);
    EXPECT_EQ(test.dequeue(), 20);
    EXPECT_EQ(test.dequeue(), 40);
    EXPECT_EQ(test.dequeue(), 50);
    EXPECT_EQ(test.dequeue(), 70);
    EXPECT_EQ(test.dequeue(), 80);
    EXPECT_EQ(test.dequeue(), 100);
}

TEST(prqueue, enqueue_func_dupes) {
    prqueue<int> queue;
    queue.enqueue(20, 2);
    queue.enqueue(25, 2);
    queue.enqueue(10, 1);

    EXPECT_EQ(10, queue.peek());
    queue.dequeue();
    EXPECT_EQ(20, queue.peek());
}

TEST(prqueue, as_string_func) {
    prqueue<int> queue;

    EXPECT_EQ(queue.as_string(), "");

    queue.enqueue(10, 1);
    EXPECT_EQ(queue.as_string(), "1 value: 10\n");

    queue.enqueue(30, 3);
    queue.enqueue(20, 2);
    EXPECT_EQ(queue.as_string(), "1 value: 10\n" "2 value: 20\n" "3 value: 30\n");

    queue.clear();
}

TEST(prqueue, clear_func) {
    prqueue<int> queue;

    queue.clear();
    EXPECT_EQ(queue.size(), 0);
    EXPECT_EQ(queue.getRoot(), nullptr);

    queue.enqueue(10, 1);
    queue.clear();
    EXPECT_EQ(queue.size(), 0);
    EXPECT_EQ(queue.getRoot(), nullptr);

    queue.enqueue(10, 1);
    queue.enqueue(30, 3);
    queue.enqueue(20, 2);
    queue.enqueue(40, 4);
    queue.enqueue(50, 5);
    queue.clear();
    EXPECT_EQ(queue.size(), 0);
    EXPECT_EQ(queue.getRoot(), nullptr);
}

TEST(prqueue, begin_func) {
    prqueue<int> queue;

    queue.enqueue(20, 2);
    queue.enqueue(10, 1);
    queue.enqueue(30, 3);
    queue.enqueue(40, 4);
    queue.enqueue(50, 5);
    queue.begin();

    int value;
    int priority;

    EXPECT_TRUE(queue.next(value, priority));
    EXPECT_EQ(value, 10);
    EXPECT_EQ(priority, 1);
}

TEST(prqueue, next_func) {
    prqueue<int> queue;
    queue.enqueue(10, 1); 
    queue.enqueue(20, 2);
    queue.enqueue(30, 3);
    queue.begin();
    int value;
    int priority;
    EXPECT_TRUE(queue.next(value, priority));
    EXPECT_EQ(value, 10);
    EXPECT_TRUE(queue.next(value, priority));
    EXPECT_EQ(value, 20);
    EXPECT_TRUE(queue.next(value, priority));
    EXPECT_EQ(value, 30);
    EXPECT_FALSE(queue.next(value, priority));
}


TEST(prqueue, copy_constuctor_func) {
    prqueue<int> queue;
    queue.enqueue(10, 1);
    queue.enqueue(20, 2);
    queue.enqueue(30, 3);

    prqueue<int> queueCpy = queue;
    EXPECT_EQ(queueCpy.dequeue(), 10);
    EXPECT_EQ(queueCpy.dequeue(), 20);
    EXPECT_EQ(queueCpy.dequeue(), 30);
    EXPECT_EQ(queueCpy.size(), 0);
}

TEST(prqueue, assignment_operator_func) {
    prqueue<int> queue;
    queue.enqueue(10, 1);
    queue.enqueue(20, 2);
    prqueue<int> queueAssign;
    queueAssign = queue; 
    EXPECT_EQ(queueAssign.dequeue(), 10);
    EXPECT_EQ(queueAssign.dequeue(), 20);
    EXPECT_EQ(queueAssign.size(), 0);
}

TEST(queue, equal_equals_operator_func) {
    prqueue<int> queue;
    queue.enqueue(10, 1);
    queue.enqueue(20, 2);

    prqueue<int> queueFrance;
    queueFrance.enqueue(10, 1);
    queueFrance.enqueue(20, 2);

    EXPECT_TRUE(queue == queueFrance);
}

TEST(queue, equal_equals_operator_func_FALSE) {
    prqueue<int> queue;
    queue.enqueue(10, 1);
    queue.enqueue(80, 8);

    prqueue<int> queueFrance;
    queueFrance.enqueue(10, 1);
    queueFrance.enqueue(20, 2);

    EXPECT_FALSE(queue == queueFrance);
}

TEST(queue, in_order_traversal) {
    prqueue<int> queue;

    queue.enqueue(10, 1);
    queue.enqueue(20, 2);
    queue.enqueue(30, 3);
    queue.enqueue(40, 4);
    queue.enqueue(50, 5);
    
    queue.begin();
    int value;
    int priority;
    string str;
    while (queue.next(value, priority)) {
        str += to_string(value) + " ";
    }

    EXPECT_EQ(str, "10 20 30 40 50 ");
}

TEST(queue, complex_in_order_traversale) {
    prqueue<int> queue;
    
    queue.enqueue(80, 8);   // Largest priority
    queue.enqueue(10, 1);   // Smallest priority
    queue.enqueue(70, 7);   // Large priority
    queue.enqueue(20, 2);   // Small priority
    queue.enqueue(60, 6);   // Large priority
    queue.enqueue(30, 3);   // Small priority
    queue.enqueue(50, 5);   // Medium priority

    queue.begin();
    int value;
    int priority;
    string str;
    while (queue.next(value, priority)) {
        str += to_string(value) + " ";
    }

   
    EXPECT_EQ(str, "10 20 30 50 60 70 80 ");
}


TEST(queue, in_order_traversal_dupes) {
    prqueue<int> queue;

    queue.enqueue(10, 1);
    queue.enqueue(20, 2);
    queue.enqueue(30, 3);
    queue.enqueue(35, 3);
    queue.enqueue(40, 4);
    queue.enqueue(50, 5);

    queue.begin();
    int value;
    int priority;
    string str;
    while (queue.next(value, priority)) {
        str += to_string(value) + " ";
    }

    EXPECT_EQ(str, "10 20 30 35 40 50 ");
}

