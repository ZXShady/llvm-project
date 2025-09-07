
namespace Math {
    struct Vec {
        constexpr Vec(int x,int y) : x(x),y(y){}
        int x, y;
        constexpr bool operator==(const Vec& other) const {
            return x == other.x && y == other.y;
        }
    };

    // Accepts Vec by value
    constexpr Vec normalize(const Vec& v) {
        return {v.x / 2, v.y / 2};
    }

    constexpr int magnitude(const Vec& v) {
        return v.x * v.x + v.y * v.y;
    }
}

namespace Util {
    constexpr int add(int a, int b) {
        return a + b;
    }
}

namespace Chain {
    struct Num {
        int value;
        constexpr bool operator==(const Num& other) const {
            return value == other.value;
        }
    };

    constexpr Num double_up(const Num& n) {
        return {n.value * 2};
    }

    constexpr Num increment(const Num& n) {
        return {n.value + 1};
    }
}

namespace Conflict {
    struct X {
        constexpr int f() const { return 10; } // member function
    };

    constexpr int f(const X&) {
        return 20; // free function
    }
}

namespace Ptr {
    struct Wrapper {
        int val;
        constexpr int get() const { return val; }
    };

    constexpr int unwrap(const Wrapper* w) {
        return w->get();
    }

    constexpr int scale(const Wrapper* w, int factor) {
        return w->get() * factor;
    }
}

constexpr bool test_obj_call() {
    Math::Vec v{4, 6};
    auto expected = Math::normalize(v);
    auto actual = v.Math::normalize();
    return actual == expected;
}

constexpr bool test_ptr_arrow_call() {
    Math::Vec v{3, 4};
    const Math::Vec* p = &v;
    return p->Math::magnitude() == 25;
}

constexpr bool test_obj_with_arg() {
    int x = 3;
    return x.Util::add(7) == 10;
}

constexpr bool test_ptr_with_arg() {
    Ptr::Wrapper w{5};
    const Ptr::Wrapper* p = &w;
    return p.Ptr::scale(3) == 15;
}

constexpr bool test_chaining_obj() {
    Chain::Num n{2};
    auto result = n.Chain::double_up().Chain::increment();
    return result == Chain::Num{5};
}

constexpr bool test_member_vs_free() {
    Conflict::X x;
    return x.f() == 10 && x.Conflict::f() == 20;
}

constexpr bool test_ptr_obj_free() {
    Ptr::Wrapper w{42};
    const Ptr::Wrapper* p = &w;
    return p.Ptr::unwrap() == 42;
}

constexpr void free(int*& p) {
    p = nullptr;
}

static int global_int ;
constexpr bool test_ptr_obj_free_global_namespace() {
    
    int* p{&global_int};
    p.::free();
    return p == nullptr;
}


namespace stdx {
    template<typename T,typename  U>
    constexpr auto value_or(T optlike,U default_)
    {
        return optlike ? *optlike : default_;
    }


    // Functor (think ranges)
    struct twice_ 
    {
        constexpr auto operator()(const auto& x) const {
                return x*2;
        }
    } constexpr static twice;
}

constexpr bool test_optional_like() {
    int** p = nullptr;
    return p.stdx::value_or(nullptr).stdx::value_or(0) == 0; 
}

// Due to parsing ambiguity with floating points put it in ()
constexpr bool test_functor() {
    return (1).stdx::twice() == 2;
}


struct ConstructorInt {
    ConstructorInt(int x) : x(x) {}
    int x; 
};

template<typename...>
struct Tuple {
};

template<int I, typename T>
constexpr int get(Tuple<T> t,void*) {
    return 0;
}

template <typename T>
constexpr bool test_dependant(T u){
    return u.::get<0>(nullptr) == 0;
}

namespace stdx {
    using ::get;
}
template <typename T>
constexpr bool test_dependant2(T u){
    // x.stdx::get<0>(nullptr); does not compile 
    // TODO: add checking whether it is a namespace name and choose it BUT make a warning about hiding the basse class
    // and adding some sort of keyword like u.typename stdx::get<0>(); that says it is a base class
    return u.::stdx::get<0>(nullptr) == 0;
}

enum class Enum { a,b,c};
constexpr const char* to_string(Enum e)
{
    switch(e)
    {
        case Enum::a: return "a";
        case Enum::b: return "b";
        case Enum::c: return "c";
    }
    return nullptr;
}

static_assert(test_obj_call(),           "obj.Namespace::func() failed");
static_assert(test_ptr_arrow_call(),     "ptr->Namespace::func() failed");
static_assert(test_obj_with_arg(),       "obj.Namespace::func(arg) failed");
static_assert(test_ptr_with_arg(),       "ptr->Namespace::func(arg) failed");
static_assert(test_chaining_obj(),       "Chaining obj.Namespace::func().Namespace::func() failed");
static_assert(test_member_vs_free(),     "Member vs free function resolution failed");
static_assert(test_ptr_obj_free(),       "Pointer to object free function call failed");
static_assert(test_ptr_obj_free_global_namespace(),       "Pointer to object global free function call failed");
static_assert(test_optional_like(),       "Optional like");
static_assert(test_functor(),       "Functor");
static_assert(test_dependant(Tuple<int>{}),       "Dependant");
static_assert(test_dependant2(Tuple<int>{}),       "Dependant 2");
static_assert(*Enum::a.::to_string() == 'a',       "Enums");


int main(){}