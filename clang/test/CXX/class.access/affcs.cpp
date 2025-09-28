
#include <cassert>
#include <optional>
#include <memory>
#include <cmath>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_map>
#include <variant>
#include <filesystem>
#include <iostream>


namespace Utils {
    


class Box {
public:
    double w{}, h{};
    double x{}, y{};

    Box(double w, double h) : w(w), h(h) {}
    Box(double w, double h, double x, double y) : w(w), h(h), x(x), y(y) {}
};

Box expand(const Box& box, double amount) {
    return Box(box.w + amount, box.h + amount, box.x, box.y);
}

Box translate(const Box& box, double dx, double dy) {
    return Box(box.w, box.h, box.x + dx, box.y + dy);
}

bool collides(const Box& a, const Box& b) {
    return !(a.x + a.w <= b.x || 
                     b.x + b.w <= a.x || 
                     a.y + a.h <= b.y || 
                     b.y + b.h <= a.y);
}

    enum class Enum {
        a,b,c
    };
    inline std::string to_string(Enum a)
    {
        switch (a) {
            case Enum::a: return "a";
            case Enum::b: return "b";
            case Enum::c: return "c";
        }
        return "Error";
    }
}



namespace stdx {
  auto value_or(auto&& opt,auto&& default_)
   {
    return opt ? *opt : default_;
   }

}


int main() {

  {
   std::optional<int> a;
   assert(a.stdx::value_or(42) == 42);
   // Free function syntax: stdx::value_or(a, 42)

   a = 1;
   assert(a.stdx::value_or(42) == 1);
   // Free function syntax: stdx::value_or(a, 42)
  }

  {
   std::unique_ptr<int> a;
   assert(a.stdx::value_or(42) == 42);
   // Free function syntax: stdx::value_or(a, 42)

   a = std::unique_ptr<int>(new int{1});
   assert(a.stdx::value_or(42) == 1);
   // Free function syntax: stdx::value_or(a, 42)
  }

  {
   int* a{};
   assert(a.stdx::value_or(42) == 42);
   // Free function syntax: stdx::value_or(a, 42)

   a = new int{1};
   assert(a.stdx::value_or(42) == 1);
   // Free function syntax: stdx::value_or(a, 42)
  }

  {
   std::unique_ptr<int> a{new int{400}};
   assert(a->std::clamp(0,15) == 15);
   // Free function syntax: std::clamp(*a, 0, 15)
  }

  {
   int* a{new int{400}};
   assert(a->std::clamp(0,15) == 15);
   // Free function syntax: std::clamp(*a, 0, 15)
  }

  {
   assert(400 .std::clamp(0,15) == 15);
   // Free function syntax: std::clamp(400, 0, 15)
  }

  {
    [](auto&& s){
        assert(s.std::ranges::find('a') != s.std::ranges::cend());
        // Free function syntax: std::ranges::find(s, 'a') != std::ranges::cend(s)
    }(std::string("a"));
  }

  {
    auto t = []{
        struct std {};
        struct tuple : 
        ::std::tuple<int> {
            tuple(int i) : ::std::tuple<int>(i) {}
        };
        return tuple{1};
    }();

    [](auto&& t){
        assert(t.std::get<0>() == 1);
        // Free function syntax: std::get<0>(t)
    }(t);
  }

  {
    [](auto&& e){
        assert(e.Utils::to_string() == "a");
        // Free function syntax: Utils::to_string(e)
    }(Utils::Enum::a);
  }

  {
    int a = 1, b = 2;
    a.std::swap(b);
    // Free function syntax: std::swap(a, b)

    assert(a == 2 && b == 1);
  }

  {
    int x = 5;
    assert(x.std::exchange(42) == 5);
    // Free function syntax: std::exchange(x, 42)

    assert(x == 42);
  }

  {
    double d = -3.14;
    assert(d.std::abs() == 3.14);
    // Free function syntax: std::abs(d)

    assert((-3.14).std::abs() == 3.14);
    // Free function syntax: std::abs(-3.14)
  }

  {
    enum class TexID;
    std::unordered_map<std::string, std::variant<TexID,std::filesystem::path>> textures
    = {
        {"cat","cat.png"}
    };

    assert( 
        textures
        .find("cat")
        ->second.std::get<std::filesystem::path>()
        .replace_extension("jpg")
        == "cat.jpg"
        );
    // Free function syntax:
    // std::get<std::filesystem::path>(textures.find("cat")->second).replace_extension("jpg") == "cat.jpg"
  }

  {
    assert(
        Utils::Box(1, 5)
              .Utils::expand(0.1)
              .Utils::translate(3, 3)
              .Utils::collides(Utils::Box(5, 5))
    );
    // Free function syntax:
    // collides(translate(expand(Utils::Box(1,5), 0.1), 3, 3), Utils::Box(5,5))
  }
}
