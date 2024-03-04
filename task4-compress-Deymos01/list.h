#ifndef TASK4_COMPRESS_LIST_H
#define TASK4_COMPRESS_LIST_H

#include "node.h"

class PriorityList {
private:
    struct ListObj {
        Node *dataObj = nullptr;
        ListObj *next = nullptr;
        ListObj *prev = nullptr;
    };

    ListObj *head;
    ListObj *tail;
    int sizeList;
public:
    PriorityList() : head(nullptr), tail(nullptr), sizeList(0) {}
    ~PriorityList() {
        ListObj *curr = head;
        while (curr != nullptr) {
            ListObj *next = curr->next;
            delete curr;
            curr = next;
        }
    }

    bool empty() const { return sizeList == 0; }
    int size() const { return sizeList; }

    /*
     * This insert method takes a Node object as a parameter and inserts it into
     * the list in order of increasing frequency.
     */
    void insert(Node *elem) {
        ++sizeList;
        ListObj *newObj = new ListObj{elem, nullptr, nullptr};

        if (sizeList == 1) {
            head = tail = newObj;
            return;
        }

        // Insert at right place
        ListObj *curr = tail;
        while ((curr != nullptr) && (curr->dataObj->frequency > newObj->dataObj->frequency)) {
            curr = curr->prev;
        }

        if (curr == nullptr) {
            // insert at the beginning of list
            head->prev = newObj;
            newObj->next = head;
            head = newObj;
        } else if (curr == tail) {
            // insert at the end of list
            tail->next = newObj;
            newObj->prev = tail;
            tail = newObj;
        } else {
            // insert at the middle of list (after curr)
            newObj->prev = curr;
            newObj->next = curr->next;
            curr->next = newObj;
            newObj->next->prev = newObj;
        }
    }

    Node *pop_front() {
        if (this->empty()) {
            return nullptr;
        }

        Node* headData = head->dataObj;

        if (sizeList == 1) {
            delete head;
            head = tail = nullptr;
        } else {
            head = head->next;
            delete head->prev;
            head->prev = nullptr;
        }

        --sizeList;
        return headData;
    }
};

#endif //TASK4_COMPRESS_LIST_H
