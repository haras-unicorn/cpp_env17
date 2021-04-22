#include <env/env.hpp>

int main(int, char**)
{
    return static_cast<int>(std::vector<int>().size());
}
