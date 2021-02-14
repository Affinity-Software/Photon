#include "photon/window.hpp"

int main(int argc, char const *argv[])
{
    photon::window test;
    auto dom = test.getDom();
    dom->loadFile("test.html");
    return 0;
}
