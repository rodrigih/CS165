#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

/* LISTNODE CLASS*/
class ListNode
{
    public:
        // Constructor and Deconstructor
        ListNode(char c, ListNode* next, ListNode* prev);

        // Class Variables
        char value;
        ListNode* next;
        ListNode* prev;
};

ListNode::ListNode(char c = '\0',ListNode* nextNode = nullptr, ListNode* prevNode = nullptr)
{
    value = c;
    next = nextNode;
    prev = prevNode;
}

/* DOUBLYLINKEDLIST CLASS */ 
class DoublyLinkedList
{
    public:
        //Constructor and Desconstructor
        DoublyLinkedList(int maxLength);
        ~DoublyLinkedList();
        void addNode(char c);
        void copyChars(int length, int offset);
        void printList();
        
        // Class variables
        ListNode* head;
        ListNode* tail;
        int length;
        int max; // The maximum length of list
};

DoublyLinkedList::DoublyLinkedList(int maxLength)
{
    head = new ListNode();
    tail = new ListNode();

    head->next = tail;
    tail->prev = head;
    length = 0;
    max = maxLength;
}

DoublyLinkedList::~DoublyLinkedList()
{
    ListNode* current = head->next;
    ListNode* temp;

    // Delete contents of list before deleting
    // Head and Tail of list
    while(current->next != nullptr)
    {
        temp = current;
        current = current->next;     
        delete temp;
    }

    delete tail;
    delete head;
}

void DoublyLinkedList::addNode(char c)
{
    ListNode* temp;

    if(length == max)
    {
        // To "delete" value of first Node, move it to the end
        // and change it's value
        temp = head->next;

        head->next = head->next->next;     
        head->next->prev = head;
        
        tail->prev->next = temp;
        temp->prev = tail->prev;
        temp->next = tail;
        temp->value = c;

        tail->prev = temp;
    }
    else
    {
        // List is empty
        tail->prev->next = new ListNode(c,tail,tail->prev);
        tail->prev = tail->prev->next;
        length++;
    }

}

void DoublyLinkedList::copyChars(int length,int offset)
{
    ListNode* current = tail;

    // Backtrack offset number of times
    for(int i = 0; i < offset; i++)
    {
        current = current->prev;
    }

    // Output chars length number of times 
    // while adding chars to window
    for(int i = 0; i < length; i++)
    {
        std::cout << current->value;
        addNode(current->value);
        current = current->next;
    }
}

void DoublyLinkedList::printList()
{
    ListNode* current = head->next;
    std::cout << "Length of list: " << length << std::endl;
    while(current != tail)
    {
        std::cout << current->value << std::endl;
        current = current->next;
    }
}

#endif /* LINKEDLIST_H */
