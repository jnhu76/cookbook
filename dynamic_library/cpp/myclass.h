#ifndef _MYCLASS_H_
#define _MYCLASS_H_

class MyClass {
public:
    explicit MyClass(int x): x(x) {}
    virtual void DoSomething();

private:
    int x;
};

#endif // _MYCLASS_H_