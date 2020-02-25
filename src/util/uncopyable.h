#ifndef _UNCOPY_
#define _UNCOPY_
class uncopyable{
protected:
    uncopyable() { }
    ~uncopyable() { }
private:
    uncopyable(const uncopyable&);
    uncopyable& operator = (const uncopyable&);
};
#endif