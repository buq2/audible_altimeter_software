#ifndef AUDIBLEA_CPP_HH
#define AUDIBLEA_CPP_HH

// If we ever run pure virtual function, stop
extern "C" void __cxa_pure_virtual()
{
    while (1);
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *g)
{
    return !*(char *)(g);
}

extern "C" void __cxa_guard_release (__guard *g)
{
    *(char *)g = 1;
}

extern "C" void __cxa_guard_abort (__guard *)
{

}

#endif //ifndef AUDIBLEA_CPP_HH
