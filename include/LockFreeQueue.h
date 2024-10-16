//
// Created by CaoMengxuan on 2024/9/21.
//

#ifndef YOLO_ONNX_CMX_LOCKFREEQUEUE_H
#define YOLO_ONNX_CMX_LOCKFREEQUEUE_H

#include <atomic>
#include <memory>
#include <iostream>
#include <stdexcept>

template<typename T>
class LockFreeQueue {
private:
    struct Node {
        std::shared_ptr<T> data;
        Node* next;
        Node() : next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;
    std::atomic<size_t> count;  // 用于存储队列中的元素数量

    Node* pop_head() {
        Node* old_head = head.load();
        if (old_head == tail.load()) {
            return nullptr;  // 队列为空
        }
        head.store(old_head->next);
        return old_head;
    }

public:
    LockFreeQueue() : head(new Node), tail(head.load()), count(0) {}

    ~LockFreeQueue() {
        while (Node* old_head = head.load()) {
            head.store(old_head->next);
            delete old_head;
        }
    }

    // Push: 添加元素到队列末尾
    void push(const T& value) {
        std::shared_ptr<T> new_data(std::make_shared<T>(value));
        Node* new_node = new Node;
        Node* old_tail = tail.load();
        old_tail->data = new_data;
        old_tail->next = new_node;
        tail.store(new_node);
        ++count;  // 更新元素计数
    }

    void push(T&& value) {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(value)));
        Node* new_node = new Node;
        Node* old_tail = tail.load();
        old_tail->data = new_data;
        old_tail->next = new_node;
        tail.store(new_node);
        ++count;  // 更新元素计数
    }

    // Emplace: 原地构造元素并添加到队列末尾
    template <class... Args>
    void emplace(Args&&... args) {
        std::shared_ptr<T> new_data = std::make_shared<T>(std::forward<Args>(args)...);
        Node* new_node = new Node;
        Node* old_tail = tail.load();
        old_tail->data = new_data;
        old_tail->next = new_node;
        tail.store(new_node);
        ++count;  // 更新元素计数
    }

    // Pop: 移除队首元素
    void pop() {
        Node* old_head = pop_head();
        if (old_head) {
            --count;  // 更新元素计数
            delete old_head;
        }
    }

    // Front: 返回队首元素
    T& front() {
        Node* first_node = head.load()->next;
        if (!first_node) {
            throw std::runtime_error("Queue is empty");
        }
        return *first_node->data;
    }

    const T& front() const {
        Node* first_node = head.load()->next;
        if (!first_node) {
            throw std::runtime_error("Queue is empty");
        }
        return *first_node->data;
    }

    // Back: 返回队尾元素
    T& back() {
        Node* last_node = tail.load();
        if (last_node == head.load()) {
            throw std::runtime_error("Queue is empty");
        }
        return *last_node->data;
    }

    const T& back() const {
        Node* last_node = tail.load();
        if (last_node == head.load()) {
            throw std::runtime_error("Queue is empty");
        }
        return *last_node->data;
    }

    // Size: 返回队列大小
    size_t size() const {
        return count.load();
    }

    // Empty: 判断队列是否为空
    bool empty() const {
        return size() == 0;
    }
};

#endif//YOLO_ONNX_CMX_LOCKFREEQUEUE_H
