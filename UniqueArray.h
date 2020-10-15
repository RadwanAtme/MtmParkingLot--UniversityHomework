#ifndef MTMPARKINGLOT_UNIQUEARRAY_H
#define MTMPARKINGLOT_UNIQUEARRAY_H

template <class Element, class Compare = std::equal_to<Element>>
class UniqueArray {
    const Element** array;
    int max_size;
    Compare compare;
    static const Element** fillArray(int size);
public:

    explicit UniqueArray(unsigned int size);
    UniqueArray(const UniqueArray& other);
    ~UniqueArray();
    UniqueArray& operator=(const UniqueArray&) = delete;
    unsigned int insert(const Element& element);
    bool getIndex(const Element& element, unsigned int& index) const;
    const Element* operator[] (const Element& element) const;
    bool remove(const Element& element);
    unsigned int getCount() const;
    unsigned int getSize() const;
    bool remove_index(int index);
    /*
     * findByIndex:
     * &param:index of type unsigned int
     *
     * &return:uniqueArray[index]
     */
    const Element* findByIndex(const unsigned int index) const;
    /*
     * firstEmpty:
     * &return:the index of the first empty element of uniqueArray
     */
    int firstEmpty();
    /*
     * insertTo:inserts an element to the array in the index place
     *
     * &param:element refrence of type Element,index of type const int
     *
     * &return:true in case of success
     *          false otherwise
     */
    bool insertTo(const Element& element,const int index);


    class Filter {
    public:
        virtual bool operator() (const Element&) const = 0;
    };
    UniqueArray filter(const Filter& f) const;

    class UniqueArrayIsFullException{};

};

#include "UniqueArrayImp.h"


#endif //MTMPARKINGLOT_UNIQUEARRAY_H
