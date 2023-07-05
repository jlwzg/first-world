//#include <iostream>
//#include <memory>
//class A
//{
//public:
//    A()
//    {
//        std::cout << "A constructor" << std::endl;
//    }
//    ~A()
//    {
//        std::cout << "A destructor" << std::endl;
//    }
//};
//
//int main()
//{
//    //初始化方式1
//    std::shared_ptr<A> sp1(new A());
//    std::shared_ptr<A> sp4(new A());
//    std::cout << "use count: " << sp1.use_count() << std::endl;
//
//    A* t = sp4.get();
//    unsigned long long ptrVal = (unsigned long long)t;
//    std::cout << ptrVal << std::endl;
//
//    A* t2 = sp1.get();
//    unsigned long long ptrVal2 = (unsigned long long)t2;
//    std::cout << ptrVal2 << std::endl;
//
//    sp1.reset();
//	sp1 = sp4;
//    A* t3 = sp1.get();
//    unsigned long long ptrVal3 = (unsigned long long)t3;
//    std::cout << ptrVal3 << std::endl;
//
//    std::cout << "use count: " << sp1.use_count() << std::endl;
//
//    //初始化方式2
//    std::shared_ptr<A> sp2(sp1);
//    std::cout << "use count: " << sp2.use_count() << std::endl;
//    std::cout << "use count: " << sp1.use_count() << std::endl;
//
//    sp2.reset();
//
//    std::cout << "use count: " << sp1.use_count() << std::endl;
//    std::cout << "use count: " << sp2.use_count() << std::endl;
//
//    {
//        std::shared_ptr<A> sp3 = sp1;
//        std::cout << "use count: " << sp1.use_count() << std::endl;
//    }
//
//    std::cout << "use count: " << sp1.use_count() << std::endl;
//    return 0;
//
//}