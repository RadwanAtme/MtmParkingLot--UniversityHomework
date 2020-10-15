#ifndef UNTITLED_UNIQUEARRAYIMP_H
#define UNTITLED_UNIQUEARRAYIMP_H
template <class Element, class Compare>
UniqueArray <Element,Compare>::UniqueArray(unsigned int size):array(fillArray(size)),max_size(size){
}
template <class Element,class Compare>
const Element** UniqueArray <Element,Compare>::fillArray(int size){
    const Element** array=new const Element*[size];
    for(int i=0;i<size;i++){
        array[i]=NULL;
    }
    return array;
}
template <class Element, class Compare>
UniqueArray <Element,Compare>::UniqueArray(const UniqueArray& other):array(fillArray(other.max_size)),max_size(other.max_size){
    for(int i=0;i<max_size;i++){
        if(other.array[i]==NULL)
        {
            continue;
        }
        array[i]=new Element(*other.array[i]);
    }

}

template <class Element, class Compare>
UniqueArray <Element,Compare>::~UniqueArray(){
    for(int i=0;i<max_size;i++){
        delete array[i];
    }
    delete[] array;
}

template <class Element, class Compare>
unsigned int UniqueArray <Element,Compare>::insert(const Element& element){
    for(int i=0;i<max_size;i++){//check if the element exists
        Element element_to_compare(element);
        if(array[i]==NULL)
        {
            continue;
        }
        if(compare(*array[i],element_to_compare)){
            return i;
        }
    }
    for(int i=0;i<max_size;i++){
        if(array[i]==NULL){
            array[i]=new Element(element);
            return i;
        }
    }
    throw UniqueArrayIsFullException();
}

template <class Element, class Compare>
bool UniqueArray <Element,Compare>::getIndex(const Element& element, unsigned int& index) const{
    for(int i=0;i<max_size;i++){
        if(array[i]==NULL){
            continue;
        }
        if(compare(*array[i],element)){
            index=i;
            return true;
        }
    }
    index=-1;
    return false;
}

template <class Element, class Compare>
const Element* UniqueArray <Element,Compare>::operator[] (const Element& element) const{
    for(int i=0;i<max_size;i++){
        if(array[i]==NULL){
            continue;
        }
        if(compare(*array[i],element)){
            return array[i];
        }
    }
    return NULL;
}



template <class Element, class Compare>
bool UniqueArray <Element,Compare>::remove(const Element& element){
    for(int i=0;i<max_size;i++){
        if(array[i]==NULL){
            continue;
        }
        if(compare(*array[i],element)){
            delete array[i];
            array[i]=NULL;
            return true;
        }
    }
    return false;
}
template <class Element, class Compare>
unsigned int UniqueArray <Element,Compare>::getCount() const{
    int count=0;
    for(int i=0;i<max_size;i++){
        if(array[i]==NULL){
            continue;
        }
        count++;
    }
    return count;
}
template <class Element, class Compare>
unsigned int UniqueArray <Element,Compare>::getSize() const{
    return max_size;
}

template <class Element, class Compare>
UniqueArray<Element,Compare> UniqueArray <Element,Compare>::filter(const Filter& f) const{
    UniqueArray element(this->max_size);
    for(int i=0;i<max_size;i++){
        if(this->array[i]==NULL)
        {
            element.array[i]=NULL;
        }
        if(!f(*(this->array[i]))){
            element.array[i]=NULL;
        } else{
            element.array[i]=new Element(*(this->array[i]));
        }

    }
    return element;
}

template <class Element, class Compare>
const Element* UniqueArray<Element,Compare>::findByIndex(const unsigned int index) const{
    if(index>=0&&index<max_size){
        return array[index];
    }
    return NULL;
}



template <class Element, class Compare>
int UniqueArray<Element,Compare>::firstEmpty(){
    for(int i=0;i<getSize();i++){
        if(array[i]==NULL){
            return i;
        }
    }
    return -1;
}

template <class Element, class Compare>
bool UniqueArray<Element,Compare>::insertTo(const Element& element,const int index){
    if(index<0||index>=max_size){
        return false;
    }
    if(array[index]!=NULL){
        return false;
    }
    array[index]=new Element(element);
    return true;
}

template <class Element, class Compare>
bool UniqueArray<Element,Compare>::remove_index(int index) {
    if(index<0||index>=max_size){
        return false;
    }
    delete array[index];
    array[index]=NULL;
    return true;
}
#endif //UNTITLED_UNIQUEARRAYIMP_H
