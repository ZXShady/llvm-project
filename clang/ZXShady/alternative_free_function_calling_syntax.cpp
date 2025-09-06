namespace Math {
    struct Vec {
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

// ========================
// Compile-time test suite
// ========================

constexpr bool test_obj_call() {
    Math::Vec v{4, 6};
    auto expected = Math::normalize(v);
    auto actual = v.Math::normalize(); // x.Math::f() → Math::f(x)
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
    auto result = n.Chain::double_up().Chain::increment(); // (2 * 2) + 1 = 5
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
    struct identity_ 
    {
        constexpr auto operator()(const auto& x) const {
                return x;
        }
    } constexpr static identity;
}

constexpr bool test_optional_like() {
    int** p = nullptr;
    return p.stdx::value_or(nullptr).stdx::value_or(0) == 0; 
}

// Due to parsing ambiguity with floating points put it in ()
constexpr bool test_identity() {
    return (0).stdx::identity() == 0;
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
static_assert(test_identity(),       "Identity");

int main(){}